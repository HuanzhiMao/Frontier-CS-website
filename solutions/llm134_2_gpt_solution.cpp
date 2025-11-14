#include <bits/stdc++.h>
using namespace std;

using int64 = long long;
using i128 = __int128_t;

int64 n;

int64 ask(int64 x, int64 y) {
    cout << x << " " << y << endl;
    cout.flush();
    int64 r;
    if (!(cin >> r)) exit(0);
    if (r == 0) exit(0);
    return r;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    if (!(cin >> n)) return 0;

    int64 LA = 1, RA = n, LB = 1, RB = n;

    auto midA = [&](int64 l, int64 r)->int64 {
        return (int64)(((i128)l + (i128)r) / 2);
    };
    auto midB = [&](int64 l, int64 r)->int64 {
        return (int64)(((i128)l + (i128)r) / 2);
    };

    while (true) {
        // If both determined, confirm and exit
        if (LA == RA && LB == RB) {
            ask(LA, LB); // This must return 0 and exit
            return 0;
        }

        // If a is determined, binary search b using x = a
        if (LA == RA && LB < RB) {
            int64 y = midB(LB, RB);
            int64 r = ask(LA, y);
            if (r == 2) {
                LB = y + 1;
            } else if (r == 3) {
                RB = y - 1;
            } else if (r == 1) {
                // Should not happen if LA == a
                // But if interactor returns 1, we can't trust it, just adjust LB minimally
                if (LB < RB) LB = max(LB, y + 1);
            }
            continue;
        }

        // If b is determined, binary search a using y = b
        if (LB == RB && LA < RA) {
            int64 x = midA(LA, RA);
            int64 r = ask(x, LB);
            if (r == 1) {
                LA = x + 1;
            } else if (r == 3) {
                RA = x - 1;
            } else if (r == 2) {
                // Should not happen if LB == b
                // But if interactor returns 2, we can't trust it, just adjust LA minimally
                if (LA < RA) LA = max(LA, x + 1);
            }
            continue;
        }

        // Both ranges not determined yet: try to shrink both
        int64 x = midA(LA, RA);
        int64 y = midB(LB, RB);
        int64 r = ask(x, y);
        if (r == 1) {
            LA = x + 1;
            continue;
        } else if (r == 2) {
            LB = y + 1;
            continue;
        } else if (r == 3) {
            // Disambiguate which side caused '3'
            // First test x against a with y at LB to avoid y>b in '3'
            int64 r2 = ask(x, LB);
            if (r2 == 3) {
                // x > a
                RA = x - 1;
                continue;
            }
            // Now test y against b with x at LA to avoid x>a in '3'
            int64 r3 = ask(LA, y);
            if (r3 == 3) {
                // y > b
                RB = y - 1;
                continue;
            }
            // If neither returned 3, it means LA < a and LB < b (both true),
            // so we can safely increase both lower bounds slightly.
            if (LA < RA) LA = LA + 1;
            if (LB < RB) LB = LB + 1;
            continue;
        }
    }

    return 0;
}