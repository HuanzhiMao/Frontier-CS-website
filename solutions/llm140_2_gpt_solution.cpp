#include <bits/stdc++.h>
using namespace std;

using ll = long long;

static ll b;
static int k, w;
static int waves_used = 0;

// Send a wave with a single probe at (s, t) and read k distances
vector<ll> ask_single(ll s, ll t) {
    cout << "? 1 " << s << " " << t << endl;
    cout.flush();
    waves_used++;
    vector<ll> res(k);
    for (int i = 0; i < k; i++) {
        if (!(cin >> res[i])) {
            // In case of I/O failure, abort
            exit(0);
        }
        if (res[i] == -1) {
            exit(0);
        }
    }
    return res;
}

// Helper to compute distances in rotated coordinates:
// For probe at (s, t), define U = s + t, V = s - t.
// Then distance to deposit with (u, v) is max(|u - U|, |v - V|).
inline ll dist_uv(ll u, ll v, ll Upar, ll Vpar) {
    ll du = llabs(u - Upar);
    ll dv = llabs(v - Vpar);
    return max(du, dv);
}

struct Coupler {
    ll Upar, Vpar;
    vector<ll> dists; // size k
    unordered_map<ll, int> count;
};

struct Matcher {
    int k;
    vector<ll> U, V;
    vector<Coupler> couplers; // couplers[0] is (0,0), others added as needed
    // pairDist[i][j][t] = distance for U[i], V[j] in coupler t
    vector<vector<vector<ll>>> pairDist;

    vector<unordered_map<ll,int>> cnt; // mutable counts during recursion
    vector<int> matchU; // size k, matchU[i] = j
    vector<char> usedU, usedV;

    Matcher(int k_, const vector<ll>& U_, const vector<ll>& V_, const vector<Coupler>& cps)
        : k(k_), U(U_), V(V_), couplers(cps) {
        pairDist.assign(k, vector<vector<ll>>(k, vector<ll>(couplers.size(), 0)));
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < k; j++) {
                for (size_t t = 0; t < couplers.size(); t++) {
                    pairDist[i][j][t] = dist_uv(U[i], V[j], couplers[t].Upar, couplers[t].Vpar);
                }
            }
        }
        cnt.resize(couplers.size());
        for (size_t t = 0; t < couplers.size(); t++) {
            cnt[t] = couplers[t].count;
        }
        matchU.assign(k, -1);
        usedU.assign(k, 0);
        usedV.assign(k, 0);
    }

    // Find the current largest remaining distance in coupler 0
    bool get_pivot_d(ll &pivotD) {
        if (cnt.empty()) return false;
        if (cnt[0].empty()) return false;
        bool found = false;
        ll mx = -1;
        for (const auto &kv : cnt[0]) {
            if (kv.second > 0) {
                if (!found || kv.first > mx) {
                    mx = kv.first;
                    found = true;
                }
            }
        }
        if (!found) return false;
        pivotD = mx;
        return true;
    }

    bool dfs(int assigned) {
        if (assigned == k) return true;

        ll pivotD;
        if (!get_pivot_d(pivotD)) return false; // No remaining, but not all assigned

        // Build candidate pairs (i, j) such that:
        // - not used
        // - pairDist[i][j][0] == pivotD
        // - parity of U[i] and V[j] matches
        // - for all t, cnt[t][pairDist[i][j][t]] > 0
        vector<pair<int,int>> candidates;
        vector<int> iu, jv;
        for (int i = 0; i < k; i++) if (!usedU[i] && llabs(U[i]) == pivotD) iu.push_back(i);
        for (int j = 0; j < k; j++) if (!usedV[j] && llabs(V[j]) == pivotD) jv.push_back(j);

        // Choose side with fewer options to branch on
        bool useUside = true;
        if (iu.empty() && !jv.empty()) useUside = false;
        else if (!iu.empty() && jv.empty()) useUside = true;
        else if (!iu.empty() && !jv.empty()) {
            // Compare possible partner counts rough estimate
            size_t cntU = iu.size();
            size_t cntV = jv.size();
            useUside = (cntU <= cntV);
        } else {
            // Neither iu nor jv has direct equal pivot; but must exist logically, else contradiction
            // Try all possible i,j with pairDist==pivotD
            for (int i = 0; i < k; i++) if (!usedU[i]) {
                for (int j = 0; j < k; j++) if (!usedV[j]) {
                    if ((U[i] & 1LL) != (V[j] & 1LL)) continue;
                    if (pairDist[i][j][0] != pivotD) continue;
                    bool ok = true;
                    for (size_t t = 0; t < couplers.size(); t++) {
                        ll d = pairDist[i][j][t];
                        auto it = cnt[t].find(d);
                        if (it == cnt[t].end() || it->second <= 0) { ok = false; break; }
                    }
                    if (ok) candidates.emplace_back(i,j);
                }
            }
        }

        if (candidates.empty()) {
            if (useUside) {
                for (int i : iu) {
                    for (int j = 0; j < k; j++) if (!usedV[j]) {
                        if (llabs(V[j]) > pivotD) continue;
                        if ((U[i] & 1LL) != (V[j] & 1LL)) continue;
                        if (pairDist[i][j][0] != pivotD) continue;
                        bool ok = true;
                        for (size_t t = 0; t < couplers.size(); t++) {
                            ll d = pairDist[i][j][t];
                            auto it = cnt[t].find(d);
                            if (it == cnt[t].end() || it->second <= 0) { ok = false; break; }
                        }
                        if (ok) candidates.emplace_back(i,j);
                    }
                }
            } else {
                for (int j : jv) {
                    for (int i = 0; i < k; i++) if (!usedU[i]) {
                        if (llabs(U[i]) > pivotD) continue;
                        if ((U[i] & 1LL) != (V[j] & 1LL)) continue;
                        if (pairDist[i][j][0] != pivotD) continue;
                        bool ok = true;
                        for (size_t t = 0; t < couplers.size(); t++) {
                            ll d = pairDist[i][j][t];
                            auto it = cnt[t].find(d);
                            if (it == cnt[t].end() || it->second <= 0) { ok = false; break; }
                        }
                        if (ok) candidates.emplace_back(i,j);
                    }
                }
            }
        }

        if (candidates.empty()) return false;

        // Order candidates by heuristic: prefer those consuming rare distances
        struct Cand { int i, j; int minCnt; int sumCnt; };
        vector<Cand> ordered;
        ordered.reserve(candidates.size());
        for (auto &p : candidates) {
            int i = p.first, j = p.second;
            int mn = INT_MAX, sm = 0;
            for (size_t t = 0; t < couplers.size(); t++) {
                ll d = pairDist[i][j][t];
                int c = 0;
                auto it = cnt[t].find(d);
                if (it != cnt[t].end()) c = it->second;
                mn = min(mn, c);
                sm += c;
            }
            ordered.push_back({i,j,mn,sm});
        }
        sort(ordered.begin(), ordered.end(), [](const Cand& a, const Cand& b){
            if (a.minCnt != b.minCnt) return a.minCnt < b.minCnt;
            return a.sumCnt < b.sumCnt;
        });

        for (auto &c : ordered) {
            int i = c.i, j = c.j;
            // apply
            bool ok = true;
            vector<pair<size_t,ll>> decs;
            decs.reserve(couplers.size());
            for (size_t t = 0; t < couplers.size(); t++) {
                ll d = pairDist[i][j][t];
                auto it = cnt[t].find(d);
                if (it == cnt[t].end() || it->second <= 0) { ok = false; break; }
                it->second--;
                decs.emplace_back(t, d);
            }
            if (!ok) {
                // revert any partial
                for (auto &dd : decs) {
                    cnt[dd.first][dd.second]++;
                }
                continue;
            }
            usedU[i] = 1;
            usedV[j] = 1;
            matchU[i] = j;

            if (dfs(assigned + 1)) return true;

            // revert
            usedU[i] = 0;
            usedV[j] = 0;
            matchU[i] = -1;
            for (auto &dd : decs) {
                cnt[dd.first][dd.second]++;
            }
        }
        return false;
    }

    bool solve(vector<int>& outMatchU) {
        bool res = dfs(0);
        if (res) outMatchU = matchU;
        return res;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> b >> k >> w)) {
        return 0;
    }

    // 1) Query at (b, b) to get u = 2b - d
    vector<ll> d_bb = ask_single(b, b);
    vector<ll> Uvals;
    Uvals.reserve(k);
    for (int i = 0; i < k; i++) {
        Uvals.push_back(2 * b - d_bb[i]);
    }

    // 2) Query at (b, -b) to get v = 2b - d
    vector<ll> d_bnb = ask_single(b, -b);
    vector<ll> Vvals;
    Vvals.reserve(k);
    for (int i = 0; i < k; i++) {
        Vvals.push_back(2 * b - d_bnb[i]);
    }

    // Prepare couplers list
    vector<Coupler> couplers;

    auto add_coupler = [&](ll s, ll t) {
        vector<ll> d = ask_single(s, t);
        Coupler cpl;
        cpl.Upar = s + t;
        cpl.Vpar = s - t;
        cpl.dists = d;
        cpl.count.clear();
        for (ll x : d) cpl.count[x]++;
        couplers.push_back(move(cpl));
    };

    // Use a list of candidate coupling probes
    vector<pair<ll,ll>> candidateProbes;
    candidateProbes.emplace_back(0, 0);
    candidateProbes.emplace_back(1, 0);
    candidateProbes.emplace_back(0, 1);
    candidateProbes.emplace_back(1, 1);
    candidateProbes.emplace_back(2, 0);
    candidateProbes.emplace_back(0, 2);
    candidateProbes.emplace_back(2, 1);
    candidateProbes.emplace_back(1, 2);
    candidateProbes.emplace_back(3, 1);
    candidateProbes.emplace_back(1, 3);
    candidateProbes.emplace_back(3, 2);
    candidateProbes.emplace_back(2, 3);
    candidateProbes.emplace_back(-1, 0);
    candidateProbes.emplace_back(0, -1);
    candidateProbes.emplace_back(2, -1);
    candidateProbes.emplace_back(-1, 2);
    candidateProbes.emplace_back(4, -2);
    candidateProbes.emplace_back(-2, 4);
    candidateProbes.emplace_back(5, 3);
    candidateProbes.emplace_back(3, 5);

    int probeIdx = 0;

    // Add at least one coupler if allowed
    if (waves_used < w && probeIdx < (int)candidateProbes.size()) {
        add_coupler(candidateProbes[probeIdx].first, candidateProbes[probeIdx].second);
        probeIdx++;
    }

    vector<int> matchUtoV;
    bool solved = false;

    // Try to solve; if fail, add more couplers as allowed
    while (!solved) {
        Matcher matcher(k, Uvals, Vvals, couplers);
        if (matcher.solve(matchUtoV)) {
            solved = true;
            break;
        }
        if (waves_used >= w || probeIdx >= (int)candidateProbes.size()) {
            break;
        }
        add_coupler(candidateProbes[probeIdx].first, candidateProbes[probeIdx].second);
        probeIdx++;
    }

    // If still not solved and we still have remaining waves, try random additional probes
    std::mt19937_64 rng(123456789);
    while (!solved && waves_used < w) {
        ll s = (ll)((rng() % 1000003) - 500000);
        ll t = (ll)((rng() % 1000003) - 500000);
        s = max(-100000000LL, min(100000000LL, s));
        t = max(-100000000LL, min(100000000LL, t));
        add_coupler(s, t);
        Matcher matcher(k, Uvals, Vvals, couplers);
        if (matcher.solve(matchUtoV)) {
            solved = true;
            break;
        }
    }

    // As a fallback, try with whatever couplers we have (again)
    if (!solved) {
        Matcher matcher(k, Uvals, Vvals, couplers);
        matcher.solve(matchUtoV);
        // Even if it fails, we'll attempt to output something plausible
        if ((int)matchUtoV.size() != k) {
            matchUtoV.assign(k, -1);
            vector<int> usedV(k, 0);
            // Greedy matching by parity
            for (int i = 0; i < k; i++) {
                for (int j = 0; j < k; j++) if (!usedV[j] && ((Uvals[i] & 1LL) == (Vvals[j] & 1LL))) {
                    matchUtoV[i] = j;
                    usedV[j] = 1;
                    break;
                }
                if (matchUtoV[i] == -1) {
                    for (int j = 0; j < k; j++) if (!usedV[j]) {
                        matchUtoV[i] = j;
                        usedV[j] = 1;
                        break;
                    }
                }
            }
        }
    }

    // Construct final coordinates
    vector<pair<ll,ll>> ans;
    ans.reserve(k);
    vector<char> used(k, 0);
    for (int i = 0; i < k; i++) {
        int j = (i < (int)matchUtoV.size() ? matchUtoV[i] : -1);
        if (j < 0 || j >= k) {
            // fallback
            j = 0;
            while (j < k && used[j]) j++;
            if (j >= k) j = 0;
        }
        used[j] = 1;
        ll u = Uvals[i];
        ll v = Vvals[j];
        ll x = (u + v) / 2;
        ll y = (u - v) / 2;
        ans.emplace_back(x, y);
    }

    cout << "!" ;
    for (int i = 0; i < k; i++) {
        cout << " " << ans[i].first << " " << ans[i].second;
    }
    cout << endl;
    cout.flush();

    return 0;
}