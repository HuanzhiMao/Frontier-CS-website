#include <iostream>
#include <vector>
#include <map>
#include <random>
#include <chrono>
#include <algorithm>

using namespace std;

long long n;
long long k;

map<pair<int, int>, long long> cache;

long long query(int r, int c) {
    if (cache.count({r, c})) {
        return cache[{r, c}];
    }
    cout << "QUERY " << r << " " << c << endl;
    long long v;
    cin >> v;
    cache[{r, c}] = v;
    return v;
}

// Counts number of elements <= val
long long count_le(long long val) {
    long long count = 0;
    int r = 1;
    int c = n;
    while (r <= n && c >= 1) {
        if (query(r, c) <= val) {
            count += c;
            r++;
        } else {
            c--;
        }
    }
    return count;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n >> k;

    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> dist(1, n);

    long long low = query(1, 1);
    long long high = query(n, n);
    
    if (k == 1) {
        cout << "DONE " << low << endl;
        return 0;
    }
    if (k == n * n) {
        cout << "DONE " << high << endl;
        return 0;
    }

    long long ans = high;

    while (low <= high) {
        long long mid;
        
        // A standard binary search on the value range [low, high] might be too slow
        // if the range is large. Instead, we can use a randomized approach.
        // By picking a random element from the matrix as the pivot, we are more likely
        // to narrow down the search space of values that actually exist in the matrix.
        // This is effectively a randomized binary search on the answer.
        int r = dist(rng);
        int c = dist(rng);
        mid = query(r, c);

        // If the random pivot is outside our current search range, it's not helpful.
        // In this case, fall back to a standard binary search step to ensure progress.
        if (mid < low || mid > high) {
            mid = low + (high - low) / 2;
        }
        
        if (count_le(mid) >= k) {
            ans = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    cout << "DONE " << ans << endl;

    return 0;
}