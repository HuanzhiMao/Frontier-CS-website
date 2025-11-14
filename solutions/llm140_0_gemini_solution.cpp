#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <map>

using namespace std;

long long b;
int k;
int w;

// Function to send a query and receive distances
vector<long long> query(const vector<pair<long long, long long>>& probes) {
    cout << "? " << probes.size();
    for (const auto& p : probes) {
        cout << " " << p.first << " " << p.second;
    }
    cout << endl;

    int n_dists = k * probes.size();
    vector<long long> dists(n_dists);
    for (int i = 0; i < n_dists; ++i) {
        cin >> dists[i];
    }
    return dists;
}

// Helper for max bipartite matching using augmenting paths (Ford-Fulkerson with DFS)
bool can_match(int u, const vector<vector<int>>& adj, vector<int>& match, vector<bool>& visited) {
    for (int v : adj[u]) {
        if (!visited[v]) {
            visited[v] = true;
            if (match[v] < 0 || can_match(match[v], adj, match, visited)) {
                match[v] = u;
                return true;
            }
        }
    }
    return false;
}

// Backtracking function to find pairings between X and Y coordinates given their sums
map<long long, int> Y_counts, U_counts;
vector<long long> X_coords_sorted, Y_distinct;
vector<pair<long long, long long>> solution_pairs;

bool find_pairing(int x_idx) {
    if (x_idx == k) {
        return true;
    }

    long long current_x = X_coords_sorted[x_idx];
    for (long long y : Y_distinct) {
        if (Y_counts[y] > 0) {
            long long s = current_x + y;
            if (U_counts.count(s) && U_counts[s] > 0) {
                Y_counts[y]--;
                U_counts[s]--;
                solution_pairs[x_idx] = {current_x, y};
                if (find_pairing(x_idx + 1)) {
                    return true;
                }
                Y_counts[y]++;
                U_counts[s]++;
            }
        }
    }
    return false;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> b >> k >> w;

    long long T = b;

    // --- Step 1: Get Y coordinates and |X| coordinates ---
    vector<long long> dists_A = query({{0, T}});
    vector<long long> dists_B = query({{0, -T}});

    vector<vector<int>> adj1(k);
    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < k; ++j) {
            long long a = dists_A[i];
            long long b_val = dists_B[j];
            if ((b_val - a) % 2 != 0 || (a + b_val - 2 * T) % 2 != 0) continue;
            long long y = (b_val - a) / 2;
            long long abs_x = (a + b_val - 2 * T) / 2;
            
            if (y >= -::b && y <= ::b && abs_x >= 0 && abs_x <= ::b) {
                adj1[i].push_back(j);
            }
        }
    }

    vector<int> match1(k, -1);
    for (int u = 0; u < k; ++u) {
        vector<bool> visited(k, false);
        can_match(u, adj1, match1, visited);
    }
    
    vector<long long> Y_coords_vec;
    for(int i = 0; i < k; ++i){
        int u = match1[i];
        long long a = dists_A[u];
        long long b_val = dists_B[i];
        Y_coords_vec.push_back((b_val - a) / 2);
    }

    // --- Step 2: Get X coordinates and |Y| coordinates ---
    vector<long long> dists_C = query({{T, 0}});
    vector<long long> dists_D = query({{-T, 0}});
    
    vector<vector<int>> adj2(k);
    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < k; ++j) {
            long long c = dists_C[i];
            long long d = dists_D[j];
            if ((d - c) % 2 != 0 || (c + d - 2 * T) % 2 != 0) continue;
            long long x = (d - c) / 2;
            long long abs_y = (c + d - 2 * T) / 2;
            
            if (x >= -::b && x <= ::b && abs_y >= 0 && abs_y <= ::b) {
                adj2[i].push_back(j);
            }
        }
    }

    vector<int> match2(k, -1);
    for (int u = 0; u < k; ++u) {
        vector<bool> visited(k, false);
        can_match(u, adj2, match2, visited);
    }
    
    vector<long long> X_coords_vec;
    for(int i = 0; i < k; ++i){
        int u = match2[i];
        long long c = dists_C[u];
        long long d_val = dists_D[i];
        X_coords_vec.push_back((d_val - c) / 2);
    }
    
    // --- Step 3: Get sums X+Y to resolve pairings ---
    vector<long long> dists_E = query({{T, T}});
    vector<long long> U_sums_vec;
    for (long long dist : dists_E) {
        U_sums_vec.push_back(2 * T - dist);
    }

    X_coords_sorted = X_coords_vec;
    sort(X_coords_sorted.begin(), X_coords_sorted.end());
    
    for (long long y : Y_coords_vec) Y_counts[y]++;
    for (long long u : U_sums_vec) U_counts[u]++;

    for(auto const& [y_val, count] : Y_counts) Y_distinct.push_back(y_val);

    solution_pairs.resize(k);
    find_pairing(0);

    cout << "!";
    for (int i = 0; i < k; ++i) {
        cout << " " << solution_pairs[i].first << " " << solution_pairs[i].second;
    }
    cout << endl;

    return 0;
}