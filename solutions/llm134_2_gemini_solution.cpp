#include <iostream>
#include <string>

// Use unsigned long long for n since it can be up to 10^18.
using ull = unsigned long long;

// Function to perform a query and read the response.
int ask(ull x, ull y) {
    std::cout << x << " " << y << std::endl;
    int response;
    std::cin >> response;
    return response;
}

int main() {
    // Fast I/O
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    ull n;
    std::cin >> n;

    // Initialize search ranges for a and b.
    ull la = 1, ra = n;
    ull lb = 1, rb = n;

    // Loop until both ranges are narrowed to a single value.
    while (la < ra || lb < rb) {
        // Calculate midpoints. If a range is already size 1, use its value.
        ull ma = (la == ra) ? la : la + (ra - la) / 2;
        ull mb = (lb == rb) ? lb : lb + (rb - lb) / 2;
        
        int resp = ask(ma, mb);

        if (resp == 0) {
            // Found the answer.
            return 0;
        } else if (resp == 1) {
            // x < a, so a is in [ma + 1, ra]. Update lower bound for a.
            la = ma + 1;
        } else if (resp == 2) {
            // y < b, so b is in [mb + 1, rb]. Update lower bound for b.
            lb = mb + 1;
        } else { // resp == 3
            // x > a OR y > b, which means a <= ma OR b <= mb.
            // This is ambiguous. We must shrink one of the ranges.
            // A good heuristic is to shrink the larger of the two ranges.
            if (la < ra && (ra - la >= rb - lb || lb == rb)) {
                // Shrink the range for a.
                ra = ma;
            } else {
                // Shrink the range for b.
                rb = mb;
            }
        }
    }

    // When the loop terminates, la == ra and lb == rb. This is our answer.
    // The problem statement guarantees a solution exists, so a final query confirms.
    ask(la, lb);
    
    return 0;
}