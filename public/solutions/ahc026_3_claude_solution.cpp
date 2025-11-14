#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m;
    cin >> n >> m;
    
    vector<vector<int>> stacks(m);
    for (int i = 0; i < m; i++) {
        stacks[i].resize(n / m);
        for (int j = 0; j < n / m; j++) {
            cin >> stacks[i][j];
        }
    }
    
    vector<pair<int, int>> operations;
    
    for (int target = 1; target <= n; target++) {
        // Find which stack contains the target box
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
        
        // If target is already at the top, carry it out
        if (target_pos == stacks[target_stack].size() - 1) {
            operations.push_back({target, 0});
            stacks[target_stack].pop_back();
        } else {
            // Find an empty stack or the stack with minimum height
            int best_dest = -1;
            int min_height = INT_MAX;
            
            for (int i = 0; i < m; i++) {
                if (i != target_stack && stacks[i].size() < min_height) {
                    min_height = stacks[i].size();
                    best_dest = i;
                }
            }
            
            // Move boxes above target to another stack
            vector<int> boxes_to_move;
            for (int j = target_pos; j < stacks[target_stack].size(); j++) {
                boxes_to_move.push_back(stacks[target_stack][j]);
            }
            
            // Remove boxes from original stack
            stacks[target_stack].erase(stacks[target_stack].begin() + target_pos, stacks[target_stack].end());
            
            // Add boxes to destination stack
            for (int box : boxes_to_move) {
                stacks[best_dest].push_back(box);
            }
            
            operations.push_back({target, best_dest + 1});
            
            // Now target is at the top of the destination stack, carry it out
            operations.push_back({target, 0});
            stacks[best_dest].pop_back();
        }
    }
    
    for (auto op : operations) {
        cout << op.first << " " << op.second << "\n";
    }
    
    return 0;
}