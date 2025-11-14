#include <bits/stdc++.h>
using namespace std;

int walk(int x) {
    cout << "walk " << x << endl;
    cout.flush();
    int result;
    cin >> result;
    return result;
}

void guess(int g) {
    cout << "guess " << g << endl;
    cout.flush();
}

int main() {
    // Start at position s
    int start = walk(0);
    
    // Use binary search approach to find cycle length
    int low = 1, high = 200000;
    int current_pos = start;
    
    while (low < high) {
        int mid = (low + high) / 2;
        
        // Walk mid steps and check if we're back to start
        int pos = walk(mid);
        
        if (pos == start) {
            // We found a multiple of n
            // Now we need to find the exact n
            high = mid;
        } else {
            low = mid + 1;
            current_pos = pos;
        }
    }
    
    // At this point, low should be close to n or a multiple of n
    int candidate = low;
    
    // If we haven't confirmed it's exactly n, do a final check
    if (candidate > 1) {
        // Check if candidate is indeed the cycle length
        int test_pos = walk(candidate);
        if (test_pos != start) {
            // Try to find the actual cycle length by checking smaller divisors
            for (int d = 1; d * d <= candidate; d++) {
                if (candidate % d == 0) {
                    // Check if d is the cycle length
                    int check_pos = walk(0); // Get current position
                    int after_d = walk(d);
                    if (after_d == check_pos) {
                        candidate = d;
                        break;
                    }
                    
                    // Check if candidate/d is the cycle length
                    int other_d = candidate / d;
                    if (other_d != d) {
                        check_pos = walk(0);
                        int after_other = walk(other_d);
                        if (after_other == check_pos) {
                            candidate = other_d;
                            break;
                        }
                    }
                }
            }
        }
    }
    
    guess(candidate);
    return 0;
}