#include <bits/stdc++.h>
using namespace std;

int query(vector<long long>& arr) {
    cout << "0 " << arr.size();
    for (long long x : arr) {
        cout << " " << x;
    }
    cout << endl;
    cout.flush();
    
    int result;
    cin >> result;
    return result;
}

void guess(int n) {
    cout << "1 " << n << endl;
    cout.flush();
}

int main() {
    // Use binary search to find n
    int left = 2, right = 1000000000;
    int totalCost = 0;
    
    while (left < right) {
        int mid = left + (right - left) / 2;
        
        // Create a query to distinguish between mid and mid+1
        vector<long long> query_arr;
        
        // Use numbers that will collide if n <= mid but not if n > mid
        // We'll use multiples of mid
        long long base = 1000000000000LL; // Large base to avoid issues
        
        // Add numbers: base, base + mid, base + 2*mid, ...
        // If n = mid, these will all go to the same bucket
        // If n > mid, they will go to different buckets
        
        int query_size = min(20, mid); // Limit query size
        for (int i = 0; i < query_size; i++) {
            query_arr.push_back(base + (long long)i * mid);
        }
        
        totalCost += query_size;
        if (totalCost > 1000000) break;
        
        int collisions = query(query_arr);
        
        // If n = mid, we expect query_size * (query_size - 1) / 2 collisions
        // If n > mid, we expect fewer collisions
        
        int expected_max_collisions = query_size * (query_size - 1) / 2;
        
        if (collisions == expected_max_collisions) {
            // All elements went to same bucket, so n divides mid
            // Try to find exact value of n
            vector<long long> test_query;
            test_query.push_back(base);
            test_query.push_back(base + mid);
            
            totalCost += 2;
            if (totalCost > 1000000) {
                guess(mid);
                return 0;
            }
            
            int test_collisions = query(test_query);
            if (test_collisions == 1) {
                right = mid;
            } else {
                left = mid + 1;
            }
        } else {
            left = mid + 1;
        }
    }
    
    guess(left);
    return 0;
}