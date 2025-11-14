#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <numeric>

using namespace std;

// Function to perform a query
int ask(int l, int r) {
    cout << "? " << l << " " << r << endl;
    int x;
    cin >> x;
    return x;
}

// Function to make a final guess
void answer(int a) {
    cout << "! " << a << endl;
    int y;
    cin >> y;
}

// Function to signal the end of a test case
void end_case() {
    cout << "#" << endl;
}

// Main logic for a single test case
void solve() {
    int n;
    cin >> n;

    vector<int> final_candidates;
    int l = 1, r = n;

    int h_streak = 0, d_streak = 0;

    while (r - l + 1 > 2 && final_candidates.size() < 2) {
        if (h_streak == 2) { // Force Dishonest answer
            int m = l + (r - l) / 2;
            int len = m - l + 1;
            int x = ask(l, m);
            // Dishonest response for an "outside" query implies absent is inside
            // Dishonest response for an "inside" query implies absent is outside
            // x == len is a "full" count, normally for an outside query
            // so if they answer x == len, it's a D for outside or H for inside.
            // Since it must be D, it implies absent is outside.
            if (x == len) { // Absent is outside [l, m]
                l = m + 1;
            } else { // Absent is inside [l, m]
                r = m;
            }
            h_streak = 0;
            d_streak = 1;
        } else if (d_streak == 2) { // Force Honest answer
            int m = l + (r - l) / 2;
            int len = m - l + 1;
            int x = ask(l, m);
            // x == len is H for outside.
            if (x == len) { // Absent is outside [l, m]
                l = m + 1;
            } else { // Absent is inside [l, m]
                r = m;
            }
            h_streak = 1;
            d_streak = 0;
        } else {
            // General case: Use a 3-query sequence to make progress
            int m = l + (r - l) / 2;
            
            // Two queries on [l, m] to create divergent scenarios.
            // If absent is in [l, m], it forces a (future) d_streak of 2.
            // If absent is in [m+1, r], it forces a (future) h_streak of 2.
            
            // Check if one of the scenarios is impossible from the current state
            bool can_HH = (h_streak < 1);
            bool can_DD = (d_streak < 1);

            if (!can_HH) { // Must be DD sequence, so absent is in [l, m]
                ask(l, m); ask(l, m); // Advance query counter
                r = m;
                // Old state (h_streak > 0) -> D -> (0,1) -> D -> (0,2)
                h_streak = 0; d_streak = 2;
                continue;
            }
            if (!can_DD) { // Must be HH sequence, so absent is in [m+1, r]
                ask(l, m); ask(l, m); // Advance query counter
                l = m + 1;
                // Old state (d_streak > 0) -> H -> (1,0) -> H -> (2,0)
                h_streak = 2; d_streak = 0;
                continue;
            }

            // Both scenarios are possible, need a third query to distinguish
            ask(l, m);
            ask(l, m);

            int x3 = ask(l, l);

            if (x3 == 1) {
                // This outcome is only possible if the absent student is in [l+1, m].
                // The honesty sequence for this case must have been D, D, H.
                l = l + 1;
                r = m;
                h_streak = 1;
                d_streak = 0;
            } else { // x3 == 0
                // This is possible if absent is `l` OR absent is in `[m+1, r]`.
                // We save `l` as a candidate and continue searching in `[m+1, r]`.
                final_candidates.push_back(l);
                l = m + 1;
                // The honesty sequence for the [m+1, r] case was H, H, D.
                h_streak = 0;
                d_streak = 1;
            }
        }
    }

    if (l <= r) final_candidates.push_back(l);
    if (l + 1 <= r) final_candidates.push_back(l + 1);

    for (int cand : final_candidates) {
        if (cand > 0 && cand <= n) {
            answer(cand);
        }
    }
    end_case();
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