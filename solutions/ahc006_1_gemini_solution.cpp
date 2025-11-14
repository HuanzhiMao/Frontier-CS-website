#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <random>
#include <chrono>

struct Point {
    int x, y;
};

int manhattan_distance(Point p1, Point p2) {
    return std::abs(p1.x - p2.x) + std::abs(p1.y - p2.y);
}

struct Order {
    int id;
    Point p, d;
};

// --- TSP Solver ---
long long calculate_tour_length(const std::vector<Point>& points, const std::vector<int>& tour) {
    long long length = 0;
    for (size_t i = 0; i < tour.size(); ++i) {
        length += manhattan_distance(points[tour[i]], points[tour[(i + 1) % tour.size()]]);
    }
    return length;
}

std::pair<long long, std::vector<int>> solve_tsp(const std::vector<Point>& points, std::mt19937& rng) {
    int n = points.size();
    if (n == 0) return {0, {}};
    if (n == 1) return {0, {0}};
    
    std::vector<int> tour(n);
    std::vector<bool> visited(n, false);
    tour[0] = 0;
    visited[0] = true;
    for (int i = 1; i < n; ++i) {
        int last_node = tour[i - 1];
        int best_next = -1;
        long long min_dist = -1;
        for (int j = 0; j < n; ++j) {
            if (!visited[j]) {
                long long d = manhattan_distance(points[last_node], points[j]);
                if (best_next == -1 || d < min_dist) {
                    min_dist = d;
                    best_next = j;
                }
            }
        }
        tour[i] = best_next;
        visited[best_next] = true;
    }
    
    bool changed = true;
    for(int iter = 0; iter < 4 && changed; ++iter) {
        changed = false;
        for (int i = 0; i < n; ++i) {
            for (int j = i + 2; j < n; ++j) {
                int i_next = i + 1;
                int j_next = (j + 1) % n;

                long long current_dist = manhattan_distance(points[tour[i]], points[tour[i_next]]) + manhattan_distance(points[tour[j]], points[tour[j_next]]);
                long long new_dist = manhattan_distance(points[tour[i]], points[tour[j]]) + manhattan_distance(points[tour[i_next]], points[tour[j_next]]);
                
                if (new_dist < current_dist) {
                    std::reverse(tour.begin() + i_next, tour.begin() + j + 1);
                    changed = true;
                }
            }
        }
    }


    return {calculate_tour_length(points, tour), tour};
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    
    auto start_time = std::chrono::high_resolution_clock::now();
    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

    std::vector<Order> all_orders(1000);
    for (int i = 0; i < 1000; ++i) {
        all_orders[i].id = i + 1;
        std::cin >> all_orders[i].p.x >> all_orders[i].p.y >> all_orders[i].d.x >> all_orders[i].d.y;
    }

    Point office = {400, 400};

    std::vector<std::vector<int>> nearest_neighbors(1000);
    for (int i = 0; i < 1000; ++i) {
        std::vector<std::pair<int, int>> dists;
        for (int j = 0; j < 1000; ++j) {
            int d = manhattan_distance(all_orders[i].p, all_orders[j].p) + manhattan_distance(all_orders[i].d, all_orders[j].d);
            dists.push_back({d, j});
        }
        std::partial_sort(dists.begin(), dists.begin() + 50, dists.end());
        for (int k = 0; k < 50; ++k) {
            nearest_neighbors[i].push_back(dists[k].second);
        }
    }

    long long best_total_score = -1;
    std::vector<int> best_s_indices;
    std::vector<int> best_tour_p_indices, best_tour_d_indices;
    int best_p_conn_idx = -1, best_d_conn_idx = -1;

    int num_seeds = 1000;
    std::vector<int> seeds(num_seeds);
    std::iota(seeds.begin(), seeds.end(), 0);
    std::shuffle(seeds.begin(), seeds.end(), rng);

    for (int i = 0; i < num_seeds; ++i) {
        int seed_idx = seeds[i];
        
        auto current_time = std::chrono::high_resolution_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count() > 2850) {
            break;
        }

        std::vector<int> s_indices = nearest_neighbors[seed_idx];
        std::vector<Point> p_points, d_points;
        for (int idx : s_indices) {
            p_points.push_back(all_orders[idx].p);
            d_points.push_back(all_orders[idx].d);
        }

        auto [len_p, tour_p] = solve_tsp(p_points, rng);
        auto [len_d, tour_d] = solve_tsp(d_points, rng);
        
        long long min_conn_cost = -1;
        int best_p_idx = -1, best_d_idx = -1;

        for (int p_i = 0; p_i < 50; ++p_i) {
            for (int d_i = 0; d_i < 50; ++d_i) {
                int p_curr_node_idx = tour_p[p_i];
                int p_next_node_idx = tour_p[(p_i + 1) % 50];
                int d_curr_node_idx = tour_d[d_i];
                int d_next_node_idx = tour_d[(d_i + 1) % 50];

                long long conn_cost = (long long)manhattan_distance(office, p_points[p_next_node_idx])
                                     + manhattan_distance(p_points[p_curr_node_idx], d_points[d_curr_node_idx])
                                     + manhattan_distance(d_points[d_next_node_idx], office)
                                     - manhattan_distance(p_points[p_curr_node_idx], p_points[p_next_node_idx])
                                     - manhattan_distance(d_points[d_curr_node_idx], d_points[d_next_node_idx]);
                
                if (min_conn_cost == -1 || conn_cost < min_conn_cost) {
                    min_conn_cost = conn_cost;
                    best_p_idx = p_i;
                    best_d_idx = d_i;
                }
            }
        }
        
        long long current_total_score = len_p + len_d + min_conn_cost;

        if (best_total_score == -1 || current_total_score < best_total_score) {
            best_total_score = current_total_score;
            best_s_indices = s_indices;
            best_tour_p_indices = tour_p;
            best_tour_d_indices = tour_d;
            best_p_conn_idx = best_p_idx;
            best_d_conn_idx = best_d_idx;
        }
    }

    std::cout << 50;
    for (int idx : best_s_indices) {
        std::cout << " " << all_orders[idx].id;
    }
    std::cout << std::endl;

    std::vector<Point> final_path;
    final_path.push_back(office);

    std::vector<Point> p_points, d_points;
    std::vector<int> s_indices_map(1000);
    for(size_t i = 0; i < best_s_indices.size(); ++i) {
        s_indices_map[best_s_indices[i]] = i;
    }
    for (int idx : best_s_indices) {
        p_points.push_back(all_orders[idx].p);
        d_points.push_back(all_orders[idx].d);
    }
    
    int p_start_tour_idx = (best_p_conn_idx + 1) % 50;
    for (int i = 0; i < 50; ++i) {
        int node_idx = best_tour_p_indices[(p_start_tour_idx + i) % 50];
        final_path.push_back(p_points[node_idx]);
    }
    
    int d_start_tour_idx = best_d_conn_idx;
    for (int i = 0; i < 50; ++i) {
        int current_tour_pos = (d_start_tour_idx - i + 50) % 50;
        int node_idx = best_tour_d_indices[current_tour_pos];
        final_path.push_back(d_points[node_idx]);
    }
    
    final_path.push_back(office);

    std::cout << final_path.size();
    for (const auto& p : final_path) {
        std::cout << " " << p.x << " " << p.y;
    }
    std::cout << std::endl;

    return 0;
}