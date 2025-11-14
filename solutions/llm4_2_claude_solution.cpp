#include <bits/stdc++.h>
using namespace std;

int n, k;
map<pair<int,int>, long long> cache;

long long query(int x, int y) {
    if (cache.count({x, y})) {
        return cache[{x, y}];
    }
    cout << "QUERY " << x << " " << y << endl;
    cout.flush();
    long long v;
    cin >> v;
    cache[{x, y}] = v;
    return v;
}

int countLessEqual(long long val) {
    int count = 0;
    for (int i = 1; i <= n; i++) {
        int left = 1, right = n, pos = 0;
        while (left <= right) {
            int mid = (left + right) / 2;
            if (query(i, mid) <= val) {
                pos = mid;
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        count += pos;
    }
    return count;
}

int main() {
    cin >> n >> k;
    
    long long left = query(1, 1);
    long long right = query(n, n);
    
    long long answer = right;
    
    while (left <= right) {
        long long mid = left + (right - left) / 2;
        int cnt = countLessEqual(mid);
        
        if (cnt >= k) {
            answer = mid;
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    
    cout << "DONE " << answer << endl;
    cout.flush();
    
    return 0;
}