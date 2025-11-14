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
    
    for (int target = 1; target <= n; target++) {
        int current_pos = pos[target];
        
        if (current_pos != target) {
            // Move element from current_pos to target position
            moves.push_back({current_pos, target});
            total_cost += target;
            
            // Update the array and positions
            int element = v[current_pos];
            
            if (current_pos < target) {
                // Shift elements left
                for (int i = current_pos; i < target; i++) {
                    v[i] = v[i + 1];
                    pos[v[i]] = i;
                }
            } else {
                // Shift elements right
                for (int i = current_pos; i > target; i--) {
                    v[i] = v[i - 1];
                    pos[v[i]] = i;
                }
            }
            
            v[target] = element;
            pos[element] = target;
        }
    }
    
    int final_cost = (total_cost + 1) * (moves.size() + 1);
    
    cout << final_cost << " " << moves.size() << "\n";
    for (auto& move : moves) {
        cout << move.first << " " << move.second << "\n";
    }
    
    return 0;
}