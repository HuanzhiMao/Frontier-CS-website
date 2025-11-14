#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int si, sj, ti, tj;
    double p;
    if (!(cin >> si >> sj >> ti >> tj >> p)) {
        return 0;
    }

    const int N = 20;
    vector<string> h(N);
    for (int i = 0; i < N; i++) {
        cin >> h[i];
    }
    vector<string> v(N - 1);
    for (int i = 0; i < N - 1; i++) {
        cin >> v[i];
    }

    auto id = [&](int r, int c) { return r * N + c; };
    auto rc = [&](int idx) { return pair<int,int>(idx / N, idx % N); };

    int src = id(si, sj);
    int dst = id(ti, tj);

    vector<int> dist(N * N, -1), par(N * N, -1);
    vector<char> pmove(N * N, '?');
    queue<int> q;
    dist[src] = 0;
    q.push(src);

    while (!q.empty()) {
        int u = q.front(); q.pop();
        auto [i, j] = rc(u);

        // Up
        if (i > 0 && v[i - 1][j] == '0') {
            int w = id(i - 1, j);
            if (dist[w] == -1) {
                dist[w] = dist[u] + 1;
                par[w] = u;
                pmove[w] = 'U';
                q.push(w);
            }
        }
        // Down
        if (i + 1 < N && v[i][j] == '0') {
            int w = id(i + 1, j);
            if (dist[w] == -1) {
                dist[w] = dist[u] + 1;
                par[w] = u;
                pmove[w] = 'D';
                q.push(w);
            }
        }
        // Left
        if (j > 0 && h[i][j - 1] == '0') {
            int w = id(i, j - 1);
            if (dist[w] == -1) {
                dist[w] = dist[u] + 1;
                par[w] = u;
                pmove[w] = 'L';
                q.push(w);
            }
        }
        // Right
        if (j + 1 < N && h[i][j] == '0') {
            int w = id(i, j + 1);
            if (dist[w] == -1) {
                dist[w] = dist[u] + 1;
                par[w] = u;
                pmove[w] = 'R';
                q.push(w);
            }
        }
    }

    string ans;
    if (src != dst && dist[dst] != -1) {
        vector<char> rev;
        int cur = dst;
        while (cur != src) {
            rev.push_back(pmove[cur]);
            cur = par[cur];
        }
        reverse(rev.begin(), rev.end());
        ans.assign(rev.begin(), rev.end());
    } else {
        ans = "";
    }

    // Ensure length <= 200 (it will be, but just in case)
    if ((int)ans.size() > 200) ans.resize(200);

    cout << ans << '\n';
    return 0;
}