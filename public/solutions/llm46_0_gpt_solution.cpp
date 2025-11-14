#include <bits/stdc++.h>
using namespace std;

struct Instance {
    int J, M;
    vector<vector<int>> mach;   // mach[j][k]
    vector<vector<long long>> proc; // proc[j][k]
    vector<vector<long long>> tail; // suffix sums of remaining processing times after op k
    vector<long long> totalProcPerMachine; // sum of proc times per machine
    vector<double> avgProcPerMachine; // averages per machine
};

struct Solution {
    vector<vector<int>> order; // order[m] = permutation of jobs processed on machine m
    long long makespan = LLONG_MAX;
};

static inline uint64_t now_micros() {
    using namespace std::chrono;
    return duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();
}

struct GTBuilder {
    const Instance &ins;
    mt19937_64 &rng;

    GTBuilder(const Instance &ins_, mt19937_64 &rng_) : ins(ins_), rng(rng_) {}

    // variant selects heuristic for tie-breaking in conflict set
    // 0: SPT, 1: LPT, 2: min est, 3: min (est+p), 4: min tail, 5: min (est+tail), 6: max remaining work, 7: weighted random, 8: random, 9: min normalized p by machine
    Solution build(int variant) {
        int J = ins.J, M = ins.M;
        Solution sol;
        sol.order.assign(M, {});
        sol.makespan = 0;

        vector<long long> jobReady(J, 0);
        vector<long long> machReady(M, 0);
        vector<int> idx(J, 0);
        vector<long long> remProc(J, 0);
        for (int j = 0; j < J; ++j) {
            long long s = 0;
            for (int k = 0; k < M; ++k) s += ins.proc[j][k];
            remProc[j] = s;
        }

        uniform_real_distribution<double> urand(0.0, 1.0);

        auto compute_value = [&](int j, int mstar)->double{
            int k = idx[j];
            long long p = ins.proc[j][k];
            long long est = max(jobReady[j], machReady[mstar]);
            long long tail = ins.tail[j][k];
            long long c = est + p;
            long long rem = remProc[j];
            double avgm = ins.avgProcPerMachine[mstar] > 0 ? ins.avgProcPerMachine[mstar] : 1.0;
            switch (variant) {
                case 0: return (double)p; // SPT
                case 1: return -(double)p; // LPT (we minimize, so negate)
                case 2: return (double)est; // min earliest start
                case 3: return (double)c; // min earliest completion
                case 4: return (double)tail; // min tail
                case 5: return (double)(est + tail); // min est + tail
                case 6: return -(double)rem; // max remaining work
                case 9: return (double)p / avgm; // normalized SPT
                default: break;
            }
            // 7: weighted random, general linear combination with random weights
            // Use fixed weights randomized per call
            // We'll prepare outside and use them here by capturing static variables
            return (double)c; // fallback
        };

        // Precompute random weights for variant 7 if used
        array<double,5> wts = {1.0, 0.0, 0.0, 0.0, 0.0};
        if (variant == 7) {
            // weights for [p, est, tail, rem, noise]
            uniform_real_distribution<double> wdist(0.0, 1.0);
            // skew to prefer ECT and SPT with some randomness
            wts[0] = wdist(rng) * 1.5; // p
            wts[1] = wdist(rng) * 1.0; // est
            wts[2] = wdist(rng) * 1.2; // tail
            wts[3] = wdist(rng) * 1.0; // remaining
            wts[4] = wdist(rng) * 0.05; // noise
        }

        auto compute_value_weighted = [&](int j, int mstar)->double{
            int k = idx[j];
            long long p = ins.proc[j][k];
            long long est = max(jobReady[j], machReady[mstar]);
            long long tail = ins.tail[j][k];
            long long rem = remProc[j];
            double noise = urand(rng);
            return wts[0]* (double)p + wts[1]*(double)est + wts[2]*(double)tail - wts[3]*(double)rem + wts[4]*noise;
        };

        // For biased randomization
        double alpha = 0.25 + urand(rng) * 0.35; // in [0.25,0.60]
        auto geometric_pick_index = [&](int n)->int{
            if (n <= 1) return 0;
            double u = urand(rng);
            double one_minus_alpha = 1.0 - alpha;
            // Avoid precision issues
            if (one_minus_alpha <= 0) return 0;
            int r = (int)(log(1.0 - u) / log(one_minus_alpha));
            if (r < 0) r = 0;
            if (r >= n) r = n - 1;
            return r;
        };

        int scheduled = 0;
        while (scheduled < J * M) {
            // Find operation with minimal earliest completion time among all next operations
            long long tStar = LLONG_MAX;
            int candJ = -1;
            int mStar = -1;
            for (int j = 0; j < J; ++j) {
                if (idx[j] >= M) continue;
                int k = idx[j];
                int m = ins.mach[j][k];
                long long p = ins.proc[j][k];
                long long est = max(jobReady[j], machReady[m]);
                long long ct = est + p;
                if (ct < tStar) {
                    tStar = ct;
                    candJ = j;
                    mStar = m;
                }
            }
            // Build conflict set B: operations on machine mStar with est < tStar
            vector<int> B;
            B.reserve(J);
            for (int j = 0; j < J; ++j) {
                if (idx[j] >= M) continue;
                int k = idx[j];
                if (ins.mach[j][k] != mStar) continue;
                long long est = max(jobReady[j], machReady[mStar]);
                if (est < tStar) {
                    B.push_back(j);
                }
            }
            // Select from B
            int chosenJ = -1;
            if (B.empty()) {
                // Should not happen; ensure candJ is chosen
                chosenJ = candJ;
            } else if (B.size() == 1 || variant == 8) {
                if (variant == 8 && B.size() > 1) {
                    uniform_int_distribution<int> dist(0, (int)B.size() - 1);
                    chosenJ = B[dist(rng)];
                } else {
                    chosenJ = B[0];
                }
            } else {
                if (variant == 6) {
                    // Max remaining work => choose argmin of -rem
                    int bestIdx = 0;
                    double bestVal = compute_value(B[0], mStar);
                    for (size_t i = 1; i < B.size(); ++i) {
                        double v = compute_value(B[i], mStar);
                        if (v < bestVal) {
                            bestVal = v;
                            bestIdx = (int)i;
                        }
                    }
                    chosenJ = B[bestIdx];
                } else if (variant == 7) {
                    // Weighted + biased randomization among sorted by score
                    vector<pair<double,int>> arr;
                    arr.reserve(B.size());
                    for (int j : B) {
                        double val = compute_value_weighted(j, mStar);
                        arr.emplace_back(val, j);
                    }
                    sort(arr.begin(), arr.end(), [](const auto& a, const auto& b){
                        if (a.first != b.first) return a.first < b.first;
                        return a.second < b.second;
                    });
                    int r = geometric_pick_index((int)arr.size());
                    chosenJ = arr[r].second;
                } else {
                    // Deterministic min by specified criteria
                    int bestIdx = 0;
                    double bestVal;
                    if (variant == 9) bestVal = compute_value(B[0], mStar);
                    else bestVal = compute_value(B[0], mStar);
                    for (size_t i = 1; i < B.size(); ++i) {
                        double v = compute_value(B[i], mStar);
                        if (v < bestVal) {
                            bestVal = v;
                            bestIdx = (int)i;
                        }
                    }
                    chosenJ = B[bestIdx];
                }
            }

            // Schedule chosenJ on mStar at earliest feasible time
            int k = idx[chosenJ];
            int m = ins.mach[chosenJ][k];
            long long p = ins.proc[chosenJ][k];
            long long start = max(jobReady[chosenJ], machReady[m]);
            long long finish = start + p;

            sol.order[m].push_back(chosenJ);
            jobReady[chosenJ] = finish;
            machReady[m] = finish;
            idx[chosenJ] += 1;
            remProc[chosenJ] -= p;
            if (finish > sol.makespan) sol.makespan = finish;

            scheduled++;
        }

        return sol;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int J, M;
    if (!(cin >> J >> M)) {
        return 0;
    }
    Instance ins;
    ins.J = J; ins.M = M;
    ins.mach.assign(J, vector<int>(M));
    ins.proc.assign(J, vector<long long>(M));
    for (int j = 0; j < J; ++j) {
        for (int k = 0; k < M; ++k) {
            int m; long long p;
            cin >> m >> p;
            ins.mach[j][k] = m;
            ins.proc[j][k] = p;
        }
    }
    // Precompute tails (suffix sums excluding current)
    ins.tail.assign(J, vector<long long>(M, 0));
    for (int j = 0; j < J; ++j) {
        long long s = 0;
        for (int k = M - 1; k >= 0; --k) {
            ins.tail[j][k] = s;
            s += ins.proc[j][k];
        }
    }
    // Machine averages
    ins.totalProcPerMachine.assign(M, 0);
    for (int j = 0; j < J; ++j) {
        for (int k = 0; k < M; ++k) {
            int m = ins.mach[j][k];
            ins.totalProcPerMachine[m] += ins.proc[j][k];
        }
    }
    ins.avgProcPerMachine.assign(M, 0.0);
    for (int m = 0; m < M; ++m) {
        ins.avgProcPerMachine[m] = (J > 0) ? (double)ins.totalProcPerMachine[m] / (double)J : 0.0;
    }

    // RNG
    uint64_t seed = (uint64_t)chrono::high_resolution_clock::now().time_since_epoch().count();
    mt19937_64 rng(seed);

    // Time management: aim for about ~1.5 seconds
    uint64_t t_start = now_micros();
    uint64_t time_budget_us = 1500000; // 1.5s
    // For very small instances, cut budget
    if (J * M <= 200) time_budget_us = 400000;
    if (J * M <= 80) time_budget_us = 200000;

    // Prepare builder
    GTBuilder builder(ins, rng);

    // Try a diversified set of variants
    vector<int> variants = {0, 3, 5, 7, 9, 1, 2, 4, 6, 8};
    // Start with a few deterministic ones to quickly get a decent baseline
    Solution best;
    best.makespan = LLONG_MAX;

    // Warm-up runs
    for (int v : variants) {
        Solution s = builder.build(v);
        if (s.makespan < best.makespan) best = std::move(s);
        if (now_micros() - t_start > time_budget_us) break;
    }

    // Randomized restarts until time budget
    uniform_int_distribution<int> vdist(0, (int)variants.size() - 1);
    while (now_micros() - t_start <= time_budget_us) {
        int v = variants[vdist(rng)];
        Solution s = builder.build(v);
        if (s.makespan < best.makespan) best = std::move(s);
    }

    // Output machine orders
    // Each line m = 0..M-1 must contain J distinct integers: permutation of 0..J-1
    // Our construction guarantees this.
    for (int m = 0; m < M; ++m) {
        for (int i = 0; i < (int)best.order[m].size(); ++i) {
            if (i) cout << ' ';
            cout << best.order[m][i];
        }
        // In rare pathological case (shouldn't happen), if order size < J,
        // fill missing jobs arbitrarily preserving permutation requirement.
        if ((int)best.order[m].size() < J) {
            // Collect missing
            vector<int> present(J, 0);
            for (int x : best.order[m]) present[x] = 1;
            for (int j = 0; j < J; ++j) if (!present[j]) {
                if (!best.order[m].empty()) cout << ' ';
                cout << j;
            }
        }
        cout << '\n';
    }

    return 0;
}