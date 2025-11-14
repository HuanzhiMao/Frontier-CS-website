#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <functional>

using namespace std;

typedef unsigned long long ll;

// Interactive problem: flush output after each query.
int ask(ll x, ll y) {
    cout << x << " " << y << endl;
    int r;
    cin >> r;
    if (r == 0) {
        exit(0);
    }
    return r;
}

// staircase[x] = y means we know that (a < x OR b < y)
map<ll, ll> staircase;

// Insert a new constraint (a < x OR b < y) and maintain the Pareto-optimal frontier.
void add_constraint(ll x, ll y) {
    // Check if the new constraint is made redundant by an existing one.
    auto it = staircase.lower_bound(x);
    if (it != staircase.end() && it->second >= y) {
        return;
    }
    if (it != staircase.begin()) {
        auto prev_it = prev(it);
        if (prev_it->second <= y) {
            return;
        }
    }
    
    // Remove existing constraints made redundant by the new one.
    it = staircase.lower_bound(x);
    while (it != staircase.end() && it->second <= y) {
        it = staircase.erase(it);
    }
    
    staircase[x] = y;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    ll n;
    cin >> n;

    ll la = 1, lb = 1;
    // Initially, a and b are known to be in [1, n].
    // This is equivalent to (a < n+1 or b < 1) and (a < 1 or b < n+1).
    staircase[n + 1] = 1; 

    while (true) {
        // Determine the current bounding box for the search space.
        // This is an over-approximation but useful for guiding the binary search.
        ll ra = staircase.begin()->first - 1;
        
        auto it_rb_calc = staircase.upper_bound(la);
        ll rb = prev(it_rb_calc)->second - 1;

        if (la > ra || lb > rb) {
             ask(la, lb); // Should find the answer.
             return 0; // Should exit inside ask.
        }

        // Decide which variable to binary search on, heuristically pick the larger range.
        bool search_a = (ra - la >= rb - lb);
        
        if (search_a) {
            ll mid_a = la + (ra - la) / 2;
            
            // Find the upper bound for b, assuming a is mid_a.
            // b must be less than y for any (x,y) in staircase with x <= mid_a.
            // Since y values decrease as x increases, this is the y of the largest x <= mid_a.
            auto it = staircase.upper_bound(mid_a);
            ll y_cap = prev(it)->second;
            
            int r = ask(mid_a, y_cap);
            if (r == 1) { // x < a
                la = mid_a + 1;
            } else if (r == 3) { // x > a or y > b
                add_constraint(mid_a, y_cap);
            } else { // r == 2 (y < b). This case should be impossible by our logic.
                     // Handle defensively by updating lb.
                 lb = max(lb, y_cap + 1);
            }
        } else { // Search for b
            ll mid_b = lb + (rb - lb) / 2;
            
            // Find the upper bound for a, assuming b is mid_b.
            // This requires finding x_i for the smallest y_i > mid_b.
            // We can iterate through the map to find this.
            ll x_cap = n + 1;
            for(auto const& [x_val, y_val] : staircase) {
                if (y_val > mid_b) {
                    x_cap = x_val;
                } else {
                    break;
                }
            }
            
            int r = ask(x_cap, mid_b);
            if (r == 2) { // y < b
                lb = mid_b + 1;
            } else if (r == 3) { // x > a or y > b
                add_constraint(x_cap, mid_b);
            } else { // r == 1 (x < a). Impossible by logic.
                 la = max(la, x_cap + 1);
            }
        }
    }

    return 0;
}