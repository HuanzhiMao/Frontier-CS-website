#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, si, sj;
    if (!(cin >> N >> si >> sj)) return 0;
    vector<string> grid(N);
    for (int i = 0; i < N; ++i) cin >> grid[i];

    int S = si * N + sj;
    vector<int> w(N * N, -1);
    vector<bool> isRoad(N * N, false);
    int R = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            char c = grid[i][j];
            int id = i * N + j;
            if (c != '#') {
                isRoad[id] = true;
                w[id] = c - '0';
                ++R;
            }
        }
    }

    // Build adjacency list (4-neighbors) for road cells
    vector<array<int,4>> dirs(N*N);
    vector<vector<int>> nbr(N * N);
    int di[4] = {-1, 1, 0, 0};
    int dj[4] = {0, 0, -1, 1};
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int id = i * N + j;
            if (!isRoad[id]) continue;
            for (int d = 0; d < 4; ++d) {
                int ni = i + di[d], nj = j + dj[d];
                if (ni < 0 || ni >= N || nj < 0 || nj >= N) continue;
                int nid = ni * N + nj;
                if (isRoad[nid]) nbr[id].push_back(nid);
            }
        }
    }

    // Build a spanning tree using a Prim-like expansion minimizing sum of parent weights
    vector<bool> inTree(N * N, false);
    vector<int> parent(N * N, -1);
    vector<vector<int>> children(N * N);
    struct Cand {
        int w, u, v;
        bool operator<(const Cand& other) const {
            if (w != other.w) return w > other.w; // min-heap by w
            if (u != other.u) return u > other.u;
            return v > other.v;
        }
    };
    priority_queue<Cand> pq;
    inTree[S] = true;
    int cnt = 1;
    for (int v : nbr[S]) if (!inTree[v]) pq.push({w[S], S, v});
    while (cnt < R && !pq.empty()) {
        auto cur = pq.top(); pq.pop();
        int u = cur.u, v = cur.v;
        if (inTree[v]) continue;
        // Add v with parent u
        parent[v] = u;
        children[u].push_back(v);
        inTree[v] = true;
        ++cnt;
        for (int x : nbr[v]) if (!inTree[x]) pq.push({w[v], v, x});
    }

    // In case something went wrong (shouldn't), fall back to simple DFS on graph
    if (cnt < R) {
        // Simple DFS to cover all roads
        vector<char> ans;
        vector<char> visited(N * N, 0);
        vector<int> st;
        vector<int> it(N*N, 0);
        st.push_back(S);
        visited[S] = 1;
        while (!st.empty()) {
            int u = st.back();
            if (it[u] < (int)nbr[u].size()) {
                int v = nbr[u][it[u]++];
                if (!visited[v]) {
                    int ui = u / N, uj = u % N;
                    int vi = v / N, vj = v % N;
                    if (vi == ui - 1 && vj == uj) ans.push_back('U');
                    else if (vi == ui + 1 && vj == uj) ans.push_back('D');
                    else if (vi == ui && vj == uj - 1) ans.push_back('L');
                    else if (vi == ui && vj == uj + 1) ans.push_back('R');
                    visited[v] = 1;
                    st.push_back(v);
                }
            } else {
                st.pop_back();
                if (!st.empty()) {
                    int p = st.back();
                    int ui = u / N, uj = u % N;
                    int pi = p / N, pj = p % N;
                    if (pi == ui - 1 && pj == uj) ans.push_back('U');
                    else if (pi == ui + 1 && pj == uj) ans.push_back('D');
                    else if (pi == ui && pj == uj - 1) ans.push_back('L');
                    else if (pi == ui && pj == uj + 1) ans.push_back('R');
                }
            }
        }
        cout.write(ans.data(), ans.size());
        cout << '\n';
        return 0;
    }

    // DFS traversal of the tree to generate a route that returns to start
    string route;
    route.reserve(2 * (R - 1));
    vector<int> st;
    vector<int> it2(N*N, 0);
    st.push_back(S);
    while (!st.empty()) {
        int u = st.back();
        if (it2[u] < (int)children[u].size()) {
            int v = children[u][it2[u]++];
            int ui = u / N, uj = u % N;
            int vi = v / N, vj = v % N;
            if (vi == ui - 1 && vj == uj) route.push_back('U');
            else if (vi == ui + 1 && vj == uj) route.push_back('D');
            else if (vi == ui && vj == uj - 1) route.push_back('L');
            else if (vi == ui && vj == uj + 1) route.push_back('R');
            st.push_back(v);
        } else {
            st.pop_back();
            if (!st.empty()) {
                int p = st.back();
                int ui = u / N, uj = u % N;
                int pi = p / N, pj = p % N;
                if (pi == ui - 1 && pj == uj) route.push_back('U');
                else if (pi == ui + 1 && pj == uj) route.push_back('D');
                else if (pi == ui && pj == uj - 1) route.push_back('L');
                else if (pi == ui && pj == uj + 1) route.push_back('R');
            }
        }
    }

    cout << route << '\n';
    return 0;
}