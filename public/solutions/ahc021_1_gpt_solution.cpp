#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    const int N = 30;
    vector<vector<int>> b(N);
    for (int x = 0; x < N; ++x) {
        b[x].resize(x + 1);
        for (int y = 0; y <= x; ++y) cin >> b[x][y];
    }

    struct Edge { int ax, ay, bx, by; };
    vector<Edge> edges;
    for (int x = 0; x < N; ++x) {
        for (int y = 0; y <= x; ++y) {
            if (y + 1 <= x) edges.push_back({x, y, x, y + 1});
            if (x + 1 < N) {
                edges.push_back({x, y, x + 1, y});
                edges.push_back({x, y, x + 1, y + 1});
            }
        }
    }

    mt19937 rng(712367);
    shuffle(edges.begin(), edges.end(), rng);

    auto Edown = [&](int x, int y)->int {
        if (x + 1 >= N) return 0;
        int v = b[x][y];
        int s = 0;
        if (v > b[x + 1][y]) ++s;
        if (v > b[x + 1][y + 1]) ++s;
        return s;
    };

    auto calc_delta = [&](const Edge &e)->int {
        int ax = e.ax, ay = e.ay, bx = e.bx, by = e.by;
        pair<int,int> P[6];
        int cnt = 0;
        auto addP = [&](int x, int y) {
            if (x < 0 || y < 0 || y > x || x >= N) return;
            for (int i = 0; i < cnt; ++i) if (P[i].first == x && P[i].second == y) return;
            P[cnt++] = {x, y};
        };
        addP(ax, ay);
        addP(bx, by);
        addP(ax - 1, ay - 1);
        addP(ax - 1, ay);
        addP(bx - 1, by - 1);
        addP(bx - 1, by);

        int before = 0;
        for (int i = 0; i < cnt; ++i) before += Edown(P[i].first, P[i].second);

        swap(b[ax][ay], b[bx][by]);

        int after = 0;
        for (int i = 0; i < cnt; ++i) after += Edown(P[i].first, P[i].second);

        swap(b[ax][ay], b[bx][by]);

        return after - before;
    };

    const int KMAX = 10000;
    vector<array<int,4>> ops;
    ops.reserve(KMAX);

    for (int iter = 0; iter < KMAX; ++iter) {
        int bestDelta = 0;
        int bestIdx = -1;
        for (int i = 0; i < (int)edges.size(); ++i) {
            int d = calc_delta(edges[i]);
            if (d < bestDelta) {
                bestDelta = d;
                bestIdx = i;
            }
        }
        if (bestIdx == -1) break;
        auto &e = edges[bestIdx];
        swap(b[e.ax][e.ay], b[e.bx][e.by]);
        ops.push_back({e.ax, e.ay, e.bx, e.by});
        if ((int)ops.size() >= KMAX) break;
    }

    cout << ops.size() << '\n';
    for (auto &op : ops) {
        cout << op[0] << ' ' << op[1] << ' ' << op[2] << ' ' << op[3] << '\n';
    }
    return 0;
}