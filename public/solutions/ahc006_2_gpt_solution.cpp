#include <bits/stdc++.h>
using namespace std;

struct Order {
    int id;
    int ax, by, cx, dy;
};

static inline int manhattan(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const int N = 1000;
    const int M = 50;
    const int OX = 400, OY = 400;

    vector<Order> ord(N);
    for (int i = 0; i < N; i++) {
        int a, b, c, d;
        if (!(cin >> a >> b >> c >> d)) return 0;
        ord[i] = {i, a, b, c, d};
    }

    vector<int> nodeCost(N);
    for (int i = 0; i < N; i++) {
        nodeCost[i] = manhattan(ord[i].ax, ord[i].by, ord[i].cx, ord[i].dy);
    }

    auto startEdge = [&](int i) -> int {
        return manhattan(OX, OY, ord[i].ax, ord[i].by);
    };
    auto endEdge = [&](int i) -> int {
        return manhattan(ord[i].cx, ord[i].dy, OX, OY);
    };
    auto trans = [&](int i, int j) -> int {
        return manhattan(ord[i].cx, ord[i].dy, ord[j].ax, ord[j].by);
    };

    auto totalCost = [&](const vector<int>& seq) -> long long {
        if (seq.empty()) return 0;
        long long cost = 0;
        cost += startEdge(seq[0]);
        for (int i = 0; i + 1 < (int)seq.size(); i++) {
            cost += trans(seq[i], seq[i + 1]);
        }
        cost += endEdge(seq.back());
        for (int id : seq) cost += nodeCost[id];
        return cost;
    };

    auto twoOpt = [&](vector<int>& seq) {
        int m = (int)seq.size();
        if (m <= 2) return;
        bool improved = true;
        while (improved) {
            improved = false;
            for (int i = 0; i < m - 1; i++) {
                for (int k = i + 1; k < m; k++) {
                    long long oldCost = 0, newCost = 0;
                    // old boundary
                    if (i == 0) oldCost += startEdge(seq[i]);
                    else oldCost += trans(seq[i - 1], seq[i]);
                    for (int t = i; t < k; t++) oldCost += trans(seq[t], seq[t + 1]);
                    if (k == m - 1) oldCost += endEdge(seq[k]);
                    else oldCost += trans(seq[k], seq[k + 1]);

                    // new boundary after reversal
                    if (i == 0) newCost += startEdge(seq[k]);
                    else newCost += trans(seq[i - 1], seq[k]);
                    for (int t = k; t > i; t--) newCost += trans(seq[t], seq[t - 1]);
                    if (k == m - 1) newCost += endEdge(seq[i]);
                    else newCost += trans(seq[i], seq[k + 1]);

                    if (newCost + 1e-9 < oldCost) {
                        reverse(seq.begin() + i, seq.begin() + k + 1);
                        improved = true;
                        goto CONTINUE_OUTER;
                    }
                }
            }
        CONTINUE_OUTER:
            ;
        }
    };

    auto improveSwap = [&](vector<int>& seq, vector<char>& used, double time_limit_ms, chrono::high_resolution_clock::time_point start_clock) {
        bool changed = false;
        int m = seq.size();
        while (true) {
            bool improved = false;
            for (int p = 0; p < m; p++) {
                auto now = chrono::high_resolution_clock::now();
                double elapsed = chrono::duration<double, std::milli>(now - start_clock).count();
                if (elapsed > time_limit_ms) return changed;

                int cur = seq[p];
                long long oldEdge = 0;
                if (p == 0) oldEdge += startEdge(cur);
                else oldEdge += trans(seq[p - 1], cur);
                if (p == m - 1) oldEdge += endEdge(cur);
                else oldEdge += trans(cur, seq[p + 1]);
                long long oldNode = nodeCost[cur];

                long long bestDelta = 0;
                int bestJ = -1;

                for (int j = 0; j < N; j++) {
                    if (used[j]) continue;
                    long long newEdge = 0;
                    if (p == 0) newEdge += startEdge(j);
                    else newEdge += manhattan(ord[seq[p - 1]].cx, ord[seq[p - 1]].dy, ord[j].ax, ord[j].by);
                    if (p == m - 1) newEdge += endEdge(j);
                    else newEdge += manhattan(ord[j].cx, ord[j].dy, ord[seq[p + 1]].ax, ord[seq[p + 1]].by);
                    long long newNode = nodeCost[j];

                    long long delta = (newEdge + newNode) - (oldEdge + oldNode);
                    if (delta < bestDelta) {
                        bestDelta = delta;
                        bestJ = j;
                    }
                }
                if (bestJ != -1) {
                    used[cur] = 0;
                    used[bestJ] = 1;
                    seq[p] = bestJ;
                    improved = true;
                }
            }
            if (!improved) break;
            twoOpt(seq);
            changed = true;
        }
        return changed;
    };

    auto buildGreedy = [&](double lambda, int topK, mt19937& rng) -> vector<int> {
        vector<int> seq;
        seq.reserve(M);
        vector<char> used(N, 0);
        int px = OX, py = OY;
        for (int s = 0; s < M; s++) {
            vector<pair<double, int>> cand;
            cand.reserve(64);
            for (int i = 0; i < N; i++) {
                if (used[i]) continue;
                double score = 0.0;
                score += manhattan(px, py, ord[i].ax, ord[i].by);
                score += manhattan(ord[i].ax, ord[i].by, ord[i].cx, ord[i].dy);
                score += lambda * manhattan(ord[i].cx, ord[i].dy, OX, OY);
                cand.emplace_back(score, i);
            }
            if (cand.empty()) break;
            if ((int)cand.size() <= topK) {
                nth_element(cand.begin(), cand.begin(), cand.end());
                uniform_int_distribution<int> dist(0, (int)cand.size() - 1);
                int idx = cand[dist(rng)].second;
                seq.push_back(idx);
                used[idx] = 1;
                px = ord[idx].cx; py = ord[idx].dy;
            } else {
                nth_element(cand.begin(), cand.begin() + topK, cand.end());
                uniform_int_distribution<int> dist(0, topK);
                int pos = dist(rng);
                int idx = cand[pos].second;
                seq.push_back(idx);
                used[idx] = 1;
                px = ord[idx].cx; py = ord[idx].dy;
            }
        }
        return seq;
    };

    mt19937 rng((uint64_t)chrono::high_resolution_clock::now().time_since_epoch().count());

    vector<int> bestSeq;
    long long bestCost = (1LL << 60);

    auto start_clock = chrono::high_resolution_clock::now();
    const double TIME_LIMIT_MS = 1800.0;

    int attempts = 0;
    while (true) {
        auto now = chrono::high_resolution_clock::now();
        double elapsed = chrono::duration<double, std::milli>(now - start_clock).count();
        if (elapsed > TIME_LIMIT_MS) break;
        attempts++;

        double lambda = uniform_real_distribution<double>(0.05, 0.25)(rng);
        int topK = uniform_int_distribution<int>(1, 8)(rng);

        vector<int> seq = buildGreedy(lambda, topK, rng);
        if ((int)seq.size() < M) {
            // If greedy failed to pick enough (shouldn't happen), fill with smallest round trip orders.
            vector<int> rem;
            vector<char> used(N, 0);
            for (int x : seq) used[x] = 1;
            vector<pair<int, int>> rr;
            rr.reserve(N);
            for (int i = 0; i < N; i++) if (!used[i]) {
                int t = startEdge(i) + nodeCost[i] + endEdge(i);
                rr.emplace_back(t, i);
            }
            sort(rr.begin(), rr.end());
            for (auto& p : rr) {
                if ((int)seq.size() >= M) break;
                seq.push_back(p.second);
                used[p.second] = 1;
            }
        } else if ((int)seq.size() > M) {
            seq.resize(M);
        }

        twoOpt(seq);

        // Swap improvements under time
        vector<char> used(N, 0);
        for (int x : seq) used[x] = 1;
        improveSwap(seq, used, TIME_LIMIT_MS, start_clock);

        long long cost = totalCost(seq);
        if (cost < bestCost) {
            bestCost = cost;
            bestSeq = seq;
        }
    }

    if ((int)bestSeq.size() == 0) {
        // Fallback: just pick 50 shortest round trips
        vector<pair<int,int>> rr;
        rr.reserve(N);
        for (int i = 0; i < N; i++) {
            int t = startEdge(i) + nodeCost[i] + endEdge(i);
            rr.emplace_back(t, i);
        }
        sort(rr.begin(), rr.end());
        for (int i = 0; i < M; i++) bestSeq.push_back(rr[i].second);
    }

    // Build route with pickup then drop per order, sequentially
    vector<pair<int,int>> route;
    auto pushPoint = [&](int x, int y) {
        if (route.empty() || route.back().first != x || route.back().second != y) {
            route.emplace_back(x, y);
        }
    };
    pushPoint(OX, OY);
    for (int id : bestSeq) {
        pushPoint(ord[id].ax, ord[id].by);
        pushPoint(ord[id].cx, ord[id].dy);
    }
    pushPoint(OX, OY);

    // Output
    cout << M;
    for (int id : bestSeq) cout << ' ' << (id + 1);
    cout << '\n';
    cout << (int)route.size();
    for (auto &p : route) cout << ' ' << p.first << ' ' << p.second;
    cout << '\n';

    return 0;
}