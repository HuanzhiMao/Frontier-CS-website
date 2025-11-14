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
    // Start at initial position
    int start = walk(0);
    
    // Use binary search approach to find cycle length
    int low = 1, high = 1000000000;
    
    while (low < high) {
        int mid = low + (high - low) / 2;
        
        // Walk mid steps and see if we return to start
        int pos = walk(mid);
        
        if (pos == start) {
            // We've completed at least one full cycle
            // The cycle length divides mid
            high = mid;
        } else {
            // We haven't completed a full cycle yet
            low = mid + 1;
        }
    }
    
    // At this point, low is the smallest number where walking low steps returns us to start
    // But we need to find the exact cycle length
    int candidate = low;
    
    // Check if smaller divisors work
    for (int i = 1; i * i <= candidate; i++) {
        if (candidate % i == 0) {
            // Check if i is the cycle length
            int pos = walk(i);
            if (pos == start) {
                guess(i);
                return 0;
            }
            
            // Check if candidate/i is the cycle length (if different from i)
            if (i != candidate / i) {
                int pos2 = walk(candidate / i - i);
                if (pos2 == start) {
                    guess(candidate / i);
                    return 0;
                }
                // Return to original position
                walk(i);
            } else {
                // Return to original position  
                walk(candidate - i);
            }
        }
    }
    
    guess(candidate);
    return 0;
}