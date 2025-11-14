#include <bits/stdc++.h>
using namespace std;

struct DSU {
    vector<int> p, sz, tail;
    DSU() {}
    DSU(int n) { init(n); }
    void init(int n) {
        p.resize(n + 1);
        sz.assign(n + 1, 1);
        tail.resize(n + 1);
        for (int i = 1; i <= n; ++i) {
            p[i] = i;
            tail[i] = i;
        }
    }
    int find(int x) {
        while (p[x] != x) {
            p[x] = p[p[x]];
            x = p[x];
        }
        return x;
    }
    void unite_keep_order(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return;
        p[b] = a;
        sz[a] += sz[b];
        tail[a] = tail[b];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;
    int a[10];
    for (int i = 0; i < 10; ++i) cin >> a[i];

    vector<pair<int,int>> edges;
    edges.reserve(m);
    vector<vector<int>> out(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        edges.emplace_back(u, v);
        out[u].push_back(v);
    }

    // Step 1: Greedy path cover using at most one outgoing and one incoming per vertex
    vector<int> nxt(n + 1, -1), prv(n + 1, -1);
    vector<char> outUsed(n + 1, 0), inUsed(n + 1, 0);

    for (auto &e : edges) {
        int u = e.first, v = e.second;
        if (!outUsed[u] && !inUsed[v]) {
            outUsed[u] = 1;
            inUsed[v] = 1;
            nxt[u] = v;
            prv[v] = u;
        }
    }

    // Step 2: Break cycles to get disjoint paths
    vector<char> vis(n + 1, 0);
    vector<int> heads;
    heads.reserve(n);

    for (int v = 1; v <= n; ++v) {
        if (prv[v] == -1) {
            int cur = v;
            heads.push_back(v);
            while (cur != -1 && !vis[cur]) {
                vis[cur] = 1;
                cur = nxt[cur];
            }
        }
    }
    for (int v = 1; v <= n; ++v) {
        if (!vis[v] && prv[v] != -1) {
            int u = prv[v];
            nxt[u] = -1;
            prv[v] = -1;
            heads.push_back(v);
            int cur = v;
            while (cur != -1 && !vis[cur]) {
                vis[cur] = 1;
                cur = nxt[cur];
            }
        }
    }

    // Step 3: DSU initialize per path head, record sizes and tails
    DSU dsu(n);
    vector<int> sizeRoot(n + 1, 1);
    for (int i = 1; i <= n; ++i) {
        dsu.p[i] = i;
        dsu.sz[i] = 1;
        dsu.tail[i] = i;
    }

    for (int h : heads) {
        if (prv[h] != -1) continue; // ensure it's a head
        int cur = h;
        int cnt = 0;
        int t = h;
        while (cur != -1) {
            dsu.p[cur] = h;
            cnt++;
            t = cur;
            cur = nxt[cur];
        }
        dsu.sz[h] = cnt;
        dsu.tail[h] = t;
    }

    // Step 4: Merge paths greedily by connecting tail -> head when possible
    deque<int> q;
    vector<char> processedTail(n + 1, 0);
    for (int h : heads) {
        if (prv[h] == -1) {
            int root = dsu.find(h);
            q.push_back(dsu.tail[root]);
        }
    }

    while (!q.empty()) {
        int u = q.front(); q.pop_front();
        if (u <= 0 || u > n) continue;
        if (processedTail[u]) continue;
        processedTail[u] = 1;

        int ru = dsu.find(u);
        if (nxt[u] != -1 || dsu.tail[ru] != u) continue; // not current tail anymore

        bool merged = false;
        for (int w : out[u]) {
            int rw = dsu.find(w);
            if (ru == rw) continue;
            if (prv[w] != -1) continue; // w must be head of its path
            // Merge ru tail (u) -> head(w) of rw
            nxt[u] = w;
            prv[w] = u;
            dsu.unite_keep_order(ru, rw);
            int newRoot = dsu.find(u);
            q.push_back(dsu.tail[newRoot]);
            merged = true;
            break;
        }
        (void)merged;
    }

    // Step 5: Choose the largest path
    int bestHead = -1;
    int bestSize = 0;
    for (int h : heads) {
        if (prv[h] == -1) {
            int root = dsu.find(h);
            int s = dsu.sz[root];
            if (s > bestSize) {
                bestSize = s;
                bestHead = h;
            }
        }
    }
    if (bestHead == -1) {
        // Fallback: choose any vertex
        bestHead = 1;
        bestSize = 1;
    }

    vector<int> ans;
    ans.reserve(bestSize);
    int cur = bestHead;
    vector<char> used(n + 1, 0);
    while (cur != -1 && !used[cur]) {
        ans.push_back(cur);
        used[cur] = 1;
        cur = nxt[cur];
    }

    cout << (int)ans.size() << "\n";
    for (int i = 0; i < (int)ans.size(); ++i) {
        if (i) cout << ' ';
        cout << ans[i];
    }
    cout << "\n";

    return 0;
}