#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <random>
#include <chrono>
#include <map>

// --- Utilities ---
struct Point {
    int x, y;
};

int manhattan_dist(Point p1, Point p2) {
    return std::abs(p1.x - p2.x) + std::abs(p1.y - p2.y);
}

struct Order {
    int id;
    Point p, d;
};

struct TourNode {
    Point pt;
    int order_id;
    bool is_delivery;
};

// --- Randomness ---
std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

// --- Main Logic ---
const Point OFFICE = {400, 400};
const int NUM_ORDERS = 1000;
const int QUOTA = 50;

std::vector<Order> all_orders;

// Order Selection
std::vector<int> select_orders() {
    std::vector<Point> midpoints(NUM_ORDERS);
    for (int i = 0; i < NUM_ORDERS; ++i) {
        midpoints[i] = { (all_orders[i].p.x + all_orders[i].d.x) / 2, (all_orders[i].p.y + all_orders[i].d.y) / 2 };
    }

    double best_score = 1e18;
    std::vector<int> best_order_indices;

    for (int i = 0; i < NUM_ORDERS; ++i) {
        Point center = midpoints[i];
        std::vector<std::pair<int, int>> dists;
        for (int j = 0; j < NUM_ORDERS; ++j) {
            dists.push_back({manhattan_dist(center, midpoints[j]), j});
        }
        std::nth_element(dists.begin(), dists.begin() + QUOTA, dists.end());
        
        std::vector<int> current_selection;
        double current_score = 0;
        Point com = {0, 0};

        for(int k=0; k<QUOTA; ++k) {
            int order_idx = dists[k].second;
            current_selection.push_back(order_idx);
            com.x += all_orders[order_idx].p.x + all_orders[order_idx].d.x;
            com.y += all_orders[order_idx].p.y + all_orders[order_idx].d.y;
        }
        com.x /= (2 * QUOTA);
        com.y /= (2 * QUOTA);
        
        current_score += 2.0 * manhattan_dist(OFFICE, com);
        for (int order_idx : current_selection) {
            current_score += manhattan_dist(all_orders[order_idx].p, com);
            current_score += manhattan_dist(all_orders[order_idx].d, com);
        }

        if (current_score < best_score) {
            best_score = current_score;
            best_order_indices = current_selection;
        }
    }
    return best_order_indices;
}

std::vector<TourNode> build_initial_tour(const std::vector<int>& selected_indices) {
    std::vector<TourNode> tour;
    tour.push_back({OFFICE, -1, false});

    std::vector<int> pickups, deliveries;
    for(int idx : selected_indices) {
        pickups.push_back(idx);
        deliveries.push_back(idx);
    }

    Point current_pos = OFFICE;
    while(!pickups.empty()) {
        size_t best_k = -1;
        int min_dist = 1e9;
        for (size_t k = 0; k < pickups.size(); ++k) {
            int d = manhattan_dist(current_pos, all_orders[pickups[k]].p);
            if (d < min_dist) {
                min_dist = d;
                best_k = k;
            }
        }
        int order_idx = pickups[best_k];
        tour.push_back({all_orders[order_idx].p, all_orders[order_idx].id, false});
        current_pos = all_orders[order_idx].p;
        pickups.erase(pickups.begin() + best_k);
    }
    
    while(!deliveries.empty()){
        size_t best_k = -1;
        int min_dist = 1e9;
        for (size_t k = 0; k < deliveries.size(); ++k) {
            int d = manhattan_dist(current_pos, all_orders[deliveries[k]].d);
            if (d < min_dist) {
                min_dist = d;
                best_k = k;
            }
        }
        int order_idx = deliveries[best_k];
        tour.push_back({all_orders[order_idx].d, all_orders[order_idx].id, true});
        current_pos = all_orders[order_idx].d;
        deliveries.erase(deliveries.begin() + best_k);
    }
    return tour;
}


long long calculate_tour_dist(const std::vector<TourNode>& tour) {
    long long dist = 0;
    for (size_t i = 0; i < tour.size() - 1; ++i) {
        dist += manhattan_dist(tour[i].pt, tour[i+1].pt);
    }
    dist += manhattan_dist(tour.back().pt, OFFICE);
    return dist;
}

void optimize_tour(std::vector<TourNode>& tour, const std::vector<int>& selected_indices, double time_limit) {
    auto start_time = std::chrono::high_resolution_clock::now();
    long long current_dist = calculate_tour_dist(tour);
    
    std::vector<int> p_pos(NUM_ORDERS + 1), d_pos(NUM_ORDERS + 1);
    
    auto update_positions = [&]() {
        for (size_t i = 1; i < tour.size(); ++i) {
            if (tour[i].is_delivery) d_pos[tour[i].order_id] = i;
            else p_pos[tour[i].order_id] = i;
        }
    };
    update_positions();

    double T_start = 500, T_end = 0.1;
    
    while (true) {
        auto now = std::chrono::high_resolution_clock::now();
        double elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count();
        if (elapsed_ms > time_limit) break;

        double T = T_start * pow(T_end / T_start, elapsed_ms / time_limit);

        int move_type = std::uniform_int_distribution<int>(0, 99)(rng);
        
        if (move_type < 50) { // Relocate
            int idx = std::uniform_int_distribution<int>(1, tour.size() - 1)(rng);
            TourNode node_to_move = tour[idx];
            int order_id = node_to_move.order_id;
            
            int min_pos, max_pos;
            if (node_to_move.is_delivery) {
                min_pos = p_pos[order_id] + 1;
                max_pos = tour.size();
            } else {
                min_pos = 1;
                max_pos = d_pos[order_id];
            }
            if (min_pos > max_pos) continue;

            int new_idx = std::uniform_int_distribution<int>(min_pos, max_pos)(rng);
            if (new_idx == idx || new_idx == idx + 1) continue;
            
            tour.erase(tour.begin() + idx);
            tour.insert(tour.begin() + new_idx, node_to_move);

            long long new_dist = calculate_tour_dist(tour);
            long long delta = new_dist - current_dist;
            
            if (delta < 0 || std::uniform_real_distribution<double>(0, 1)(rng) < exp(-delta / T)) {
                current_dist = new_dist;
                update_positions();
            } else {
                tour.erase(tour.begin() + new_idx);
                tour.insert(tour.begin() + idx, node_to_move);
            }
        } else { // 2-opt
            if (tour.size() <= 4) continue;
            int i = std::uniform_int_distribution<int>(0, tour.size() - 3)(rng);
            int j = std::uniform_int_distribution<int>(i + 2, tour.size() - 1)(rng);

            bool ok = true;
            for (int k = i + 1; k <= j; ++k) {
                int oid = tour[k].order_id;
                bool p_in = (p_pos[oid] >= i + 1 && p_pos[oid] <= j);
                bool d_in = (d_pos[oid] >= i + 1 && d_pos[oid] <= j);
                if (p_in && !d_in) {
                    if (i + 1 + j - p_pos[oid] > d_pos[oid]) { ok = false; break; }
                }
                if (!p_in && d_in) {
                    if (p_pos[oid] > i + 1 + j - d_pos[oid]) { ok = false; break; }
                }
            }
            if (!ok) continue;

            Point pi = tour[i].pt, pi1 = tour[i+1].pt;
            Point pj = tour[j].pt, pj1 = (j + 1 == tour.size()) ? OFFICE : tour[j+1].pt;
            long long delta = manhattan_dist(pi, pj) + manhattan_dist(pi1, pj1) - manhattan_dist(pi, pi1) - manhattan_dist(pj, pj1);

            if (delta < 0 || std::uniform_real_distribution<double>(0, 1)(rng) < exp(-delta / T)) {
                std::reverse(tour.begin() + i + 1, tour.begin() + j + 1);
                current_dist += delta;
                update_positions();
            }
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    for (int i = 0; i < NUM_ORDERS; ++i) {
        all_orders.push_back({i + 1, {}, {}});
        std::cin >> all_orders.back().p.x >> all_orders.back().p.y >> all_orders.back().d.x >> all_orders.back().d.y;
    }

    auto selected_indices = select_orders();
    auto tour = build_initial_tour(selected_indices);
    optimize_tour(tour, selected_indices, 1900);

    // Output
    std::cout << QUOTA;
    for (int idx : selected_indices) {
        std::cout << " " << all_orders[idx].id;
    }
    std::cout << std::endl;

    std::vector<Point> path_pts;
    for (size_t i = 0; i < tour.size(); ++i) {
        path_pts.push_back(tour[i].pt);
    }
    path_pts.push_back(OFFICE);
    
    std::cout << path_pts.size();
    for(const auto& p : path_pts) {
        std::cout << " " << p.x << " " << p.y;
    }
    std::cout << std::endl;

    return 0;
}