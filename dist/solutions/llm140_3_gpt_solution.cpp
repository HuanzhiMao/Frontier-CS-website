#include <bits/stdc++.h>
using namespace std;

struct WaveConstraint {
    long long sx, ty;
    map<long long, int> cnt;
};

static int K;
static long long B;

bool query_point(long long s, long long t, vector<long long>& distances) {
    cout << "? 1 " << s << " " << t << endl;
    cout.flush();
    distances.assign(K, 0);
    for (int i = 0; i < K; ++i) {
        if (!(cin >> distances[i])) {
            return false;
        }
        if (distances[i] < 0) {
            // Judge may send -1 on error
            exit(0);
        }
    }
    return true;
}

map<long long,int> toCount(const vector<long long>& v) {
    map<long long,int> mp;
    for (auto x: v) mp[x]++;
    return mp;
}

struct Solver {
    int k;
    long long b;
    vector<long long> U;               // size k
    map<long long,int> Vcnt;           // multiset of V values
    vector<WaveConstraint> waves;      // additional constraints (after the two corner queries)
    vector<bool> used;
    vector<pair<long long,long long>> ans; // per U index: (x,y)
    bool solved;

    Solver(long long b_, int k_, const vector<long long>& U_, const vector<long long>& V_, const vector<WaveConstraint>& waves_)
        : k(k_), b(b_), U(U_), used(k_, false), ans(k_, {0,0}), solved(false) {
        for (auto v: V_) Vcnt[v]++;
        waves = waves_;
    }

    inline bool inside(long long x, long long y) {
        return (x >= -b && x <= b && y >= -b && y <= b);
    }

    struct Cand {
        long long v;
        vector<long long> ds; // distances for each wave
        long long x, y;
    };

    vector<Cand> computeCandidatesForU(long long u) {
        vector<Cand> res;
        for (auto &kv : Vcnt) {
            long long v = kv.first;
            int cnt = kv.second;
            if (cnt <= 0) continue;
            if (((u + v) & 1LL) != 0) continue; // ensure integer x,y
            long long x = (u + v) / 2;
            long long y = (u - v) / 2;
            if (!inside(x, y)) continue;
            vector<long long> ds; ds.reserve(waves.size());
            bool ok = true;
            for (auto &w : waves) {
                long long d = llabs(x - w.sx) + llabs(y - w.ty);
                auto it = w.cnt.find(d);
                if (it == w.cnt.end() || it->second <= 0) { ok = false; break; }
                ds.push_back(d);
            }
            if (ok) {
                res.push_back(Cand{v, ds, x, y});
            }
        }
        return res;
    }

    bool dfs(int placed) {
        if (placed == k) {
            solved = true;
            return true;
        }

        // Choose the next U index with minimum branching factor
        int bestIdx = -1;
        vector<Cand> bestCands;
        size_t bestSize = (size_t)1e9;

        for (int i = 0; i < k; ++i) {
            if (used[i]) continue;
            auto cands = computeCandidatesForU(U[i]);
            if (cands.empty()) return false;
            if (cands.size() < bestSize) {
                bestSize = cands.size();
                bestIdx = i;
                bestCands.swap(cands);
                if (bestSize == 1) break;
            }
        }

        // Simple heuristic: sort candidates by v to have deterministic behavior
        sort(bestCands.begin(), bestCands.end(), [](const Cand& a, const Cand& b){
            if (a.v != b.v) return a.v < b.v;
            if (a.x != b.x) return a.x < b.x;
            return a.y < b.y;
        });

        // Try candidates
        for (auto &cand : bestCands) {
            long long v = cand.v;
            // apply
            Vcnt[v]--;
            used[bestIdx] = true;
            ans[bestIdx] = {cand.x, cand.y};
            for (size_t m = 0; m < waves.size(); ++m) {
                waves[m].cnt[cand.ds[m]]--;
            }

            if (dfs(placed + 1)) return true;

            // rollback
            for (size_t m = 0; m < waves.size(); ++m) {
                waves[m].cnt[cand.ds[m]]++;
            }
            ans[bestIdx] = {0,0};
            used[bestIdx] = false;
            Vcnt[v]++;
        }
        return false;
    }

    bool solve() {
        // Parity sanity: counts of parity classes should match
        int Ueven=0, Uodd=0, Veven=0, Vodd=0;
        for (auto u : U) ((u&1)? Uodd:Ueven)++;
        for (auto &kv : Vcnt) {
            if ((kv.first & 1LL) == 0) Veven += kv.second;
            else Vodd += kv.second;
        }
        if (Ueven != Veven || Uodd != Vodd) {
            return false;
        }
        return dfs(0);
    }

    vector<pair<long long,long long>> get_result() const {
        vector<pair<long long,long long>> res;
        for (int i = 0; i < k; ++i) {
            if (ans[i].first != 0 || ans[i].second != 0 || used[i]) {
                res.push_back(ans[i]);
            }
        }
        // If some remained (e.g., all zeros case), still push any used entries
        if ((int)res.size() < k) {
            for (int i = 0; i < k; ++i) {
                res.push_back(ans[i]);
            }
            res.resize(k);
        }
        return res;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int W;
    if (!(cin >> B >> K >> W)) {
        return 0;
    }

    vector<long long> d_bb, d_bnb;
    if (!query_point(B, B, d_bb)) return 0; // Wave 1
    if (!query_point(B, -B, d_bnb)) return 0; // Wave 2
    int waves_used = 2;

    // Derive U and V
    vector<long long> U(K), V(K);
    for (int i = 0; i < K; ++i) U[i] = 2*B - d_bb[i]; // x+y
    for (int i = 0; i < K; ++i) V[i] = 2*B - d_bnb[i]; // x - y

    // For safety, sort U/V to have deterministic order
    sort(U.begin(), U.end());
    sort(V.begin(), V.end());

    vector<WaveConstraint> constraints;
    auto add_constraint = [&](long long sx, long long ty) -> bool {
        vector<long long> dists;
        if (!query_point(sx, ty, dists)) return false;
        WaveConstraint w;
        w.sx = sx; w.ty = ty;
        w.cnt = toCount(dists);
        constraints.push_back(std::move(w));
        ++waves_used;
        return true;
    };

    vector<pair<long long,long long>> answer;

    auto try_solve = [&](const vector<WaveConstraint>& cons) -> bool {
        Solver solver(B, K, U, V, cons);
        if (solver.solve()) {
            answer = solver.get_result();
            return true;
        }
        return false;
    };

    bool solved = false;

    if (W >= 3) {
        if (!add_constraint(0, 0)) return 0;
        solved = try_solve(constraints);
    }

    // If not solved, add more structured constraints if allowed
    vector<pair<long long,long long>> extra_points = {
        {B, 0},
        {0, B},
        {-B, 0},
        {0, -B},
        {1, 0},
        {0, 1},
        {1, 2},
        {-1, 3},
        {2, -1}
    };
    int ep_idx = 0;
    while (!solved && waves_used < W && ep_idx < (int)extra_points.size()) {
        if (!add_constraint(extra_points[ep_idx].first, extra_points[ep_idx].second)) return 0;
        solved = try_solve(constraints);
        ep_idx++;
    }

    if (!solved) {
        // Fallback when only two waves available or solver failed:
        // pair by parity and sorted order
        vector<long long> Ueven, Uodd, Veven, Vodd;
        for (auto u: U) ((u&1)? Uodd:Ueven).push_back(u);
        for (auto v: V) ((v&1)? Vodd:Veven).push_back(v);

        // Sort each
        sort(Ueven.begin(), Ueven.end());
        sort(Uodd.begin(), Uodd.end());
        sort(Veven.begin(), Veven.end());
        sort(Vodd.begin(), Vodd.end());

        vector<pair<long long,long long>> coords;

        int ne = min(Ueven.size(), Veven.size());
        for (int i = 0; i < ne; ++i) {
            long long u = Ueven[i], v = Veven[i];
            long long x = (u + v) / 2;
            long long y = (u - v) / 2;
            coords.push_back({x, y});
        }
        int no = min(Uodd.size(), Vodd.size());
        for (int i = 0; i < no; ++i) {
            long long u = Uodd[i], v = Vodd[i];
            long long x = (u + v) / 2;
            long long y = (u - v) / 2;
            coords.push_back({x, y});
        }

        // If sizes don't match exactly (shouldn't happen), fill arbitrarily
        while ((int)coords.size() < K) {
            coords.push_back({0, 0});
        }

        answer = coords;
    }

    // Output final answer
    cout << "!";
    for (int i = 0; i < K; ++i) {
        cout << " " << answer[i].first << " " << answer[i].second;
    }
    cout << endl;
    cout.flush();

    return 0;
}