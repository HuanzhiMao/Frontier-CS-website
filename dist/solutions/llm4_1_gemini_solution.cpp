#include <iostream>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>

using namespace std;

// Global variables for problem parameters and memoization cache.
long long n;
long long k;
map<pair<int, int>, long long> memo;

// A wrapper function for making queries to the judge.
// It uses a map for memoization to cache the results of queries,
// preventing redundant calls for the same cell coordinates.
long long query(int r, int c) {
    // Check if the result for (r, c) is already cached.
    if (memo.count({r, c})) {
        return memo.at({r, c});
    }
    
    // If not cached, make a new query.
    cout << "QUERY " << r << " " << c << endl;
    long long v;
    cin >> v;
    
    // Store the result in the cache before returning.
    memo[{r, c}] = v;
    return v;
}

// Counts the number of elements in the matrix that are less than or equal to a given value `v`.
// This function leverages the sorted properties of the matrix to perform the count
// in O(n) queries using a staircase walk (saddleback search).
// It starts from the top-right corner of the matrix.
long long count_less_equal(long long v) {
    long long count = 0;
    int r = 1;
    int c = n;
    while (r <= n && c >= 1) {
        // If the current element a[r][c] is less than or equal to v,
        // all elements in the current row to its left (including itself) are also <= v.
        // We add these 'c' elements to our count and move down to the next row.
        if (query(r, c) <= v) {
            count += c;
            r++;
        } else {
            // If a[r][c] is greater than v, it's too large. Since columns are sorted,
            // we can discard the entire current column and move one column to the left.
            c--;
        }
    }
    return count;
}

int main() {
    // Fast I/O for performance.
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Read problem size n and rank k.
    cin >> n >> k;

    // The core of the solution is a binary search on the answer's value.
    // The k-th smallest element must lie within the range [min_val, max_val] of the matrix.
    // Due to the matrix properties, a[1][1] is the minimum value and a[n][n] is the maximum.
    // We query these two values to establish a tight search range for our binary search.
    long long low = query(1, 1);
    long long high = query(n, n);
    long long ans = high;

    // The binary search aims to find the smallest value `v` such that there are at least `k`
    // elements in the matrix less than or equal to `v`. This value is the k-th smallest element.
    while (low <= high) {
        long long mid = low + (high - low) / 2;
        if (count_less_equal(mid) >= k) {
            // If count >= k, `mid` is a potential answer. We try to find an even smaller
            // value that still satisfies the condition, so we record `mid` and search in [low, mid-1].
            ans = mid;
            high = mid - 1;
        } else {
            // If count < k, `mid` is too small. The k-th element must be larger.
            // We search in [mid+1, high].
            low = mid + 1;
        }
    }

    // After the binary search converges, `ans` holds the k-th smallest value.
    cout << "DONE " << ans << endl;

    return 0;
}