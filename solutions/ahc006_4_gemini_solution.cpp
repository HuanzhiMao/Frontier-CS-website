#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <chrono>
#include <random>

using namespace std;

// --- Utilities ---
struct Point {
    int x, y;
};

struct Order {
    int id;
    Point p, d;
    Point mid;
};

long long manhattan_dist(const Point& p1, const Point& p2) {
    return abs(p1.x - p2.x) + abs(p1.y - p2.y);
}

// Random number generator
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

// --- Constants ---
const int NUM_ORDERS = 1000;
const int ORDERS_TO_SELECT = 50;
const int OFFICE_X = 400;
const int OFFICE_Y = 400;
const int GRID_SIZE = 7;
const int CELL_SIZE = (800 + GRID_SIZE - 1) / GRID_SIZE;
const int TIME_LIMIT_MS = 1950;

// --- Global State for current candidate set ---
vector<Point> current_points(ORDERS_TO_SELECT * 2 + 1);

// --- Tour Calculation ---
long long calculate_tour_cost(const vector<int>& tour) {
    long long cost = 0;
    for (size_t i = 0; i < tour.size() - 1; ++i) {
        cost += manhattan_dist(current_points[tour[i]], current_points[tour[i+1]]);
    }
    return cost;
}

// --- Greedy Tour Construction ---
vector<int> build_greedy_tour() {
    vector<int> tour;
    tour.reserve(ORDERS_TO_SELECT * 2 + 2);
    tour.push_back(0); // Start at office

    vector<bool> visited(ORDERS_TO_SELECT * 2 + 1, false);
    vector<bool> picked_up(ORDERS_TO_SELECT + 1, false);
    visited[0] = true;

    int current_pos_idx = 0;
    for (int i = 0; i < ORDERS_TO_SELECT * 2; ++i) {
        long long min_dist = -1;
        int next_pos_idx = -1;

        for (int j = 1; j <= ORDERS_TO_SELECT * 2; ++j) {
            if (visited[j]) continue;

            bool is_delivery = j > ORDERS_TO_SELECT;
            if (is_delivery && !picked_up[j - ORDERS_TO_SELECT]) {
                continue;
            }

            long long d = manhattan_dist(current_points[current_pos_idx], current_points[j]);
            if (next_pos_idx == -1 || d < min_dist) {
                min_dist = d;
                next_pos_idx = j;
            }
        }

        tour.push_back(next_pos_idx);
        visited[next_pos_idx] = true;
        if (next_pos_idx <= ORDERS_TO_SELECT) { // It's a pickup
            picked_up[next_pos_idx] = true;
        }
        current_pos_idx = next_pos_idx;
    }

    tour.push_back(0); // Return to office
    return tour;
}

// --- Simulated Annealing for Tour Improvement ---
vector<int> run_sa(const vector<int>& initial_tour, double time_limit_sec) {
    auto sa_start_time = chrono::high_resolution_clock::now();
    
    vector<int> current_tour = initial_tour;
    long long current_cost = calculate_tour_cost(current_tour);

    vector<int> best_tour = current_tour;
    long long best_cost = current_cost;

    vector<int> pos_in_tour(ORDERS_TO_SELECT * 2 + 1);
    for(size_t i = 1; i < current_tour.size() - 1; ++i) {
        pos_in_tour[current_tour[i]] = i;
    }

    double T_start = 2000.0;
    double T_end = 0.1;

    uniform_int_distribution<int> dist_idx(1, ORDERS_TO_SELECT * 2);
    uniform_real_distribution<double> dist_prob(0.0, 1.0);
    
    while (true) {
        auto now = chrono::high_resolution_clock::now();
        double elapsed_sec = chrono::duration_cast<chrono::duration<double>>(now - sa_start_time).count();
        if (elapsed_sec >= time_limit_sec) break;

        int i = dist_idx(rng);
        int j = dist_idx(rng);
        if (i == j) continue;
        if (i > j) swap(i, j);

        bool possible = true;
        for (int k = 1; k <= ORDERS_TO_SELECT; ++k) {
            bool p_in = pos_in_tour[k] >= i && pos_in_tour[k] <= j;
            bool d_in = pos_in_tour[k + ORDERS_TO_SELECT] >= i && pos_in_tour[k + ORDERS_TO_SELECT] <= j;
            if (p_in && d_in) {
                possible = false;
                break;
            }
        }
        if (!possible) continue;
        
        long long cost_diff = 0;
        cost_diff -= manhattan_dist(current_points[current_tour[i - 1]], current_points[current_tour[i]]);
        cost_diff -= manhattan_dist(current_points[current_tour[j]], current_points[current_tour[j + 1]]);
        cost_diff += manhattan_dist(current_points[current_tour[i - 1]], current_points[current_tour[j]]);
        cost_diff += manhattan_dist(current_points[current_tour[i]], current_points[current_tour[j + 1]]);

        double T = T_start * pow(T_end / T_start, elapsed_sec / time_limit_sec);
        
        if (cost_diff < 0 || dist_prob(rng) < exp(-cost_diff / T)) {
            reverse(current_tour.begin() + i, current_tour.begin() + j + 1);
            current_cost += cost_diff;

            for(int k = i; k <= j; ++k) {
                pos_in_tour[current_tour[k]] = k;
            }

            if (current_cost < best_cost) {
                best_cost = current_cost;
                best_tour = current_tour;
            }
        }
    }
    return best_tour;
}

// --- Main Solver ---
void solve() {
    vector<Order> all_orders(NUM_ORDERS);
    for (int i = 0; i < NUM_ORDERS; ++i) {
        all_orders[i].id = i + 1;
        cin >> all_orders[i].p.x >> all_orders[i].p.y >> all_orders[i].d.x >> all_orders[i].d.y;
        all_orders[i].mid.x = (all_orders[i].p.x + all_orders[i].d.x) / 2;
        all_orders[i].mid.y = (all_orders[i].p.y + all_orders[i].d.y) / 2;
    }

    vector<vector<int>> grid(GRID_SIZE * GRID_SIZE);
    for (int i = 0; i < NUM_ORDERS; ++i) {
        int gx = all_orders[i].mid.x / CELL_SIZE;
        int gy = all_orders[i].mid.y / CELL_SIZE;
        grid[gy * GRID_SIZE + gx].push_back(i);
    }
    
    vector<int> best_order_ids;
    vector<int> best_tour;
    long long best_cost = -1;

    int num_candidates = GRID_SIZE * GRID_SIZE;
    double time_per_candidate = (double)TIME_LIMIT_MS / 1000.0 / num_candidates;

    for (int cy = 0; cy < GRID_SIZE; ++cy) {
        for (int cx = 0; cx < GRID_SIZE; ++cx) {
            vector<int> candidate_pool;
            for (int ny = max(0, cy - 1); ny <= min(GRID_SIZE - 1, cy + 1); ++ny) {
                for (int nx = max(0, cx - 1); nx <= min(GRID_SIZE - 1, cx + 1); ++nx) {
                    candidate_pool.insert(candidate_pool.end(), grid[ny * GRID_SIZE + nx].begin(), grid[ny * GRID_SIZE + nx].end());
                }
            }

            if (candidate_pool.size() < ORDERS_TO_SELECT) continue;

            Point region_center = {(int)((cx + 0.5) * CELL_SIZE), (int)((cy + 0.5) * CELL_SIZE)};
            
            partial_sort(candidate_pool.begin(), candidate_pool.begin() + ORDERS_TO_SELECT, candidate_pool.end(), 
            [&](int a_idx, int b_idx){
                return manhattan_dist(all_orders[a_idx].mid, region_center) < manhattan_dist(all_orders[b_idx].mid, region_center);
            });
            
            vector<int> selected_order_ids;
            selected_order_ids.reserve(ORDERS_TO_SELECT);
            for (int i = 0; i < ORDERS_TO_SELECT; ++i) {
                selected_order_ids.push_back(all_orders[candidate_pool[i]].id);
            }
            
            current_points[0] = {OFFICE_X, OFFICE_Y};
            for (int i = 0; i < ORDERS_TO_SELECT; ++i) {
                int order_idx = selected_order_ids[i] - 1;
                current_points[i + 1] = all_orders[order_idx].p;
                current_points[i + 1 + ORDERS_TO_SELECT] = all_orders[order_idx].d;
            }

            vector<int> greedy_tour = build_greedy_tour();
            vector<int> sa_tour = run_sa(greedy_tour, time_per_candidate);
            long long current_cost = calculate_tour_cost(sa_tour);

            if (best_cost == -1 || current_cost < best_cost) {
                best_cost = current_cost;
                best_order_ids = selected_order_ids;
                best_tour = sa_tour;
            }
        }
    }
    
    // Output
    cout << ORDERS_TO_SELECT;
    sort(best_order_ids.begin(), best_order_ids.end());
    for (int id : best_order_ids) {
        cout << " " << id;
    }
    cout << endl;

    current_points[0] = {OFFICE_X, OFFICE_Y};
    for (int i = 0; i < ORDERS_TO_SELECT; ++i) {
        int order_idx = best_order_ids[i] - 1;
        current_points[i + 1] = all_orders[order_idx].p;
        current_points[i + 1 + ORDERS_TO_SELECT] = all_orders[order_idx].d;
    }
    
    cout << best_tour.size();
    for (int point_idx : best_tour) {
        cout << " " << current_points[point_idx].x << " " << current_points[point_idx].y;
    }
    cout << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}