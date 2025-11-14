#include <iostream>
#include <vector>
#include <numeric>

// Using a direct implementation of the greedy strategy described.
// At each step, every node that is not at its target color checks if any of its neighbors
// (in the previous state) has the target color. If so, it flips to the target color.
// This process is repeated until the target state is reached.
// The "simultaneous" update is handled by computing the next state based entirely on the current state.

void print_state(int n, const std::vector<int>& state) {
    for (int i = 0; i < n; ++i) {
        std::cout << state[i] << (i == n - 1 ? "" : " ");
    }
    std::cout << "\n";
}

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

    if (current_state == target_state) {
        std::cout << 0 << "\n";
        print_state(n, initial_state);
        return 0;
    }

    // The problem guarantees a solution exists within 20,000 transformations.
    // The greedy approach should be much faster.
    for (int k = 0; k < 20001; ++k) {
        std::vector<int> next_state = current_state;

        for (int i = 0; i < n; ++i) {
            if (current_state[i] == target_state[i]) {
                continue;
            }

            bool can_flip = false;
            // A node can change its color to the color of one of its neighbors.
            // Since the node is not at its target color, it cannot source the
            // target color from itself. It must come from a neighbor.
            for (int neighbor : adj[i]) {
                if (current_state[neighbor] == target_state[i]) {
                    can_flip = true;
                    break;
                }
            }
            
            if (can_flip) {
                next_state[i] = target_state[i];
            }
        }
        
        current_state = next_state;
        history.push_back(current_state);

        if (current_state == target_state) {
            break;
        }
    }

    std::cout << history.size() - 1 << "\n";
    for (const auto& state : history) {
        print_state(n, state);
    }

    return 0;
}