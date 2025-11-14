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
    
    // Find which pole should contain each color
    for (int color = 1; color <= n; color++) {
        for (int pole = 1; pole <= n; pole++) {
            bool all_same = true;
            int count = 0;
            
            for (int ball : poles[pole]) {
                if (ball == color) count++;
                else if (ball != 0) all_same = false;
            }
            
            if (count == m && all_same) {
                target_pole[color] = pole;
                break;
            }
        }
        
        if (target_pole[color] == -1) {
            // Find pole with most balls of this color
            int best_pole = 1;
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
    }
    
    // Process each pole
    for (int pole = 1; pole <= n; pole++) {
        // Find target color for this pole
        int target_color = -1;
        for (int color = 1; color <= n; color++) {
            if (target_pole[color] == pole) {
                target_color = color;
                break;
            }
        }
        
        if (target_color == -1) continue;
        
        // Remove wrong colored balls from top
        while (!poles[pole].empty() && poles[pole].back() != target_color) {
            int ball = poles[pole].back();
            poles[pole].pop_back();
            
            // Move to auxiliary pole
            poles[n + 1].push_back(ball);
            operations.push_back({pole, n + 1});
        }
        
        // Move correct colored balls from auxiliary pole
        vector<int> temp;
        while (!poles[n + 1].empty()) {
            int ball = poles[n + 1].back();
            poles[n + 1].pop_back();
            
            if (ball == target_color && poles[pole].size() < m) {
                poles[pole].push_back(ball);
                operations.push_back({n + 1, pole});
            } else {
                temp.push_back(ball);
            }
        }
        
        // Put back remaining balls
        for (int i = temp.size() - 1; i >= 0; i--) {
            poles[n + 1].push_back(temp[i]);
        }
    }
    
    // Final cleanup - move remaining balls to their target poles
    while (!poles[n + 1].empty()) {
        int ball = poles[n + 1].back();
        poles[n + 1].pop_back();
        
        int target = target_pole[ball];
        if (poles[target].size() < m) {
            poles[target].push_back(ball);
            operations.push_back({n + 1, target});
        }
    }
    
    // Additional passes to fix remaining issues
    bool changed = true;
    while (changed) {
        changed = false;
        
        for (int pole = 1; pole <= n; pole++) {
            int target_color = -1;
            for (int color = 1; color <= n; color++) {
                if (target_pole[color] == pole) {
                    target_color = color;
                    break;
                }
            }
            
            if (target_color == -1) continue;
            
            while (!poles[pole].empty() && poles[pole].back() != target_color) {
                int ball = poles[pole].back();
                poles[pole].pop_back();
                poles[n + 1].push_back(ball);
                operations.push_back({pole, n + 1});
                changed = true;
            }
        }
        
        while (!poles[n + 1].empty()) {
            int ball = poles[n + 1].back();
            poles[n + 1].pop_back();
            
            int target = target_pole[ball];
            if (poles[target].size() < m) {
                poles[target].push_back(ball);
                operations.push_back({n + 1, target});
                changed = true;
            }
        }
    }
    
    cout << operations.size() << "\n";
    for (auto op : operations) {
        cout << op.first << " " << op.second << "\n";
    }
    
    return 0;
}