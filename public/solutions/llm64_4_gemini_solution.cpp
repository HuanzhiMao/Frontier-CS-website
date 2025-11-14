#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <bitset>
#include <limits>

using namespace std;

typedef long long ll;

struct Subset {
    ll sum;
    bitset<100> mask;

    bool operator<(const Subset& other) const {
        return sum < other.sum;
    }
};

int n;
ll T;
vector<ll> a;
vector<Subset> first_half_subsets;
ll min_diff;
bitset<100> best_mask;

void generate_first_half(int k, int end, ll current_sum, bitset<100>& current_mask) {
    if (k == end) {
        first_half_subsets.push_back({current_sum, current_mask});
        return;
    }
    generate_first_half(k + 1, end, current_sum, current_mask);
    current_mask[k] = 1;
    generate_first_half(k + 1, end, current_sum + a[k], current_mask);
    current_mask[k] = 0;
}

void find_closest_in_second_half(int k, int end, ll current_sum, bitset<100>& current_mask) {
    if (k == end) {
        ll target = T - current_sum;
        
        auto it = lower_bound(first_half_subsets.begin(), first_half_subsets.end(), Subset{target, bitset<100>()});

        if (it != first_half_subsets.end()) {
            ll diff = abs(it->sum + current_sum - T);
            if (diff < min_diff) {
                min_diff = diff;
                best_mask = current_mask | it->mask;
            }
        }
        
        if (it != first_half_subsets.begin()) {
            it--;
            ll diff = abs(it->sum + current_sum - T);
            if (diff < min_diff) {
                min_diff = diff;
                best_mask = current_mask | it->mask;
            }
        }
        return;
    }

    find_closest_in_second_half(k + 1, end, current_sum, current_mask);
    current_mask[k] = 1;
    find_closest_in_second_half(k + 1, end, current_sum + a[k], current_mask);
    current_mask[k] = 0;
}

void solve_greedy() {
    vector<pair<ll, int>> sorted_a(n);
    for(int i=0; i<n; ++i) sorted_a[i] = {a[i], i};
    
    // Greedy on descending order
    sort(sorted_a.rbegin(), sorted_a.rend());

    ll current_sum = 0;
    bitset<100> current_mask;
    
    for(int i=0; i<n; ++i) {
        if (abs(current_sum + sorted_a[i].first - T) < abs(current_sum - T)) {
            current_sum += sorted_a[i].first;
            current_mask[sorted_a[i].second] = 1;
        }
    }
    min_diff = abs(current_sum - T);
    best_mask = current_mask;

    // Greedy on ascending order
    sort(sorted_a.begin(), sorted_a.end());
    current_sum = 0;
    current_mask.reset();
    for(int i=0; i<n; ++i) {
        if (abs(current_sum + sorted_a[i].first - T) < abs(current_sum - T)) {
            current_sum += sorted_a[i].first;
            current_mask[sorted_a[i].second] = 1;
        }
    }
    if (abs(current_sum - T) < min_diff) {
        min_diff = abs(current_sum - T);
        best_mask = current_mask;
    }
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n >> T;
    a.resize(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    if (n <= 44) {
        min_diff = numeric_limits<ll>::max();
        best_mask.reset();

        ll current_min_diff = abs(T);
        if (current_min_diff < min_diff) {
            min_diff = current_min_diff;
            best_mask.reset();
        }

        int n1 = n / 2;
        bitset<100> mask;

        generate_first_half(0, n1, 0, mask);
        sort(first_half_subsets.begin(), first_half_subsets.end());
        
        find_closest_in_second_half(n1, n, 0, mask);
    } else {
        solve_greedy();
    }
    
    for (int i = 0; i < n; ++i) {
        cout << best_mask[i];
    }
    cout << endl;

    return 0;
}