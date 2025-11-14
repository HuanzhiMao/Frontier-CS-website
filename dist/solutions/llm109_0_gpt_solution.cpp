#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    if (!(cin >> N)) return 0;
    int r0, c0;
    cin >> r0 >> c0;
    --r0; --c0;
    int S = N * N;
    int startId = r0 * N + c0;

    const int K = 8;
    int dr[K] = {2, 2, -2, -2, 1, -1, 1, -1};
    int dc[K] = {1, -1, 1, -1, 2, 2, -2, -2};

    auto inb = [&](int r, int c) -> bool {
        return (unsigned)r < (unsigned)N && (unsigned)c < (unsigned)N;
    };

    vector<int> stamp(S, 0);
    int attempt = 0;

    mt19937_64 rng(chrono::high_resolution_clock::now().time_since_epoch().count());
    array<int, K> moveOrder;
    iota(moveOrder.begin(), moveOrder.end(), 0);

    auto generate = [&](int start, array<int,K> order) -> vector<int> {
        ++attempt;
        vector<int> path;
        path.reserve(S);
        int cur = start;
        stamp[cur] = attempt;
        path.push_back(cur);
        int r = cur / N, c = cur % N;

        // Preallocate fixed-size arrays for candidates
        int cand1[8], cnt1 = 0;
        int cand2[8], cnt2 = 0;

        for (int step = 1; step < S; ++step) {
            int bestDeg = 9;
            cnt1 = 0;
            // First-level candidates based on degree
            for (int idx = 0; idx < K; ++idx) {
                int d = order[idx];
                int nr = r + dr[d], nc = c + dc[d];
                if (!inb(nr, nc)) continue;
                int nid = nr * N + nc;
                if (stamp[nid] == attempt) continue;

                int deg = 0;
                // degree of neighbor
                for (int j = 0; j < K; ++j) {
                    int d2 = order[j];
                    int rr = nr + dr[d2], cc = nc + dc[d2];
                    if (!inb(rr, cc)) continue;
                    int id2 = rr * N + cc;
                    if (stamp[id2] != attempt) ++deg;
                }
                if (deg < bestDeg) {
                    bestDeg = deg;
                    cand1[0] = nid;
                    cnt1 = 1;
                } else if (deg == bestDeg) {
                    cand1[cnt1++] = nid;
                }
            }

            if (cnt1 == 0) break;

            int nextId;
            if (cnt1 == 1) {
                nextId = cand1[0];
            } else {
                // Second-level tie-break: choose by minimal neighbor's minimal degree
                int best2 = 9;
                cnt2 = 0;
                for (int i = 0; i < cnt1; ++i) {
                    int nid = cand1[i];
                    int nr = nid / N, nc = nid % N;
                    int minDeg2 = 9;
                    for (int idx = 0; idx < K; ++idx) {
                        int d = order[idx];
                        int rr = nr + dr[d], cc = nc + dc[d];
                        if (!inb(rr, cc)) continue;
                        int id2 = rr * N + cc;
                        if (stamp[id2] == attempt) continue;
                        int deg2 = 0;
                        for (int j = 0; j < K; ++j) {
                            int d2 = order[j];
                            int r3 = rr + dr[d2], c3 = cc + dc[d2];
                            if (!inb(r3, c3)) continue;
                            int id3 = r3 * N + c3;
                            if (stamp[id3] != attempt) ++deg2;
                        }
                        if (deg2 < minDeg2) minDeg2 = deg2;
                    }
                    if (minDeg2 < best2) {
                        best2 = minDeg2;
                        cand2[0] = nid;
                        cnt2 = 1;
                    } else if (minDeg2 == best2) {
                        cand2[cnt2++] = nid;
                    }
                }
                if (cnt2 == 0) {
                    // Fallback to random among first-level candidates
                    nextId = cand1[uniform_int_distribution<int>(0, cnt1 - 1)(rng)];
                } else if (cnt2 == 1) {
                    nextId = cand2[0];
                } else {
                    nextId = cand2[uniform_int_distribution<int>(0, cnt2 - 1)(rng)];
                }
            }

            r = nextId / N;
            c = nextId % N;
            stamp[nextId] = attempt;
            path.push_back(nextId);
        }
        return path;
    };

    vector<int> bestPath;
    bestPath.reserve(S);

    auto tStart = chrono::high_resolution_clock::now();
    long long timeBudgetMs = 900; // aim to leave time for printing
    int tries = 0;

    while (true) {
        ++tries;
        shuffle(moveOrder.begin(), moveOrder.end(), rng);
        vector<int> path = generate(startId, moveOrder);
        if ((int)path.size() > (int)bestPath.size()) {
            bestPath.swap(path);
            if ((int)bestPath.size() == S) break;
        }
        auto tNow = chrono::high_resolution_clock::now();
        long long elapsed = chrono::duration_cast<chrono::milliseconds>(tNow - tStart).count();
        if (elapsed >= timeBudgetMs) break;

        // Reduce number of retries for very large N to save time for printing
        if (S > 200000 && tries >= 3) break;
    }

    cout << (int)bestPath.size() << '\n';
    for (int i = 0; i < (int)bestPath.size(); ++i) {
        int id = bestPath[i];
        int r = id / N + 1;
        int c = id % N + 1;
        if (i + 1 == (int)bestPath.size()) {
            cout << r << ' ' << c;
        } else {
            cout << r << ' ' << c << '\n';
        }
    }
    return 0;
}