#include <bits/stdc++.h>
using namespace std;

using ll = long long;

static const ll LIM = 100000000LL;

struct Solver {
    ll b;
    int k;
    int w;
    ll M;

    // Query function: send probes and read back k*d distances
    vector<ll> query(const vector<pair<ll,ll>>& probes) {
        int d = (int)probes.size();
        cout << "? " << d;
        for (auto &p : probes) {
            cout << " " << p.first << " " << p.second;
        }
        cout << endl;
        cout.flush();

        vector<ll> res;
        res.reserve((size_t)k * d);
        for (int i = 0; i < k * d; ++i) {
            ll x;
            if (!(cin >> x)) {
                // In interactive problems, failure to read indicates an issue.
                // We exit to avoid undefined behavior.
                exit(0);
            }
            res.push_back(x);
        }
        return res;
    }

    // Backtracking to match U and V using distances at a set of near probes
    bool backtrack(int idx, const vector<int>& order,
                   const vector<vector<int>>& edges,
                   const vector<vector<vector<ll>>>& predDist, // predDist[i][j][p]
                   vector<unordered_map<ll,int>>& counts,
                   vector<int>& matchV, vector<int>& matchUUsed) {
        if (idx == k) return true;

        int i = order[idx];
        for (int jj = 0; jj < (int)edges[i].size(); ++jj) {
            int j = edges[i][jj];
            if (matchUUsed[j]) continue;
            bool ok = true;
            // verify availability in counts for all probes
            for (int p = 0; p < (int)counts.size(); ++p) {
                ll val = predDist[i][j][p];
                auto it = counts[p].find(val);
                if (it == counts[p].end() || it->second == 0) {
                    ok = false;
                    break;
                }
            }
            if (!ok) continue;
            // commit
            for (int p = 0; p < (int)counts.size(); ++p) {
                ll val = predDist[i][j][p];
                counts[p][val]--;
            }
            matchV[i] = j;
            matchUUsed[j] = 1;

            if (backtrack(idx + 1, order, edges, predDist, counts, matchV, matchUUsed)) return true;

            // revert
            for (int p = 0; p < (int)counts.size(); ++p) {
                ll val = predDist[i][j][p];
                counts[p][val]++;
            }
            matchV[i] = -1;
            matchUUsed[j] = 0;
        }
        return false;
    }

    void run() {
        if (!(cin >> b >> k >> w)) {
            return;
        }

        // Choose M >= b (and <= LIM)
        M = LIM; // since b <= 1e8, LIM satisfies M >= b
        if (M < b) M = b; // safety

        vector<ll> Us, Vs;
        Us.reserve(k);
        Vs.reserve(k);

        // Wave 1: measure U set using (-M, -M)
        vector<pair<ll,ll>> probes1;
        probes1.emplace_back(-M, -M);
        vector<ll> ans1 = query(probes1);
        // U_i = d - 2M
        for (ll d : ans1) Us.push_back(d - 2*M);

        // Wave 2: measure V set using (-M, +M)
        vector<pair<ll,ll>> probes2;
        probes2.emplace_back(-M, M);
        vector<ll> ans2 = query(probes2);
        for (ll d : ans2) Vs.push_back(d - 2*M);

        // Decide how many additional waves for coupling
        int remaining = max(0, w - 2);

        // Prepare near-probe points (inside [-b,b] for S; T = M to keep coupling)
        vector<pair<ll,ll>> nearProbesList;
        if (remaining > 0) {
            // We'll use up to 4 near probes if allowed
            vector<ll> S_candidates;
            // ensure candidates within [-b,b]
            S_candidates.push_back(0);
            if (b >= 1) S_candidates.push_back(1);
            if (b >= 1) S_candidates.push_back(-1);
            if (b >= 2) S_candidates.push_back(2);
            if (b >= 2) S_candidates.push_back(-2);
            // select up to 'remaining'
            for (ll S : S_candidates) {
                if ((int)nearProbesList.size() >= remaining) break;
                nearProbesList.emplace_back(S, M);
            }
        }

        // For each near-probe, query separately (one probe per wave to avoid mixing)
        vector<vector<ll>> nearAns;
        vector<pair<ll,ll>> nearPts; // store actual used near probes
        for (auto &pt : nearProbesList) {
            vector<pair<ll,ll>> q;
            q.push_back(pt);
            vector<ll> an = query(q);
            nearAns.push_back(an);
            nearPts.push_back(pt);
        }

        // Build counts per near-probe: distance value -> multiplicity
        vector<unordered_map<ll,int>> counts;
        counts.reserve(nearAns.size());
        for (auto &vec : nearAns) {
            unordered_map<ll,int> mp;
            mp.reserve(vec.size() * 2 + 10);
            for (ll x : vec) mp[x]++;
            counts.push_back(move(mp));
        }

        // Build candidate edges based on parity and bounds, plus existence check in counts
        vector<vector<int>> edges(k);
        // Precompute predicted distances for each possible pair and near probe
        vector<vector<vector<ll>>> predDist(k, vector<vector<ll>>(k, vector<ll>(nearPts.size(), 0)));

        for (int i = 0; i < k; ++i) {
            for (int j = 0; j < k; ++j) {
                ll u = Us[i];
                ll v = Vs[j];
                // parity check
                if (((u + v) & 1LL) != 0LL) continue;
                ll x = (u + v) / 2;
                ll y = (u - v) / 2;
                if (x < -b || x > b || y < -b || y > b) continue;
                bool ok = true;
                for (int p = 0; p < (int)nearPts.size(); ++p) {
                    ll S = nearPts[p].first;
                    ll T = nearPts[p].second;
                    ll d = llabs(x - S) + llabs(y - T);
                    predDist[i][j][p] = d;
                    if (!nearPts.empty()) {
                        auto it = counts[p].find(d);
                        if (it == counts[p].end()) {
                            ok = false;
                            break;
                        }
                    }
                }
                if (ok) edges[i].push_back(j);
            }
        }

        // Order U indices by fewest options to speed up backtracking
        vector<int> order(k);
        iota(order.begin(), order.end(), 0);
        stable_sort(order.begin(), order.end(), [&](int a, int b){
            return edges[a].size() < edges[b].size();
        });

        vector<int> matchV(k, -1), usedV(k, 0);

        bool success = false;

        if (!nearPts.empty()) {
            // With coupling probes, perform backtracking
            success = backtrack(0, order, edges, predDist, counts, matchV, usedV);
        } else {
            // No coupling probes available (w == 2). Fall back to any perfect matching based on parity/bounds.
            // Build a bipartite graph and greedily match.
            vector<int> rightUsed(k, 0);
            for (int idx = 0; idx < k; ++idx) {
                int i = order[idx];
                bool found = false;
                for (int j : edges[i]) {
                    if (!rightUsed[j]) {
                        rightUsed[j] = 1;
                        matchV[i] = j;
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    // Try any remaining j with correct parity/bounds (even if not in edges due to no near probes)
                    for (int j = 0; j < k; ++j) {
                        if (rightUsed[j]) continue;
                        ll u = Us[i], v = Vs[j];
                        if (((u + v) & 1LL) != 0LL) continue;
                        ll x = (u + v) / 2;
                        ll y = (u - v) / 2;
                        if (x < -b || x > b || y < -b || y > b) continue;
                        rightUsed[j] = 1;
                        matchV[i] = j;
                        found = true;
                        break;
                    }
                }
            }
            success = true;
        }

        // Construct answer coordinates
        vector<pair<ll,ll>> result;
        result.reserve(k);
        if (success) {
            for (int i = 0; i < k; ++i) {
                int j = matchV[i];
                if (j < 0 || j >= k) {
                    // Fallback pairing if anything went wrong
                    j = i % k;
                }
                ll u = Us[i], v = Vs[j];
                if (((u + v) & 1LL) != 0LL) {
                    // Adjust fallback: try to find some V with correct parity
                    bool fixed = false;
                    for (int jj = 0; jj < k; ++jj) {
                        if (((u + Vs[jj]) & 1LL) == 0) {
                            v = Vs[jj];
                            fixed = true;
                            break;
                        }
                    }
                    if (!fixed) {
                        // As a last resort
                        v = Vs[0];
                    }
                }
                ll x = (u + v) / 2;
                ll y = (u - v) / 2;
                result.emplace_back(x, y);
            }
        } else {
            // Shouldn't happen, but fallback: arbitrary pairing
            for (int i = 0; i < k; ++i) {
                ll u = Us[i], v = Vs[i % k];
                if (((u + v) & 1LL) != 0LL) {
                    // quick fix
                    v = Vs[0];
                }
                ll x = (u + v) / 2;
                ll y = (u - v) / 2;
                result.emplace_back(x, y);
            }
        }

        // Output final answer
        cout << "!" ;
        for (int i = 0; i < k; ++i) {
            cout << " " << result[i].first << " " << result[i].second;
        }
        cout << endl;
        cout.flush();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    Solver solver;
    solver.run();
    return 0;
}