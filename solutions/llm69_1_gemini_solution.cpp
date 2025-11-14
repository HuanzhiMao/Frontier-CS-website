#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <utility>
#include <algorithm>

using namespace std;

// Calculates the number of distinct substrings for S = O^{au}X^{bu}O^{av}X^{bv}.
long long calculate_power(long long au, long long bu, long long av, long long bv) {
    long long max_a = max(au, av);
    long long max_b = max(bu, bv);

    // Count substrings of type O+X+
    long long term_ox;
    if (au == av && bu == bv) {
        // If u=v, w_u and w_v are the same, so O+X+ substrings only come from one source.
        term_ox = au * bu;
    } else {
        // If u!=v, the pairs (au,bu) and (av,bv) are different, so the sets of O+X+ substrings are disjoint.
        term_ox = au * bu + av * bv;
    }

    // Count substrings of other types. These are always unique across patterns.
    long long term_xo = bu * av;
    long long term_oxo = au * bu * av;
    long long term_xox = bu * av * bv;
    long long term_oxox = au * bu * av * bv;

    return max_a + max_b + term_ox + term_xo + term_oxo + term_xox + term_oxox;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    // We choose a_i = i and b_i = n+i.
    // For i in [1,n], a_i is in [1,n] and b_i is in [n+1, 2n].
    // These value ranges are disjoint, simplifying substring counting.
    vector<pair<int, int>> params(n + 1);
    for (int i = 1; i <= n; ++i) {
        params[i] = {i, n + i};
    }
    
    // Phase 1: Output the generated magic words.
    for (int i = 1; i <= n; ++i) {
        cout << string(params[i].first, 'O') << string(params[i].second, 'X') << "\n";
    }
    cout << flush;

    int q;
    cin >> q;

    // Precompute the power for all n*n pairs and store in a map.
    map<long long, pair<int, int>> power_to_indices;
    for (int u = 1; u <= n; ++u) {
        for (int v = 1; v <= n; ++v) {
            long long au = params[u].first;
            long long bu = params[u].second;
            long long av = params[v].first;
            long long bv = params[v].second;
            long long power = calculate_power(au, bu, av, bv);
            power_to_indices[power] = {u, v};
        }
    }

    // Phase 2: Answer queries using the precomputed map.
    for (int i = 0; i < q; ++i) {
        long long p;
        cin >> p;
        pair<int, int> result = power_to_indices.at(p);
        cout << result.first << " " << result.second << "\n";
        cout << flush;
    }

    return 0;
}