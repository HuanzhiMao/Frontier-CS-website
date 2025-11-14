#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N;
    if (!(cin >> N)) return 0;
    vector<vector<int>> h(N, vector<int>(N));
    long long sum = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> h[i][j];
            sum += h[i][j];
        }
    }
    // Initialize counts
    int posCount = 0, negCount = 0;
    for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) {
        if (h[i][j] > 0) posCount++;
        else if (h[i][j] < 0) negCount++;
    }

    vector<string> ops;
    int r = 0, c = 0;
    long long load = 0;
    const int SAFETY_LIMIT = 100000 - 200; // margin for safety

    auto move_to = [&](int nr, int nc) {
        while (r < nr && (int)ops.size() < SAFETY_LIMIT) { ops.emplace_back("D"); r++; }
        while (r > nr && (int)ops.size() < SAFETY_LIMIT) { ops.emplace_back("U"); r--; }
        while (c < nc && (int)ops.size() < SAFETY_LIMIT) { ops.emplace_back("R"); c++; }
        while (c > nc && (int)ops.size() < SAFETY_LIMIT) { ops.emplace_back("L"); c--; }
    };

    auto find_nearest = [&](bool wantPos) -> pair<int,int> {
        int bestDist = INT_MAX, br = -1, bc = -1;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                int v = h[i][j];
                if ((wantPos && v > 0) || (!wantPos && v < 0)) {
                    int d = abs(i - r) + abs(j - c);
                    if (d < bestDist) { bestDist = d; br = i; bc = j; }
                }
            }
        }
        return {br, bc};
    };

    while ((posCount > 0 || load > 0) && (int)ops.size() < SAFETY_LIMIT) {
        if (load == 0) {
            if (posCount == 0) break;
            auto [pr, pc] = find_nearest(true);
            if (pr == -1) break; // no positive found
            move_to(pr, pc);
            if ((int)ops.size() >= SAFETY_LIMIT) break;
            int d = h[pr][pc];
            if (d > 0) {
                ops.emplace_back("+" + to_string(d));
                load += d;
                h[pr][pc] = 0;
                posCount--;
            }
        } else {
            // deliver to nearest negative
            if (negCount == 0) break; // should not happen if load>0 and sum==0
            auto [nr, nc] = find_nearest(false);
            if (nr == -1) break; // no negative found
            move_to(nr, nc);
            if ((int)ops.size() >= SAFETY_LIMIT) break;
            int need = -h[nr][nc];
            if (need > 0) {
                int d = (int)min<long long>(need, load);
                ops.emplace_back("-" + to_string(d));
                load -= d;
                h[nr][nc] += d;
                if (h[nr][nc] == 0) negCount--;
            }
        }
    }

    for (const auto &s : ops) cout << s << '\n';
    return 0;
}