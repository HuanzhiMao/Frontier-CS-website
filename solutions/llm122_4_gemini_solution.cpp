#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <algorithm>

using namespace std;

long long ceil_div(long long a, long long b) {
    if (b == 0) return 2e9; // Represents infinity
    return (a + b - 1) / b;
}

void solve() {
    // First query: 100000 words of length 1
    int n1 = 100000;
    cout << "? " << n1;
    for (int i = 0; i < n1; ++i) {
        cout << " 1";
    }
    cout << endl;

    long long L1;
    cin >> L1;
    if (L1 == -1) return;

    if (L1 == 1) {
        cout << "! 100000" << endl;
        return;
    }
    
    // Determine the range of possible W values from L1
    // We know ceil(100000 / W) = L1
    long long W_min, W_max;

    // Binary search for W_min, the smallest W such that ceil(100000/W) <= L1
    long long low = 1, high = 100000, mid;
    W_min = high;
    while(low <= high) {
        mid = low + (high - low) / 2;
        if (mid == 0) {
            low = 1;
            continue;
        }
        if (ceil_div(100000, mid) <= L1) {
            W_min = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    // Binary search for W_max, the largest W such that ceil(100000/W) >= L1
    low = 1, high = 100000;
    W_max = low;
    while(low <= high) {
        mid = low + (high - low) / 2;
        if (mid == 0) {
           low = 1;
           continue;
        }
        if (ceil_div(100000, mid) >= L1) {
            W_max = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    if (W_min == W_max) {
        cout << "! " << W_min << endl;
        return;
    }
    
    // Second query
    int k = W_min;
    int num_ones = W_max - W_min + 2; 
    if (num_ones > 100000 - 1) num_ones = 100000 - 1;

    vector<int> a2;
    a2.push_back(k);
    for (int i = 0; i < num_ones; ++i) {
        a2.push_back(1);
    }
    int n2 = a2.size();

    cout << "? " << n2;
    for (int x : a2) {
        cout << " " << x;
    }
    cout << endl;
    
    long long L2;
    cin >> L2;
    if (L2 == -1) return;

    for (long long w_cand = W_min; w_cand <= W_max; ++w_cand) {
        long long predicted_L2;
        if (k > w_cand) {
            predicted_L2 = 0;
        } else {
            long long lines = 1;
            long long rem_space = w_cand - k;
            long long ones_on_first_line = min((long long)num_ones, rem_space);
            long long rem_ones = num_ones - ones_on_first_line;
            if (rem_ones > 0) {
                lines += ceil_div(rem_ones, w_cand);
            }
            predicted_L2 = lines;
        }

        if (predicted_L2 == L2) {
            cout << "! " << w_cand << endl;
            return;
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.flush();
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}