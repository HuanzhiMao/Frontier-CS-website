#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

const int INF = 1e9;

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n, m;
    std::cin >> n >> m;

    std::vector<int> initial_state(n);
    std::vector<int> target_state(n);
    std::vector<int> q0, q1;

    for (int i = 0; i < n; ++i) {
        std::cin >> initial_state[i];
        if (initial_state[i] == 0) {
            q0.push_back(i);
        } else {
            q1.push_back(i);
        }
    }

    for (int i = 0; i < n; ++i) {
        std::cin >> target_state[i];
    }

    std::vector<std::vector<int>> adj_no_self(n);
    for (int i = 0; i < m; ++i) {
        int u, v;
        std::cin >> u >> v;
        --u; --v;
        adj_no_self[u].push_back(v);
        adj_no_self[v].push_back(u);
    }
    
    std::vector<std::vector<int>> adj(n);
    for(int i = 0; i < n; ++i) {
        adj[i] = adj_no_self[i];
        adj[i].push_back(i);
    }
    
    auto bfs_multi_source = [&](const std::vector<int>& sources) {
        std::vector<int> d(n, INF);
        std::queue<int> q;
        for (int s : sources) {
            if (s < n) {
                d[s] = 0;
                q.push(s);
            }
        }
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : adj_no_self[u]) {
                if (d[v] == INF) {
                    d[v] = d[u] + 1;
                    q.push(v);
                }
            }
        }
        return d;
    };

    std::vector<int> d0 = bfs_multi_source(q0);
    std::vector<int> d1 = bfs_multi_source(q1);
    
    std::vector<std::vector<int>> d_to_color(n, std::vector<int>(2));
    for (int i = 0; i < n; ++i) {
        d_to_color[i][0] = d0[i];
        d_to_color[i][1] = d1[i];
    }
    
    int k = 0;
    for (int i = 0; i < n; ++i) {
        if (d_to_color[i][target_state[i]] != INF) {
            k = std::max(k, d_to_color[i][target_state[i]]);
        }
    }

    std::vector<std::vector<int>> states(k + 1, std::vector<int>(n));
    states[k] = target_state;

    for (int t = k; t >= 1; --t) {
        std::vector<int>& next_state = states[t];
        std::vector<int>& current_state = states[t - 1];
        
        std::fill(current_state.begin(), current_state.end(), -1);
        
        std::queue<std::pair<int, int>> q_prop;
        std::vector<std::vector<bool>> possible(n, std::vector<bool>(2, true));

        for (int i = 0; i < n; ++i) {
            if (d_to_color[i][0] > t - 1) possible[i][0] = false;
            if (d_to_color[i][1] > t - 1) possible[i][1] = false;

            if (!possible[i][0] && possible[i][1]) q_prop.push({i, 1});
            if (possible[i][0] && !possible[i][1]) q_prop.push({i, 0});
        }

        std::vector<std::vector<int>> supporters(n);
        std::vector<int> supporter_count(n);

        for(int v = 0; v < n; ++v) {
            int needed_color = next_state[v];
            for(int w : adj[v]) {
                if(possible[w][needed_color]) {
                    supporters[v].push_back(w);
                }
            }
            supporter_count[v] = supporters[v].size();
            if(supporter_count[v] == 1) {
                q_prop.push({supporters[v][0], needed_color});
            }
        }

        while(!q_prop.empty()){
            auto p = q_prop.front();
            q_prop.pop();
            int u = p.first;
            int c = p.second;
            
            if (current_state[u] != -1 && current_state[u] != c) continue; 
            if (current_state[u] == c) continue;
            if (!possible[u][c]) continue;

            current_state[u] = c;
            possible[u][1 - c] = false;

            for (int z : adj_no_self[u]) {
                if (c != next_state[z]) {
                    supporter_count[z]--;
                    if (supporter_count[z] == 1) {
                        for (int w : supporters[z]) {
                            if (current_state[w] == -1 && possible[w][next_state[z]]) {
                                q_prop.push({w, next_state[z]});
                                break;
                            }
                        }
                    }
                }
            }
        }
        
        for(int i = 0; i < n; ++i) {
            if(current_state[i] == -1) {
                if(possible[i][0]) {
                    current_state[i] = 0;
                } else {
                    current_state[i] = 1;
                }
            }
        }
    }
    
    std::cout << k << "\n";
    std::cout << initial_state[0];
    for (int i = 1; i < n; ++i) {
        std::cout << " " << initial_state[i];
    }
    std::cout << "\n";

    for (int t = 1; t <= k; ++t) {
        std::cout << states[t][0];
        for (int i = 1; i < n; ++i) {
            std::cout << " " << states[t][i];
        }
        std::cout << "\n";
    }

    return 0;
}