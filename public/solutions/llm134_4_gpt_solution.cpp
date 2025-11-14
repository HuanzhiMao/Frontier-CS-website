#include <bits/stdc++.h>
using namespace std;

using int64 = long long;

int64 n;
int query_cnt = 0;

int ask(int64 x, int64 y) {
    x = max<int64>(1, min<int64>(x, n));
    y = max<int64>(1, min<int64>(y, n));
    cout << x << " " << y << endl;
    cout.flush();
    int r;
    if (!(cin >> r)) exit(0);
    query_cnt++;
    if (r == 0) exit(0);
    return r;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    if (!(cin >> n)) return 0;

    int64 LA = 1, UA = n;
    int64 LB = 1, UB = n;

    while (true) {
        if (LA > UA) LA = UA;
        if (LB > UB) LB = UB;

        // If one coordinate is known, binary search the other robustly
        if (LB == UB) {
            int64 y = LB;
            int64 l = LA, r = UA;
            while (l <= r) {
                int64 mid = l + (r - l) / 2;
                int res = ask(mid, y);
                if (res == 1) {
                    l = mid + 1;
                } else if (res == 3) {
                    r = mid - 1;
                } else if (res == 0) {
                    return 0;
                } else {
                    // res == 2 should not happen if y == b; ignore and adjust
                    // try to move l to avoid infinite loop
                    l = max(l, mid + 1);
                }
            }
            // final confirmation
            ask(l, y);
            return 0;
        }
        if (LA == UA) {
            int64 x = LA;
            int64 l = LB, r = UB;
            while (l <= r) {
                int64 mid = l + (r - l) / 2;
                int res = ask(x, mid);
                if (res == 2) {
                    l = mid + 1;
                } else if (res == 3) {
                    r = mid - 1;
                } else if (res == 0) {
                    return 0;
                } else {
                    // res == 1 should not happen if x == a; ignore and adjust
                    l = max(l, mid + 1);
                }
            }
            ask(x, l);
            return 0;
        }

        int64 x = LA + (UA - LA) / 2;
        int64 y = LB + (UB - LB) / 2;

        int res = ask(x, y);
        if (res == 0) return 0;
        if (res == 1) {
            LA = x + 1;
            continue;
        }
        if (res == 2) {
            LB = y + 1;
            continue;
        }
        // res == 3, try to disambiguate with auxiliary queries
        // First try to see if x > a using y = LB to avoid 3 due to y.
        int res2 = ask(x, LB);
        if (res2 == 3) {
            // Likely x > a
            UA = min(UA, x - 1);
            continue;
        }
        // Try to see if y > b using x = LA to avoid 3 due to x.
        int res3 = ask(LA, y);
        if (res3 == 3) {
            UB = min(UB, y - 1);
            continue;
        }
        // If neither gave 3, use obtained info to progress
        // Prefer larger jumps when possible
        bool progressed = false;
        if (res2 == 1) { // x < a
            LA = max(LA, x + 1);
            progressed = true;
        } else if (res2 == 2) { // LB < b
            // small progress; try use res3 to get larger LB jump
            if (res3 == 2) {
                LB = max(LB, y + 1);
                progressed = true;
            }
        }
        if (!progressed) {
            if (res3 == 1) {
                LA = max(LA, LA + 1);
            } else if (res3 == 2) {
                LB = max(LB, y + 1);
            } else {
                // As a fallback, shrink conservatively
                if (UA > LA) UA--;
                if (UB > LB) UB--;
            }
        }
    }
}