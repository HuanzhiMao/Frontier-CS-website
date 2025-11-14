#include <iostream>
#include <vector>
#include <numeric>
#include <functional>
#include <queue>
#include <cmath>

using namespace std;

long long ask1(const vector<int>& nodes) {
    if (nodes.empty()) {
        return 0;
    }
    cout << "? 1 " << nodes.size();
    for (int node : nodes) {
        cout << " " << node;
    }
    cout << endl;
    long long response;
    cin >> response;
    return response;
}

void solve() {
    int n;
    cin >> n;
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    vector<int> color(n + 1, -1);
    vector<int> p0, p1;
    function<void(int, int, int)> dfs_color = 
        [&](int u, int p, int c) {
        color[u] = c;
        if (c == 0) p0.push_back(u);
        else p1.push_back(u);
        for (int v : adj[u]) {
            if (v != p) {
                dfs_color(v, u, 1 - c);
            }
        }
    };
    
    dfs_color(1, 0, 0);

    vector<long long> f(n + 1);

    auto get_f_for_partition = [&](const vector<int>& p) {
        if (p.empty()) return;
        if (p.size() == 1) {
            f[p[0]] = ask1(p);
            return;
        }
        
        long long total_sum = ask1(p);
        vector<int> query_nodes;
        for (size_t i = 0; i < p.size() - 1; ++i) {
            query_nodes.clear();
            for (size_t j = 0; j < p.size(); ++j) {
                if (i != j) {
                    query_nodes.push_back(p[j]);
                }
            }
            long long partial_sum = ask1(query_nodes);
            f[p[i]] = total_sum - partial_sum;
        }

        long long sum_found = 0;
        for (size_t i = 0; i < p.size() - 1; ++i) {
            sum_found += f[p[i]];
        }
        f[p.back()] = total_sum - sum_found;
    };
    
    if (p0.size() <= p1.size()) {
        get_f_for_partition(p0);
        get_f_for_partition(p1);
    } else {
        get_f_for_partition(p1);
        get_f_for_partition(p0);
    }

    vector<vector<int>> dist(n + 1, vector<int>(n + 1, -1));
    for (int i = 1; i <= n; ++i) {
        queue<pair<int, int>> q;
        q.push({i, 0});
        dist[i][i] = 0;
        vector<bool> visited(n + 1, false);
        visited[i] = true;
        while (!q.empty()) {
            auto [u, d] = q.front();
            q.pop();
            for (int v : adj[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    dist[i][v] = d + 1;
                    q.push({v, d + 1});
                }
            }
        }
    }

    vector<int> ans(n + 1);
    for (int r = 1; r <= n; ++r) {
        bool parity_ok = true;
        for (int i = 1; i <= n; ++i) {
            if ((f[i] % 2 + 2) % 2 != (dist[r][i] + 1) % 2) {
                parity_ok = false;
                break;
            }
        }

        if (parity_ok) {
            vector<int> p(n + 1, 0);
            queue<int> q;
            q.push(r);
            vector<bool> visited(n+1, false);
            visited[r] = true;

            while(!q.empty()){
                int u = q.front();
                q.pop();
                for(int v : adj[u]){
                    if(!visited[v]){
                        visited[v] = true;
                        p[v] = u;
                        q.push(v);
                    }
                }
            }

            vector<int> current_ans(n + 1);
            bool values_ok = true;
            
            if (abs(f[r]) != 1) {
                values_ok = false;
            } else {
                current_ans[r] = f[r];
            }

            if(values_ok) {
                for (int i = 1; i <= n; ++i) {
                    if (i == r) continue;
                    current_ans[i] = f[i] - f[p[i]];
                    if (abs(current_ans[i]) != 1) {
                        values_ok = false;
                        break;
                    }
                }
            }

            if(values_ok) {
                ans = current_ans;
                break;
            }
        }
    }

    cout << "!";
    for (int i = 1; i <= n; ++i) {
        cout << " " << ans[i];
    }
    cout << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}