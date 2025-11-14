#include <bits/stdc++.h>
using namespace std;

int query(vector<long long>& nums) {
    cout << "0 " << nums.size();
    for (long long x : nums) {
        cout << " " << x;
    }
    cout << endl;
    cout.flush();
    
    int collisions;
    cin >> collisions;
    return collisions;
}

void guess(int n) {
    cout << "1 " << n << endl;
    cout.flush();
}

int main() {
    // First, try to find approximate range using powers of 2
    vector<long long> test = {1, 2};
    int collisions = query(test);
    
    if (collisions == 1) {
        guess(2);
        return 0;
    }
    
    // Binary search approach
    int left = 2, right = 1000000;
    
    while (right - left > 1) {
        int mid = (left + right) / 2;
        
        // Test with numbers that would collide if n <= mid
        vector<long long> nums;
        for (int i = 0; i < min(100, mid + 1); i++) {
            nums.push_back(1LL * i * mid + 1);
        }
        
        int cols = query(nums);
        
        if (cols > 0) {
            // There were collisions, so n <= mid
            right = mid;
        } else {
            // No collisions, so n > mid
            left = mid;
        }
    }
    
    // Now we know n is either left or right
    // Test specifically for right
    vector<long long> test_right = {1, 1 + right};
    int cols_right = query(test_right);
    
    if (cols_right == 1) {
        guess(right);
    } else {
        guess(left);
    }
    
    return 0;
}