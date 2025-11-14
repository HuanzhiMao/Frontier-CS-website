#include <bits/stdc++.h>
using namespace std;

int main() {
    int N, M;
    cin >> N >> M;
    vector<vector<bool>> has_dot(N, vector<bool>(N, false));
    for (int i = 0; i < M; i++) {
        int x, y;
        cin >> x >> y;
        has_dot[x][y] = true;
    }
    int c = (N - 1) / 2;
    vector<vector<long long>> ww(N, vector<long long>(N));
    for (int x = 0; x < N; x++) {
        for (int y = 0; y < N; y++) {
            ww[x][y] = 1LL * (x - c) * (x - c) + 1LL * (y - c) * (y - c) + 1;
        }
    }
    set<pair<pair<int, int>, pair<int, int>>> drawn;
    vector<vector<int>> ops;
    while (true) {
        long long best_w = -1;
        vector<int> best_op;
        // Axis-aligned 1x1
        for (int lx = 0; lx < N - 1; lx++) {
            for (int ly = 0; ly < N - 1; ly++) {
                vector<pair<int, int>> points = {{lx, ly}, {lx + 1, ly}, {lx, ly + 1}, {lx + 1, ly + 1}};
                int cnt = 0;
                vector<bool> hhas(4, false);
                for (int i = 0; i < 4; i++) {
                    int px = points[i].first, py = points[i].second;
                    hhas[i] = has_dot[px][py];
                    if (hhas[i]) cnt++;
                }
                if (cnt != 3) continue;
                int miss = -1;
                for (int i = 0; i < 4; i++) {
                    if (!hhas[i]) {
                        miss = i;
                        break;
                    }
                }
                auto p1 = points[miss];
                // Check edges
                vector<pair<pair<int, int>, pair<int, int>>> edges;
                auto add_edge = [&](pair<int, int> aa, pair<int, int> bb) {
                    if (aa > bb) swap(aa, bb);
                    edges.push_back({aa, bb});
                };
                add_edge(points[0], points[1]);
                add_edge(points[1], points[3]);
                add_edge(points[3], points[2]);
                add_edge(points[2], points[0]);
                bool can = true;
                for (auto e : edges) {
                    if (drawn.count(e)) {
                        can = false;
                        break;
                    }
                }
                if (!can) continue;
                // Orders for axis
                vector<vector<int>> orders = {{2, 3, 1}, {0, 2, 3}, {0, 1, 3}, {1, 0, 2}};
                vector<int> op = {points[miss].first, points[miss].second};
                for (int idx : orders[miss]) {
                    op.push_back(points[idx].first);
                    op.push_back(points[idx].second);
                }
                long long this_w = ww[p1.first][p1.second];
                if (this_w > best_w) {
                    best_w = this_w;
                    best_op = op;
                }
            }
        }
        // 45-degree small
        for (int cx = 1; cx < N - 1; cx++) {
            for (int cy = 1; cy < N - 1; cy++) {
                vector<pair<int, int>> points(4);
                points[0] = {cx - 1, cy}; // west
                points[1] = {cx, cy + 1}; // north
                points[2] = {cx + 1, cy}; // east
                points[3] = {cx, cy - 1}; // south
                int cnt = 0;
                vector<bool> hhas(4, false);
                for (int i = 0; i < 4; i++) {
                    int px = points[i].first, py = points[i].second;
                    hhas[i] = has_dot[px][py];
                    if (hhas[i]) cnt++;
                }
                if (cnt != 3) continue;
                int miss = -1;
                for (int i = 0; i < 4; i++) {
                    if (!hhas[i]) {
                        miss = i;
                        break;
                    }
                }
                auto p1 = points[miss];
                // Check edges
                vector<pair<pair<int, int>, pair<int, int>>> edges;
                auto add_edge = [&](int i, int j) {
                    auto aa = points[i], bb = points[j];
                    if (aa > bb) swap(aa, bb);
                    edges.push_back({aa, bb});
                };
                add_edge(0, 1);
                add_edge(1, 2);
                add_edge(2, 3);
                add_edge(3, 0);
                bool can = true;
                for (auto e : edges) {
                    if (drawn.count(e)) {
                        can = false;
                        break;
                    }
                }
                if (!can) continue;
                // Orders for 45
                vector<vector<int>> orders45 = {{3, 2, 1}, {0, 3, 2}, {1, 0, 3}, {2, 1, 0}};
                vector<int> op = {points[miss].first, points[miss].second};
                for (int idx : orders45[miss]) {
                    op.push_back(points[idx].first);
                    op.push_back(points[idx].second);
                }
                long long this_w = ww[p1.first][p1.second];
                if (this_w > best_w) {
                    best_w = this_w;
                    best_op = op;
                }
            }
        }
        if (best_w == -1) break;
        // Add the operation
        ops.push_back(best_op);
        // Place dot
        int x1 = best_op[0], y1 = best_op[1];
        has_dot[x1][y1] = true;
        // Add edges to drawn
        vector<pair<int, int>> ps = {{best_op[0], best_op[1]}, {best_op[2], best_op[3]}, {best_op[4], best_op[5]}, {best_op[6], best_op[7]}};
        for (int i = 0; i < 4; i++) {
            auto aa = ps[i], bb = ps[(i + 1) % 4];
            if (aa > bb) swap(aa, bb);
            drawn.insert({aa, bb});
        }
    }
    // Output
    cout << ops.size() << endl;
    for (auto& op : ops) {
        for (int i = 0; i < 8; i++) {
            if (i > 0) cout << " ";
            cout << op[i];
        }
        cout << endl;
    }
    return 0;
}