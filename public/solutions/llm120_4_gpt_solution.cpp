#include <bits/stdc++.h>
using namespace std;

static const int N = 100;

int ask(int a, int b, int c) {
    cout << "? " << a << " " << b << " " << c << endl;
    cout.flush();
    int x;
    if (!(cin >> x)) exit(0);
    if (x < 0) exit(0);
    return x;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Arrays to store query results
    int S12[N + 1] = {0};                 // S(i,1,2) for i >= 3
    int S1[N + 1][N + 1];                 // S(i,j,1) for i,j >=3
    int S2[N + 1][N + 1];                 // S(i,j,2) for i,j >=3
    memset(S1, 0, sizeof(S1));
    memset(S2, 0, sizeof(S2));

    // Query S(i,1,2) for i = 3..N
    for (int i = 3; i <= N; ++i) {
        S12[i] = ask(i, 1, 2);
    }

    // Query S(i,j,1) and S(i,j,2) for all 3 <= i < j <= N
    for (int i = 3; i <= N; ++i) {
        for (int j = i + 1; j <= N; ++j) {
            S1[i][j] = ask(i, j, 1);
            S1[j][i] = S1[i][j];
            S2[i][j] = ask(i, j, 2);
            S2[j][i] = S2[i][j];
        }
    }

    // Determine e12
    int e12 = -1;
    for (int i = 3; i <= N && e12 == -1; ++i) {
        for (int j = i + 1; j <= N && e12 == -1; ++j) {
            int T = S1[i][j] + S2[i][j] - S12[i] - S12[j]; // = 2(e_ij - e_12)
            if (T == 2) e12 = 0; // e_ij=1, so e_12 must be 0
            else if (T == -2) e12 = 1; // e_ij=0, so e_12 must be 1
        }
    }
    if (e12 == -1) {
        int a = 3, b = 4, c = 5;
        int Tab = S1[a][b] + S2[a][b] - S12[a] - S12[b];
        int Tac = S1[a][c] + S2[a][c] - S12[a] - S12[c];
        int Tbc = S1[b][c] + S2[b][c] - S12[b] - S12[c];
        int Sabc = ask(a, b, c); // = e_ab + e_ac + e_bc
        // e12 = (Sabc - (Tab + Tac + Tbc)/2) / 3
        int sumTdiv2 = (Tab + Tac + Tbc) / 2;
        e12 = (Sabc - sumTdiv2) / 3;
        if (e12 < 0) e12 = 0;
        if (e12 > 1) e12 = 1;
    }

    // Compute s_i = u_i + v_i and d_i = u_i - v_i for i >= 3
    vector<int> s(N + 1, 0), d(N + 1, 0), u(N + 1, 0), v(N + 1, 0);
    for (int i = 3; i <= N; ++i) {
        s[i] = S12[i] - e12; // s_i in {0,1,2}
    }

    // Compute d using A_ij = S1(i,j,1) - S2(i,j,2) = d_i + d_j
    // Use vertices 3,4,5 as base
    int A34 = S1[3][4] - S2[3][4];
    int A35 = S1[3][5] - S2[3][5];
    int A45 = S1[4][5] - S2[4][5];
    d[3] = (A34 + A35 - A45) / 2;
    for (int k = 4; k <= N; ++k) {
        d[k] = (S1[3][k] - S2[3][k]) - d[3];
    }

    // Compute u_i and v_i
    for (int i = 3; i <= N; ++i) {
        u[i] = (s[i] + d[i]) / 2; // u_i in {0,1}
        v[i] = s[i] - u[i];       // v_i in {0,1}
    }

    // Build adjacency matrix
    vector<string> ans(N + 1, string(N + 1, '0'));
    auto set_edge = [&](int x, int y, int val) {
        ans[x][y] = ans[y][x] = (val ? '1' : '0');
    };

    // Edges among 3..N: e_ij = S1(i,j,1) - u_i - u_j
    for (int i = 3; i <= N; ++i) {
        for (int j = i + 1; j <= N; ++j) {
            int eij = S1[i][j] - u[i] - u[j];
            if (eij < 0) eij = 0;
            if (eij > 1) eij = 1;
            set_edge(i, j, eij);
        }
    }

    // Edges involving 1 and 2
    set_edge(1, 2, e12);
    for (int i = 3; i <= N; ++i) {
        set_edge(1, i, u[i]);
        set_edge(2, i, v[i]);
    }

    // No self-loops
    for (int i = 1; i <= N; ++i) {
        ans[i][i] = '0';
    }

    // Output the result
    cout << "!" << endl;
    for (int i = 1; i <= N; ++i) {
        for (int j = 1; j <= N; ++j) cout << ans[i][j];
        cout << endl;
    }
    cout.flush();

    return 0;
}