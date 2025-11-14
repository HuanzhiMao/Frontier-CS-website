#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    const int H = 50, W = 50;
    int si, sj;
    if (!(cin >> si >> sj)) return 0;
    vector<vector<int>> t(H, vector<int>(W));
    vector<vector<int>> p(H, vector<int>(W));
    int tmax = -1;
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            cin >> t[i][j];
            tmax = max(tmax, t[i][j]);
        }
    }
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) cin >> p[i][j];
    }
    int M = tmax + 1;
    vector<char> visited(M, false);
    int ci = si, cj = sj;
    visited[t[ci][cj]] = true;
    int visitedTiles = 1;

    const int di[4] = {-1, 1, 0, 0};
    const int dj[4] = {0, 0, -1, 1};
    const char dc[4] = {'U', 'D', 'L', 'R'};

    auto inb = [&](int i, int j) -> bool { return (0 <= i && i < H && 0 <= j && j < W); };

    auto deg_at = [&](int i, int j) -> int {
        int cnt = 0;
        for (int d = 0; d < 4; d++) {
            int ni = i + di[d], nj = j + dj[d];
            if (!inb(ni, nj)) continue;
            int tt = t[ni][nj];
            if (!visited[tt]) cnt++;
        }
        return cnt;
    };

    uint64_t seed = 1234567891234567ULL ^ (uint64_t)chrono::high_resolution_clock::now().time_since_epoch().count();
    auto rng = [&]() -> uint64_t {
        seed ^= seed << 7;
        seed ^= seed >> 9;
        return seed;
    };

    string path;
    while (true) {
        struct Cand {
            int dir;
            int ni, nj;
            int t1;
            int deg1;
            double eval;
        };
        array<Cand, 4> vec;
        int cc = 0;
        for (int d = 0; d < 4; d++) {
            int ni = ci + di[d], nj = cj + dj[d];
            if (!inb(ni, nj)) continue;
            int t1 = t[ni][nj];
            if (visited[t1]) continue;
            vec[cc++] = {d, ni, nj, t1, 0, -1e100};
        }
        if (cc == 0) break;

        double frac = (double)visitedTiles / (double)M;

        // Dynamic weights
        double w1 = 1.0;
        double w2 = 0.5 + 0.2 * (1.0 - frac); // more lookahead early
        double w3 = 1.5 * (1.0 - frac);       // degree at step+1
        double w4 = 0.7 * (1.0 - frac);       // degree at step+2

        // Compute deg1 and best second step contribution
        int maxdeg1 = -1;
        for (int i = 0; i < cc; i++) {
            visited[vec[i].t1] = true;
            int d1 = 0;
            // compute degree after stepping to this cell
            d1 = deg_at(vec[i].ni, vec[i].nj);
            vec[i].deg1 = d1;
            maxdeg1 = max(maxdeg1, d1);

            // best second move from there
            double best2 = 0.0;
            bool has2 = false;
            for (int d2 = 0; d2 < 4; d2++) {
                int n2i = vec[i].ni + di[d2], n2j = vec[i].nj + dj[d2];
                if (!inb(n2i, n2j)) continue;
                int t2 = t[n2i][n2j];
                if (visited[t2]) continue;
                visited[t2] = true;
                int d2deg = deg_at(n2i, n2j);
                visited[t2] = false;
                double s2 = w2 * (double)p[n2i][n2j] + w4 * (double)d2deg;
                if (!has2 || s2 > best2) {
                    best2 = s2;
                    has2 = true;
                }
            }
            visited[vec[i].t1] = false;
            if (!has2) best2 = 0.0;

            vec[i].eval = w1 * (double)p[vec[i].ni][vec[i].nj] + w3 * (double)vec[i].deg1 + best2;
        }

        // Degree gating
        int gate = 0;
        if (frac < 0.60) gate = 2;
        else if (frac < 0.85) gate = 1;
        else gate = 0;
        gate = min(gate, maxdeg1);

        double bestEval = -1e100;
        int bestIdx = -1;
        for (int i = 0; i < cc; i++) {
            double e = vec[i].eval;
            if (vec[i].deg1 < gate) e -= 1e6; // penalize below gate
            // small random noise to break ties
            e += (double)((rng() % 1000)) * 1e-6;
            if (e > bestEval) {
                bestEval = e;
                bestIdx = i;
            }
        }
        if (bestIdx == -1) break;

        // Make the move
        int d = vec[bestIdx].dir;
        path.push_back(dc[d]);
        ci = vec[bestIdx].ni;
        cj = vec[bestIdx].nj;
        if (!visited[t[ci][cj]]) {
            visited[t[ci][cj]] = true;
            visitedTiles++;
        } else {
            // Should not happen, but break to be safe
            break;
        }
    }

    cout << path << '\n';
    return 0;
}