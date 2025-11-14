#include <bits/stdc++.h>
using namespace std;

int main() {
    vector<vector<int>> grid(30);
    for (int x = 0; x < 30; x++) {
        grid[x].resize(x + 1);
        for (int y = 0; y <= x; y++) {
            cin >> grid[x][y];
        }
    }
    vector<vector<int>> swaps;
    auto heapify = [&](int px, int py) {
        int curx = px, cury = py;
        while (true) {
            int minx = curx, miny = cury;
            int lx = curx + 1;
            if (lx >= 30) break;
            int ly = cury;
            if (grid[lx][ly] < grid[minx][miny]) {
                minx = lx;
                miny = ly;
            }
            int rx = curx + 1, ry = cury + 1;
            if (grid[rx][ry] < grid[minx][miny]) {
                minx = rx;
                miny = ry;
            }
            if (minx == curx) break;
            swaps.push_back({curx, cury, minx, miny});
            swap(grid[curx][cury], grid[minx][miny]);
            curx = minx;
            cury = miny;
        }
    };
    for (int lev = 28; lev >= 0; lev--) {
        for (int y = 0; y <= lev; y++) {
            heapify(lev, y);
        }
    }
    cout << swaps.size() << endl;
    for (auto& s : swaps) {
        cout << s[0] << " " << s[1] << " " << s[2] << " " << s[3] << endl;
    }
    return 0;
}