#include <bits/stdc++.h>
using namespace std;

struct Move {
    int x1, y1, x2, y2;
};

struct Edge {
    int px, py, cx, cy;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const int N = 30;
    vector<vector<int>> a(N);
    for (int x = 0; x < N; x++) {
        a[x].resize(x + 1);
        for (int y = 0; y <= x; y++) cin >> a[x][y];
    }

    const int LIMIT = 10000;
    vector<Move> ops;
    ops.reserve(LIMIT + 10);

    vector<Edge> q;
    q.reserve(200000);
    auto enqueueEdge = [&](int px, int py, int cx, int cy) {
        if (a[px][py] > a[cx][cy]) {
            q.push_back({px, py, cx, cy});
        }
    };
    auto enqueueAround = [&](int x, int y) {
        // Down edges
        if (x + 1 < N) {
            enqueueEdge(x, y, x + 1, y);
            enqueueEdge(x, y, x + 1, y + 1);
        }
        // Up edges (from parents to this)
        if (x > 0) {
            if (y > 0) enqueueEdge(x - 1, y - 1, x, y);
            if (y <= x - 1) enqueueEdge(x - 1, y, x, y);
        }
    };

    // Initialize queue with all violating parent->child edges
    for (int x = 0; x < N - 1; x++) {
        for (int y = 0; y <= x; y++) {
            if (a[x][y] > a[x + 1][y]) q.push_back({x, y, x + 1, y});
            if (a[x][y] > a[x + 1][y + 1]) q.push_back({x, y, x + 1, y + 1});
        }
    }

    size_t qi = 0;
    while (qi < q.size() && (int)ops.size() < LIMIT) {
        Edge e = q[qi++];
        int px = e.px, py = e.py;
        int cx = e.cx, cy = e.cy;
        if (a[px][py] <= a[cx][cy]) continue;

        // Swap parent and child
        swap(a[px][py], a[cx][cy]);
        ops.push_back({px, py, cx, cy});
        if ((int)ops.size() >= LIMIT) break;

        enqueueAround(px, py);
        enqueueAround(cx, cy);

        // Bubble up from the new position of the smaller value (now at (px,py))
        int x = px, y = py;
        while ((int)ops.size() < LIMIT) {
            if (x == 0) break;
            int best_px = -1, best_py = -1;
            int best_val = -1;

            if (y <= x - 1) {
                int vx = a[x - 1][y];
                if (vx > a[x][y]) {
                    best_val = vx;
                    best_px = x - 1;
                    best_py = y;
                }
            }
            if (y > 0) {
                int vx = a[x - 1][y - 1];
                if (vx > a[x][y]) {
                    if (vx > best_val) {
                        best_val = vx;
                        best_px = x - 1;
                        best_py = y - 1;
                    }
                }
            }
            if (best_px == -1) break;

            // Swap with the chosen parent
            swap(a[best_px][best_py], a[x][y]);
            ops.push_back({best_px, best_py, x, y});
            enqueueAround(best_px, best_py);
            enqueueAround(x, y);
            x = best_px; y = best_py;
        }
    }

    cout << ops.size() << '\n';
    for (auto &m : ops) {
        cout << m.x1 << ' ' << m.y1 << ' ' << m.x2 << ' ' << m.y2 << '\n';
    }
    return 0;
}