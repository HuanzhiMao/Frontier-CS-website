#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, si, sj;
    if (!(cin >> N >> si >> sj)) return 0;
    vector<string> c(N);
    for (int i = 0; i < N; i++) cin >> c[i];

    vector<vector<int>> id(N, vector<int>(N, -1));
    vector<int> w;
    vector<int> posi, posj;
    int r = 0;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (c[i][j] != '#') {
                id[i][j] = r++;
                w.push_back(c[i][j] - '0');
                posi.push_back(i);
                posj.push_back(j);
            }
        }
    }

    if (r == 0) {
        cout << "\n";
        return 0;
    }

    int start = id[si][sj];
    vector<vector<int>> adj(r);
    const int di[4] = {-1, 1, 0, 0};
    const int dj[4] = {0, 0, -1, 1};

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (id[i][j] == -1) continue;
            int u = id[i][j];
            for (int d = 0; d < 4; d++) {
                int ni = i + di[d], nj = j + dj[d];
                if (0 <= ni && ni < N && 0 <= nj && nj < N && id[ni][nj] != -1) {
                    int v = id[ni][nj];
                    adj[u].push_back(v);
                }
            }
        }
    }

    const long long INF = (1LL << 60);
    vector<long long> key(r, INF);
    vector<int> parent(r, -1);
    vector<char> inMST(r, false);
    using P = pair<long long, int>;
    priority_queue<P, vector<P>, greater<P>> pq;

    key[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        auto [kc, u] = pq.top(); pq.pop();
        if (inMST[u]) continue;
        inMST[u] = true;
        for (int v : adj[u]) {
            if (inMST[v]) continue;
            long long wuv = w[u] + w[v];
            if (wuv < key[v]) {
                key[v] = wuv;
                parent[v] = u;
                pq.push({key[v], v});
            }
        }
    }

    vector<vector<int>> tree(r);
    for (int v = 0; v < r; v++) {
        if (v != start && parent[v] != -1) {
            int u = parent[v];
            tree[u].push_back(v);
            tree[v].push_back(u);
        }
    }

    auto moveChar = [&](int u, int v) -> char {
        int dx = posi[v] - posi[u];
        int dy = posj[v] - posj[u];
        if (dx == -1 && dy == 0) return 'U';
        if (dx == 1 && dy == 0) return 'D';
        if (dx == 0 && dy == -1) return 'L';
        return 'R';
    };

    vector<int> iter(r, 0);
    vector<char> visited(r, false);
    vector<int> st;
    st.reserve(r);
    st.push_back(start);
    visited[start] = true;
    string ans;
    ans.reserve(max(2 * r, 1));

    while (true) {
        int u = st.back();
        while (iter[u] < (int)tree[u].size() && visited[tree[u][iter[u]]]) iter[u]++;
        if (iter[u] < (int)tree[u].size()) {
            int v = tree[u][iter[u]++];
            ans.push_back(moveChar(u, v));
            st.push_back(v);
            visited[v] = true;
        } else {
            if (st.size() == 1) break;
            int p = st[st.size() - 2];
            ans.push_back(moveChar(u, p));
            st.pop_back();
        }
    }

    cout << ans << "\n";
    return 0;
}