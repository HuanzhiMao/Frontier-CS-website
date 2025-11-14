#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

long long n;
long long k;
map<pair<int, int>, long long> cache;

long long query(int r, int c) {
    if (cache.count({r, c})) {
        return cache.at({r, c});
    }
    cout << "QUERY " << r << " " << c << endl;
    long long v;
    cin >> v;
    cache[{r, c}] = v;
    return v;
}

long long count_le(long long val) {
    long long count = 0;
    int c = n;
    for (int r = 1; r <= n; ++r) {
        while (c > 0 && query(r, c) > val) {
            c--;
        }
        count += c;
    }
    return count;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n >> k;

    long long low = query(1, 1);
    long long high = query(n, n);
    
    long long ans = high;

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