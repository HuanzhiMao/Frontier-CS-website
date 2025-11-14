#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    vector<int> v(n + 1);
    vector<int> pos(n + 1);
    
    for (int i = 1; i <= n; i++) {
        cin >> v[i];
        pos[v[i]] = i;
    }
    
    vector<pair<int, int>> moves;
    int total_cost = 0;
    
    // Process elements from 1 to n
    for (int target = 1; target <= n; target++) {
        int current_pos = pos[target];
        
        if (current_pos == target) {
            continue; // Already in correct position
        }
        
        // Move element from current_pos to target
        moves.push_back({current_pos, target});
        total_cost += target;
        
        // Update positions of affected elements
        if (current_pos > target) {
            // Moving left: elements at positions [target, current_pos-1] shift right
            for (int i = 1; i <= n; i++) {
                if (pos[i] >= target && pos[i] < current_pos) {
                    pos[i]++;
                }
            }
        } else {
            // Moving right: elements at positions [current_pos+1, target] shift left
            for (int i = 1; i <= n; i++) {
                if (pos[i] > current_pos && pos[i] <= target) {
                    pos[i]--;
                }
            }
        }
        
        pos[target] = target;
    }
    
    int final_cost = (total_cost + 1) * (moves.size() + 1);
    cout << final_cost << " " << moves.size() << "\n";
    
    for (auto& move : moves) {
        cout << move.first << " " << move.second << "\n";
    }
    
    return 0;
}