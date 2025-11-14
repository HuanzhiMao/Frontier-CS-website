#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <string>

using namespace std;

int n;
vector<vector<int>> C;
vector<vector<int>> adj[2];

// Check if a given permutation p is almost monochromatic
bool is_almost_monochromatic(const vector<int>& p) {
    if (p.size() != n) {
        return false;
    }
    vector<int> c(n);
    for (int i = 0; i < n - 1; ++i) {
        c[i] = C[p[i] - 1][p[i + 1] - 1];
    }
    c[n - 1] = C[p[n - 1] - 1][p[0] - 1];

    int changes = 0;
    for (int i = 0; i < n - 1; ++i) {
        if (c[i] != c[i + 1]) {
            changes++;
        }
    }
    return changes <= 1;
}

void solve() {
    for (int c = 0; c < 2; ++c) {
        adj[c].assign(n + 1, vector<int>());
    }
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            adj[C[i][j]][i + 1].push_back(j + 1);
            adj[C[i][j]][j + 1].push_back(i + 1);
        }
    }
    for (int c = 0; c < 2; ++c) {
        for (int i = 1; i <= n; ++i) {
            sort(adj[c][i].begin(), adj[c][i].end());
        }
    }

    vector<int> best_p;
    auto update_best_p = [&](vector<int>& p) {
        if (p.size() == n && is_almost_monochromatic(p)) {
            if (best_p.empty() || p < best_p) {
                best_p = p;
            }
        }
    };
    
    for (int p1 = 1; p1 <= n; ++p1) {
        for (int p2 = 1; p2 <= n; ++p2) {
            if (p1 == p2) continue;

            // Try to build path starting with (p1, p2)
            vector<int> p_base = {p1, p2};
            vector<bool> used_base(n + 1, false);
            used_base[p1] = used_base[p2] = true;
            
            int c1 = C[p1-1][p2-1];

            // monochromatic path
            vector<int> current_p = p_base;
            vector<bool> current_used = used_base;
            int curr = p2;
            bool possible = true;
            while(current_p.size() < n) {
                int next_v = -1;
                for(int neighbor : adj[c1][curr]) {
                    if(!current_used[neighbor]) {
                        next_v = neighbor;
                        break;
                    }
                }
                if (next_v == -1) {possible = false; break;}
                current_p.push_back(next_v);
                current_used[next_v] = true;
                curr = next_v;
            }
            if(possible) update_best_p(current_p);

            // mixed color path
            current_p = p_base;
            current_used = used_base;
            curr = p2;
            
            // Phase 1: extend with color c1
            while(true) {
                int next_v = -1;
                for(int neighbor : adj[c1][curr]) {
                    if(!current_used[neighbor]) {
                        next_v = neighbor;
                        break;
                    }
                }
                if(next_v == -1) break;
                current_p.push_back(next_v);
                current_used[next_v] = true;
                curr = next_v;
            }
            
            // Phase 2: extend with color 1-c1
            if(current_p.size() < n) {
                int c2 = 1 - c1;
                 while(current_p.size() < n) {
                    int next_v = -1;
                    for(int neighbor : adj[c2][curr]) {
                        if(!current_used[neighbor]) {
                            next_v = neighbor;
                            break;
                        }
                    }
                    if(next_v == -1) break;
                    current_p.push_back(next_v);
                    current_used[next_v] = true;
                    curr = next_v;
                }
            }
            update_best_p(current_p);
        }
    }

    if (best_p.empty()) {
        cout << -1 << endl;
    } else {
        for (int i = 0; i < n; ++i) {
            cout << best_p[i] << (i == n - 1 ? "" : " ");
        }
        cout << endl;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    while (cin >> n) {
        C.assign(n, vector<int>(n));
        string row_str;
        for (int i = 0; i < n; ++i) {
            cin >> row_str;
            for (int j = 0; j < n; ++j) {
                C[i][j] = row_str[j] - '0';
            }
        }
        solve();
    }
    return 0;
}