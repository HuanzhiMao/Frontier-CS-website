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
    // First, use small numbers to find patterns
    vector<long long> test1 = {1, 2};
    int collisions1 = query(test1);
    
    if (collisions1 == 0) {
        // n >= 2, try larger set
        vector<long long> test2 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        int collisions2 = query(test2);
        
        if (collisions2 == 0) {
            // n >= 10, need to search larger values
            vector<long long> test3;
            for (int i = 1; i <= 100; i++) {
                test3.push_back(i);
            }
            int collisions3 = query(test3);
            
            // Binary search approach for larger n
            long long left = collisions3 + 1, right = 100;
            
            while (left < right) {
                long long mid = (left + right) / 2;
                vector<long long> test;
                for (long long i = 1; i <= mid + 10; i++) {
                    test.push_back(i);
                }
                int c = query(test);
                
                if (c == 0) {
                    left = mid + 1;
                } else {
                    right = mid;
                }
            }
            guess(left);
        } else {
            // n is between 2 and 10
            for (long long n = 2; n <= 10; n++) {
                vector<long long> test;
                for (int i = 1; i <= 10; i++) {
                    test.push_back(i);
                }
                int expected = 0;
                vector<int> bucket_count(n, 0);
                for (int x : test) {
                    expected += bucket_count[x % n];
                    bucket_count[x % n]++;
                }
                if (expected == collisions2) {
                    guess(n);
                    return 0;
                }
            }
        }
    } else {
        // n = 2 since we got collision with just {1, 2}
        guess(2);
    }
    
    return 0;
}