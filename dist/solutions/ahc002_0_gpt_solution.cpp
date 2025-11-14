#include <bits/stdc++.h>
using namespace std;

struct Timer {
    chrono::high_resolution_clock::time_point st;
    Timer() { st = chrono::high_resolution_clock::now(); }
    double elapsed() const {
        auto now = chrono::high_resolution_clock::now();
        return chrono::duration<double>(now - st).count();
    }
};

struct RNG {
    uint64_t x;
    RNG() { x = chrono::high_resolution_clock::now().time_since_epoch().count(); }
    inline uint64_t next() {
        x ^= x << 7;
        x ^= x >> 9;
        return x;
    }
    inline int randint(int l, int r) { // [l, r)
        return (int)((next() % (uint64_t)(r - l)) + l);
    }
    inline double randdbl() {
        return (next() >> 11) * (1.0 / (1ULL << 53));
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const int H = 50, W = 50, N = H * W;
    int si, sj;
    if (!(cin >> si >> sj)) return 0;

    vector<int> T(N), P(N);
    int maxT = -1;
    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
            int v; cin >> v;
            T[i*W + j] = v;
            if (v > maxT) maxT = v;
        }
    }
    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
            int v; cin >> v;
            P[i*W + j] = v;
        }
    }
    int M = maxT + 1;

    // Build adjacency per cell, excluding edges that stay within the same tile
    const int di[4] = {-1, 1, 0, 0};
    const int dj[4] = {0, 0, -1, 1};
    const char dc[4] = {'U', 'D', 'L', 'R'};

    struct Edge { int to; char ch; };
    vector<array<int,4>> nbrId(N); // neighbor cell id or -1
    vector<array<char,4>> nbrCh(N);
    vector<int> degAll(N, 0);

    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
            int id = i*W + j;
            for (int d = 0; d < 4; ++d) {
                int ni = i + di[d], nj = j + dj[d];
                if (ni < 0 || ni >= H || nj < 0 || nj >= W) {
                    nbrId[id][d] = -1;
                    nbrCh[id][d] = 0;
                    continue;
                }
                int nid = ni*W + nj;
                if (T[nid] == T[id]) { // same tile, forbidden
                    nbrId[id][d] = -1;
                    nbrCh[id][d] = 0;
                    continue;
                }
                nbrId[id][d] = nid;
                nbrCh[id][d] = dc[d];
                degAll[id]++;
            }
        }
    }

    // Precompute list of neighbor indices for quick scan
    vector<array<int,4>> nbrIdx(N); // neighbor indices in 0..3 or -1
    vector<int> nbrCnt(N,0);
    for (int id = 0; id < N; ++id) {
        int cnt = 0;
        for (int d = 0; d < 4; ++d) if (nbrId[id][d] != -1) {
            nbrIdx[id][cnt++] = d;
        }
        for (int k = cnt; k < 4; ++k) nbrIdx[id][k] = -1;
        nbrCnt[id] = cnt;
    }

    int startId = si * W + sj;

    Timer timer;
    RNG rng;

    string bestMoves;
    long long bestScore = -1;
    int bestLen = 0;

    // Heuristic run function: greedy with Warnsdorff-like rule and randomized tie-breaking
    auto run = [&](double greedNoise) {
        vector<char> moves;
        moves.reserve(N * 2);
        vector<char> visitedTile(M, 0);
        visitedTile[T[startId]] = 1;
        int cur = startId;
        long long score = P[cur];
        int steps = 0;

        // For performance, we won't store visited cells; tile-visited suffices.
        while (true) {
            // Gather candidates
            int candD[4], candV[4], candDeg[4], candPC[4];
            int csz = 0;

            for (int k = 0; k < nbrCnt[cur]; ++k) {
                int d = nbrIdx[cur][k];
                int v = nbrId[cur][d];
                if (v == -1) continue;
                int t = T[v];
                if (visitedTile[t]) continue;

                // Compute degree of v to unvisited neighbors (excluding going back to current since current tile is now visited)
                int deg = 0;
                for (int kk = 0; kk < nbrCnt[v]; ++kk) {
                    int dd = nbrIdx[v][kk];
                    int w = nbrId[v][dd];
                    if (w == -1) continue;
                    if (!visitedTile[T[w]]) deg++;
                }
                candD[csz] = d;
                candV[csz] = v;
                candDeg[csz] = deg;
                candPC[csz] = P[v];
                ++csz;
            }

            if (csz == 0) break;

            // Prefer candidates with deg > 0; among them choose with minimal deg, tie-break by high p and random noise
            int choiceIdx = -1;
            int minDegPos = INT_MAX;
            // First, find if any has deg > 0
            for (int i = 0; i < csz; ++i) {
                if (candDeg[i] > 0) minDegPos = min(minDegPos, candDeg[i]);
            }

            if (minDegPos != INT_MAX) {
                // gather all with deg == minDegPos
                double bestVal = -1e100;
                for (int i = 0; i < csz; ++i) {
                    if (candDeg[i] == minDegPos) {
                        // value = p + small noise
                        double val = (double)candPC[i] + greedNoise * (rng.randdbl() - 0.5);
                        // occasionally favor exploring higher-degree a bit to keep flexibility when tie on minDeg
                        // minor term
                        val += 0.05 * (double)(degAll[candV[i]]);
                        if (val > bestVal) {
                            bestVal = val;
                            choiceIdx = i;
                        }
                    }
                }
            } else {
                // all deg == 0, choose highest p (plus noise)
                double bestVal = -1e100;
                for (int i = 0; i < csz; ++i) {
                    double val = (double)candPC[i] + greedNoise * (rng.randdbl() - 0.5);
                    if (val > bestVal) {
                        bestVal = val;
                        choiceIdx = i;
                    }
                }
            }

            if (choiceIdx == -1) break;

            // Move
            int d = candD[choiceIdx];
            int v = candV[choiceIdx];
            moves.push_back(nbrCh[cur][d]);
            visitedTile[T[v]] = 1;
            cur = v;
            score += P[cur];
            steps++;

            // Optional small random early stop to diversify (very rarely)
            // Disabled by default for stability
        }

        return tuple<long long, int, string>(score, steps, string(moves.begin(), moves.end()));
    };

    // Initial deterministic run with small noise to seed
    {
        auto [score, len, mov] = run(0.0001);
        if (score > bestScore) {
            bestScore = score;
            bestLen = len;
            bestMoves = mov;
        }
    }

    // Time-bounded randomized runs
    // Vary greedNoise slightly and rely on RNG tie-breaking
    int iter = 0;
    while (timer.elapsed() < 1.95) {
        double noise = 0.001 + 0.004 * rng.randdbl(); // small random noise
        auto [score, len, mov] = run(noise);
        if (score > bestScore || (score == bestScore && len > bestLen)) {
            bestScore = score;
            bestLen = len;
            bestMoves = mov;
        }
        ++iter;
    }

    cout << bestMoves << '\n';
    return 0;
}