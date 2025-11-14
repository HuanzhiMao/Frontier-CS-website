#include <bits/stdc++.h>
using namespace std;

struct Frac {
    long long p, q; // p/q
    Frac(long long _p = 0, long long _q = 1) {
        p = _p; q = _q;
        normalize();
    }
    static long long gcdll(long long a, long long b) {
        if (a < 0) a = -a;
        if (b < 0) b = -b;
        while (b) { long long t = a % b; a = b; b = t; }
        return a;
    }
    void normalize() {
        if (q < 0) { p = -p; q = -q; }
        if (q == 0) return;
        long long g = gcdll(p, q);
        if (g) { p /= g; q /= g; }
    }
    bool isZero() const { return p == 0; }
};

Frac add(const Frac& a, const Frac& b) {
    __int128 num = (__int128)a.p * b.q + (__int128)b.p * a.q;
    __int128 den = (__int128)a.q * b.q;
    long long numll = (long long)num;
    long long denll = (long long)den;
    return Frac(numll, denll);
}
Frac subf(const Frac& a, const Frac& b) {
    __int128 num = (__int128)a.p * b.q - (__int128)b.p * a.q;
    __int128 den = (__int128)a.q * b.q;
    long long numll = (long long)num;
    long long denll = (long long)den;
    return Frac(numll, denll);
}
Frac mul(const Frac& a, const Frac& b) {
    __int128 num = (__int128)a.p * b.p;
    __int128 den = (__int128)a.q * b.q;
    long long numll = (long long)num;
    long long denll = (long long)den;
    return Frac(numll, denll);
}
Frac divf(const Frac& a, const Frac& b) {
    // assume b != 0
    __int128 num = (__int128)a.p * b.q;
    __int128 den = (__int128)a.q * b.p;
    long long numll = (long long)num;
    long long denll = (long long)den;
    return Frac(numll, denll);
}

int ask(int a, int b, int c) {
    cout << "? " << a << " " << b << " " << c << endl;
    cout.flush();
    int x;
    if (!(cin >> x)) {
        exit(0);
    }
    return x;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const int N = 100;
    vector<vector<int>> A(N + 1, vector<int>(N + 1, 0));

    // Anchors
    int a = 1, b = 2, c = 3, d = 4;
    // Query triple sums among anchors
    int E1 = ask(a, b, c);
    int E2 = ask(a, b, d);
    int E3 = ask(a, c, d);
    int E4 = ask(b, c, d);

    // Choose t = 5
    int t = 5;
    int F12 = ask(a, b, t);
    int F13 = ask(a, c, t);
    int F14 = ask(a, d, t);
    int F23 = ask(b, c, t);
    int F24 = ask(b, d, t);
    int F34 = ask(c, d, t);

    // Solve the 10x10 linear system to get x12..x34 and y1..y4 for t
    // Unknowns order: [x12,x13,x14,x23,x24,x34,y1,y2,y3,y4]
    vector<vector<Frac>> M(10, vector<Frac>(11, Frac(0,1)));
    // Eq 0: x12 + x13 + x23 = E1
    M[0][0] = Frac(1); M[0][1] = Frac(1); M[0][3] = Frac(1); M[0][10] = Frac(E1);
    // Eq 1: x12 + x14 + x24 = E2
    M[1][0] = Frac(1); M[1][2] = Frac(1); M[1][4] = Frac(1); M[1][10] = Frac(E2);
    // Eq 2: x13 + x14 + x34 = E3
    M[2][1] = Frac(1); M[2][2] = Frac(1); M[2][5] = Frac(1); M[2][10] = Frac(E3);
    // Eq 3: x23 + x24 + x34 = E4
    M[3][3] = Frac(1); M[3][4] = Frac(1); M[3][5] = Frac(1); M[3][10] = Frac(E4);
    // Eq 4: x12 + y1 + y2 = F12
    M[4][0] = Frac(1); M[4][6] = Frac(1); M[4][7] = Frac(1); M[4][10] = Frac(F12);
    // Eq 5: x13 + y1 + y3 = F13
    M[5][1] = Frac(1); M[5][6] = Frac(1); M[5][8] = Frac(1); M[5][10] = Frac(F13);
    // Eq 6: x14 + y1 + y4 = F14
    M[6][2] = Frac(1); M[6][6] = Frac(1); M[6][9] = Frac(1); M[6][10] = Frac(F14);
    // Eq 7: x23 + y2 + y3 = F23
    M[7][3] = Frac(1); M[7][7] = Frac(1); M[7][8] = Frac(1); M[7][10] = Frac(F23);
    // Eq 8: x24 + y2 + y4 = F24
    M[8][4] = Frac(1); M[8][7] = Frac(1); M[8][9] = Frac(1); M[8][10] = Frac(F24);
    // Eq 9: x34 + y3 + y4 = F34
    M[9][5] = Frac(1); M[9][8] = Frac(1); M[9][9] = Frac(1); M[9][10] = Frac(F34);

    // Gauss-Jordan elimination
    int R = 10, C = 10;
    vector<int> where(C, -1);
    int row = 0;
    for (int col = 0; col < C && row < R; ++col) {
        int sel = -1;
        for (int i = row; i < R; ++i) {
            if (!M[i][col].isZero()) { sel = i; break; }
        }
        if (sel == -1) continue;
        swap(M[row], M[sel]);
        where[col] = row;

        // Normalize pivot row
        Frac piv = M[row][col];
        for (int j = col; j <= C; ++j) {
            M[row][j] = divf(M[row][j], piv);
        }
        // Eliminate in other rows
        for (int i = 0; i < R; ++i) {
            if (i == row) continue;
            if (M[i][col].isZero()) continue;
            Frac factor = M[i][col];
            for (int j = col; j <= C; ++j) {
                M[i][j] = subf(M[i][j], mul(factor, M[row][j]));
            }
        }
        ++row;
    }

    vector<Frac> sol(C, Frac(0));
    for (int i = 0; i < C; ++i) {
        if (where[i] != -1) sol[i] = M[where[i]][C];
    }

    auto toInt = [](const Frac& f)->int {
        // f should be integer
        long long p = f.p, q = f.q;
        if (q == 0) return 0;
        if (p % q == 0) return (int)(p / q);
        // Should not happen; round as fallback
        long double v = (long double)p / (long double)q;
        int r = (int)llround(v);
        return r;
    };

    int x12 = toInt(sol[0]);
    int x13 = toInt(sol[1]);
    int x14 = toInt(sol[2]);
    int x23 = toInt(sol[3]);
    int x24 = toInt(sol[4]);
    int x34 = toInt(sol[5]);
    int y1_5 = toInt(sol[6]);
    int y2_5 = toInt(sol[7]);
    int y3_5 = toInt(sol[8]);
    int y4_5 = toInt(sol[9]);

    // Set anchors edges
    A[a][b] = A[b][a] = x12;
    A[a][c] = A[c][a] = x13;
    A[a][d] = A[d][a] = x14;
    A[b][c] = A[c][b] = x23;
    A[b][d] = A[d][b] = x24;
    A[c][d] = A[d][c] = x34;

    // Set edges from t=5 to anchors
    A[a][t] = A[t][a] = y1_5;
    A[b][t] = A[t][b] = y2_5;
    A[c][t] = A[t][c] = y3_5;
    A[d][t] = A[t][d] = y4_5;

    // For each vertex s = 6..100, query and compute y's to anchors
    for (int s = 6; s <= N; ++s) {
        int f12 = ask(a, b, s);
        int f13 = ask(a, c, s);
        int f23 = ask(b, c, s);
        int f14s = ask(a, d, s);

        // y1(s) = (f12 + f13 - f23 - x12 - x13 + x23)/2
        int y1s = (f12 + f13 - f23 - x12 - x13 + x23) / 2;
        int y2s = (f12 + f23 - f13 - x12 - x23 + x13) / 2;
        int y3s = (f13 + f23 - f12 - x13 - x23 + x12) / 2;
        int y4s = f14s - x14 - y1s;

        A[a][s] = A[s][a] = y1s;
        A[b][s] = A[s][b] = y2s;
        A[c][s] = A[s][c] = y3s;
        A[d][s] = A[s][d] = y4s;
    }

    // Now compute edges among vertices 5..100 via queries with anchor a=1
    for (int i = 5; i <= N; ++i) {
        for (int j = i + 1; j <= N; ++j) {
            int val = ask(a, i, j);
            int eij = val - A[a][i] - A[a][j];
            A[i][j] = A[j][i] = eij;
        }
    }

    // Output adjacency matrix
    cout << "!" << endl;
    for (int i = 1; i <= N; ++i) {
        string s(N, '0');
        for (int j = 1; j <= N; ++j) {
            if (i == j) s[j - 1] = '0';
            else s[j - 1] = (A[i][j] ? '1' : '0');
        }
        cout << s << endl;
    }
    cout.flush();
    return 0;
}