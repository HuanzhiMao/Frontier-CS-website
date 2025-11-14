#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>

using namespace std;

// Function to perform a query
bool query(int c) {
    cout << "? " << c << endl;
    char response;
    cin >> response;
    return response == 'Y';
}

// Function to perform a reset
void reset() {
    cout << "R" << endl;
}

// Function to output the final answer
void answer(int d) {
    cout << "! " << d << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, k;
    cin >> n >> k;

    vector<bool> is_first(n + 1, true);

    if (k == n) {
        // If k=n, all bakeries fit in memory. One pass is enough.
        reset();
        int distinct_count = 0;
        for (int i = 1; i <= n; ++i) {
            if (!query(i)) {
                distinct_count++;
            }
        }
        answer(distinct_count);
        return 0;
    }
    
    if (k == 1) {
        // For k=1, memory holds only the last item. Pairwise comparison is needed.
        for (int i = 1; i <= n; ++i) {
            if (is_first[i]) {
                for (int j = i + 1; j <= n; ++j) {
                    if (is_first[j]) {
                        query(i);
                        if (query(j)) {
                            is_first[j] = false;
                        }
                    }
                }
            }
        }
    } else { // k > 1
        // Block-based comparison strategy
        int group_size = k / 2;
        int num_groups = n / group_size;

        vector<vector<int>> groups(num_groups);
        for (int i = 0; i < n; ++i) {
            groups[i / group_size].push_back(i + 1);
        }

        for (int i = 0; i < num_groups; ++i) {
            for (int j = i; j < num_groups; ++j) {
                reset();
                vector<int> group_i_cands;
                for (int val : groups[i]) {
                    if (is_first[val]) {
                        group_i_cands.push_back(val);
                    }
                }

                if (i == j) {
                    // Intra-group duplicate check
                    for (int val : group_i_cands) {
                        if (query(val)) {
                            is_first[val] = false;
                        }
                    }
                } else {
                    // Inter-group duplicate check
                    vector<int> group_j_cands;
                    for (int val : groups[j]) {
                        if (is_first[val]) {
                            group_j_cands.push_back(val);
                        }
                    }
                    // Load group i
                    for (int val : group_i_cands) {
                        query(val);
                    }
                    // Check group j against group i
                    for (int val : group_j_cands) {
                        if (query(val)) {
                            is_first[val] = false;
                        }
                    }
                }
            }
        }
    }

    int distinct_count = 0;
    for (int i = 1; i <= n; ++i) {
        if (is_first[i]) {
            distinct_count++;
        }
    }

    answer(distinct_count);

    return 0;
}