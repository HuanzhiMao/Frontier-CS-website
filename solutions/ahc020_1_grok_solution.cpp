#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

struct Edge {
    int u, v, id;
    ll w;
};

struct AdjEntry {
    int to;
    ll w;
    int edge_id;
};

int tfind(int x, vector<int>& parent) {
    if (parent[x] == x) return x;
    return parent[x] = tfind(parent[x], parent);
}

void tunite(int x, int y, vector<int>& parent) {
    x = tfind(x, parent);
    y = tfind(y, parent);
    if (x != y) parent[x] = y;
}

int get_min_p(ll dist2) {
    if (dist2 == 0) return 0;
    double s = sqrt((double)dist2);
    int p = (int)ceil(s);
    while (1LL * p * p < dist2) p++;
    while (p > 0 && 1LL * (p - 1) * (p - 1) >= dist2) p--;
    return min(p, 5000);
}

int main() {
    int N, M, K;
    cin >> N >> M >> K;
    vector<int> x(N + 1), y(N + 1);
    for (int i = 1; i <= N; i++) cin >> x[i] >> y[i];
    vector<Edge> edges(M);
    for (int j = 0; j < M; j++) {
        int u, v;
        ll w;
        cin >> u >> v >> w;
        edges[j] = {u, v, j + 1, w};
    }
    vector<int> aa(K + 1), bb(K + 1);
    for (int k = 1; k <= K; k++) cin >> aa[k] >> bb[k];

    vector<vector<AdjEntry>> g(N + 1);
    for (auto& e : edges) {
        g[e.u].push_back({e.v, e.w, e.id});
        g[e.v].push_back({e.u, e.w, e.id});
    }

    set<int> used;
    for (int k = 1; k <= K; k++) {
        ll min_d2 = LLONG_MAX;
        int best = -1;
        for (int i = 1; i <= N; i++) {
            ll dx = x[i] - aa[k];
            ll dy = y[i] - bb[k];
            ll d2 = dx * dx + dy * dy;
            if (d2 < min_d2) {
                min_d2 = d2;
                best = i;
            }
        }
        used.insert(best);
    }
    used.insert(1);

    ll inf = 1LL << 60;
    vector<vector<ll>> sp(N + 1, vector<ll>(N + 1, inf));
    for (int i = 1; i <= N; i++) sp[i][i] = 0;
    for (auto& e : edges) {
        sp[e.u][e.v] = sp[e.v][e.u] = e.w;
    }
    for (int kk = 1; kk <= N; kk++)
        for (int i = 1; i <= N; i++)
            for (int j = 1; j <= N; j++)
                if (sp[i][kk] < inf && sp[kk][j] < inf)
                    sp[i][j] = min(sp[i][j], sp[i][kk] + sp[kk][j]);

    vector<int> terms(used.begin(), used.end());
    int nt = terms.size();

    struct TEdge {
        int a, b;
        ll w;
    };
    vector<TEdge> tedgs;
    for (int p = 0; p < nt; p++)
        for (int q = p + 1; q < nt; q++) {
            int u = terms[p], v = terms[q];
            tedgs.push_back({p, q, sp[u][v]});
        }
    sort(tedgs.begin(), tedgs.end(), [](const TEdge& a, const TEdge& b) { return a.w < b.w; });

    vector<int> tparent(nt);
    for (int i = 0; i < nt; i++) tparent[i] = i;
    vector<pair<int, int>> chosen_tedges;
    for (auto& te : tedgs) {
        if (tfind(te.a, tparent) != tfind(te.b, tparent)) {
            tunite(te.a, te.b, tparent);
            chosen_tedges.push_back({te.a, te.b});
        }
    }

    vector<bool> on(M + 1, false);
    auto get_path = [&](int s, int t) {
        vector<ll> d(N + 1, inf);
        vector<int> prev(N + 1, -1);
        vector<int> prev_eid(N + 1, -1);
        priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;
        d[s] = 0;
        pq.push({0, s});
        while (!pq.empty()) {
            auto [cost, ver] = pq.top();
            pq.pop();
            if (cost > d[ver]) continue;
            for (auto& ent : g[ver]) {
                int to = ent.to;
                ll ww = ent.w;
                int eid = ent.edge_id;
                if (d[to] > d[ver] + ww) {
                    d[to] = d[ver] + ww;
                    prev[to] = ver;
                    prev_eid[to] = eid;
                    pq.push({d[to], to});
                }
            }
        }
        vector<int> path_eids;
        int cur = t;
        while (cur != s) {
            int pre = prev[cur];
            int eid = prev_eid[cur];
            path_eids.push_back(eid);
            cur = pre;
        }
        return path_eids;
    };

    for (auto& pe : chosen_tedges) {
        int u = terms[pe.first], v = terms[pe.second];
        auto path_eids = get_path(u, v);
        for (int eid : path_eids) {
            on[eid] = true;
        }
    }

    vector<bool> reachable(N + 1, false);
    queue<int> q;
    q.push(1);
    reachable[1] = true;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (auto& ent : g[u]) {
            if (on[ent.edge_id] && !reachable[ent.to]) {
                reachable[ent.to] = true;
                q.push(ent.to);
            }
        }
    }

    vector<vector<int>> groups(N + 1);
    for (int k = 1; k <= K; k++) {
        ll min_d2 = LLONG_MAX;
        int best_i = -1;
        for (int i = 1; i <= N; i++) {
            if (!reachable[i]) continue;
            ll dx = x[i] - aa[k];
            ll dy = y[i] - bb[k];
            ll d2 = dx * dx + dy * dy;
            if (d2 < min_d2) {
                min_d2 = d2;
                best_i = i;
            }
        }
        groups[best_i].push_back(k);
    }

    vector<int> P(N + 1, 0);
    for (int i = 1; i <= N; i++) {
        if (!reachable[i]) continue;
        int max_p = 0;
        for (int kk : groups[i]) {
            ll dx = x[i] - aa[kk];
            ll dy = y[i] - bb[kk];
            ll d2 = dx * dx + dy * dy;
            int pp = get_min_p(d2);
            max_p = max(max_p, pp);
        }
        P[i] = max_p;
    }

    for (int i = 1; i <= N; i++) {
        if (i > 1) cout << " ";
        cout << P[i];
    }
    cout << endl;
    for (int j = 1; j <= M; j++) {
        if (j > 1) cout << " ";
        cout << (on[j] ? 1 : 0);
    }
    cout << endl;
    return 0;
}