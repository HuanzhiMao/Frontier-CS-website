#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    if (!(cin >> t)) return 0;
    while (t--) {
        int n;
        cin >> n;
        vector<vector<int>> adj(n + 1);
        for (int i = 0; i < n - 1; ++i) {
            int u, v;
            cin >> u >> v;
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        // Root the tree at 1; compute parent, depth, tin/tout, euler order
        vector<int> par(n + 1, 0), dep(n + 1, 0), tin(n + 1, 0), tout(n + 1, 0), euler(n + 1, 0);
        par[1] = 1;
        int timer = 0;
        vector<pair<int,int>> st;
        st.reserve(2*n);
        st.emplace_back(1, 0);
        while (!st.empty()) {
            auto [u, state] = st.back(); st.pop_back();
            if (state == 0) {
                tin[u] = ++timer;
                euler[timer] = u;
                st.emplace_back(u, 1);
                for (int v : adj[u]) {
                    if (v == par[u]) continue;
                    par[v] = u;
                    dep[v] = dep[u] + 1;
                    st.emplace_back(v, 0);
                }
            } else {
                tout[u] = timer;
            }
        }
        par[1] = 1; // make root's parent itself to simplify parent lifting

        auto inSub = [&](int u, int x) -> bool {
            return tin[u] >= tin[x] && tin[u] <= tout[x];
        };

        // Initialize S as all nodes
        vector<int> Slist;
        Slist.reserve(n);
        for (int i = 1; i <= n; ++i) Slist.push_back(i);

        vector<int> arr(n + 2, 0), pref(n + 2, 0), s1(n + 1, 0);
        vector<int> seen(n + 1, 0), seen2(n + 1, 0);
        int epoch = 1, epoch2 = 1;

        int queries_used = 0;

        while (true) {
            if ((int)Slist.size() == 1) {
                int ans = Slist[0];
                cout << "! " << ans << '\n';
                cout.flush();
                break;
            }

            int M = (int)Slist.size();

            // Build Euler presence array for Slist
            fill(arr.begin(), arr.end(), 0);
            for (int u : Slist) arr[tin[u]] = 1;
            pref[0] = 0;
            for (int i = 1; i <= n; ++i) pref[i] = pref[i - 1] + arr[i];

            // Compute s1[x] for all x
            for (int x = 1; x <= n; ++x) {
                s1[x] = pref[tout[x]] - pref[tin[x] - 1];
            }

            // Prepare candidate nodes
            vector<pair<int,int>> clos;
            clos.reserve(n);
            for (int x = 1; x <= n; ++x) {
                int c = abs(2 * s1[x] - M);
                clos.emplace_back(c, x);
            }
            sort(clos.begin(), clos.end()); // ascending by closeness

            int Kc = min(40, n); // number of candidates from closeness
            vector<int> candidates;
            candidates.reserve(Kc + 5);
            vector<char> used(n + 1, 0);
            for (int i = 0; i < Kc; ++i) {
                int x = clos[i].second;
                if (!used[x]) {
                    candidates.push_back(x);
                    used[x] = 1;
                }
            }
            // ensure root included
            if (!used[1]) {
                candidates.push_back(1);
                used[1] = 1;
            }
            // include node with minimal s1 (could be 0)
            int minVal = INT_MAX, minNode = 1;
            for (int x = 1; x <= n; ++x) {
                if (s1[x] < minVal) {
                    minVal = s1[x];
                    minNode = x;
                }
            }
            if (!used[minNode]) {
                candidates.push_back(minNode);
                used[minNode] = 1;
            }

            // If S small, consider all nodes as candidates to be thorough
            if (M <= 150) {
                candidates.clear();
                candidates.reserve(n);
                for (int x = 1; x <= n; ++x) candidates.push_back(x);
            }

            // Pick best candidate minimizing max(s1, s0)
            int best_x = 1;
            int best_w = INT_MAX;

            for (int x : candidates) {
                int inCount = s1[x];
                // compute s0 = number of distinct parents among S \ subtree(x)
                ++epoch;
                int distinctParents = 0;
                for (int u : Slist) {
                    if (!inSub(u, x)) {
                        int p = par[u];
                        if (seen[p] != epoch) {
                            seen[p] = epoch;
                            ++distinctParents;
                        }
                    }
                }
                int w = max(inCount, distinctParents);
                if (w < best_w) {
                    best_w = w;
                    best_x = x;
                }
            }

            // Ask query
            cout << "? " << best_x << '\n';
            cout.flush();
            int r;
            if (!(cin >> r)) return 0;
            if (r < 0) return 0;
            ++queries_used;

            if (r == 1) {
                // S = S ∩ subtree(best_x)
                vector<int> NewS;
                NewS.reserve(Slist.size());
                for (int u : Slist) {
                    if (inSub(u, best_x)) NewS.push_back(u);
                }
                Slist.swap(NewS);
            } else {
                // r == 0: S = parent(S \ subtree(best_x))
                ++epoch2;
                vector<int> NewS;
                NewS.reserve(Slist.size());
                for (int u : Slist) {
                    if (!inSub(u, best_x)) {
                        int p = par[u]; // par[1]=1
                        if (seen2[p] != epoch2) {
                            seen2[p] = epoch2;
                            NewS.push_back(p);
                        }
                    }
                }
                Slist.swap(NewS);
            }
        }
    }

    return 0;
}