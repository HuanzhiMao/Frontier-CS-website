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
    auto sift_down = [&](auto&& self, int px, int py) -> void {
        int cx = px + 1;
        if (cx >= 30) return;
        int cy1 = py;
        int cy2 = py + 1;
        int min_cy = cy1;
        if (grid[cx][cy2] < grid[cx][cy1]) {
            min_cy = cy2;
        }
        if (grid[px][py] > grid[cx][min_cy]) {
            swaps.push_back({px, py, cx, min_cy});
            swap(grid[px][py], grid[cx][min_cy]);
            self(self, cx, min_cy);
        }
    };
    for (int x = 28; x >= 0; x--) {
        for (int y = 0; y <= x; y++) {
            sift_down(sift_down, x, y);
        }
    }
    cout << swaps.size() << endl;
    for (auto& s : swaps) {
        cout << s[0] << " " << s[1] << " " << s[2] << " " << s[3] << endl;
    }
    return 0;
}