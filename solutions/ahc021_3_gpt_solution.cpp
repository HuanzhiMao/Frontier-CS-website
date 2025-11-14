#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const int N = 30;
    vector<vector<int>> b(N);
    for (int x = 0; x < N; ++x) {
        b[x].resize(x + 1);
        for (int y = 0; y <= x; ++y) {
            cin >> b[x][y];
        }
    }

    struct Op { int x1, y1, x2, y2; };
    vector<Op> ops;
    ops.reserve(5000);

    auto do_swap = [&](int x1, int y1, int x2, int y2) {
        swap(b[x1][y1], b[x2][y2]);
        ops.push_back({x1, y1, x2, y2});
    };

    // Bottom-up heapify (Floyd's method) on the triangular binary heap
    for (int x = N - 2; x >= 0; --x) {
        for (int y = 0; y <= x; ++y) {
            int cx = x, cy = y;
            while (cx < N - 1) {
                int bx = cx, by = cy;
                int bestVal = b[cx][cy];
                // child (cx+1, cy)
                if (b[cx + 1][cy] < bestVal) {
                    bestVal = b[cx + 1][cy];
                    bx = cx + 1;
                    by = cy;
                }
                // child (cx+1, cy+1)
                if (b[cx + 1][cy + 1] < bestVal) {
                    bestVal = b[cx + 1][cy + 1];
                    bx = cx + 1;
                    by = cy + 1;
                }
                if (bestVal < b[cx][cy]) {
                    do_swap(cx, cy, bx, by);
                    cx = bx; cy = by;
                } else break;
            }
        }
    }

    cout << ops.size() << '\n';
    for (auto &op : ops) {
        cout << op.x1 << ' ' << op.y1 << ' ' << op.x2 << ' ' << op.y2 << '\n';
    }

    return 0;
}