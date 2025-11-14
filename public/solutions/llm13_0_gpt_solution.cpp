#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int sx, sy;
    if (!(cin >> sx >> sy)) return 0;

    long long step = 0;
    while (true) {
        int xm = (int)(step % 1000) + 1;
        int ym = 1;
        cout << xm << " " << ym << "\n";
        cout.flush();

        int nx, ny;
        if (!(cin >> nx >> ny)) break;
        if (nx == 0 && ny == 0) break;

        ++step;
        if (step > 1000000) break; // safety
    }

    return 0;
}