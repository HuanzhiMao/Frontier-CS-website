#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n;
    if (!(cin >> n)) return 0;

    long long lowA = 1, lowB = 1;
    long long capA = n, capB = n;

    auto mid = [](long long l, long long r) -> long long {
        return l + (r - l) / 2;
    };

    const int MAX_QUERIES = 100000;

    for (int q = 0; q < MAX_QUERIES; ++q) {
        long long x, y;

        if (lowA <= capA) x = mid(lowA, capA);
        else x = lowA;

        if (lowB <= capB) y = mid(lowB, capB);
        else y = lowB;

        // Ensure within [1, n]
        x = max(1LL, min(x, n));
        y = max(1LL, min(y, n));

        cout << x << " " << y << endl;
        cout.flush();

        int res;
        if (!(cin >> res)) return 0;
        if (res == 0) {
            return 0;
        } else if (res == 1) {
            // x < a
            if (x + 1 > lowA) lowA = x + 1;
            if (lowA > n) lowA = n; // safety
        } else if (res == 2) {
            // y < b
            if (y + 1 > lowB) lowB = y + 1;
            if (lowB > n) lowB = n; // safety
        } else if (res == 3) {
            // x > a or y > b
            if (x - 1 < capA) capA = x - 1;
            if (y - 1 < capB) capB = y - 1;
            if (capA < 0) capA = 0; // cap can go below 1; keep non-negative for checks
            if (capB < 0) capB = 0;
        } else {
            // Unexpected response; terminate to avoid infinite loop
            return 0;
        }
    }

    return 0;
}