#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;

// --- Timer ---
auto start_time = chrono::high_resolution_clock::now();
double time_limit = 3.8;

bool is_time_up() {
    auto current_time = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = current_time - start_time;
    return elapsed.count() > time_limit;
}

// --- Data Structures ---
struct Point {
    int x, y;
};

struct Order {
    int id;
    Point p, d;
};

// --- Globals ---
Point office = {400, 400};
vector<Order> all_orders(1000);
vector<Point> all_points;
vector<int> point_to_order_id;
vector<bool> is_pickup;

// --- Utility Functions ---
int manhattan_dist(const Point& p1, const Point& p2) {
    return abs(p1.x - p2.x) + abs(p1.y - p2.y);
}

long long calculate_tour_length(const vector<int>& route) {
    long long total_dist = 0;
    for (size_t i = 0; i < route.size() - 1; ++i) {
        total_dist += manhattan_dist(all_points[route[i]], all_points[route[i+1]]);
    }
    return total_dist;
}

// --- Greedy Route Construction ---
vector<int> build_greedy_route(const vector<int>& order_indices) {
    vector<int> route;
    route.reserve(102);
    route.push_back(0); // Start at office

    vector<int> unvisited_pickups;
    unvisited_pickups.reserve(50);
    for (int order_idx : order_indices) {
        unvisited_pickups.push_back(2 * order_idx + 1);
    }
    vector<bool> deliverable_mask(1000, false);
    vector<int> deliverable_points;
    deliverable_points.reserve(50);
    
    Point current_pos = office;

    for (int i = 0; i < 100; ++i) {
        int best_point_idx = -1;
        int min_dist = -1;

        int best_pickup_idx_in_vec = -1;
        for (size_t j = 0; j < unvisited_pickups.size(); ++j) {
            int p_idx = unvisited_pickups[j];
            int dist = manhattan_dist(current_pos, all_points[p_idx]);
            if (min_dist == -1 || dist < min_dist) {
                min_dist = dist;
                best_point_idx = p_idx;
                best_pickup_idx_in_vec = j;
            }
        }

        int best_delivery_idx_in_vec = -1;
        for (size_t j = 0; j < deliverable_points.size(); ++j) {
            int d_idx = deliverable_points[j];
            int dist = manhattan_dist(current_pos, all_points[d_idx]);
            if (min_dist == -1 || dist < min_dist) {
                min_dist = dist;
                best_point_idx = d_idx;
                best_delivery_idx_in_vec = j;
            }
        }
        
        route.push_back(best_point_idx);
        current_pos = all_points[best_point_idx];
        
        int order_id = point_to_order_id[best_point_idx];
        if (is_pickup[best_point_idx]) {
            unvisited_pickups.erase(unvisited_pickups.begin() + best_pickup_idx_in_vec);
            if (!deliverable_mask[order_id]) {
                deliverable_points.push_back(2 * order_id + 2);
                deliverable_mask[order_id] = true;
            }
        } else {
            deliverable_points.erase(deliverable_points.begin() + best_delivery_idx_in_vec);
        }
    }

    route.push_back(0); // Return to office
    return route;
}

// --- Local Search ---
void local_search(vector<int>& route, const vector<int>& order_indices) {
    vector<int> p_indices(50), d_indices(50);
    for(size_t i = 0; i < order_indices.size(); ++i) {
        p_indices[i] = 2 * order_indices[i] + 1;
        d_indices[i] = 2 * order_indices[i] + 2;
    }

    vector<int> pos(all_points.size());
    bool improved = true;
    while(improved) {
        improved = false;
        if(is_time_up()) break;
        
        for (size_t i = 1; i < route.size() - 1; ++i) pos[route[i]] = i;

        // 2-opt
        for (int i = 1; i < 100; ++i) {
            for (int j = i + 1; j < 101; ++j) {
                bool is_valid = true;
                for (int k = 0; k < 50; ++k) {
                    if (pos[p_indices[k]] >= i && pos[p_indices[k]] <= j &&
                        pos[d_indices[k]] >= i && pos[d_indices[k]] <= j) {
                        is_valid = false;
                        break;
                    }
                }
                
                if (is_valid) {
                    long long current_edges_dist = manhattan_dist(all_points[route[i - 1]], all_points[route[i]]) +
                                                 manhattan_dist(all_points[route[j]], all_points[route[j + 1]]);
                    long long new_edges_dist = manhattan_dist(all_points[route[i - 1]], all_points[route[j]]) +
                                               manhattan_dist(all_points[route[i]], all_points[route[j + 1]]);
                    
                    if (new_edges_dist < current_edges_dist) {
                        reverse(route.begin() + i, route.begin() + j + 1);
                        improved = true;
                        for (size_t k = 1; k < route.size() - 1; ++k) pos[route[k]] = k;
                    }
                }
            }
        }
    }
}

// --- Main Logic ---
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    all_points.push_back(office);
    for (int i = 0; i < 1000; ++i) {
        all_orders[i].id = i;
        cin >> all_orders[i].p.x >> all_orders[i].p.y >> all_orders[i].d.x >> all_orders[i].d.y;
        all_points.push_back(all_orders[i].p);
        all_points.push_back(all_orders[i].d);
    }
    
    point_to_order_id.resize(all_points.size());
    is_pickup.resize(all_points.size());
    for(int i=0; i<1000; ++i){
        point_to_order_id[2*i+1] = i;
        is_pickup[2*i+1] = true;
        point_to_order_id[2*i+2] = i;
        is_pickup[2*i+2] = false;
    }


    vector<pair<long long, vector<int>>> candidates;

    vector<int> seed_indices(1000);
    iota(seed_indices.begin(), seed_indices.end(), 0);
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    shuffle(seed_indices.begin(), seed_indices.end(), rng);

    for (int i = 0; i < 1000; ++i) {
        int seed_order_idx = seed_indices[i];
        
        vector<pair<int, int>> dists;
        for (int j = 0; j < 1000; ++j) {
            if (seed_order_idx == j) continue;
            int dist = manhattan_dist(all_orders[seed_order_idx].p, all_orders[j].p) + 
                       manhattan_dist(all_orders[seed_order_idx].d, all_orders[j].d);
            dists.push_back({dist, j});
        }
        sort(dists.begin(), dists.end());
        
        vector<int> current_orders;
        current_orders.reserve(50);
        current_orders.push_back(seed_order_idx);
        for (int k = 0; k < 49; ++k) {
            current_orders.push_back(dists[k].second);
        }

        vector<int> initial_route = build_greedy_route(current_orders);
        long long tour_len = calculate_tour_length(initial_route);
        candidates.push_back({tour_len, current_orders});
    }

    sort(candidates.begin(), candidates.end());

    long long best_len = -1;
    vector<int> best_orders_indices;
    vector<int> best_route;

    for (size_t i = 0; i < candidates.size(); ++i) {
        if (is_time_up()) {
            break;
        }
        
        vector<int> current_orders = candidates[i].second;
        vector<int> route = build_greedy_route(current_orders);
        local_search(route, current_orders);
        long long current_len = calculate_tour_length(route);
        
        if (best_len == -1 || current_len < best_len) {
            best_len = current_len;
            best_orders_indices = current_orders;
            best_route = route;
        }
    }
    
    // Output
    cout << best_orders_indices.size();
    for (int order_idx : best_orders_indices) {
        cout << " " << order_idx + 1;
    }
    cout << endl;

    cout << best_route.size();
    for (int p_idx : best_route) {
        cout << " " << all_points[p_idx].x << " " << all_points[p_idx].y;
    }
    cout << endl;

    return 0;
}