#include <bits/stdc++.h>
using namespace std;

int query(vector<long long>& vals) {
    cout << "0 " << vals.size();
    for (long long x : vals) {
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
    // Use binary search approach to find n
    int cost = 0;
    
    // First, try to get an upper bound estimate using collision patterns
    vector<long long> test1 = {1, 2};
    int c1 = query(test1);
    cost += 2;
    
    if (c1 == 1) {
        guess(2);
        return 0;
    }
    
    // Try some small values first
    vector<long long> test2 = {1, 2, 3};
    int c2 = query(test2);
    cost += 3;
    
    if (c2 == 0) {
        guess(3);
        return 0;
    }
    
    // Binary search for the answer
    int left = 2, right = 1000000;
    
    while (left < right && cost < 950000) {
        int mid = (left + right) / 2;
        
        // Create a query to test if n <= mid
        vector<long long> test;
        int query_size = min(100, (950000 - cost) / 2);
        
        for (int i = 0; i < query_size; i++) {
            test.push_back(1LL * i * mid + 1);
        }
        
        int collisions = query(test);
        cost += query_size;
        
        if (collisions == 0) {
            // All elements went to different buckets, so n >= query_size
            // This suggests n might be >= mid
            left = mid + 1;
        } else {
            right = mid;
        }
        
        // Try another approach with consecutive numbers
        if (right - left > 1000 && cost < 900000) {
            vector<long long> consecutive;
            int size = min(50, 900000 - cost);
            for (int i = 1; i <= size; i++) {
                consecutive.push_back(i);
            }
            
            int cons_collisions = query(consecutive);
            cost += size;
            
            // If we get specific collision patterns, we can narrow down
            if (cons_collisions == 0) {
                left = max(left, size);
            } else {
                // Use collision count to estimate n
                // Expected collisions for consecutive numbers mod n
                double expected_n = (double)(size * size) / (2.0 * (cons_collisions + 1));
                if (expected_n >= left && expected_n <= right) {
                    right = min(right, (int)(expected_n * 1.5));
                    left = max(left, (int)(expected_n * 0.5));
                }
            }
        }
    }
    
    // Final refinement
    int best_guess = (left + right) / 2;
    
    // Try to verify with a small query if we have budget left
    if (cost < 990000 && right - left > 10) {
        vector<long long> final_test = {1, best_guess + 1};
        int final_collisions = query(final_test);
        cost += 2;
        
        if (final_collisions == 1) {
            // They collided, so n divides best_guess
            for (int d = best_guess; d >= left; d--) {
                if (best_guess % d == 0) {
                    best_guess = d;
                    break;
                }
            }
        }
    }
    
    guess(best_guess);
    return 0;
}