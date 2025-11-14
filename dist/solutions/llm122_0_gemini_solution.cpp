#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <algorithm>

using namespace std;

// Function to send a query to the judge
long long ask(const vector<long long>& a) {
    if (a.empty()) {
        cout << "? 0" << endl;
    } else {
        cout << "? " << a.size();
        for (long long x : a) {
            cout << " " << x;
        }
        cout << endl;
    }
    long long response;
    cin >> response;
    // Exit immediately if the judge returns -1 (error)
    if (response == -1) exit(0);
    return response;
}

// Function to submit the final answer
void answer(long long w) {
    cout << "! " << w << endl;
}

void solve() {
    // Query 1: Use a large number of '1's to find a range for W.
    long long N_first = 100000;
    vector<long long> first_query_a(N_first, 1);
    long long l1 = ask(first_query_a);

    // If l1 is 1, W must be at least 100000. Given W <= 100000, W must be 100000.
    if (l1 == 1) {
        answer(N_first);
        return;
    }

    // Calculate the range [w_min, w_max] for W using integer arithmetic.
    // l1 = ceil(N/W)  =>  l1-1 < N/W <= l1  =>  N/l1 <= W < N/(l1-1)
    long long w_min = (N_first + l1 - 1) / l1;
    long long w_max = (N_first - 1) / (l1 - 1);
    
    // The range could be a single value.
    if (w_min == w_max) {
        answer(w_min);
        return;
    }

    // Query 2: Construct an article to pinpoint W within [w_min, w_max].
    long long k = w_max - w_min;
    vector<long long> second_query_a;
    second_query_a.push_back(w_min);
    for (int i = 0; i < k; ++i) {
        second_query_a.push_back(1);
    }

    long long l2 = ask(second_query_a);
    
    // Based on the response l2, calculate the exact value of W.
    // l2 = 1 + ceil((w_max - W) / W)  => W = floor(w_max / l2)
    long long w_guess = w_max / l2;
    
    answer(w_guess);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}