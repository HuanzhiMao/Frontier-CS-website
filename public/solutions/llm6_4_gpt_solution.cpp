#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> create_map(int N, int M, vector<int> A, vector<int> B) {
    vector<vector<pair<int,int>>> g(N + 1);
    for (int i = 0; i < M; ++i) {
        int u = A[i], v = B[i];
        g[u].push_back({v, i});
        g[v].push_back({u, i});
    }
    vector<char> used(M, 0);
    vector<int> seq;

    int start = 1;
    if (N > 1) {
        for (int i = 1; i <= N; ++i) {
            if (!g[i].empty()) { start = i; break; }
        }
    }

    function<void(int)> dfs = [&](int u) {
        for (auto &e : g[u]) {
            int v = e.first, id = e.second;
            if (!used[id]) {
                used[id] = 1;
                seq.push_back(v);
                dfs(v);
                seq.push_back(u);
            }
        }
    };

    if (N == 1) {
        seq.push_back(1);
    } else {
        seq.push_back(start);
        dfs(start);
    }

    int K = (int)seq.size();
    if (K == 0) {
        K = 1;
        seq = {1};
    }

    vector<vector<int>> C(K, vector<int>(K));
    for (int i = 0; i < K; ++i) {
        for (int j = 0; j < K; ++j) {
            C[i][j] = seq[j];
        }
    }
    return C;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        int N, M;
        cin >> N >> M;
        vector<int> A(M), B(M);
        for (int i = 0; i < M; ++i) cin >> A[i] >> B[i];

        auto C = create_map(N, M, A, B);
        int P = (int)C.size();
        cout << P << "\n";
        for (int i = 0; i < P; ++i) {
            if (i) cout << ' ';
            cout << C[i].size();
        }
        cout << "\n\n";
        for (int i = 0; i < P; ++i) {
            for (int j = 0; j < (int)C[i].size(); ++j) {
                if (j) cout << ' ';
                cout << C[i][j];
            }
            cout << "\n";
        }
    }
    return 0;
}