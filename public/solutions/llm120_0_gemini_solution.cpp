#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>

int adj[101][101] = {0};

int query(int a, int b, int c) {
    std::cout << "? " << a << " " << b << " " << c << std::endl;
    int response;
    std::cin >> response;
    if (response == -1) {
        exit(0);
    }
    return response;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int p[3];
    bool base_found = false;
    for (int i = 1; i <= 100 && !base_found; ++i) {
        for (int j = i + 1; j <= 100 && !base_found; ++j) {
            for (int k = j + 1; k <= 100 && !base_found; ++k) {
                int val = query(i, j, k);
                if (val == 0 || val == 3) {
                    p[0] = i;
                    p[1] = j;
                    p[2] = k;
                    int edge_val = val / 3;
                    adj[p[0]][p[1]] = adj[p[1]][p[0]] = edge_val;
                    adj[p[0]][p[2]] = adj[p[2]][p[0]] = edge_val;
                    adj[p[1]][p[2]] = adj[p[2]][p[1]] = edge_val;
                    base_found = true;
                }
            }
        }
    }

    std::vector<int> others;
    for (int i = 1; i <= 100; ++i) {
        if (i != p[0] && i != p[1] && i != p[2]) {
            others.push_back(i);
        }
    }

    int val_base = adj[p[0]][p[1]] + adj[p[0]][p[2]] + adj[p[1]][p[2]];
    for (int k : others) {
        int q01k = query(p[0], p[1], k);
        int q02k = query(p[0], p[2], k);
        int q12k = query(p[1], p[2], k);

        int sum_edges = (q01k + q02k + q12k - val_base) / 2;

        adj[p[0]][k] = adj[k][p[0]] = sum_edges - (q12k - adj[p[1]][p[2]]);
        adj[p[1]][k] = adj[k][p[1]] = sum_edges - (q02k - adj[p[0]][p[2]]);
        adj[p[2]][k] = adj[k][p[2]] = sum_edges - (q01k - adj[p[0]][p[1]]);
    }

    for (size_t i = 0; i < others.size(); ++i) {
        for (size_t j = i + 1; j < others.size(); ++j) {
            int u = others[i];
            int v = others[j];
            int q_val = query(p[0], u, v);
            adj[u][v] = adj[v][u] = q_val - adj[p[0]][u] - adj[p[0]][v];
        }
    }

    std::cout << "!" << std::endl;
    for (int i = 1; i <= 100; ++i) {
        for (int j = 1; j <= 100; ++j) {
            std::cout << adj[i][j];
        }
        std::cout << std::endl;
    }

    return 0;
}