#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <stack>
#include <map>

// Generates an Eulerian path in a complete directed graph of 10 vertices (0-9).
// The sequence of vertices in this path forms a de Bruijn sequence B(10, 2).
std::vector<int> generate_debruijn_path() {
    std::map<int, std::vector<int>> adj;
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            adj[i].push_back(j);
        }
        // Sorting neighbors ensures a deterministic sequence.
        // Reverse order is a common heuristic for de Bruijn sequence generation.
        sort(adj[i].rbegin(), adj[i].rend());
    }

    std::vector<int> path;
    std::stack<int> st;
    st.push(0); // Start the path at node 0

    while (!st.empty()) {
        int u = st.top();
        if (adj[u].empty()) {
            path.push_back(u);
            st.pop();
        } else {
            int v = adj[u].back();
            adj[u].pop_back();
            st.push(v);
        }
    }
    std::reverse(path.begin(), path.end());
    return path;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::vector<int> path = generate_debruijn_path();
    
    char grid[8][14];
    int path_idx = 0;

    // Fill the first 7 rows (98 cells) with the de Bruijn sequence in a snaking pattern.
    for (int r = 0; r < 7; ++r) {
        if (r % 2 == 0) { // Forward rows (0, 2, 4, 6)
            for (int c = 0; c < 14; ++c) {
                grid[r][c] = path[path_idx++] + '0';
            }
        } else { // Backward rows (1, 3, 5)
            for (int c = 13; c >= 0; --c) {
                grid[r][c] = path[path_idx++] + '0';
            }
        }
    }
    
    // Fill the last row with the remaining part of the sequence,
    // a full range of digits 0-9 for connectivity, and some padding.
    grid[7][0] = path[98] + '0';
    grid[7][1] = path[99] + '0';
    
    for (int i = 0; i < 10; ++i) {
        grid[7][2 + i] = i + '0';
    }
    
    grid[7][12] = '0';
    grid[7][13] = '0';

    // Print the generated grid.
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 14; ++c) {
            std::cout << grid[r][c];
        }
        std::cout << '\n';
    }

    return 0;
}