#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <functional>
#include <cmath>
#include <algorithm>

using namespace std;

bool query(int c) {
    cout << "? " << c << endl;
    char response;
    cin >> response;
    return response == 'Y';
}

void reset() {
    cout << "R" << endl;
}

void answer(int d) {
    cout << "! " << d << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, k;
    cin >> n >> k;

    if (k == n) {
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

    int block_size = k / 2;
    if (block_size == 0) { // Handles k=1 case
        block_size = 1;
    }

    int num_blocks = n / block_size;
    
    vector<bool> is_candidate(n + 1, false);
    vector<vector<int>> reps(num_blocks);

    for (int i = 0; i < num_blocks; ++i) {
        reset();
        int start_bakery = i * block_size + 1;
        int end_bakery = (i + 1) * block_size;
        for (int j = start_bakery; j <= end_bakery; ++j) {
            if (!query(j)) {
                reps[i].push_back(j);
            }
        }
    }

    for (const auto& block_reps : reps) {
        for (int bakery_idx : block_reps) {
            is_candidate[bakery_idx] = true;
        }
    }

    for (int i = 0; i < num_blocks; ++i) {
        for (int j = i + 1; j < num_blocks; ++j) {
            if (reps[i].empty() || reps[j].empty()) continue;

            reset();
            for (int u : reps[i]) {
                query(u);
            }
            for (int v : reps[j]) {
                if (is_candidate[v]) {
                    if (query(v)) {
                        is_candidate[v] = false;
                    }
                }
            }
        }
    }
    
    int distinct_count = 0;
    for (int i = 1; i <= n; ++i) {
        if (is_candidate[i]) {
            distinct_count++;
        }
    }

    answer(distinct_count);

    return 0;
}