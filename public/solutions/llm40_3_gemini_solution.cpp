#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <cmath>
#include <algorithm>

using namespace std;

// Function to perform a query
long long query(const vector<int>& indices) {
    if (indices.empty()) {
        return 0;
    }
    cout << "0 " << indices.size();
    for (int idx : indices) {
        cout << " " << idx;
    }
    cout << endl;
    long long response;
    cin >> response;
    if (response == -1) exit(0); // Exit on error
    return response;
}

// Function to output the final answer
void answer(const string& s) {
    cout << "1 " << s << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    string s(n, ' ');

    vector<int> all_indices(n);
    iota(all_indices.begin(), all_indices.end(), 1);

    long long total_rbs = query(all_indices);

    // Case 1: s is of the form ")))(((", detected by f(s) = 0
    if (total_rbs == 0) {
        // s_n must be '(', use it as an anchor
        s[n - 1] = '(';
        int last_close_idx = 0; // 1-based index of the last ')'
        int l = 1, r = n - 1;
        while (l <= r) {
            int mid = l + (r - l) / 2;
            vector<int> q_indices = {n, mid};
            // Query f(s_n, s_mid). It's 1 iff s_mid is ')'
            if (query(q_indices) == 1) { 
                last_close_idx = mid;
                l = mid + 1;
            } else {
                r = mid - 1;
            }
        }

        for (int i = 1; i <= n; ++i) {
            if (i <= last_close_idx) {
                s[i - 1] = ')';
            } else {
                s[i - 1] = '(';
            }
        }
        answer(s);
        return 0;
    }

    // Case 2: s has at least one RBS. Find the first one.
    int k0 = -1;
    int l = 1, r = n;
    while(l <= r) {
        int mid = l + (r - l) / 2;
        if (mid == 0) { // prefix of length 0 has 0 RBS
            l = mid + 1;
            continue;
        }
        vector<int> prefix_indices;
        for (int i = 1; i <= mid; ++i) {
            prefix_indices.push_back(i);
        }
        if (query(prefix_indices) > 0) {
            k0 = mid;
            r = mid - 1;
        } else {
            l = mid + 1;
        }
    }
    
    // s_{k0-1}s_{k0} is the first "()"
    int p_anchor = k0 - 1;
    s[p_anchor - 1] = '(';
    s[k0 - 1] = ')';

    // Determine all other characters one by one using the anchor
    for (int i = 1; i <= n; ++i) {
        if (s[i-1] != ' ') { // Skip already known characters
            continue;
        }
        vector<int> current_q = {p_anchor, i};
        if (query(current_q) == 1) {
            s[i - 1] = ')';
        } else {
            s[i - 1] = '(';
        }
    }
    
    answer(s);

    return 0;
}