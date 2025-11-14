#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int sx, sy;
    if (!(cin >> sx >> sy)) return 0;

    vector<pair<int,int>> moves;
    const int T = 1000;

    if (sx <= 1) {
        // Use right boundary as the wall
        moves.push_back({2, 1});
        moves.push_back({2, 2});
        moves.push_back({1, 2});
    } else if (sx >= T) {
        // Use left boundary as the wall
        moves.push_back({T-1, 1});
        moves.push_back({T-1, 2});
        moves.push_back({T, 2});
    } else {
        // General case: create a trap around column sx
        moves.push_back({sx + 1, 1});
        moves.push_back({sx - 1, 1});
        moves.push_back({sx + 1, 2});
        moves.push_back({sx - 1, 2});
        moves.push_back({sx, 2});
    }

    for (auto [x, y] : moves) {
        cout << x << ' ' << y << '\n' << flush;
        int nx, ny;
        if (!(cin >> nx >> ny)) return 0;
        if (nx == 0 && ny == 0) return 0;
    }

    return 0;
}