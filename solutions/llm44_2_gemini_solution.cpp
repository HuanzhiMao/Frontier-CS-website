#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <chrono>
#include <random>

using namespace std;

struct Point {
    long long x, y;
};

int N;
vector<Point> coords;
vector<bool> is_prime;

inline double dist(int c1, int c2) {
    long long dx = coords[c1].x - coords[c2].x;
    long long dy = coords[c1].y - coords[c2].y;
    return sqrt((double)dx * dx + (double)dy * dy);
}

void sieve(int n) {
    is_prime.assign(n + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int p = 2; p * p <= n; ++p) {
        if (is_prime[p]) {
            for (int i = p * p; i <= n; i += p)
                is_prime[i] = false;
        }
    }
}

inline double get_multiplier(int step, int city_id) {
    if (step % 10 == 0 && !is_prime[city_id]) {
        return 1.1;
    }
    return 1.0;
}

double calculate_delta_cost(const vector<int>& path, int i, int j) {
    int u = path[i];
    int v = path[j];

    if (j == i + 1) { // Adjacent case
        int prev_u = path[i-1];
        int next_v = path[j+1]; 

        double old_cost = get_multiplier(i, prev_u) * dist(prev_u, u) +
                          get_multiplier(i+1, u) * dist(u, v) +
                          get_multiplier(i+2, v) * dist(v, next_v);
        
        double new_cost = get_multiplier(i, prev_u) * dist(prev_u, v) +
                          get_multiplier(i+1, v) * dist(v, u) +
                          get_multiplier(i+2, u) * dist(u, next_v);
        
        return new_cost - old_cost;
    }

    // General non-adjacent case
    int prev_u = path[i-1];
    int next_u = path[i+1];
    int prev_v = path[j-1];
    int next_v = path[j+1];

    double old_cost = get_multiplier(i, prev_u) * dist(prev_u, u) +
                      get_multiplier(i+1, u) * dist(u, next_u) +
                      get_multiplier(j, prev_v) * dist(prev_v, v) +
                      get_multiplier(j+1, v) * dist(v, next_v);

    double new_cost = get_multiplier(i, prev_u) * dist(prev_u, v) +
                      get_multiplier(i+1, v) * dist(v, next_u) +
                      get_multiplier(j, prev_v) * dist(prev_v, u) +
                      get_multiplier(j+1, u) * dist(u, next_v);
                      
    return new_cost - old_cost;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> N;
    coords.resize(N);
    for (int i = 0; i < N; ++i) {
        cin >> coords[i].x >> coords[i].y;
    }

    sieve(N);

    vector<int> path;
    path.push_back(0);

    int block_size = 400;
    
    vector<pair<long long, int>> block_cities;
    bool forward = true;
    for (int i = 1; i < N; ) {
        int end = min(i + block_size, N);
        block_cities.clear();
        for (int k = i; k < end; ++k) {
            block_cities.push_back({coords[k].y, k});
        }

        sort(block_cities.begin(), block_cities.end());

        if (forward) {
            for (const auto& p : block_cities) {
                path.push_back(p.second);
            }
        } else {
            for (int k = block_cities.size() - 1; k >= 0; --k) {
                path.push_back(block_cities[k].second);
            }
        }
        forward = !forward;
        i = end;
    }
    path.push_back(0);

    vector<int> bad_pos_indices;
    vector<int> wasted_prime_indices;
    for (int i = 1; i < N; ++i) { // path indices
        // City path[i] is the start of step i+1
        if ((i + 1) % 10 == 0 && !is_prime[path[i]]) {
            bad_pos_indices.push_back(i);
        } else if (is_prime[path[i]] && (i + 1) % 10 != 0) {
            wasted_prime_indices.push_back(i);
        }
    }

    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    auto start_time = chrono::high_resolution_clock::now();
    const int time_limit_ms = 1900;

    while (chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start_time).count() < time_limit_ms) {
        if (bad_pos_indices.empty() || wasted_prime_indices.empty()) break;
        
        uniform_int_distribution<int> bad_dist(0, bad_pos_indices.size() - 1);
        int idx1_vec = bad_dist(rng);
        int i = bad_pos_indices[idx1_vec];

        uniform_int_distribution<int> wasted_dist(0, wasted_prime_indices.size() - 1);
        int idx2_vec = wasted_dist(rng);
        int j = wasted_prime_indices[idx2_vec];

        if (i == j) continue;
        if (i > j) {
            swap(i, j);
            swap(idx1_vec, idx2_vec);
        }

        double delta = calculate_delta_cost(path, i, j);

        if (delta < 0) {
            swap(path[i], path[j]);
            
            bad_pos_indices[idx1_vec] = bad_pos_indices.back();
            bad_pos_indices.pop_back();
            
            wasted_prime_indices[idx2_vec] = wasted_prime_indices.back();
            wasted_prime_indices.pop_back();
            
            if ((i + 1) % 10 != 0) {
                wasted_prime_indices.push_back(i);
            }
            if ((j + 1) % 10 == 0) {
                bad_pos_indices.push_back(j);
            }
        }
    }

    cout << N + 1 << "\n";
    for (int city_id : path) {
        cout << city_id << "\n";
    }

    return 0;
}