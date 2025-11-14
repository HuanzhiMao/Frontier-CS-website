#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <random>
#include <chrono>

#define TIME_LIMIT_MS 2950

struct Point {
    int x, y;
};

struct Order {
    int id;
    Point p, d;
};

struct PathNode {
    Point pos;
    int order_id;
    bool is_pickup;
};

int manhattan_dist(Point p1, Point p2) {
    return std::abs(p1.x - p2.x) + std::abs(p1.y - p2.y);
}

long long calculate_path_cost(const std::vector<PathNode>& path) {
    long long total_dist = 0;
    for (size_t i = 0; i < path.size() - 1; ++i) {
        total_dist += manhattan_dist(path[i].pos, path[i+1].pos);
    }
    return total_dist;
}

std::vector<Order> all_orders;
Point office = {400, 400};

std::pair<long long, std::vector<PathNode>> solve_route_interleaved_nn(const std::vector<int>& order_indices) {
    std::vector<PathNode> path;
    path.push_back({office, -1, false});

    std::vector<bool> is_picked_up(1001, false);
    std::vector<bool> is_delivered(1001, false);
    
    int num_visited = 0;
    while (num_visited < 2 * order_indices.size()) {
        Point current_pos = path.back().pos;
        int best_dist = -1;
        int best_order_id = -1;
        bool best_is_pickup = false;

        for (int order_id : order_indices) {
            if (!is_picked_up[order_id]) {
                int dist = manhattan_dist(current_pos, all_orders[order_id - 1].p);
                if (best_dist == -1 || dist < best_dist) {
                    best_dist = dist;
                    best_order_id = order_id;
                    best_is_pickup = true;
                }
            } else if (!is_delivered[order_id]) {
                int dist = manhattan_dist(current_pos, all_orders[order_id - 1].d);
                if (best_dist == -1 || dist < best_dist) {
                    best_dist = dist;
                    best_order_id = order_id;
                    best_is_pickup = false;
                }
            }
        }
        
        if (best_is_pickup) {
            path.push_back({all_orders[best_order_id - 1].p, best_order_id, true});
            is_picked_up[best_order_id] = true;
        } else {
            path.push_back({all_orders[best_order_id - 1].d, best_order_id, false});
            is_delivered[best_order_id] = true;
        }
        num_visited++;
    }

    path.push_back({office, -1, false});
    return {calculate_path_cost(path), path};
}


std::vector<PathNode> two_opt(std::vector<PathNode> path, std::chrono::high_resolution_clock::time_point start_time) {
    if (path.size() <= 3) return path;

    std::vector<bool> order_in_segment(1001);
    std::mt19937 gen(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    
    while(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start_time).count() < TIME_LIMIT_MS - 50) {
        int n = path.size();
        int i = std::uniform_int_distribution<int>(0, n - 3)(gen);
        int j = std::uniform_int_distribution<int>(i + 2, n - 2)(gen);

        long long current_edges_cost = manhattan_dist(path[i].pos, path[i+1].pos) + manhattan_dist(path[j].pos, path[j+1].pos);
        long long new_edges_cost = manhattan_dist(path[i].pos, path[j].pos) + manhattan_dist(path[i+1].pos, path[j+1].pos);

        if (new_edges_cost < current_edges_cost) {
            bool is_valid = true;
            std::fill(order_in_segment.begin(), order_in_segment.end(), false);
            for (int k = i + 1; k <= j; ++k) {
                int order_id = path[k].order_id;
                if (order_in_segment[order_id]) {
                    is_valid = false;
                    break;
                }
                order_in_segment[order_id] = true;
            }

            if (is_valid) {
                std::reverse(path.begin() + i + 1, path.begin() + j + 1);
            }
        }
    }
    return path;
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 1; i <= 1000; ++i) {
        int a, b, c, d;
        std::cin >> a >> b >> c >> d;
        all_orders.push_back({i, {a, b}, {c, d}});
    }
    
    std::vector<Point> centers;
    int grid_size = 5;
    for (int i = 0; i < grid_size; ++i) {
        for (int j = 0; j < grid_size; ++j) {
            centers.push_back({800 / (2 * grid_size) + i * 800 / grid_size, 800 / (2 * grid_size) + j * 800 / grid_size});
        }
    }
    centers.push_back(office);

    std::vector<int> best_order_set;
    long long min_cost = -1;

    for (const auto& center : centers) {
        std::vector<std::pair<int, int>> order_scores;
        for (const auto& order : all_orders) {
            int score = manhattan_dist(order.p, center) + manhattan_dist(order.d, center);
            order_scores.push_back({score, order.id});
        }
        std::sort(order_scores.begin(), order_scores.end());

        std::vector<int> current_order_set;
        for (int i = 0; i < 50; ++i) {
            current_order_set.push_back(order_scores[i].second);
        }

        auto result = solve_route_interleaved_nn(current_order_set);
        if (min_cost == -1 || result.first < min_cost) {
            min_cost = result.first;
            best_order_set = current_order_set;
        }
    }

    std::mt19937 gen(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    
    std::vector<int> current_set = best_order_set;
    long long current_cost = min_cost;

    std::vector<bool> in_current_set(1001, false);
    for (int id : current_set) in_current_set[id] = true;
    std::vector<int> not_in_current_set;
    not_in_current_set.reserve(950);
    for (int i = 1; i <= 1000; ++i) if (!in_current_set[i]) not_in_current_set.push_back(i);

    double temp = 5000.0;
    double cooling_rate = 0.9999;
    
    while(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start_time).count() < TIME_LIMIT_MS - 300) {
        int idx_to_remove_from_current = std::uniform_int_distribution<int>(0, current_set.size() - 1)(gen);
        int order_to_remove = current_set[idx_to_remove_from_current];

        int idx_to_add_from_notin = std::uniform_int_distribution<int>(0, not_in_current_set.size() - 1)(gen);
        int order_to_add = not_in_current_set[idx_to_add_from_notin];
        
        std::vector<int> new_set = current_set;
        new_set[idx_to_remove_from_current] = order_to_add;

        auto result = solve_route_interleaved_nn(new_set);
        long long new_cost = result.first;
        
        bool accepted = false;
        if (new_cost < current_cost) {
            accepted = true;
        } else {
            double prob = std::exp((current_cost - new_cost) / temp);
            if (std::uniform_real_distribution<double>(0.0, 1.0)(gen) < prob) {
                accepted = true;
            }
        }
        
        if (accepted) {
            current_set = new_set;
            current_cost = new_cost;
            not_in_current_set[idx_to_add_from_notin] = order_to_remove;

            if (current_cost < min_cost) {
                min_cost = current_cost;
                best_order_set = current_set;
            }
        }
        temp *= cooling_rate;
    }

    auto final_nn_res = solve_route_interleaved_nn(best_order_set);
    std::vector<PathNode> final_path = two_opt(final_nn_res.second, start_time);

    std::cout << best_order_set.size();
    for (int id : best_order_set) {
        std::cout << " " << id;
    }
    std::cout << std::endl;

    std::cout << final_path.size();
    for (const auto& node : final_path) {
        std::cout << " " << node.pos.x << " " << node.pos.y;
    }
    std::cout << std::endl;

    return 0;
}