#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    // Start with walk(0) to get starting position
    cout << "walk 0" << endl;
    int start;
    cin >> start;
    
    // Try powers of 2 to find cycle length
    int current_pos = start;
    vector<pair<int, int>> positions; // (steps, position)
    positions.push_back({0, start});
    
    int step = 1;
    while(step <= 100000) {
        cout << "walk " << step << endl;
        cin >> current_pos;
        
        // Check if we've returned to start
        if(current_pos == start) {
            cout << "guess " << step << endl;
            return 0;
        }
        
        positions.push_back({step, current_pos});
        step *= 2;
    }
    
    // If we haven't found it with powers of 2, do binary search
    // Find the interval where we should search
    int left = positions.back().first / 2;
    int right = positions.back().first;
    
    while(left < right) {
        int mid = left + (right - left) / 2;
        cout << "walk " << (mid - positions.back().first) << endl;
        cin >> current_pos;
        positions.push_back({mid, current_pos});
        
        if(current_pos == start) {
            cout << "guess " << mid << endl;
            return 0;
        }
        
        // We need to determine if cycle length is <= mid or > mid
        // If we haven't returned to start after mid steps, cycle is > mid
        left = mid + 1;
    }
    
    cout << "guess " << left << endl;
    return 0;
}