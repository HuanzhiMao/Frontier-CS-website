#include <iostream>
#include <vector>
#include <queue>
#include <numeric>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n, m;
    std::cin >> n >> m;

    std::vector<int> initial_state(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> initial_state[i];
    }

    std::vector<int> target_state(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> target_state[i];
    }

    std::vector<std::vector<int>> adj(n);
    for (int i = 0; i < m; ++i) {
        int u, v;
        std::cin >> u >> v;
        --u; --v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    std::vector<std::vector<int>> history;
    history.push_back(initial_state);

    std::vector<int> current_state = initial_state;

    if (current_state != target_state) {
        while (true) {
            std::vector<int> next_state(n);
            bool changed = false;
            for (int i = 0; i < n; ++i) {
                bool target_color_available = false;
                
                // Check self for target color
                if (current_state[i] == target_state[i]) {
                    target_color_available = true;
                }
                
                // Check neighbors for target color
                if (!target_color_available) {
                    for (int neighbor : adj[i]) {
                        if (current_state[neighbor] == target_state[i]) {
                            target_color_available = true;
                            break;
                        }
                    }
                }

                if (target_color_available) {
                    next_state[i] = target_state[i];
                } else {
                    next_state[i] = current_state[i];
                }
            }
            
            current_state = next_state;
            history.push_back(current_state);
            
            if (current_state == target_state) {
                break;
            }
        }
    }

    std::cout << history.size() - 1 << std::endl;
    for (const auto& state : history) {
        for (int i = 0; i < n; ++i) {
            std::cout << state[i] << (i == n - 1 ? "" : " ");
        }
        std::cout << std::endl;
    }

    return 0;
}