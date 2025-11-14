#include <iostream>
#include <vector>
#include <optional>

// Global variables for convenience in a contest setting.
int n;
long long k;
// Cache to store results of queries to avoid redundant calls.
// Using std::optional (C++17) is a robust way to handle caching,
// as the matrix could contain any long long value.
std::vector<std::vector<std::optional<long long>>> cache;

/**
 * @brief Performs a QUERY for the value at a[r][c], using a cache.
 * @param r Row index (1-based).
 * @param c Column index (1-based).
 * @return The value of a[r][c].
 */
long long do_query(int r, int c) {
    if (cache[r][c].has_value()) {
        return cache[r][c].value();
    }

    std::cout << "QUERY " << r << " " << c << std::endl;
    long long v;
    std::cin >> v;

    cache[r][c] = v;
    return v;
}

/**
 * @brief Counts elements in the matrix less than or equal to `val`.
 *
 * This function uses an efficient "staircase" search from the top-right corner.
 * It makes at most 2n-1 queries in the worst case (if no values are cached).
 *
 * @param val The value to compare against.
 * @return The number of elements in the matrix <= val.
 */
long long count_less_equal(long long val) {
    long long count = 0;
    int r = 1, c = n;
    while (r <= n && c >= 1) {
        if (do_query(r, c) <= val) {
            // If a[r][c] <= val, everything in the current row to its left is also <= val.
            // Add all `c` elements of this row segment to the count and move to the next row.
            count += c;
            r++;
        } else {
            // If a[r][c] > val, it's too large. Discard this column and move left.
            c--;
        }
    }
    return count;
}

int main() {
    // Standard fast I/O setup. The key for interactive problems is flushing stdout.
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::cin >> n >> k;

    // Initialize the cache.
    cache.resize(n + 1, std::vector<std::optional<long long>>(n + 1));

    // Binary search for the k-th smallest value.
    // The search space is the range of possible values in the matrix.
    long long low = 0; 
    long long high = 1e18; 
    long long ans = high;

    while (low <= high) {
        // Prevent overflow when low/high are large.
        long long mid = low + (high - low) / 2;

        if (count_less_equal(mid) >= k) {
            // `mid` could be our answer. Let's see if a smaller value also works.
            ans = mid;
            high = mid - 1;
        } else {
            // `mid` is too small, the k-th element must be larger.
            low = mid + 1;
        }
    }

    std::cout << "DONE " << ans << std::endl;

    return 0;
}