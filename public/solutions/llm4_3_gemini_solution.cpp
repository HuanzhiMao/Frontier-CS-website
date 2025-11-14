#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

using namespace std;

long long n;
long long k;
map<pair<int, int>, long long> cache;

long long do_query(int r, int c) {
    if (cache.count({r, c})) {
        return cache[{r, c}];
    }
    cout << "QUERY " << r << " " << c << endl;
    long long val;
    cin >> val;
    cache[{r, c}] = val;
    return val;
}

// Counts number of elements less than or equal to val
long long count_le(long long val) {
    long long count = 0;
    int r = 1;
    int c = n;
    while (r <= n && c >= 1) {
        if (do_query(r, c) <= val) {
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

    long long low = 0, high = 1e18, ans = high;

    while (low <= high) {
        long long mid = low + (high - low) / 2;
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