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
    long long val;
    cin >> val;
    cache[{x, y}] = val;
    return val;
}

int countLessEqual(long long target) {
    int count = 0;
    for (int i = 1; i <= n; i++) {
        int left = 1, right = n;
        int lastValid = 0;
        
        while (left <= right) {
            int mid = (left + right) / 2;
            long long val = query(i, mid);
            
            if (val <= target) {
                lastValid = mid;
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        count += lastValid;
    }
    return count;
}

int main() {
    cin >> n >> k;
    
    // Get some sample values to establish range
    long long minVal = query(1, 1);
    long long maxVal = query(n, n);
    
    // Binary search on answer
    long long left = minVal, right = maxVal;
    long long answer = maxVal;
    
    while (left <= right) {
        long long mid = left + (right - left) / 2;
        int count = countLessEqual(mid);
        
        if (count >= k) {
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