#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include <set>
#include <chrono>

using namespace std;

int ask_query(int x, const vector<int>& s) {
    if (s.empty()) {
        return 0;
    }
    cout << "? " << x << " " << s.size();
    for (int idx : s) {
        cout << " " << idx;
    }
    cout << endl;
    int response;
    cin >> response;
    if (response == -1) {
        exit(0);
    }
    return response;
}

void solve() {
    const int n = 300;
    const int array_size = 2 * n - 1;

    vector<int> candidates(n);
    iota(candidates.begin(), candidates.end(), 1);

    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

    while (candidates.size() > 1) {
        vector<int> p1, p2;
        for (int i = 1; i <= array_size; ++i) {
            if (uniform_int_distribution<int>(0, 1)(rng)) {
                p1.push_back(i);
            } else {
                p2.push_back(i);
            }
        }
        
        if (p1.empty() || p2.empty()) {
            continue;
        }
        
        vector<int> next_candidates;
        for (int x : candidates) {
            bool in_p1 = ask_query(x, p1);
            bool in_p2 = ask_query(x, p2);
            if (in_p1 != in_p2) {
                next_candidates.push_back(x);
            }
        }
        candidates = next_candidates;
    }

    cout << "! " << candidates[0] << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int t;
    cin >> t;
    while (t--) {
        int n_val;
        cin >> n_val;
        if (n_val == -1) {
            return 0; 
        }
        solve();
    }

    return 0;
}