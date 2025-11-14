#include <bits/stdc++.h>
using namespace std;

static const int N = 100;

map<long long, int> memo;

long long makeKey(int a, int b, int c) {
    int x = a, y = b, z = c;
    if (x > y) swap(x, y);
    if (y > z) swap(y, z);
    if (x > y) swap(x, y);
    // pack into key; base 1000 is safe since vertices <= 100
    return (long long)x * 1000000LL + (long long)y * 1000LL + (long long)z;
}

int ask(int a, int b, int c) {
    long long key = makeKey(a, b, c);
    auto it = memo.find(key);
    if (it != memo.end()) return it->second;
    cout << "? " << a << " " << b << " " << c << endl;
    cout.flush();
    int ans;
    if (!(cin >> ans)) exit(0);
    if (ans < 0) exit(0);
    memo[key] = ans;
    return ans;
}

// Solve linear system (possibly overdetermined) using Gauss-Jordan elimination
// Returns pair(success, solution vector)
pair<bool, vector<double>> solveLinear(vector<vector<double>> a, int rows, int cols) {
    const double EPS = 1e-9;
    vector<int> where(cols, -1);
    int row = 0;
    for (int col = 0; col < cols && row < rows; ++col) {
        int sel = row;
        for (int i = row; i < rows; ++i) {
            if (fabs(a[i][col]) > fabs(a[sel][col])) sel = i;
        }
        if (fabs(a[sel][col]) < EPS) continue;
        swap(a[sel], a[row]);
        where[col] = row;
        double div = a[row][col];
        for (int j = col; j <= cols; ++j) a[row][j] /= div;
        for (int i = 0; i < rows; ++i) {
            if (i == row) continue;
            double factor = a[i][col];
            if (fabs(factor) < EPS) continue;
            for (int j = col; j <= cols; ++j) a[i][j] -= factor * a[row][j];
        }
        ++row;
    }
    vector<double> ans(cols, 0.0);
    for (int i = 0; i < cols; ++i) {
        if (where[i] != -1) ans[i] = a[where[i]][cols];
    }
    // Check consistency
    for (int i = 0; i < rows; ++i) {
        double sum = 0.0;
        for (int j = 0; j < cols; ++j) sum += a[i][j] * ans[j];
        if (fabs(sum - a[i][cols]) > 1e-6) {
            return {false, {}};
        }
    }
    // Success (possibly with free vars set to 0)
    return {true, ans};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const int n = N;
    vector<vector<int>> adj(n + 1, vector<int>(n + 1, 0));

    auto try_with_K = [&](int K, vector<int>& anchors, vector<vector<int>>& anchorEdge) -> bool {
        anchors.clear();
        for (int i = 1; i <= K; ++i) anchors.push_back(i);

        // Build unknowns mapping for anchor pairs
        vector<pair<int,int>> pairs;
        map<pair<int,int>, int> pid;
        int idx = 0;
        for (int i = 0; i < K; ++i) {
            for (int j = i + 1; j < K; ++j) {
                pairs.push_back({anchors[i], anchors[j]});
                pid[{anchors[i], anchors[j]}] = idx++;
            }
        }
        int E = (int)pairs.size(); // C(K,2)

        // Build equations for all anchor triples
        vector<array<int,3>> triples;
        for (int i = 0; i < K; ++i) {
            for (int j = i + 1; j < K; ++j) {
                for (int k = j + 1; k < K; ++k) {
                    triples.push_back({anchors[i], anchors[j], anchors[k]});
                }
            }
        }
        int R = (int)triples.size(); // C(K,3)

        vector<vector<double>> A(R, vector<double>(E + 1, 0.0));
        for (int r = 0; r < R; ++r) {
            int a = triples[r][0], b = triples[r][1], c = triples[r][2];
            int x = min(a,b), y = max(a,b);
            int id1 = pid[{x, y}];
            x = min(a,c); y = max(a,c);
            int id2 = pid[{x, y}];
            x = min(b,c); y = max(b,c);
            int id3 = pid[{x, y}];
            A[r][id1] = 1.0;
            A[r][id2] = 1.0;
            A[r][id3] = 1.0;
            int val = ask(a, b, c);
            A[r][E] = val;
        }

        auto res = solveLinear(A, R, E);
        if (!res.first) return false;
        vector<double> sol = res.second;

        anchorEdge.assign(n + 1, vector<int>(n + 1, -1));
        for (int e = 0; e < E; ++e) {
            int u = pairs[e].first, v = pairs[e].second;
            int val = (int)llround(sol[e]);
            if (val < 0) val = 0;
            if (val > 1) val = 1;
            anchorEdge[u][v] = anchorEdge[v][u] = val;
            adj[u][v] = adj[v][u] = val;
        }

        // Verify consistency
        for (int r = 0; r < R; ++r) {
            int a = triples[r][0], b = triples[r][1], c = triples[r][2];
            int sum = anchorEdge[a][b] + anchorEdge[a][c] + anchorEdge[b][c];
            int val = (int)A[r][E];
            if (sum != val) {
                return false;
            }
        }
        return true;
    };

    vector<int> anchors;
    vector<vector<int>> anchorEdge;
    int K = 5;
    if (!try_with_K(K, anchors, anchorEdge)) {
        K = 6;
        if (!try_with_K(K, anchors, anchorEdge)) {
            // As a last resort, try with K=7
            K = 7;
            if (!try_with_K(K, anchors, anchorEdge)) {
                // If still failing, exit
                return 0;
            }
        }
    }

    // For each non-anchor vertex v, determine edges to each anchor using 2K-3 queries:
    // Query pairs: (1,2,v), (1,3,v), (2,3,v), and for i=4..K: (1,i,v), (2,i,v)
    // Then compute:
    // y1 = (b12 + b13 - b23)/2
    // y2 = (b12 + b23 - b13)/2
    // y3 = (b13 + b23 - b12)/2
    // yi = (b1i + b2i - b12)/2
    auto getAE = [&](int u, int v)->int{ // anchor edge between anchors u and v
        if (u > v) swap(u, v);
        return anchorEdge[u][v];
    };

    for (int v = K + 1; v <= n; ++v) {
        int a1 = 1, a2 = 2, a3 = 3;
        int m12v = ask(a1, a2, v);
        int m13v = ask(a1, a3, v);
        int m23v = ask(a2, a3, v);
        int b12 = m12v - getAE(a1, a2);
        int b13 = m13v - getAE(a1, a3);
        int b23 = m23v - getAE(a2, a3);
        int y1 = (b12 + b13 - b23) / 2;
        int y2 = (b12 + b23 - b13) / 2;
        int y3 = (b13 + b23 - b12) / 2;
        y1 = max(0, min(1, y1));
        y2 = max(0, min(1, y2));
        y3 = max(0, min(1, y3));
        adj[a1][v] = adj[v][a1] = y1;
        adj[a2][v] = adj[v][a2] = y2;
        adj[a3][v] = adj[v][a3] = y3;
        for (int ai = 4; ai <= K; ++ai) {
            int m1iv = ask(a1, ai, v);
            int m2iv = ask(a2, ai, v);
            int b1i = m1iv - getAE(a1, ai);
            int b2i = m2iv - getAE(a2, ai);
            int yi = (b1i + b2i - b12) / 2;
            yi = max(0, min(1, yi));
            int aa = ai;
            adj[aa][v] = adj[v][aa] = yi;
        }
    }

    // For all pairs of non-anchors, query m(1,u,v) to get e_uv = m - e(1,u) - e(1,v)
    for (int u = K + 1; u <= n; ++u) {
        for (int v = u + 1; v <= n; ++v) {
            int m1uv = ask(1, u, v);
            int e1u = adj[1][u];
            int e1v = adj[1][v];
            int e_uv = m1uv - e1u - e1v;
            e_uv = max(0, min(1, e_uv));
            adj[u][v] = adj[v][u] = e_uv;
        }
    }

    // Ensure diagonal zero and symmetry
    for (int i = 1; i <= n; ++i) {
        adj[i][i] = 0;
        for (int j = 1; j <= n; ++j) {
            if (i != j) {
                if (adj[i][j] != adj[j][i]) {
                    int v = (adj[i][j] + adj[j][i]) / 2;
                    adj[i][j] = adj[j][i] = v;
                }
            }
        }
    }

    // Output the final graph
    cout << "!" << endl;
    for (int i = 1; i <= n; ++i) {
        string s;
        s.reserve(n);
        for (int j = 1; j <= n; ++j) s.push_back(adj[i][j] ? '1' : '0');
        cout << s << endl;
    }
    cout.flush();
    return 0;
}