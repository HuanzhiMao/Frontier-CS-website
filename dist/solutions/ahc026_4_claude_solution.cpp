#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;
    
    vector<vector<int>> stacks(m);
    vector<int> pos(n + 1);
    
    for (int i = 0; i < m; i++) {
        stacks[i].resize(n / m);
        for (int j = 0; j < n / m; j++) {
            cin >> stacks[i][j];
        }
    }
    
    vector<pair<int, int>> operations;
    vector<bool> carried_out(n + 1, false);
    
    for (int target = 1; target <= n; target++) {
        // Find which stack contains the target
        int target_stack = -1;
        int target_pos = -1;
        
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < stacks[i].size(); j++) {
                if (stacks[i][j] == target) {
                    target_stack = i;
                    target_pos = j;
                    break;
                }
            }
            if (target_stack != -1) break;
        }
        
        // Move boxes above target to other stacks
        while (target_pos < stacks[target_stack].size() - 1) {
            // Find the best stack to move to (prefer empty stacks, then smallest stacks)
            int best_stack = -1;
            int min_size = INT_MAX;
            
            for (int i = 0; i < m; i++) {
                if (i != target_stack && stacks[i].size() < min_size) {
                    min_size = stacks[i].size();
                    best_stack = i;
                }
            }
            
            // Move from target_pos + 1 to end
            int box_to_move = stacks[target_stack][target_pos + 1];
            int boxes_moved = stacks[target_stack].size() - (target_pos + 1);
            
            // Move boxes
            for (int j = target_pos + 1; j < stacks[target_stack].size(); j++) {
                stacks[best_stack].push_back(stacks[target_stack][j]);
            }
            stacks[target_stack].resize(target_pos + 1);
            
            operations.push_back({box_to_move, best_stack + 1});
        }
        
        // Now target is at the top, carry it out
        operations.push_back({target, 0});
        stacks[target_stack].pop_back();
        carried_out[target] = true;
    }
    
    for (auto op : operations) {
        cout << op.first << " " << op.second << "\n";
    }
    
    return 0;
}