#include <iostream>
#include <vector>
#include <string>
#include <numeric>

using namespace std;

const int N = 100;
int adj[N + 1][N + 1];
int R1[N + 1][N + 1];
int x[N + 1];

int query(int a, int b, int c) {
    cout << "? " << a << " " << b << " " << c << endl;
    int response;
    cin >> response;
    return response;
}

void solve() {
    // Step 1: Query all q(1,i,j) to establish relations
    for (int i = 2; i <= N; ++i) {
        for (int j = i + 1; j <= N; ++j) {
            R1[i][j] = R1[j][i] = query(1, i, j);
        }
    }

    // Step 2: Determine x_i = E(1,i) for i=2..N
    auto get_S = [&](int i, int j, int k) {
        int q_ijk = query(i, j, k);
        return (R1[i][j] + R1[j][k] + R1[k][i] - q_ijk) / 2;
    };

    // 2a: Find x_2, x_3, x_4, x_5 using a small system of equations
    int S234 = get_S(2, 3, 4);
    int S235 = get_S(2, 3, 5);
    int S245 = get_S(2, 4, 5);
    int S345 = get_S(3, 4, 5);
    
    int T = (S234 + S235 + S245 + S345) / 3;

    x[2] = T - S345;
    x[3] = T - S245;
    x[4] = T - S235;
    x[5] = T - S234;

    // 2b: Find remaining x_i using x_2 and x_3 as base
    for (int i = 6; i <= N; ++i) {
        int S23i = get_S(2, 3, i);
        x[i] = S23i - (x[2] + x[3]);
    }

    // Step 3: Reconstruct the full adjacency matrix
    // Edges connected to vertex 1
    for (int i = 2; i <= N; ++i) {
        adj[1][i] = adj[i][1] = x[i];
    }
    // All other edges
    for (int i = 2; i <= N; ++i) {
        for (int j = i + 1; j <= N; ++j) {
            adj[i][j] = adj[j][i] = R1[i][j] - x[i] - x[j];
        }
    }

    cout << "!" << endl;
    for (int i = 1; i <= N; ++i) {
        string row = "";
        for (int j = 1; j <= N; ++j) {
            row += to_string(adj[i][j]);
        }
        cout << row << endl;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}