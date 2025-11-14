#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
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
    set<int> remaining;
    for (int i = 1; i <= n; i++) {
        remaining.insert(i);
    }
    
    while (!remaining.empty()) {
        int target = *remaining.begin();
        
        // Find which stack contains target and its position
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
        
        // If target is at the top, carry it out
        if (target_pos == stacks[target_stack].size() - 1) {
            operations.push_back({target, 0});
            stacks[target_stack].pop_back();
            remaining.erase(target);
            continue;
        }
        
        // Find best stack to move boxes above target
        int best_stack = -1;
        int min_size = INT_MAX;
        
        for (int i = 0; i < m; i++) {
            if (i != target_stack && stacks[i].size() < min_size) {
                min_size = stacks[i].size();
                best_stack = i;
            }
        }
        
        // Move boxes above target to best_stack
        int boxes_to_move = stacks[target_stack].size() - target_pos - 1;
        if (boxes_to_move > 0) {
            int move_box = stacks[target_stack][target_pos + 1];
            operations.push_back({move_box, best_stack + 1});
            
            // Update stacks
            vector<int> moved_boxes;
            for (int i = target_pos + 1; i < stacks[target_stack].size(); i++) {
                moved_boxes.push_back(stacks[target_stack][i]);
            }
            
            stacks[target_stack].erase(stacks[target_stack].begin() + target_pos + 1, stacks[target_stack].end());
            
            for (int box : moved_boxes) {
                stacks[best_stack].push_back(box);
            }
        }
    }
    
    for (auto op : operations) {
        cout << op.first << " " << op.second << "\n";
    }
    
    return 0;
}