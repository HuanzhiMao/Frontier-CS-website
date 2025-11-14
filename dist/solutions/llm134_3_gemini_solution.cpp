#include <iostream>

// Use unsigned long long for coordinates since n can be up to 10^18.
using ull = unsigned long long;

// Helper function to ask a query and return the response.
// Flushes the output stream to ensure the interactor receives the query.
int ask(ull x, ull y) {
    std::cout << x << " " << y << std::endl;
    int response;
    std::cin >> response;
    return response;
}

int main() {
    // Fast I/O.
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    ull n;
    std::cin >> n;

    // Initialize the search space for a and b as [1, n].
    ull la = 1, ra = n;
    ull lb = 1, rb = n;

    while (true) {
        // If the search space has converged to a single point, that must be the answer.
        // We query it to get response 0 and terminate.
        if (la == ra && lb == rb) {
            ask(la, lb);
            return 0;
        }

        // Calculate midpoints for the current search ranges.
        // Using la + (ra - la) / 2 to prevent potential overflow with (la + ra) / 2.
        ull ma, mb;
        if (la == ra) {
            ma = la;
        } else {
            ma = la + (ra - la) / 2;
        }
        if (lb == rb) {
            mb = lb;
        } else {
            mb = lb + (rb - lb) / 2;
        }

        // Ask the primary query with the midpoints.
        int r = ask(ma, mb);
        
        if (r == 0) {
            // Found the answer.
            return 0;
        } else if (r == 1) {
            // Response 1: ma < a.
            // This gives a new lower bound for a.
            la = ma + 1;
        } else if (r == 2) {
            // Response 2: mb < b.
            // This gives a new lower bound for b.
            lb = mb + 1;
        } else { // r == 3
            // Response 3: ma > a or mb > b.
            // This means a <= ma or b <= mb.
            
            // If one range is already determined, we don't need a second query.
            if (la == ra) { // a is already known, so it must be that b <= mb.
                rb = mb;
            } else if (lb == rb) { // b is already known, so it must be that a <= ma.
                ra = ma;
            } else {
                // Both ranges have size > 1. We need a second query to disambiguate.
                // Heuristically, we try to shrink the larger of the two ranges.
                if (ra - la >= rb - lb) {
                    // Try to shrink a's range by testing if 'a > ma'.
                    int r2 = ask(ma, rb);
                    if (r2 == 0) return 0;
                    if (r2 == 1) { // ma < a confirmed, so b <= mb must hold.
                        la = ma + 1;
                        rb = mb;
                    } else { // r2 == 3 implies a <= ma.
                        ra = ma;
                    }
                } else {
                    // Symmetrically, try to shrink b's range by testing 'b > mb'.
                    int r2 = ask(ra, mb);
                    if (r2 == 0) return 0;
                    if (r2 == 2) { // mb < b confirmed, so a <= ma must hold.
                        lb = mb + 1;
                        ra = ma;
                    } else { // r2 == 3 implies b <= mb.
                        rb = mb;
                    }
                }
            }
        }
    }

    return 0;
}