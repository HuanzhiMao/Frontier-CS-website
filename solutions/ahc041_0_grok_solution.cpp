#include <bits/stdc++.h>
using namespace std;

int main() {
    int N, M, H;
    cin >> N >> M >> H;
    vector<int> A(N);
    for (int i = 0; i < N; i++) cin >> A[i];
    vector<vector<int>> G(N);
    for (int i = 0; i < M; i++) {
        int u, v;
        cin >> u >> v;
        G[u].push_back(v);
        G[v].push_back(u);
    }
    vector<int> X(N), Y(N);
    for (int i = 0; i < N; i++) cin >> X[i] >> Y[i];

    vector<vector<int>> dist(N, vector<int>(N, -1));
    for (int s = 0; s < N; s++) {
        queue<int> q;
        q.push(s);
        dist[s][s] = 0;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int nei : G[u]) {
                if (dist[s][nei] == -1) {
                    dist[s][nei] = dist[s][u] + 1;
                    q.push(nei);
                }
            }
        }
    }

    vector<vector<int>> ball(N);
    vector<vector<int>> coverers(N);
    for (int u = 0; u < N; u++) {
        for (int r = 0; r < N; r++) {
            if (dist[u][r] <= H && dist[u][r] >= 0) {
                ball[u].push_back(r);
                coverers[r].push_back(u);
            }
        }
    }

    vector<bool> covered(N, false);
    vector<int> roots;
    while (true) {
        bool all_covered = true;
        for (int i = 0; i < N; i++) {
            if (!covered[i]) {
                all_covered = false;
                break;
            }
        }
        if (all_covered) break;

        vector<int> cnt(N, 0);
        vector<long long> ben(N, 0);
        for (int i = 0; i < N; i++) {
            if (!covered[i]) {
                for (int r : ball[i]) {
                    cnt[r]++;
                    ben[r] += (long long)dist[i][r] * A[i];
                }
            }
        }

        int best_count = -1;
        long long best_ben = -1LL;
        int best_A = INT_MAX;
        int best_r = -1;
        for (int r = 0; r < N; r++) {
            int cc = cnt[r];
            long long bb = ben[r];
            if (cc > best_count) {
                best_count = cc;
                best_ben = bb;
                best_A = A[r];
                best_r = r;
            } else if (cc == best_count) {
                if (bb > best_ben || (bb == best_ben && A[r] < best_A)) {
                    best_ben = bb;
                    best_A = A[r];
                    best_r = r;
                }
            }
        }

        if (best_count <= 0) break;

        roots.push_back(best_r);
        for (int u : ball[best_r]) {
            covered[u] = true;
        }
    }

    vector<int> p(N, -1);
    vector<int> dep(N, -1);
    vector<bool> visited(N, false);
    queue<int> q;
    for (int r : roots) {
        p[r] = -1;
        dep[r] = 0;
        visited[r] = true;
        q.push(r);
    }
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : G[u]) {
            if (!visited[v]) {
                visited[v] = true;
                p[v] = u;
                dep[v] = dep[u] + 1;
                q.push(v);
            }
        }
    }

    for (int i = 0; i < N; i++) {
        cout << p[i];
        if (i < N - 1) cout << " ";
        else cout << "\n";
    }
    return 0;
}