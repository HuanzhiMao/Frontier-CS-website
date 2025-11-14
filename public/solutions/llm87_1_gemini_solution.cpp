#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

const int INF = 1e9;

// Multi-source BFS to calculate shortest distances from a set of start nodes
vector<int> bfs(int n, const vector<vector<int>>& adj, const vector<int>& start_nodes) {
    vector<int> dist(n + 1, INF);
    queue<int> q;

    for (int node : start_nodes) {
        if (node >= 1 && node <= n) {
            dist[node] = 0;
            q.push(node);
        }
    }

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

    return dist;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    cin >> n >> m;

    vector<int> initial_state(n + 1);
    vector<int> target_state(n + 1);
    vector<int> t0_nodes, t1_nodes;

    for (int i = 1; i <= n; ++i) {
        cin >> initial_state[i];
    }
    for (int i = 1; i <= n; ++i) {
        cin >> target_state[i];
        if (target_state[i] == 0) {
            t0_nodes.push_back(i);
        } else {
            t1_nodes.push_back(i);
        }
    }

    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    // Each node can also take its own color, effectively a self-loop in the transition graph
    vector<vector<int>> adj_with_self_loops = adj;
    for(int i = 1; i <= n; ++i) {
        adj_with_self_loops[i].push_back(i);
    }

    vector<int> dist_to_t0 = bfs(n, adj, t0_nodes);
    vector<int> dist_to_t1 = bfs(n, adj, t1_nodes);

    vector<vector<int>> history;
    history.push_back(initial_state);

    vector<int> current_state = initial_state;

    if (current_state == target_state) {
        cout << 0 << endl;
        for (int i = 1; i <= n; ++i) {
            cout << current_state[i] << (i == n ? "" : " ");
        }
        cout << endl;
        return 0;
    }

    for (int k = 0; k < 20000; ++k) {
        vector<int> next_state(n + 1);
        
        for (int i = 1; i <= n; ++i) {
            bool can_be_0 = false;
            bool can_be_1 = false;
            for (int neighbor : adj_with_self_loops[i]) {
                if (current_state[neighbor] == 0) {
                    can_be_0 = true;
                } else {
                    can_be_1 = true;
                }
            }

            if (can_be_0 && !can_be_1) {
                next_state[i] = 0;
            } else if (!can_be_0 && can_be_1) {
                next_state[i] = 1;
            } else if (can_be_0 && can_be_1) {
                if (dist_to_t0[i] <= dist_to_t1[i]) {
                    next_state[i] = 0;
                } else {
                    next_state[i] = 1;
                }
            } else {
                next_state[i] = current_state[i];
            }
        }

        history.push_back(next_state);
        current_state = next_state;

        if (current_state == target_state) {
            break;
        }
    }

    cout << history.size() - 1 << endl;
    for (const auto& state : history) {
        for (int i = 1; i <= n; ++i) {
            cout << state[i] << (i == n ? "" : " ");
        }
        cout << endl;
    }

    return 0;
}