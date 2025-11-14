#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    cin >> n >> m;
    
    vector<vector<int>> stacks(m);
    vector<int> pos(n + 1); // position of each box (stack index)
    vector<int> height(n + 1); // height of each box in its stack
    
    for (int i = 0; i < m; i++) {
        stacks[i].resize(n / m);
        for (int j = 0; j < n / m; j++) {
            cin >> stacks[i][j];
            pos[stacks[i][j]] = i;
            height[stacks[i][j]] = j;
        }
    }
    
    vector<pair<int, int>> operations;
    vector<bool> carried_out(n + 1, false);
    
    for (int target = 1; target <= n; target++) {
        // Find the stack containing target box
        int target_stack = pos[target];
        int target_height = height[target];
        
        // Check if target is at the top of its stack
        bool at_top = true;
        for (int h = target_height + 1; h < stacks[target_stack].size(); h++) {
            if (!carried_out[stacks[target_stack][h]]) {
                at_top = false;
                break;
            }
        }
        
        if (!at_top) {
            // Find an empty stack or the stack with minimum boxes
            int best_stack = -1;
            int min_size = INT_MAX;
            
            for (int i = 0; i < m; i++) {
                if (i == target_stack) continue;
                
                int current_size = 0;
                for (int j = 0; j < stacks[i].size(); j++) {
                    if (!carried_out[stacks[i][j]]) {
                        current_size++;
                    }
                }
                
                if (current_size < min_size) {
                    min_size = current_size;
                    best_stack = i;
                }
            }
            
            // Move target and all boxes above it to best_stack
            operations.push_back({target, best_stack});
            
            // Update data structures
            vector<int> boxes_to_move;
            for (int h = target_height; h < stacks[target_stack].size(); h++) {
                if (!carried_out[stacks[target_stack][h]]) {
                    boxes_to_move.push_back(stacks[target_stack][h]);
                }
            }
            
            // Remove boxes from original stack
            vector<int> new_stack;
            for (int h = 0; h < target_height; h++) {
                if (!carried_out[stacks[target_stack][h]]) {
                    new_stack.push_back(stacks[target_stack][h]);
                }
            }
            stacks[target_stack] = new_stack;
            
            // Add boxes to destination stack
            int start_height = stacks[best_stack].size();
            for (int i = 0; i < boxes_to_move.size(); i++) {
                stacks[best_stack].push_back(boxes_to_move[i]);
                pos[boxes_to_move[i]] = best_stack;
                height[boxes_to_move[i]] = start_height + i;
            }
        }
        
        // Carry out the target box
        operations.push_back({target, 0});
        carried_out[target] = true;
        
        // Remove target from its stack
        int stack_idx = pos[target];
        stacks[stack_idx].pop_back();
    }
    
    for (auto op : operations) {
        cout << op.first << " " << op.second << "\n";
    }
    
    return 0;
}