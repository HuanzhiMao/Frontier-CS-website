#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;
    
    vector<vector<int>> stacks(m);
    vector<int> pos(n + 1), stack_id(n + 1);
    
    for (int i = 0; i < m; i++) {
        stacks[i].resize(n / m);
        for (int j = 0; j < n / m; j++) {
            cin >> stacks[i][j];
            pos[stacks[i][j]] = j;
            stack_id[stacks[i][j]] = i;
        }
    }
    
    vector<pair<int, int>> operations;
    
    for (int target = 1; target <= n; target++) {
        int stack = stack_id[target];
        
        // Find position of target in its stack
        int target_pos = -1;
        for (int i = 0; i < stacks[stack].size(); i++) {
            if (stacks[stack][i] == target) {
                target_pos = i;
                break;
            }
        }
        
        // Move boxes above target to other stacks
        while (target_pos < stacks[stack].size() - 1) {
            // Find the best stack to move to (shortest one)
            int best_stack = -1;
            int min_size = INT_MAX;
            
            for (int i = 0; i < m; i++) {
                if (i != stack && stacks[i].size() < min_size) {
                    min_size = stacks[i].size();
                    best_stack = i;
                }
            }
            
            // Move the topmost box and everything above target
            int box_to_move = stacks[stack][target_pos + 1];
            int boxes_to_move = stacks[stack].size() - (target_pos + 1);
            
            operations.push_back({box_to_move, best_stack + 1});
            
            // Update stacks
            for (int i = target_pos + 1; i < stacks[stack].size(); i++) {
                stacks[best_stack].push_back(stacks[stack][i]);
                stack_id[stacks[stack][i]] = best_stack;
            }
            
            stacks[stack].erase(stacks[stack].begin() + target_pos + 1, stacks[stack].end());
        }
        
        // Carry out the target box
        operations.push_back({target, 0});
        stacks[stack].pop_back();
    }
    
    for (auto op : operations) {
        cout << op.first << " " << op.second << "\n";
    }
    
    return 0;
}