#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct Edge {
    int j;
    ll x, y;
    ll d0;   // |x| + |y|
    ll dM0;  // M - x + |y|
    ll d0M;  // M - y + |x|
};

struct Counter {
    unordered_map<long long, int> cnt;
    void build(const vector<ll>& arr) {
        cnt.clear();
        for (ll v : arr) cnt[v]++;
    }
    bool has(ll v) const {
        auto it = cnt.find(v);
        return it != cnt.end() && it->second > 0;
    }
    void add(ll v, int delta) {
        auto it = cnt.find(v);
        if (it == cnt.end()) {
            if (delta > 0) cnt[v] = delta;
            // else ignore negative (shouldn't happen if used correctly)
        } else {
            it->second += delta;
            if (it->second == 0) cnt.erase(it);
        }
    }
    bool empty() const { return cnt.empty(); }
};

int g_b = 0, g_k = 0, g_w = 0;
ll g_M = 100000000LL;

vector<ll> queryPoints(const vector<pair<ll,ll>>& pts) {
    int d = (int)pts.size();
    cout << "? " << d;
    for (auto &p : pts) {
        cout << " " << p.first << " " << p.second;
    }
    cout << endl;
    cout.flush();

    vector<ll> res;
    res.resize((size_t)g_k * d);
    for (int i = 0; i < g_k * d; i++) {
        if (!(cin >> res[i])) {
            // If interactive fails, exit
            exit(0);
        }
    }
    return res;
}

bool hopcroftKarp(const vector<vector<int>>& adj, vector<int>& pairU, vector<int>& pairV) {
    int n = (int)adj.size();
    int m = n; // right side size equals k
    const int INF = 1e9;
    vector<int> dist(n);
    pairU.assign(n, -1);
    pairV.assign(m, -1);

    auto bfs = [&]() -> bool {
        queue<int> q;
        for (int u = 0; u < n; u++) {
            if (pairU[u] == -1) {
                dist[u] = 0;
                q.push(u);
            } else {
                dist[u] = INF;
            }
        }
        bool found = false;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) {
                int u2 = pairV[v];
                if (u2 != -1 && dist[u2] == INF) {
                    dist[u2] = dist[u] + 1;
                    q.push(u2);
                }
                if (u2 == -1) found = true;
            }
        }
        return found;
    };

    function<bool(int)> dfs = [&](int u) -> bool {
        for (int v : adj[u]) {
            int u2 = pairV[v];
            if (u2 == -1 || (u2 != -1 && dfs(u2) && (pairU[u2] == -1 || true))) {
                pairU[u] = v;
                pairV[v] = u;
                return true;
            }
        }
        // Layered dfs: we should use dist, but for small k it's ok
        return false;
    };

    int matching = 0;
    while (bfs()) {
        vector<int> used(n, 0);
        for (int u = 0; u < n; u++) {
            if (pairU[u] == -1) {
                if (dfs(u)) matching++;
            }
        }
    }
    return matching == n;
}

bool backtrackSolve(const vector<vector<Edge>>& edges,
                    const vector<int>& order,
                    vector<int>& usedR,
                    vector<pair<ll,ll>>& ansOrdered,
                    int pos,
                    Counter* c0, Counter* cM0, Counter* c0M) {
    int k = (int)order.size();
    if (pos == k) {
        // All assigned; counters should be empty if used
        return true;
    }
    int i = order[pos];
    const auto& eds = edges[i];
    for (const auto& e : eds) {
        if (usedR[e.j]) continue;
        if (c0 && !c0->has(e.d0)) continue;
        if (cM0 && !cM0->has(e.dM0)) continue;
        if (c0M && !c0M->has(e.d0M)) continue;
        // choose
        usedR[e.j] = 1;
        if (c0) c0->add(e.d0, -1);
        if (cM0) cM0->add(e.dM0, -1);
        if (c0M) c0M->add(e.d0M, -1);
        ansOrdered[pos] = {e.x, e.y};
        if (backtrackSolve(edges, order, usedR, ansOrdered, pos+1, c0, cM0, c0M)) return true;
        // backtrack
        if (c0) c0->add(e.d0, +1);
        if (cM0) cM0->add(e.dM0, +1);
        if (c0M) c0M->add(e.d0M, +1);
        usedR[e.j] = 0;
    }
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> g_b >> g_k >> g_w)) {
        return 0;
    }
    g_M = 100000000LL;
    if (g_M < g_b) g_M = g_b;

    int wavesUsed = 0;

    // Wave 1: probe at (M, M) to get s = x + y
    vector<pair<ll,ll>> pts1 = { {g_M, g_M} };
    vector<ll> res1 = queryPoints(pts1);
    wavesUsed++;

    vector<ll> sList;
    sList.reserve(g_k);
    for (int i = 0; i < g_k; i++) {
        ll d = res1[i];
        ll s = 2*g_M - d; // s = x + y
        sList.push_back(s);
    }

    // Wave 2: probe at (M, -M) to get t = x - y
    vector<pair<ll,ll>> pts2 = { {g_M, -g_M} };
    vector<ll> res2 = queryPoints(pts2);
    wavesUsed++;

    vector<ll> tList;
    tList.reserve(g_k);
    for (int i = 0; i < g_k; i++) {
        ll d = res2[i];
        ll t = 2*g_M - d; // t = x - y
        tList.push_back(t);
    }

    // Optional additional constraints
    vector<ll> d0List; // distances to (0,0) = |x|+|y|
    vector<ll> dM0List; // distances to (M,0) = M - x + |y|
    vector<ll> d0MList; // distances to (0,M) = M - y + |x|
    Counter c0, cM0, c0M;

    bool have_c0 = false, have_cM0 = false, have_c0M = false;

    if (wavesUsed < g_w) {
        // Wave 3: (0,0)
        vector<pair<ll,ll>> pts3 = { {0, 0} };
        d0List = queryPoints(pts3);
        wavesUsed++;
        c0.build(d0List);
        have_c0 = true;
    }

    // Build edges
    int k = g_k;
    vector<vector<Edge>> edges(k);
    for (int i = 0; i < k; i++) {
        ll s = sList[i];
        for (int j = 0; j < k; j++) {
            ll t = tList[j];
            if (((s + t) & 1LL) != 0) continue; // parity
            ll x = (s + t) / 2;
            ll y = (s - t) / 2;
            if (x < -g_b || x > g_b || y < -g_b || y > g_b) continue;
            Edge e;
            e.j = j;
            e.x = x; e.y = y;
            e.d0 = llabs(x) + llabs(y);
            e.dM0 = g_M - x + llabs(y);
            e.d0M = g_M - y + llabs(x);
            edges[i].push_back(e);
        }
    }

    // If we have constraints, try backtracking with them
    vector<pair<ll,ll>> ans;
    bool solved = false;

    // Order left side by increasing adjacency size
    vector<int> order(k);
    iota(order.begin(), order.end(), 0);
    stable_sort(order.begin(), order.end(), [&](int a, int b){
        return edges[a].size() < edges[b].size();
    });

    if (have_c0) {
        vector<int> usedR(k, 0);
        vector<pair<ll,ll>> ansOrdered(k);
        Counter* pc0 = &c0;
        Counter* pcM0 = nullptr;
        Counter* pc0M = nullptr;
        if (backtrackSolve(edges, order, usedR, ansOrdered, 0, pc0, pcM0, pc0M)) {
            solved = true;
            ans = ansOrdered;
        } else if (wavesUsed + 1 <= g_w) {
            // Add another constraint (M,0)
            vector<pair<ll,ll>> pts4 = { {g_M, 0} };
            dM0List = queryPoints(pts4);
            wavesUsed++;
            cM0.build(dM0List);
            have_cM0 = true;

            fill(usedR.begin(), usedR.end(), 0);
            if (backtrackSolve(edges, order, usedR, ansOrdered, 0, &c0, &cM0, nullptr)) {
                solved = true;
                ans = ansOrdered;
            } else if (wavesUsed + 1 <= g_w) {
                // Add (0,M) too
                vector<pair<ll,ll>> pts5 = { {0, g_M} };
                d0MList = queryPoints(pts5);
                wavesUsed++;
                c0M.build(d0MList);
                have_c0M = true;

                fill(usedR.begin(), usedR.end(), 0);
                if (backtrackSolve(edges, order, usedR, ansOrdered, 0, &c0, &cM0, &c0M)) {
                    solved = true;
                    ans = ansOrdered;
                }
            }
        }
    }

    if (!solved) {
        // No constraints (w==2) or backtracking failed: use Hopcroft-Karp to get some matching
        vector<vector<int>> adj(k);
        for (int i = 0; i < k; i++) {
            for (auto &e : edges[i]) adj[i].push_back(e.j);
        }
        vector<int> pairU, pairV;
        bool ok = hopcroftKarp(adj, pairU, pairV);
        if (!ok) {
            // Should not happen if inputs are consistent; fallback to greedy/backtracking without counters
            vector<int> usedR(k, 0);
            vector<pair<ll,ll>> ansOrdered(k);
            if (backtrackSolve(edges, order, usedR, ansOrdered, 0, nullptr, nullptr, nullptr)) {
                ans = ansOrdered;
                solved = true;
            } else {
                // As last resort, output zeros
                ans.clear();
                for (int i = 0; i < k; i++) ans.push_back({0,0});
                solved = true;
            }
        } else {
            ans.clear();
            ans.reserve(k);
            for (int i = 0; i < k; i++) {
                int j = pairU[i];
                // Find the corresponding edge to get x,y
                bool found = false;
                for (auto &e : edges[i]) {
                    if (e.j == j) {
                        ans.push_back({e.x, e.y});
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    // Shouldn't happen, fallback
                    ans.push_back({0,0});
                }
            }
            solved = true;
        }

        // If we still have waves left, we can verify with (0,0) and adjust if mismatch using backtracking
        if (solved && wavesUsed < g_w) {
            vector<pair<ll,ll>> pts3 = { {0, 0} };
            d0List = queryPoints(pts3);
            wavesUsed++;
            vector<ll> pred;
            pred.reserve(k);
            for (auto &p : ans) pred.push_back(llabs(p.first) + llabs(p.second));
            sort(pred.begin(), pred.end());
            vector<ll> d0Sorted = d0List;
            sort(d0Sorted.begin(), d0Sorted.end());
            if (pred != d0Sorted) {
                c0.build(d0List);
                // Re-run backtracking with this constraint
                vector<int> usedR(k, 0);
                vector<pair<ll,ll>> ansOrdered(k);
                if (backtrackSolve(edges, order, usedR, ansOrdered, 0, &c0, nullptr, nullptr)) {
                    ans = ansOrdered;
                }
            }
        }
    }

    // Output answer
    cout << "! ";
    for (int i = 0; i < (int)ans.size(); i++) {
        cout << ans[i].first << " " << ans[i].second;
        if (i + 1 < (int)ans.size()) cout << " ";
    }
    cout << endl;
    cout.flush();

    return 0;
}