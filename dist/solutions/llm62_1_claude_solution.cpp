#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m;
    cin >> n >> m;
    
    vector<stack<int>> pillars(n + 2);
    
    for (int i = 1; i <= n; i++) {
        vector<int> temp(m);
        for (int j = 0; j < m; j++) {
            cin >> temp[j];
        }
        for (int j = m - 1; j >= 0; j--) {
            pillars[i].push(temp[j]);
        }
    }
    
    vector<pair<int, int>> operations;
    
    // For each color, gather all balls to one pillar
    for (int color = 1; color <= n; color++) {
        int target_pillar = color;
        
        // First, clear the target pillar if it has wrong colored balls on top
        while (!pillars[target_pillar].empty() && pillars[target_pillar].top() != color) {
            int ball = pillars[target_pillar].top();
            pillars[target_pillar].pop();
            pillars[n + 1].push(ball);
            operations.push_back({target_pillar, n + 1});
        }
        
        // Now collect all balls of this color
        for (int p = 1; p <= n + 1; p++) {
            if (p == target_pillar) continue;
            
            vector<int> temp_balls;
            
            // Move balls from top until we find balls of current color
            while (!pillars[p].empty()) {
                int ball = pillars[p].top();
                pillars[p].pop();
                
                if (ball == color) {
                    // Found a ball of target color, move it to target pillar
                    pillars[target_pillar].push(ball);
                    operations.push_back({p, target_pillar});
                } else {
                    temp_balls.push_back(ball);
                }
            }
            
            // Put back the non-target colored balls
            for (int i = temp_balls.size() - 1; i >= 0; i--) {
                pillars[p].push(temp_balls[i]);
                operations.push_back({target_pillar, p});
                
                // Move the target ball we just placed back
                if (!pillars[target_pillar].empty()) {
                    int moved_ball = pillars[target_pillar].top();
                    pillars[target_pillar].pop();
                    pillars[p].push(moved_ball);
                }
            }
        }
    }
    
    cout << operations.size() << "\n";
    for (auto& op : operations) {
        cout << op.first << " " << op.second << "\n";
    }
    
    return 0;
}