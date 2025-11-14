#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int sx, sy;
    if (!(cin >> sx >> sy)) return 0;

    int xi = 1, yi = 1;
    while (true) {
        cout << xi << ' ' << yi << '\n';
        cout.flush();

        int nx, ny;
        if (!(cin >> nx >> ny)) break;
        if (nx == 0 && ny == 0) break;

        xi++;
        if (xi > 1000) {
            xi = 1;
            yi++;
            if (yi > 1000) yi = 1;
        }
    }

    return 0;
}