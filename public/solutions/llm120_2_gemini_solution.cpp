#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>

using namespace std;

// Function to perform a query and get the result.
// Vertices are 0-indexed in the program, but 1-indexed for interaction.
int query(int a, int b, int c) {
    cout << "? " << a + 1 << " " << b + 1 << " " << c + 1 << endl;
    int result;
    cin >> result;
    if (result == -1) exit(0); // Exit on error from interactor
    return result;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    const int n = 100;
    vector<vector<int>> adj(n, vector<int>(n, 0));

    int b1 = -1, b2 = -1, b3 = -1;
    int base_q = -1;

    // 1. Find a base of 3 vertices (b1, b2, b3)
    // A query result of 0 or 3 uniquely determines the edges between the three vertices.
    // We iterate through triples until such a result is found.
    // This search is exhaustive but breaks as soon as a base is found.
    // In practice, such a base is likely found very early.
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            for (int k = j + 1; k < n; ++k) {
                int res = query(i, j, k);
                if (res == 0 || res == 3) {
                    b1 = i;
                    b2 = j;
                    b3 = k;
                    base_q = res;
                    goto found_base;
                }
            }
        }
    }

found_base:
    // Set the edges for the base vertices.
    int edge_val = (base_q == 3);
    adj[b1][b2] = adj[b2][b1] = edge_val;
    adj[b1][b3] = adj[b3][b1] = edge_val;
    adj[b2][b3] = adj[b3][b2] = edge_val;

    vector<int> others;
    for (int i = 0; i < n; ++i) {
        if (i != b1 && i != b2 && i != b3) {
            others.push_back(i);
        }
    }

    // 2. Determine edges from the base to all other vertices.
    for (int i : others) {
        int q_b1_b2_i = query(b1, b2, i);
        int q_b1_b3_i = query(b1, b3, i);
        int q_b2_b3_i = query(b2, b3, i);

        int sum_b1_i_b2_i = q_b1_b2_i - adj[b1][b2];
        int sum_b1_i_b3_i = q_b1_b3_i - adj[b1][b3];
        int sum_b2_i_b3_i = q_b2_b3_i - adj[b2][b3];

        // We have a system of linear equations:
        // adj[b1][i] + adj[b2][i] = sum_b1_i_b2_i
        // adj[b1][i] + adj[b3][i] = sum_b1_i_b3_i
        // adj[b2][i] + adj[b3][i] = sum_b2_i_b3_i
        // Solving this gives:
        // 2*adj[b1][i] = sum_b1_i_b2_i + sum_b1_i_b3_i - sum_b2_i_b3_i
        
        adj[b1][i] = adj[i][b1] = (sum_b1_i_b2_i + sum_b1_i_b3_i - sum_b2_i_b3_i) / 2;
        adj[b2][i] = adj[i][b2] = sum_b1_i_b2_i - adj[b1][i];
        adj[b3][i] = adj[i][b3] = sum_b1_i_b3_i - adj[b1][i];
    }
    
    // 3. Determine all remaining edges between non-base vertices.
    for (size_t i = 0; i < others.size(); ++i) {
        for (size_t j = i + 1; j < others.size(); ++j) {
            int u = others[i];
            int v = others[j];
            int q_res = query(b1, u, v);
            adj[u][v] = adj[v][u] = q_res - adj[b1][u] - adj[b1][v];
        }
    }

    // Output the final adjacency matrix.
    cout << "!" << endl;
    for (int i = 0; i < n; ++i) {
        string row = "";
        for (int j = 0; j < n; ++j) {
            row += to_string(adj[i][j]);
        }
        cout << row << endl;
    }

    return 0;
}