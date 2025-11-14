#include <bits/stdc++.h>
using namespace std;

struct Op {
    int x1, y1, x2, y2;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const int N = 30;
    vector<vector<int>> a(N);
    for (int x = 0; x < N; ++x) {
        a[x].resize(x + 1);
        for (int y = 0; y <= x; ++y) {
            cin >> a[x][y];
        }
    }

    auto valid = [&](int x, int y) -> bool {
        return (0 <= x && x < N && 0 <= y && y <= x);
    };

    vector<Op> ops;
    ops.reserve(10000);

    struct Item {
        int diff, x, y, dir; // dir: 0 -> child (x+1, y), 1 -> child (x+1, y+1)
    };
    struct Cmp {
        bool operator()(const Item& a, const Item& b) const {
            return a.diff < b.diff; // max-heap by diff
        }
    };
    priority_queue<Item, vector<Item>, Cmp> pq;

    auto pushEdge = [&](int x, int y, int dir) {
        if (!valid(x, y)) return;
        int x2 = x + 1, y2 = y + dir;
        if (!valid(x2, y2)) return;
        int d = a[x][y] - a[x2][y2];
        if (d > 0) pq.push({d, x, y, dir});
    };

    auto recordSwap = [&](int x1, int y1, int x2, int y2) {
        swap(a[x1][y1], a[x2][y2]);
        ops.push_back({x1, y1, x2, y2});
    };

    auto afterSwap = [&](int x1, int y1, int x2, int y2) {
        auto pushAround = [&](int x, int y) {
            if (x - 1 >= 0 && y <= x - 1) pushEdge(x - 1, y, 0);
            if (x - 1 >= 0 && y - 1 >= 0) pushEdge(x - 1, y - 1, 1);
            if (x <= N - 2) {
                pushEdge(x, y, 0);
                pushEdge(x, y, 1);
            }
        };
        pushAround(x1, y1);
        pushAround(x2, y2);
    };

    // Initialize all violating edges
    for (int x = 0; x <= N - 2; ++x) {
        for (int y = 0; y <= x; ++y) {
            pushEdge(x, y, 0);
            pushEdge(x, y, 1);
        }
    }

    const int K_LIMIT = 10000;

    auto bubbleUp = [&](int ux, int uy) {
        while ((int)ops.size() < K_LIMIT) {
            int bestpx = -1, bestpy = -1, bestVal = INT_MIN;
            if (valid(ux - 1, uy) && a[ux - 1][uy] > a[ux][uy]) {
                bestpx = ux - 1; bestpy = uy; bestVal = a[bestpx][bestpy];
            }
            if (valid(ux - 1, uy - 1) && a[ux - 1][uy - 1] > a[ux][uy]) {
                if (a[ux - 1][uy - 1] > bestVal) {
                    bestpx = ux - 1; bestpy = uy - 1; bestVal = a[bestpx][bestpy];
                }
            }
            if (bestpx == -1) break;
            recordSwap(bestpx, bestpy, ux, uy);
            afterSwap(bestpx, bestpy, ux, uy);
            ux = bestpx; uy = bestpy;
        }
    };

    auto siftDown = [&](int cx, int cy) {
        while ((int)ops.size() < K_LIMIT && cx < N - 1) {
            int nx = cx + 1;
            int lefty = cy, righty = cy + 1;
            int bestChild = -1;
            int leftVal = INT_MAX, rightVal = INT_MAX;
            if (valid(nx, lefty)) leftVal = a[nx][lefty];
            if (valid(nx, righty)) rightVal = a[nx][righty];
            if (a[cx][cy] > min(leftVal, rightVal)) {
                if (leftVal <= rightVal) {
                    bestChild = 0;
                } else {
                    bestChild = 1;
                }
                int ny = (bestChild == 0 ? lefty : righty);
                recordSwap(cx, cy, nx, ny);
                afterSwap(cx, cy, nx, ny);
                cx = nx; cy = ny;
            } else {
                break;
            }
        }
    };

    while (!pq.empty() && (int)ops.size() < K_LIMIT) {
        Item it = pq.top(); pq.pop();
        int x = it.x, y = it.y, dir = it.dir;
        if (!valid(x, y) || !valid(x + 1, y + dir)) continue;
        int d = a[x][y] - a[x + 1][y + dir];
        if (d <= 0) continue;

        if ((int)ops.size() >= K_LIMIT) break;
        // Initial swap on the violating edge
        recordSwap(x, y, x + 1, y + dir);
        afterSwap(x, y, x + 1, y + dir);

        if ((int)ops.size() >= K_LIMIT) break;
        // Bubble the smaller value upward from (x, y)
        bubbleUp(x, y);

        if ((int)ops.size() >= K_LIMIT) break;
        // Sift the larger value downward from (x+1, y+dir)
        siftDown(x + 1, y + dir);
    }

    cout << ops.size() << '\n';
    for (auto &op : ops) {
        cout << op.x1 << ' ' << op.y1 << ' ' << op.x2 << ' ' << op.y2 << '\n';
    }

    return 0;
}