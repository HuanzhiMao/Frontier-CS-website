#include <bits/stdc++.h>
using namespace std;

struct Order {
    int a, b, c, d;
};

inline int manhattan(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

long long computeTotalTime(const vector<int>& perm, const vector<Order>& ord) {
    const int CX = 400, CY = 400;
    int m = (int)perm.size();
    if (m == 0) return 0;
    long long T = 0;
    // Start to first pickup
    T += manhattan(CX, CY, ord[perm[0]].a, ord[perm[0]].b);
    for (int k = 0; k < m; ++k) {
        const Order& o = ord[perm[k]];
        // Pickup to delivery
        T += manhattan(o.a, o.b, o.c, o.d);
        // Delivery to next pickup
        if (k + 1 < m) {
            const Order& o2 = ord[perm[k+1]];
            T += manhattan(o.c, o.d, o2.a, o2.b);
        }
    }
    // Last delivery to end
    const Order& last = ord[perm[m-1]];
    T += manhattan(last.c, last.d, CX, CY);
    return T;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const int N = 1000;
    const int M = 50;
    const int CX = 400, CY = 400;

    vector<Order> ord(N);
    for (int i = 0; i < N; ++i) {
        int a, b, c, d;
        if (!(cin >> a >> b >> c >> d)) return 0;
        ord[i] = {a, b, c, d};
    }

    // Greedy selection of 50 orders
    vector<int> selected;
    selected.reserve(M);
    vector<char> used(N, 0);

    int curx = CX, cury = CY;

    // Weights for greedy selection
    const double w_pos = 1.0;      // distance from current position to pickup
    const double w_len = 0.5;      // pickup->delivery distance
    const double w_end_base = 0.2; // delivery->center distance weight scaled by progress

    for (int step = 0; step < M; ++step) {
        int bestIdx = -1;
        double bestScore = 1e100;
        double t = (M == 1) ? 1.0 : (double)step / (double)(M - 1);
        for (int i = 0; i < N; ++i) {
            if (used[i]) continue;
            const Order& o = ord[i];
            double c1 = (double)manhattan(curx, cury, o.a, o.b);
            double c2 = (double)manhattan(o.a, o.b, o.c, o.d);
            double c3 = (double)manhattan(o.c, o.d, CX, CY);
            double score = w_pos * c1 + w_len * c2 + (w_end_base * t) * c3;
            if (score < bestScore) {
                bestScore = score;
                bestIdx = i;
            }
        }
        if (bestIdx == -1) break; // should not happen
        used[bestIdx] = 1;
        selected.push_back(bestIdx);
        curx = ord[bestIdx].c;
        cury = ord[bestIdx].d;
    }

    // If for some reason less than M selected, fill arbitrarily
    for (int i = 0; i < N && (int)selected.size() < M; ++i) {
        if (!used[i]) {
            used[i] = 1;
            selected.push_back(i);
        }
    }

    // Local search to improve order: relocation and swap
    long long bestT = computeTotalTime(selected, ord);
    bool improved = true;
    int iter_guard = 0;
    while (improved && iter_guard < 50) {
        improved = false;
        ++iter_guard;

        // Relocation
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < M; ++j) {
                if (i == j) continue;
                vector<int> perm2 = selected;
                int node = perm2[i];
                perm2.erase(perm2.begin() + i);
                int ins = j;
                if (j > i) ins = j - 1;
                perm2.insert(perm2.begin() + ins, node);
                long long T2 = computeTotalTime(perm2, ord);
                if (T2 < bestT) {
                    bestT = T2;
                    selected.swap(perm2);
                    improved = true;
                    goto next_iteration; // restart from beginning after improvement
                }
            }
        }

        // Swap
        for (int i = 0; i < M; ++i) {
            for (int j = i + 1; j < M; ++j) {
                vector<int> perm2 = selected;
                swap(perm2[i], perm2[j]);
                long long T2 = computeTotalTime(perm2, ord);
                if (T2 < bestT) {
                    bestT = T2;
                    selected.swap(perm2);
                    improved = true;
                    goto next_iteration; // restart
                }
            }
        }

        next_iteration:
        if (!improved) break;
    }

    // Output
    cout << M;
    for (int i = 0; i < M; ++i) cout << ' ' << (selected[i] + 1);
    cout << '\n';

    // Build route: start, then for each order pickup and delivery, then end
    int n = 2 * M + 2;
    cout << n;
    // Start
    cout << ' ' << CX << ' ' << CY;
    for (int i = 0; i < M; ++i) {
        const Order& o = ord[selected[i]];
        cout << ' ' << o.a << ' ' << o.b << ' ' << o.c << ' ' << o.d;
    }
    // End
    cout << ' ' << CX << ' ' << CY << '\n';

    return 0;
}