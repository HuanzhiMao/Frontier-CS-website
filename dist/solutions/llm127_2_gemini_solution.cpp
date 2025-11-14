#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <random>
#include <chrono>
#include <numeric>

using namespace std;

// Function to perform a query
pair<int, int> ask(int i) {
    cout << "? " << i << endl;
    int a0, a1;
    cin >> a0 >> a1;
    return {a0, a1};
}

// Function to report the answer
void answer(int i) {
    cout << "! " << i << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    // Use a random number generator for probing
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> dist(0, n - 1);

    int p_best = -1;
    int k_best = n + 1;

    // Phase 1: Random probing to find a reference prize.
    // The goal is to find a prize that is as valuable as possible.
    // A valuable prize has a small number of prizes more expensive than it.
    // We query random boxes and find the one that gives the minimum non-zero (a0+a1).
    int num_random_probes = min(n, 80);

    for (int i = 0; i < num_random_probes; ++i) {
        int p = dist(rng);
        pair<int, int> res = ask(p);
        int k = res.first + res.second;

        if (k == 0) {
            answer(p);
            return 0;
        }
        if (k < k_best) {
            k_best = k;
            p_best = p;
        }
    }

    // Phase 2: Locate all candidate prizes.
    // The `k_best` prizes that are more valuable than our reference prize are the candidates.
    // A prize at index `c` is a candidate if a query `? c` yields `k_c < k_best`.
    // We can find all candidates by performing a binary search for each one.
    vector<int> candidates;
    int last_found = -1;

    for (int i = 0; i < k_best; ++i) {
        int l = last_found + 1;
        int r = n - 1;
        int next_cand = -1;

        while (l <= r) {
            int mid = l + (r - l) / 2;
            pair<int, int> res = ask(mid);
            int k_mid = res.first + res.second;

            if (k_mid == 0) {
                answer(mid);
                return 0;
            }

            if (k_mid < k_best) {
                // This is a candidate, possibly the one we are looking for.
                // Try to find an even earlier one.
                next_cand = mid;
                r = mid - 1;
            } else {
                // This is not a candidate. The one we search must be to the right.
                l = mid + 1;
            }
        }
        
        if (next_cand != -1) {
            candidates.push_back(next_cand);
            last_found = next_cand;
        } else {
            // Should not happen if k_best is correct
            break;
        }
    }

    // Phase 3: Check each candidate to find the diamond.
    // The diamond is the only prize for which a query yields a0+a1=0.
    for (int cand_idx : candidates) {
        pair<int, int> res = ask(cand_idx);
        if (res.first + res.second == 0) {
            answer(cand_idx);
            return 0;
        }
    }

    // Fallback: This part should not be reached in a normal run.
    // It's included to guarantee an answer is always printed.
    if (!candidates.empty()) {
        answer(candidates[0]);
    } else {
        answer(0);
    }

    return 0;
}