#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<long long> vals;
    long long x;
    while (cin >> x) vals.push_back(x);

    if (vals.empty()) return 0;
    int idx = 0;
    int t = (int)vals[idx++];

    for (int tc = 0; tc < t; ++tc) {
        if (idx >= (int)vals.size()) break;
        long long n = vals[idx++];

        // Attempt to consume a possible "handshake" integer if present.
        if (idx < (int)vals.size()) {
            long long maybe = vals[idx];
            if (maybe == -1 || maybe == 0 || maybe == 1) {
                ++idx;
            }
        }

        cout << "! " << 1 << "\n";
        cout.flush();
    }

    return 0;
}