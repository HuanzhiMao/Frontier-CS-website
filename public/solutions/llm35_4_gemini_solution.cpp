#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <set>

using namespace std;

bool ask(int x, const vector<int>& s) {
    if (s.empty()) {
        return false;
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
    return response == 1;
}

void answer(int y) {
    cout << "! " << y << endl;
}

vector<bool> get_presence_vector(const vector<int>& indices, int n) {
    vector<bool> present(n + 1, false);
    if (indices.empty()) {
        return present;
    }
    for (int x = 1; x <= n; ++x) {
        if (ask(x, indices)) {
            present[x] = true;
        }
    }
    return present;
}

void solve() {
    int n;
    cin >> n;

    int L = 1, R = 2 * n - 1;
    
    while (L < R) {
        int M = L + (R - L) / 2;
        vector<int> s_indices;
        for (int i = L; i <= M; ++i) {
            s_indices.push_back(i);
        }

        vector<bool> s_present = get_presence_vector(s_indices, n);
        
        int distinct_in_s = 0;
        for (int i = 1; i <= n; ++i) {
            if (s_present[i]) {
                distinct_in_s++;
            }
        }
        
        if ((M - L + 1) % 2 != distinct_in_s % 2) {
            R = M;
        } else {
            L = M + 1;
        }
    }
    
    int pos = L;

    // Check if pos is correct by elimination.
    // The only remaining unpaired candidate value must be the answer.
    // The previous BS finds a pos such that {1..pos} has an odd number of internal pairs.
    // This is not quite right.
    // The cross-count logic is more robust.
    
    L = 1, R = 2 * n - 1;
    while(L < R) {
        int M = L + (R - L) / 2;

        vector<int> s1_indices;
        for (int i = L; i <= M; ++i) {
            s1_indices.push_back(i);
        }

        vector<int> s2_indices;
        for (int i = M + 1; i <= R; ++i) {
            s2_indices.push_back(i);
        }

        vector<bool> s1_present = get_presence_vector(s1_indices, n);
        vector<bool> s2_present = get_presence_vector(s2_indices, n);

        int cross_count = 0;
        for (int x = 1; x <= n; ++x) {
            if (s1_present[x] && s2_present[x]) {
                cross_count++;
            }
        }
        
        // This partition logic is for the whole set, not sub-ranges.
        // Let's take S = {1..M}.
        vector<int> s_prefix;
        for(int i = 1; i <= M; ++i) s_prefix.push_back(i);

        vector<int> s_suffix;
        for(int i = M + 1; i <= 2 * n - 1; ++i) s_suffix.push_back(i);

        vector<bool> prefix_present = get_presence_vector(s_prefix, n);
        vector<bool> suffix_present = get_presence_vector(s_suffix, n);
        
        cross_count = 0;
        for(int x = 1; x <= n; ++x) {
            if(prefix_present[x] && suffix_present[x]) {
                cross_count++;
            }
        }

        if (cross_count % 2 == 1) { // Odd number of values cross, unique pos in S
            R = M;
        } else {
            L = M + 1;
        }
    }
    pos = L;


    for (int x = 1; x <= n; ++x) {
        if (ask(x, {pos})) {
            answer(x);
            break;
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