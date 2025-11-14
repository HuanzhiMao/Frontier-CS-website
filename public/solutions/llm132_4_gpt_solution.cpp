#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int R, H;
    if (!(cin >> R >> H)) return 0;

    // Choose primes whose product exceeds 999*999 to guarantee uniqueness
    vector<int> primes = {2, 5, 7, 11, 13, 17, 19};
    vector<pair<int,int>> query_info;

    // Send robots: for each prime p and residue r, query all positions x in [1..1000] with x % p == r
    for (int ip = 0; ip < (int)primes.size(); ++ip) {
        int p = primes[ip];
        for (int r = 0; r < p; ++r) {
            vector<int> items;
            items.reserve(1000 / p + 2);
            for (int x = 1; x <= 1000; ++x) {
                if (x % p == r) items.push_back(x);
            }
            cout << "? " << items.size();
            for (int v : items) cout << ' ' << v;
            cout << '\n';
            cout.flush();
            query_info.emplace_back(ip, r);
        }
    }

    // Get responses
    cout << "@\n";
    cout.flush();

    int L;
    if (!(cin >> L)) return 0;
    vector<int> ret(L);
    for (int i = 0; i < L; ++i) cin >> ret[i];

    // Map responses to per-prime residue presence
    vector<vector<int>> has(primes.size());
    for (int ip = 0; ip < (int)primes.size(); ++ip) has[ip].assign(primes[ip], 0);
    for (int i = 0; i < L; ++i) {
        int ip = query_info[i].first;
        int r = query_info[i].second;
        has[ip][r] = ret[i];
    }

    // Find candidates: numbers whose residues match one of the detected residues for every prime
    vector<int> cand;
    for (int x = 1; x <= 1000; ++x) {
        bool ok = true;
        for (int ip = 0; ip < (int)primes.size(); ++ip) {
            int p = primes[ip];
            if (has[ip][x % p] == 0) { ok = false; break; }
        }
        if (ok) cand.push_back(x);
    }

    // Output the two chairmen positions (a=b allowed)
    if (cand.empty()) {
        // Fallback, shouldn't happen with the chosen primes
        cout << "! 1 1\n";
        cout.flush();
        return 0;
    } else if (cand.size() == 1) {
        cout << "! " << cand[0] << ' ' << cand[0] << '\n';
        cout.flush();
        return 0;
    } else {
        // Usually exactly two
        cout << "! " << cand[0] << ' ' << cand[1] << '\n';
        cout.flush();
        return 0;
    }
}