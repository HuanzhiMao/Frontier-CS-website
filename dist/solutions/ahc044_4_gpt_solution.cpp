#include <bits/stdc++.h>
using namespace std;

int N;
long long L;
vector<int> T;

// Pick index with maximum need, optionally excluding one index
int pick_max_index(const vector<long long>& need, int exclude = -1) {
    long long bestVal = LLONG_MIN;
    int best = -1;
    for (int i = 0; i < (int)need.size(); ++i) {
        if (i == exclude) continue;
        if (need[i] > bestVal) {
            bestVal = need[i];
            best = i;
        }
    }
    if (best == -1) best = (exclude == -1 ? 0 : exclude);
    return best;
}

// Kosaraju's algorithm for SCC
void dfs1(int u, const vector<vector<int>>& g, vector<char>& used, vector<int>& order) {
    used[u] = 1;
    for (int v : g[u]) if (!used[v]) dfs1(v, g, used, order);
    order.push_back(u);
}
void dfs2(int u, const vector<vector<int>>& gr, vector<int>& comp, int cid) {
    comp[u] = cid;
    for (int v : gr[u]) if (comp[v] == -1) dfs2(v, gr, comp, cid);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N >> L;
    T.assign(N, 0);
    for (int i = 0; i < N; ++i) cin >> T[i];

    vector<int> a(N, 0), b(N, 0);
    vector<char> lockA(N, false), lockB(N, false);

    // Weighted balancing by T: ensure sum_{u edges to v} T[u] ~ 2*T[v]
    {
        vector<long long> need(N);
        for (int i = 0; i < N; ++i) need[i] = 2LL * T[i];

        vector<int> idx(N);
        iota(idx.begin(), idx.end(), 0);

        // Try two orders: descending T and ascending T; choose better by simple heuristic (sum of absolute residual need)
        auto build_with_order = [&](const vector<int>& order, vector<int>& outA, vector<int>& outB, long long& score) {
            vector<long long> curNeed = need;
            vector<int> A(N), B(N);
            for (int u : order) {
                int v1 = pick_max_index(curNeed, -1);
                A[u] = v1;
                curNeed[v1] -= T[u];

                int v2 = (N == 1) ? v1 : pick_max_index(curNeed, v1);
                B[u] = v2;
                curNeed[v2] -= T[u];
            }
            long long sc = 0;
            for (int i = 0; i < N; ++i) sc += llabs(curNeed[i]);
            outA = A; outB = B; score = sc;
        };

        vector<int> order_desc = idx, order_asc = idx;
        sort(order_desc.begin(), order_desc.end(), [&](int i, int j){ return T[i] > T[j]; });
        sort(order_asc.begin(), order_asc.end(), [&](int i, int j){ return T[i] < T[j]; });

        vector<int> a1, b1, a2, b2;
        long long s1 = 0, s2 = 0;
        build_with_order(order_desc, a1, b1, s1);
        build_with_order(order_asc, a2, b2, s2);

        if (s2 < s1) {
            a = a2; b = b2;
        } else {
            a = a1; b = b1;
        }
    }

    // Ensure strong connectivity by connecting SCCs in a ring using b-edges with minimal T[u] in each component
    {
        vector<vector<int>> g(N), gr(N);
        for (int i = 0; i < N; ++i) {
            g[i].push_back(a[i]);
            g[i].push_back(b[i]);
            gr[a[i]].push_back(i);
            gr[b[i]].push_back(i);
        }
        vector<char> used(N, false);
        vector<int> order;
        order.reserve(N);
        for (int i = 0; i < N; ++i) if (!used[i]) dfs1(i, g, used, order);
        vector<int> comp(N, -1);
        int cid = 0;
        for (int i = N - 1; i >= 0; --i) {
            int v = order[i];
            if (comp[v] == -1) {
                dfs2(v, gr, comp, cid++);
            }
        }
        if (cid > 1) {
            vector<vector<int>> nodes(cid);
            for (int i = 0; i < N; ++i) nodes[comp[i]].push_back(i);
            vector<int> rep(cid, -1);
            for (int i = 0; i < cid; ++i) rep[i] = nodes[i][0];
            vector<int> pick(cid, -1);
            for (int i = 0; i < cid; ++i) {
                int best = nodes[i][0];
                for (int u : nodes[i]) {
                    if (T[u] < T[best]) best = u;
                }
                pick[i] = best;
            }
            for (int i = 0; i < cid; ++i) {
                int u = pick[i];
                int v = rep[(i + 1) % cid];
                b[u] = v;
                lockB[u] = true; // lock ring edges
            }
        }
    }

    // Local improvement: adjust edges to reduce residual need difference (sum_{in} T[u] vs 2*T[v])
    {
        vector<long long> inbound(N, 0);
        for (int u = 0; u < N; ++u) {
            inbound[a[u]] += T[u];
            inbound[b[u]] += T[u];
        }
        vector<long long> need(N);
        for (int v = 0; v < N; ++v) need[v] = 2LL * T[v] - inbound[v];

        // Collect under and over sets
        vector<int> under, over;
        for (int v = 0; v < N; ++v) {
            if (need[v] > 0) under.push_back(v);
            else if (need[v] < 0) over.push_back(v);
        }
        sort(under.begin(), under.end(), [&](int i, int j){ return need[i] > need[j]; }); // descending need

        // Build edge representation
        auto get_target = [&](int e, const vector<int>& A, const vector<int>& B) {
            int u = (e < N ? e : e - N);
            return (e < N ? A[u] : B[u]);
        };
        auto set_target = [&](int e, int v, vector<int>& A, vector<int>& B) {
            int u = (e < N ? e : e - N);
            if (e < N) A[u] = v;
            else B[u] = v;
        };
        auto is_locked = [&](int e) {
            int u = (e < N ? e : e - N);
            if (e < N) return lockA[u];
            else return lockB[u];
        };

        // Improvement iterations
        for (int uv : under) {
            int iter_guard = 0;
            while (need[uv] > 0 && iter_guard < 1000) {
                ++iter_guard;
                long long bestImprove = 0;
                int bestEdge = -1;
                int bestFrom = -1;
                int bestU = -1;
                for (int e = 0; e < 2 * N; ++e) {
                    if (is_locked(e)) continue;
                    int u = (e < N ? e : e - N);
                    if (T[u] == 0) continue;
                    int w = get_target(e, a, b);
                    if (need[w] >= 0) continue; // only take from oversupplied
                    long long oldAbs = llabs(need[uv]) + llabs(need[w]);
                    long long newAbs = llabs(need[uv] - T[u]) + llabs(need[w] + T[u]);
                    long long improve = oldAbs - newAbs;
                    if (improve > bestImprove) {
                        bestImprove = improve;
                        bestEdge = e;
                        bestFrom = w;
                        bestU = u;
                    }
                }
                if (bestEdge == -1 || bestImprove <= 0) break;

                // apply move
                set_target(bestEdge, uv, a, b);
                inbound[bestFrom] -= T[bestU];
                inbound[uv] += T[bestU];
                need[bestFrom] += T[bestU];
                need[uv] -= T[bestU];
            }
        }
    }

    // Simulate rotor-router to compute t counts
    vector<int> t(N, 0);
    int pos = 0;
    for (long long step = 0; step < L; ++step) {
        t[pos] += 1;
        if (step + 1 < L) {
            if (t[pos] % 2 == 1) pos = a[pos];
            else pos = b[pos];
        }
    }

    // Output the plan
    for (int i = 0; i < N; ++i) {
        cout << a[i] << " " << b[i] << "\n";
    }

    return 0;
}