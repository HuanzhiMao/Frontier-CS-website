#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

typedef long long ll;

vector<ll> generate_permutation(ll k) {
    if (k == 1) {
        return {};
    }
    if (k == 2) {
        return {0};
    }

    if (k % 2 == 0) {
        // Operation to get 2*k from k
        vector<ll> p_half = generate_permutation(k / 2);
        vector<ll> res;
        res.push_back(0);
        for (ll val : p_half) {
            res.push_back(val + 1);
        }
        return res;
    } else { // k is odd
        // Operation to get k+1 from k
        vector<ll> p_minus_one = generate_permutation(k - 1);
        vector<ll> res;
        ll n = p_minus_one.size();
        res.push_back(n);
        for (ll val : p_minus_one) {
            res.push_back(val);
        }
        return res;
    }
}

void solve() {
    ll k;
    cin >> k;

    vector<ll> p = generate_permutation(k);
    cout << p.size() << "\n";
    for (size_t i = 0; i < p.size(); ++i) {
        cout << p[i] << (i == p.size() - 1 ? "" : " ");
    }
    cout << "\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int q;
    cin >> q;
    while (q--) {
        solve();
    }
    return 0;
}