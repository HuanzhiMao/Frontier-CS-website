#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <random>

using namespace std;

const int N_CONST = 100;
const int L_CONST = 500000;

int N, L;
vector<long long> T;

struct State {
    vector<int> a, b;
    vector<long long> t;
    long long error;
};

chrono::steady_clock::time_point start_time;
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

void simulate(const vector<int>& a, const vector<int>& b, vector<long long>& t, long long& error) {
    t.assign(N, 0);
    int current = 0;
    t[current]++;
    for (int week = 1; week < L; ++week) {
        if (t[current] % 2 != 0) {
            current = a[current];
        } else {
            current = b[current];
        }
        t[current]++;
    }
    error = 0;
    for (int i = 0; i < N; ++i) {
        error += abs(t[i] - T[i]);
    }
}

bool check_reachability(const vector<int>& a, const vector<int>& b) {
    vector<bool> visited(N, false);
    vector<int> q;
    q.reserve(N);
    q.push_back(0);
    visited[0] = true;
    int head = 0;
    int count = 1;
    while(head < q.size()){
        int u = q[head++];
        int v1 = a[u], v2 = b[u];
        if(!visited[v1]){
            visited[v1] = true;
            q.push_back(v1);
            count++;
        }
        if(!visited[v2]){
            visited[v2] = true;
            q.push_back(v2);
            count++;
        }
    }
    return count == N;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    start_time = chrono::steady_clock::now();

    cin >> N >> L;
    T.resize(N);
    for (int i = 0; i < N; ++i) {
        cin >> T[i];
    }

    State current_state, best_state;

    current_state.a.resize(N);
    current_state.b.resize(N);

    for (int i = 0; i < N; ++i) {
        current_state.a[i] = (i + 1) % N;
    }

    vector<pair<long long, int>> even_bundles;
    for(int i=0; i<N; ++i) {
        even_bundles.push_back({T[i] / 2, i});
    }
    sort(even_bundles.rbegin(), even_bundles.rend());

    vector<long long> remaining_demand(N);
    for(int j=0; j<N; ++j) {
        int prev = (j - 1 + N) % N;
        remaining_demand[j] = T[j] - (T[prev] + 1) / 2;
    }
    
    vector<pair<long long, int>> demand_nodes;
    demand_nodes.reserve(N);
    for(int i=0; i<N; ++i) {
        demand_nodes.push_back({remaining_demand[i], i});
    }
    
    for(const auto& bundle : even_bundles) {
        auto it = max_element(demand_nodes.begin(), demand_nodes.end());
        int target_node_idx = it->second;
        current_state.b[bundle.second] = target_node_idx;
        it->first -= bundle.first;
    }

    simulate(current_state.a, current_state.b, current_state.t, current_state.error);
    best_state = current_state;

    int outer_iter_count = 0;
    double time_limit = 2.95;

    while (true) {
        auto now = chrono::steady_clock::now();
        double elapsed = chrono::duration_cast<chrono::milliseconds>(now - start_time).count() / 1000.0;
        if (elapsed > time_limit) break;
        outer_iter_count++;
        
        State candidate_state;
        candidate_state.a = current_state.a;
        candidate_state.b = current_state.b;

        vector<long long> O(N), E(N);
        for(int i=0; i<N; ++i) {
            O[i] = (current_state.t[i] > 0) ? (current_state.t[i] + 1) / 2 : 0;
            E[i] = (current_state.t[i] > 0) ? current_state.t[i] / 2 : 0;
        }
        
        vector<long long> loads(N, 0);
        for(int i=0; i<N; ++i) {
            loads[candidate_state.a[i]] += O[i];
            loads[candidate_state.b[i]] += E[i];
        }
        
        long long abstract_error_sq = 0;
        for(int i=0; i<N; ++i) {
            abstract_error_sq += (loads[i] - T[i]) * (loads[i] - T[i]);
        }
        
        int inner_loops = 25000;
        
        double start_temp_abs = 2000;
        double end_temp_abs = 10;

        for (int i = 0; i < inner_loops; ++i) {
            int node_to_change = rng() % N;
            bool is_a = rng() % 2;
            int new_target = rng() % N;

            int old_target;
            long long bundle_size;
            
            if (is_a) {
                if (candidate_state.a[node_to_change] == new_target) continue;
                old_target = candidate_state.a[node_to_change];
                bundle_size = O[node_to_change];
            } else {
                if (candidate_state.b[node_to_change] == new_target) continue;
                old_target = candidate_state.b[node_to_change];
                bundle_size = E[node_to_change];
            }

            long long loads_old = loads[old_target];
            long long loads_new = loads[new_target];
            long long T_old = T[old_target];
            long long T_new = T[new_target];

            long long next_abstract_error_sq = abstract_error_sq;
            next_abstract_error_sq -= (loads_old - T_old) * (loads_old - T_old);
            next_abstract_error_sq -= (loads_new - T_new) * (loads_new - T_new);
            next_abstract_error_sq += (loads_old - bundle_size - T_old) * (loads_old - bundle_size - T_old);
            next_abstract_error_sq += (loads_new + bundle_size - T_new) * (loads_new + bundle_size - T_new);

            double temp = start_temp_abs * pow(end_temp_abs / start_temp_abs, (double)i / inner_loops);
            
            if (next_abstract_error_sq < abstract_error_sq || 
                exp((double)(abstract_error_sq - next_abstract_error_sq) / temp) > (uniform_real_distribution<double>(0.0, 1.0)(rng)) ) {
                
                if (is_a) candidate_state.a[node_to_change] = new_target;
                else      candidate_state.b[node_to_change] = new_target;

                if (!check_reachability(candidate_state.a, candidate_state.b)) {
                    if (is_a) candidate_state.a[node_to_change] = old_target;
                    else      candidate_state.b[node_to_change] = old_target;
                    continue;
                }
                
                loads[old_target] -= bundle_size;
                loads[new_target] += bundle_size;
                abstract_error_sq = next_abstract_error_sq;
            }
        }
        
        simulate(candidate_state.a, candidate_state.b, candidate_state.t, candidate_state.error);
        
        double temp_outer = 2000 * pow(0.01, elapsed / time_limit);
        
        if (candidate_state.error < current_state.error ||
            (current_state.error > 0 && exp((double)(current_state.error - candidate_state.error) / temp_outer) > uniform_real_distribution<double>(0.0, 1.0)(rng))
           ) {
            current_state = candidate_state;
            if (current_state.error < best_state.error) {
                best_state = current_state;
            }
        }
    }

    for (int i = 0; i < N; ++i) {
        cout << best_state.a[i] << " " << best_state.b[i] << "\n";
    }

    return 0;
}