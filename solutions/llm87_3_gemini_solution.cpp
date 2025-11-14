#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

const int INF = 1e9;

void bfs(int n, const std::vector<std::vector<int>>& adj, std::vector<int>& dist, int color, const std::vector<int>& initial_state) {
    std::queue<int> q;
    dist.assign(n + 1, INF);

    for (int i = 1; i <= n; ++i) {
        if (initial_state[i] == color) {
            q.push(i);
            dist[i] = 0;
        }
    }

    int head = 0;
    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v : adj[u]) {
            if (dist[v] == INF) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n, m;
    std::cin >> n >> m;

    std::vector<int> initial_state(n + 1);
    for (int i = 1; i <= n; ++i) {
        std::cin >> initial_state[i];
    }

    std::vector<int> target_state(n + 1);
    for (int i = 1; i <= n; ++i) {
        std::cin >> target_state[i];
    }

    std::vector<std::vector<int>> adj(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v;
        std::cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    for (int i = 1; i <= n; ++i) {
        adj[i].push_back(i);
    }

    std::vector<int> d0, d1;
    bfs(n, adj, d0, 0, initial_state);
    bfs(n, adj, d1, 1, initial_state);

    std::vector<int> s(n + 1);
    std::vector<int> switching_nodes;
    for (int i = 1; i <= n; ++i) {
        if (initial_state[i] != target_state[i]) {
            switching_nodes.push_back(i);
            if (target_state[i] == 0) {
                s[i] = d0[i];
            } else {
                s[i] = d1[i];
            }
        } else {
            s[i] = 0;
        }
    }

    while (true) {
        bool changed = false;
        for (int u : switching_nodes) {
            bool has_provider = false;
            int target_color = target_state[u];

            for (int v : adj[u]) {
                int provider_color;
                if (s[u] - 1 >= s[v]) {
                    provider_color = target_state[v];
                } else {
                    provider_color = initial_state[v];
                }
                
                if (provider_color == target_color) {
                    has_provider = true;
                    break;
                }
            }

            if (!has_provider) {
                s[u]++;
                changed = true;
            }
        }
        if (!changed) {
            break;
        }
    }

    int k = 0;
    for (int i = 1; i <= n; ++i) {
        k = std::max(k, s[i]);
    }

    std::cout << k << "\n";

    for (int t = 0; t <= k; ++t) {
        for (int i = 1; i <= n; ++i) {
            if (t >= s[i]) {
                std::cout << target_state[i] << (i == n ? "" : " ");
            } else {
                std::cout << initial_state[i] << (i == n ? "" : " ");
            }
        }
        std::cout << "\n";
    }

    return 0;
}