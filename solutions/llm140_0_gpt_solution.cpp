#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct Counts {
    vector<ll> vals;
    vector<int> cnt;
    unordered_map<ll,int> id;
};

static const ll LIM = 100000000LL;

Counts compress_vec(const vector<ll>& a) {
    Counts C;
    for (ll x : a) {
        auto it = C.id.find(x);
        if (it == C.id.end()) {
            int idx = (int)C.vals.size();
            C.id[x] = idx;
            C.vals.push_back(x);
            C.cnt.push_back(1);
        } else {
            C.cnt[it->second]++;
        }
    }
    return C;
}

ll manhattan(ll x, ll y, ll s, ll t) {
    return llabs(x - s) + llabs(y - t);
}

struct Candidate {
    int j;
    int idxE, idxN, idxO;
};

struct Solver {
    int k;
    ll b;

    vector<ll> U, V;
    vector<ll> dE, dN, dO;
    bool useE, useN, useO;

    Counts CE, CN, CO;

    vector<vector<Candidate>> cand; // per i
    vector<int> order;
    vector<int> matchJ; // match for each i (by original i index)
    vector<char> usedJ;

    vector<int> cntE, cntN, cntO;

    bool build_candidates() {
        cand.assign(k, {});
        // Precompute maps for counts
        if (useE) CE = compress_vec(dE);
        if (useN) CN = compress_vec(dN);
        if (useO) CO = compress_vec(dO);

        for (int i = 0; i < k; ++i) {
            for (int j = 0; j < k; ++j) {
                ll u = U[i], v = V[j];
                if (((u + v) & 1LL) != 0) continue; // parity mismatch
                ll x = (u + v) / 2;
                ll y = (u - v) / 2;
                if (x < -b || x > b || y < -b || y > b) continue;
                int idxE = -1, idxN = -1, idxO = -1;
                if (useE) {
                    ll val = manhattan(x, y, b, 0);
                    auto it = CE.id.find(val);
                    if (it == CE.id.end()) continue;
                    idxE = it->second;
                }
                if (useN) {
                    ll val = manhattan(x, y, 0, b);
                    auto it = CN.id.find(val);
                    if (it == CN.id.end()) continue;
                    idxN = it->second;
                }
                if (useO) {
                    ll val = manhattan(x, y, 0, 0);
                    auto it = CO.id.find(val);
                    if (it == CO.id.end()) continue;
                    idxO = it->second;
                }
                cand[i].push_back({j, idxE, idxN, idxO});
            }
        }
        // If any i has no candidates, impossible
        for (int i = 0; i < k; ++i) if (cand[i].empty()) return false;
        // Prepare order (least candidate first)
        order.resize(k);
        iota(order.begin(), order.end(), 0);
        sort(order.begin(), order.end(), [&](int a, int b){
            return cand[a].size() < cand[b].size();
        });
        return true;
    }

    bool upper_bound_prune() {
        // For each value in E/N/O with remaining count, compute an upper bound from remaining candidates
        // Upper bound: sum over remaining i of number of unused j that can realize this value (ignoring that j must be unique).
        // If for any value, upper bound < remaining count -> prune
        // Note: Only check those waves that are used.
        vector<int> remainingI;
        for (int pos = 0; pos < k; ++pos) {
            int i = order[pos];
            if (matchJ[i] == -1) remainingI.push_back(i);
        }
        if (useE) {
            for (int v = 0; v < (int)CE.vals.size(); ++v) {
                if (cntE[v] <= 0) continue;
                int ub = 0;
                for (int i : remainingI) {
                    for (const auto &c : cand[i]) {
                        if (!usedJ[c.j] && c.idxE == v) {
                            ub++;
                        }
                    }
                }
                if (ub < cntE[v]) return false;
            }
        }
        if (useN) {
            for (int v = 0; v < (int)CN.vals.size(); ++v) {
                if (cntN[v] <= 0) continue;
                int ub = 0;
                for (int i : remainingI) {
                    for (const auto &c : cand[i]) {
                        if (!usedJ[c.j] && c.idxN == v) {
                            ub++;
                        }
                    }
                }
                if (ub < cntN[v]) return false;
            }
        }
        if (useO) {
            for (int v = 0; v < (int)CO.vals.size(); ++v) {
                if (cntO[v] <= 0) continue;
                int ub = 0;
                for (int i : remainingI) {
                    for (const auto &c : cand[i]) {
                        if (!usedJ[c.j] && c.idxO == v) {
                            ub++;
                        }
                    }
                }
                if (ub < cntO[v]) return false;
            }
        }
        return true;
    }

    bool dfs(int pos) {
        if (pos == k) return true;
        int i = order[pos];

        // Sort candidates for i by rarity heuristic (sum of counts rarity)
        vector<pair<int, Candidate>> ordered;
        ordered.reserve(cand[i].size());
        for (int idx = 0; idx < (int)cand[i].size(); ++idx) {
            const auto &c = cand[i][idx];
            long long rarity = 0;
            if (useE) rarity += cntE[c.idxE];
            if (useN) rarity += cntN[c.idxN];
            if (useO) rarity += cntO[c.idxO];
            ordered.push_back({(int)rarity, c});
        }
        sort(ordered.begin(), ordered.end(), [&](const pair<int,Candidate>& A, const pair<int,Candidate>& B){
            if (A.first != B.first) return A.first < B.first;
            return A.second.j < B.second.j;
        });

        for (auto &pc : ordered) {
            const Candidate &c = pc.second;
            int j = c.j;
            if (usedJ[j]) continue;
            if (useE && cntE[c.idxE] <= 0) continue;
            if (useN && cntN[c.idxN] <= 0) continue;
            if (useO && cntO[c.idxO] <= 0) continue;

            // choose
            usedJ[j] = 1;
            matchJ[i] = j;
            if (useE) cntE[c.idxE]--;
            if (useN) cntN[c.idxN]--;
            if (useO) cntO[c.idxO]--;

            bool ok = upper_bound_prune();
            if (ok && dfs(pos + 1)) return true;

            // backtrack
            if (useE) cntE[c.idxE]++;
            if (useN) cntN[c.idxN]++;
            if (useO) cntO[c.idxO]++;
            matchJ[i] = -1;
            usedJ[j] = 0;
        }
        return false;
    }

    bool solve(vector<pair<ll,ll>>& outPoints) {
        if (!build_candidates()) return false;

        matchJ.assign(k, -1);
        usedJ.assign(k, 0);
        if (useE) cntE = CE.cnt; else cntE.clear();
        if (useN) cntN = CN.cnt; else cntN.clear();
        if (useO) cntO = CO.cnt; else cntO.clear();

        bool ok = dfs(0);
        if (!ok) return false;

        outPoints.clear();
        outPoints.reserve(k);
        for (int i = 0; i < k; ++i) {
            int j = matchJ[i];
            ll u = U[i], v = V[j];
            ll x = (u + v) / 2;
            ll y = (u - v) / 2;
            outPoints.emplace_back(x, y);
        }
        return true;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll b;
    int k, w;
    if (!(cin >> b >> k >> w)) return 0;

    auto ask_point = [&](ll s, ll t) -> vector<ll> {
        cout << "? 1 " << s << " " << t << endl;
        cout.flush();
        vector<ll> d(k);
        for (int i = 0; i < k; ++i) cin >> d[i];
        // The judge returns in non-decreasing order
        return d;
    };

    // Use up to 5 waves if available: NE, NW, E, N, O
    ll S = b, T = b; // ensure >= max |x|,|y|
    vector<ll> dNE = ask_point(S, T);
    vector<ll> U(k);
    for (int i = 0; i < k; ++i) U[i] = (S + T) - dNE[i];

    vector<ll> dNW = ask_point(-S, T);
    vector<ll> V(k);
    for (int i = 0; i < k; ++i) V[i] = dNW[i] - (S + T);

    vector<ll> dE, dN, dO;
    bool useE = false, useN = false, useO = false;

    if (w >= 3) {
        dE = ask_point(S, 0);
        useE = true;
    }
    if (w >= 4) {
        dN = ask_point(0, T);
        useN = true;
    }
    if (w >= 5) {
        dO = ask_point(0, 0);
        useO = true;
    }

    vector<pair<ll,ll>> ans;

    // Try strongest constraints first, then relax
    vector<tuple<bool,bool,bool>> tries;
    if (useE && useN && useO) tries.emplace_back(true,true,true);
    if (useE && useN) tries.emplace_back(true,true,false);
    if (useE && useO) tries.emplace_back(true,false,true);
    if (useN && useO) tries.emplace_back(false,true,true);
    if (useE) tries.emplace_back(true,false,false);
    if (useN) tries.emplace_back(false,true,false);
    if (useO) tries.emplace_back(false,false,true);
    tries.emplace_back(false,false,false);

    bool success = false;
    for (auto [e,n,o] : tries) {
        Solver solver;
        solver.k = k;
        solver.b = b;
        solver.U = U;
        solver.V = V;
        solver.useE = e;
        solver.useN = n;
        solver.useO = o;
        solver.dE = dE;
        solver.dN = dN;
        solver.dO = dO;
        if (solver.solve(ans)) {
            success = true;
            break;
        }
    }

    // Fallback (should rarely be needed): greedy pairing by parity and bounds
    if (!success) {
        vector<int> usedV(k, 0);
        ans.clear();
        for (int i = 0; i < k; ++i) {
            bool found = false;
            for (int j = 0; j < k; ++j) {
                if (usedV[j]) continue;
                ll u = U[i], v = V[j];
                if (((u + v) & 1LL) != 0) continue;
                ll x = (u + v) / 2;
                ll y = (u - v) / 2;
                if (x < -b || x > b || y < -b || y > b) continue;
                usedV[j] = 1;
                ans.emplace_back(x, y);
                found = true;
                break;
            }
            if (!found) {
                // as a last resort, pick some unused j and clamp
                for (int j = 0; j < k; ++j) if (!usedV[j]) {
                    usedV[j] = 1;
                    ll u = U[i], v = V[j];
                    ll x = (u + v) / 2;
                    ll y = (u - v) / 2;
                    x = min(max(x, -b), b);
                    y = min(max(y, -b), b);
                    ans.emplace_back(x, y);
                    break;
                }
            }
        }
    }

    cout << "!";
    for (auto &p : ans) {
        cout << " " << p.first << " " << p.second;
    }
    cout << endl;
    cout.flush();

    return 0;
}