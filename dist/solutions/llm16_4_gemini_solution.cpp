#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <cmath>

// Global variable for the number of vertices
long long n;

// Function to perform a query
long long ask(long long x, long long y) {
    if (x == y) return 0;
    
    // Ensure vertex numbers are within [1, n] and handle cyclic arithmetic
    x = (x - 1 + n) % n + 1;
    y = (y - 1 + n) % n + 1;

    std::cout << "? " << x << " " << y << std::endl;
    long long dist;
    std::cin >> dist;
    return dist;
}

// Function to submit the final answer
void answer(long long u, long long v) {
    // Ensure vertex numbers are within [1, n]
    u = (u - 1 + n) % n + 1;
    v = (v - 1 + n) % n + 1;

    if (u > v) std::swap(u, v);
    std::cout << "! " << u << " " << v << std::endl;
    int result;
    std::cin >> result;
    // Exit immediately on wrong answer as per problem statement
    if (result == -1) {
        exit(0);
    }
}

// Main logic for a single test case
void solve() {
    std::cin >> n;
    long long half = n / 2;

    // Define two fixed, diametrically opposite reference points
    long long ref1 = 1;
    long long ref2 = 1 + half;

    // The function to minimize. It's the sum of distances to the reference points.
    auto f = [&](long long x) {
        return ask(x, ref1) + ask(x, ref2);
    };

    // Ternary search to find one of the minima of f(x), which corresponds to an endpoint
    long long l = 1, r = n;
    // 60 iterations are sufficient to narrow down a range of 10^9 to about size 1.
    for (int i = 0; i < 60; ++i) {
        if (l >= r) break;
        long long m1 = l + (r - l) / 3;
        long long m2 = r - (r - l) / 3;
        if (f(m1) < f(m2)) {
            r = m2 - 1;
        } else {
            l = m1 + 1;
        }
    }

    // After ternary search, we have a small range [l, r].
    // Perform a local search in this small window to find the precise minimum.
    long long min_val = -1;
    long long u = -1;
    for (long long cand = std::max(1LL, l - 5); cand <= std::min(n, r + 5); ++cand) {
        long long val = f(cand);
        if (u == -1 || val < min_val) {
            min_val = val;
            u = cand;
        }
    }

    // Now that we have one endpoint 'u', find the other 'v'.
    long long u_opp = (u - 1 + half) % n + 1;
    long long d = ask(u, u_opp);
    
    // From the geometric properties of the cycle and the chord:
    // d = 1 + half - dist_cycle(u, v)
    // So, dist_cycle(u, v) = half - d + 1
    long long dist_uv = half - d + 1;

    // This gives two candidates for 'v'
    long long v1 = u + dist_uv;
    long long v2 = u - dist_uv;

    // One last query to distinguish between the two candidates
    if (ask(u, v1) == 1) {
        answer(u, v1);
    } else {
        answer(u, v2);
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int t;
    std::cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}