#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct Instance {
    int J, M;
    vector<vector<int>> mach;       // mach[j][k] = machine of k-th op of job j
    vector<vector<ll>> proc;        // proc[j][k] = processing time
    vector<vector<int>> pos;        // pos[j][m] = position k of machine m in job j
    vector<vector<ll>> remSum;      // remSum[j][k] = sum of processing times from k..M-1 for job j
};

struct Result {
    vector<vector<int>> order;      // order[m] = sequence of jobs on machine m
    ll makespan = (ll)4e18;
};

struct Solver {
    Instance I;
    mt19937_64 rng;
    chrono::steady_clock::time_point t0;
    double timeLimitSec = 1.80;

    Solver(const Instance& inst) : I(inst) {
        rng.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    }

    void preprocess() {
        I.pos.assign(I.J, vector<int>(I.M, -1));
        for (int j = 0; j < I.J; ++j) {
            for (int k = 0; k < I.M; ++k) {
                int m = I.mach[j][k];
                I.pos[j][m] = k;
            }
        }
        I.remSum.assign(I.J, vector<ll>(I.M + 1, 0));
        for (int j = 0; j < I.J; ++j) {
            for (int k = I.M - 1; k >= 0; --k) {
                I.remSum[j][k] = I.remSum[j][k + 1] + I.proc[j][k];
            }
        }
    }

    // heuristicMode in [0..9]
    Result gifflerThompson(int heuristicMode, int randomnessTopK) {
        const int J = I.J;
        const int M = I.M;
        vector<int> nextStep(J, 0);
        vector<ll> jobReady(J, 0);
        vector<ll> macReady(M, 0);
        vector<vector<int>> order(M);
        for (int m = 0; m < M; ++m) order[m].reserve(J);

        ll makespan = 0;
        int scheduled = 0;
        const ll INF = (ll)4e18;

        // Preallocated temporary arrays
        vector<ll> est(J, 0), efi(J, INF);

        while (scheduled < J * M) {
            // Find operation with minimal earliest finish time (efi)
            ll minEFI = INF;
            int minEFIMachine = -1;

            for (int j = 0; j < J; ++j) {
                if (nextStep[j] >= M) {
                    efi[j] = INF;
                    continue;
                }
                int k = nextStep[j];
                int m = I.mach[j][k];
                ll p = I.proc[j][k];
                ll e = max(jobReady[j], macReady[m]);
                est[j] = e;
                efi[j] = e + p;
                if (efi[j] < minEFI) {
                    minEFI = efi[j];
                    minEFIMachine = m;
                }
            }

            // Build conflict set on machine minEFIMachine
            vector<int> conflict;
            conflict.reserve(J);
            for (int j = 0; j < J; ++j) {
                if (nextStep[j] >= M) continue;
                int k = nextStep[j];
                if (I.mach[j][k] != minEFIMachine) continue;
                if (max(jobReady[j], macReady[minEFIMachine]) < minEFI) {
                    conflict.push_back(j);
                }
            }

            // Select one from conflict set according to heuristic
            int chosenJob = -1;
            if (conflict.size() == 1) {
                chosenJob = conflict[0];
            } else {
                // Build scores
                vector<pair<double,int>> scored; // lower score is better (we will sort ascending)
                scored.reserve(conflict.size());

                for (int j : conflict) {
                    int k = nextStep[j];
                    int m = minEFIMachine;
                    ll p = I.proc[j][k];
                    ll e = max(jobReady[j], macReady[m]);
                    ll rem = I.remSum[j][k];
                    int remOps = I.M - k;
                    double score = 0.0;

                    switch (heuristicMode) {
                        case 0: // MWKR: largest remaining work -> lower negative score is better
                            score = -double(rem);
                            break;
                        case 1: // SPT
                            score = double(p);
                            break;
                        case 2: // LPT
                            score = -double(p);
                            break;
                        case 3: // LWKR
                            score = double(rem);
                            break;
                        case 4: // MOPNR
                            score = -double(remOps);
                            break;
                        case 5: // FIFO (earliest job ready)
                            score = double(jobReady[j]);
                            break;
                        case 6: // EST
                            score = double(e);
                            break;
                        case 7: // MIN (job earliest completion lower bound)
                            score = double(jobReady[j] + rem);
                            break;
                        case 8: // RANDOM pure
                            score = uniform_real_distribution<double>(0.0, 1.0)(rng);
                            break;
                        case 9: // Critical Ratio-like: (remaining work)/(p + 1)
                            score = -double(rem) / double(p + 1);
                            break;
                        default:
                            score = double(p);
                            break;
                    }
                    // small random noise to diversify
                    score += uniform_real_distribution<double>(-1e-9, 1e-9)(rng);
                    scored.emplace_back(score, j);
                }

                sort(scored.begin(), scored.end(),
                     [](const pair<double,int>& a, const pair<double,int>& b){
                         if (a.first != b.first) return a.first < b.first;
                         return a.second < b.second;
                     });

                int topk = max(1, min((int)scored.size(), randomnessTopK));
                int idx = uniform_int_distribution<int>(0, topk - 1)(rng);
                chosenJob = scored[idx].second;
            }

            // Schedule chosen job on machine
            int j = chosenJob;
            int k = nextStep[j];
            int m = I.mach[j][k];
            ll p = I.proc[j][k];
            ll s = max(jobReady[j], macReady[m]);
            ll f = s + p;

            // Record order and update times
            order[m].push_back(j);
            jobReady[j] = f;
            macReady[m] = f;
            nextStep[j]++;

            if (f > makespan) makespan = f;
            scheduled++;
        }

        Result res;
        res.order = move(order);
        res.makespan = makespan;
        return res;
    }

    Result solve() {
        preprocess();
        t0 = chrono::steady_clock::now();

        Result best;
        best.makespan = (ll)4e18;

        // Try several deterministic heuristics first
        vector<int> initialHeur = {0,1,2,3,4,5,6,7,9};
        for (int h : initialHeur) {
            if (elapsed() > timeLimitSec) break;
            Result r = gifflerThompson(h, 1);
            if (r.makespan < best.makespan) best = move(r);
        }

        // Randomized restarts within time limit
        while (elapsed() < timeLimitSec) {
            int h = uniform_int_distribution<int>(0, 9)(rng);
            int topk = 1 + uniform_int_distribution<int>(0, 3)(rng); // pick among top 1..4
            Result r = gifflerThompson(h, topk);
            if (r.makespan < best.makespan) best = move(r);
        }
        return best;
    }

    double elapsed() {
        auto now = chrono::steady_clock::now();
        chrono::duration<double> diff = now - t0;
        return diff.count();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int J, M;
    if (!(cin >> J >> M)) {
        return 0;
    }
    Instance I;
    I.J = J;
    I.M = M;
    I.mach.assign(J, vector<int>(M, 0));
    I.proc.assign(J, vector<ll>(M, 0));

    for (int j = 0; j < J; ++j) {
        for (int k = 0; k < M; ++k) {
            int m; long long p;
            cin >> m >> p;
            I.mach[j][k] = m;
            I.proc[j][k] = p;
        }
    }

    Solver solver(I);
    Result best = solver.solve();

    // Output exactly M lines, each a permutation of {0..J-1} for machine m
    for (int m = 0; m < M; ++m) {
        for (int i = 0; i < (int)best.order[m].size(); ++i) {
            if (i) cout << ' ';
            cout << best.order[m][i];
        }
        // Ensure each line has exactly J jobs; if not, fill with missing jobs (shouldn't happen)
        if ((int)best.order[m].size() < J) {
            vector<int> used(J, 0);
            for (int x : best.order[m]) if (0 <= x && x < J) used[x] = 1;
            for (int j = 0; j < J; ++j) if (!used[j]) {
                if (!best.order[m].empty() || j) cout << ' ';
                cout << j;
            }
        }
        cout << '\n';
    }
    return 0;
}