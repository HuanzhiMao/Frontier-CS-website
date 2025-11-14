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

void guess(long long n) {
    cout << "1 " << n << endl;
    cout.flush();
}

int main() {
    vector<long long> candidates;
    
    // Generate potential values for n
    for (long long i = 2; i <= 1000000; i++) {
        candidates.push_back(i);
    }
    
    long long base = 1000000000000LL; // 10^12
    
    while (candidates.size() > 1) {
        // Find a good number to test with
        long long test_num = base;
        base++;
        
        // Query with just this number to see which bucket it goes to
        vector<long long> single = {test_num};
        int collisions1 = query(single);
        
        if (collisions1 > 0) continue; // This shouldn't happen with single element
        
        // Now query with two copies to see if they collide
        vector<long long> pair = {test_num, test_num + 1};
        int collisions2 = query(pair);
        
        // Filter candidates based on whether test_num and test_num+1 should collide
        vector<long long> new_candidates;
        for (long long n : candidates) {
            bool should_collide = (test_num % n) == ((test_num + 1) % n);
            if ((collisions2 > 0) == should_collide) {
                new_candidates.push_back(n);
            }
        }
        
        candidates = new_candidates;
        
        if (candidates.size() <= 1) break;
        
        // If we still have too many candidates, try another approach
        if (candidates.size() > 100) {
            // Use a larger query to distinguish between more candidates
            vector<long long> large_query;
            for (int i = 0; i < min(50LL, (long long)candidates.size()); i++) {
                large_query.push_back(base + i);
            }
            base += large_query.size();
            
            int collisions = query(large_query);
            
            vector<long long> new_candidates2;
            for (long long n : candidates) {
                int expected_collisions = 0;
                vector<int> bucket_counts(min(n, (long long)large_query.size()), 0);
                
                for (long long x : large_query) {
                    int bucket = x % n;
                    if (bucket < bucket_counts.size()) {
                        expected_collisions += bucket_counts[bucket];
                        bucket_counts[bucket]++;
                    }
                }
                
                if (expected_collisions == collisions) {
                    new_candidates2.push_back(n);
                }
            }
            candidates = new_candidates2;
        }
    }
    
    if (candidates.empty()) {
        guess(1000000); // fallback
    } else {
        guess(candidates[0]);
    }
    
    return 0;
}