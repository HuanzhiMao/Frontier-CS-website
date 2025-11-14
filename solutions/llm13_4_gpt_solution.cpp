#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    const int T = 1000;
    int sx, sy;
    if (!(cin >> sx >> sy)) return 0;
    int rx = sx, ry = sy;

    auto print_and_read = [&](int x, int y)->bool {
        x = max(1, min(T, x));
        y = max(1, min(T, y));
        cout << x << ' ' << y << '\n';
        cout.flush();
        int nx, ny;
        if (!(cin >> nx >> ny)) return false;
        if (nx == 0 && ny == 0) return false;
        rx = nx; ry = ny;
        return true;
    };

    // Choose a trap center near the initial x, away from borders
    int cx;
    if (sx <= 1) cx = 2;
    else if (sx >= T) cx = T - 1;
    else cx = sx;

    // Build a 5-cell trap around (cx,1)
    vector<pair<int,int>> trap;
    trap.push_back({cx + 1, 1});
    trap.push_back({cx - 1, 1});
    trap.push_back({cx + 1, 2});
    trap.push_back({cx - 1, 2});
    trap.push_back({cx, 2});

    for (auto [x, y] : trap) {
        if (!print_and_read(x, y)) return 0;
    }

    // After building the trap, try to push the robot downward.
    // Alternate blocking above and diagonal toward center to encourage descent.
    for (int step = 0; step < 3000; ++step) {
        if (ry == 1) {
            if (rx >= cx - 1 && rx <= cx + 1) {
                // Robot is under the trap; any move will explode. Just print the trap center again.
                if (!print_and_read(cx, 2)) return 0;
            } else {
                // If far from trap, try to rebuild a new trap around current rx
                int ncx = rx;
                if (ncx <= 1) ncx = 2;
                else if (ncx >= T) ncx = T - 1;
                if (ncx != cx) {
                    cx = ncx;
                    vector<pair<int,int>> new_trap = {
                        {cx + 1, 1}, {cx - 1, 1}, {cx + 1, 2}, {cx - 1, 2}, {cx, 2}
                    };
                    for (auto [x, y] : new_trap) {
                        if (!print_and_read(x, y)) return 0;
                    }
                    continue;
                }
                // Otherwise keep blocking above to avoid going up
                if (!print_and_read(rx, 2)) return 0;
            }
        } else {
            // ry > 1: alternate blocking above and diagonal toward center
            if (step % 2 == 0) {
                if (!print_and_read(rx, ry + 1)) return 0;
            } else {
                int dir = 0;
                if (rx > cx) dir = -1;
                else if (rx < cx) dir = 1;
                if (!print_and_read(rx + dir, ry + 1)) return 0;
            }
        }
    }
    return 0;
}