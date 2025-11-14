#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    // Start with binary search approach
    // We'll use Pollard's rho-like algorithm to find the cycle length
    
    int start_pos;
    cout << "walk 0" << endl;
    cin >> start_pos;
    
    // Try to find cycle using Floyd's cycle detection with modifications
    vector<int> positions;
    positions.push_back(start_pos);
    
    int step = 1;
    int current_pos = start_pos;
    
    // Move step by step and look for patterns
    while(step <= 100000) {
        cout << "walk 1" << endl;
        cin >> current_pos;
        
        // Check if we've seen this position at this distance before
        for(int i = 0; i < positions.size(); i++) {
            if(positions[i] == current_pos) {
                int cycle_len = step - i;
                cout << "guess " << cycle_len << endl;
                return 0;
            }
        }
        
        positions.push_back(current_pos);
        step++;
        
        // Optimization: if we have many positions, try larger jumps
        if(step > 1000 && step % 100 == 0) {
            // Try jumping by powers of 2 to find cycle faster
            int jump = min(1000, (int)positions.size() / 10);
            if(jump > 1) {
                cout << "walk " << jump << endl;
                cin >> current_pos;
                
                for(int i = 0; i < positions.size(); i++) {
                    if(positions[i] == current_pos) {
                        // Found potential cycle, verify
                        int potential_cycle = step + jump - i;
                        
                        // Test if this is correct by making another jump
                        cout << "walk " << potential_cycle << endl;
                        int test_pos;
                        cin >> test_pos;
                        
                        if(test_pos == current_pos) {
                            cout << "guess " << potential_cycle << endl;
                            return 0;
                        }
                        
                        positions.push_back(current_pos);
                        positions.push_back(test_pos);
                        step += jump + potential_cycle;
                        current_pos = test_pos;
                        break;
                    }
                }
            }
        }
    }
    
    // Fallback: if no pattern found, guess based on maximum seen
    cout << "guess " << positions.size() << endl;
    
    return 0;
}