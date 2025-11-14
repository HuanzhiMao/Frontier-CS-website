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

int countSmaller(long long target) {
    int count = 0;
    for (int i = 1; i <= n; i++) {
        int left = 1, right = n;
        int pos = n + 1;
        
        while (left <= right) {
            int mid = (left + right) / 2;
            long long val = query(i, mid);
            
            if (val > target) {
                pos = mid;
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }
        count += pos - 1;
    }
    return count;
}

int main() {
    cin >> n >> k;
    
    vector<long long> candidates;
    
    // Sample some values
    for (int i = 1; i <= n; i += max(1, n/20)) {
        for (int j = 1; j <= n; j += max(1, n/20)) {
            candidates.push_back(query(i, j));
        }
    }
    
    // Add corners and some strategic points
    candidates.push_back(query(1, 1));
    candidates.push_back(query(n, n));
    candidates.push_back(query(1, n));
    candidates.push_back(query(n, 1));
    
    sort(candidates.begin(), candidates.end());
    candidates.erase(unique(candidates.begin(), candidates.end()), candidates.end());
    
    long long answer = candidates[0];
    
    for (long long val : candidates) {
        int smaller = countSmaller(val - 1);
        if (smaller < k) {
            answer = val;
        }
    }
    
    cout << "DONE " << answer << endl;
    cout.flush();
    
    return 0;
}