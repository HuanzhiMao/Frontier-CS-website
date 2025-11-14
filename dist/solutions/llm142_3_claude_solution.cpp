#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m;
    cin >> n >> m;
    
    vector<vector<int>> poles(n + 2);
    
    for (int i = 1; i <= n; i++) {
        poles[i].resize(m);
        for (int j = 0; j < m; j++) {
            cin >> poles[i][j];
        }
    }
    
    vector<pair<int, int>> operations;
    vector<int> target_pole(n + 1, -1);
    
    // Find target pole for each color
    for (int color = 1; color <= n; color++) {
        int best_pole = -1;
        int max_count = 0;
        
        for (int pole = 1; pole <= n; pole++) {
            if (target_pole[color] != -1) break;
            
            int count = 0;
            for (int ball : poles[pole]) {
                if (ball == color) count++;
            }
            
            if (count > max_count) {
                max_count = count;
                best_pole = pole;
            }
        }
        
        target_pole[color] = best_pole;
    }
    
    // Process each pole
    for (int pole = 1; pole <= n; pole++) {
        // First, move all balls that don't belong here to temporary pole
        while (!poles[pole].empty()) {
            int top_color = poles[pole].back();
            if (target_pole[top_color] == pole) {
                break;
            }
            
            // Move to temporary pole
            poles[n + 1].push_back(top_color);
            poles[pole].pop_back();
            operations.push_back({pole, n + 1});
        }
        
        // Now move back the balls that belong to this pole
        vector<int> temp_stack;
        while (!poles[n + 1].empty()) {
            int top_color = poles[n + 1].back();
            poles[n + 1].pop_back();
            
            if (target_pole[top_color] == pole) {
                poles[pole].push_back(top_color);
                operations.push_back({n + 1, pole});
            } else {
                temp_stack.push_back(top_color);
            }
        }
        
        // Put back remaining balls to temporary pole
        for (int i = temp_stack.size() - 1; i >= 0; i--) {
            poles[n + 1].push_back(temp_stack[i]);
        }
    }
    
    // Final cleanup - distribute remaining balls from temporary pole
    while (!poles[n + 1].empty()) {
        int top_color = poles[n + 1].back();
        poles[n + 1].pop_back();
        
        int dest_pole = target_pole[top_color];
        poles[dest_pole].push_back(top_color);
        operations.push_back({n + 1, dest_pole});
    }
    
    cout << operations.size() << "\n";
    for (auto op : operations) {
        cout << op.first << " " << op.second << "\n";
    }
    
    return 0;
}