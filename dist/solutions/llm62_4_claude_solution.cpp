#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m;
    cin >> n >> m;
    
    vector<stack<int>> pillars(n + 2);
    
    for (int i = 1; i <= n; i++) {
        vector<int> balls(m);
        for (int j = 0; j < m; j++) {
            cin >> balls[j];
        }
        for (int j = m - 1; j >= 0; j--) {
            pillars[i].push(balls[j]);
        }
    }
    
    vector<pair<int, int>> operations;
    vector<int> target_pillar(n + 1, 0);
    
    // Assign colors to target pillars
    int pillar_idx = 1;
    for (int color = 1; color <= n; color++) {
        target_pillar[color] = pillar_idx++;
    }
    
    // Move all balls to achieve the goal
    for (int color = 1; color <= n; color++) {
        int target = target_pillar[color];
        
        // First, clear the target pillar by moving non-target balls to auxiliary pillar
        while (!pillars[target].empty() && pillars[target].top() != color) {
            operations.push_back({target, n + 1});
            pillars[n + 1].push(pillars[target].top());
            pillars[target].pop();
        }
        
        // Now collect all balls of this color
        for (int p = 1; p <= n + 1; p++) {
            if (p == target) continue;
            
            vector<int> temp_balls;
            // Remove balls until we find balls of target color or pillar is empty
            while (!pillars[p].empty()) {
                int ball_color = pillars[p].top();
                pillars[p].pop();
                
                if (ball_color == color) {
                    // Move this ball to target pillar
                    operations.push_back({p, target});
                    pillars[target].push(ball_color);
                    
                    // Put back the temp balls
                    while (!temp_balls.empty()) {
                        operations.push_back({p, p});
                        pillars[p].push(temp_balls.back());
                        temp_balls.pop_back();
                    }
                    break;
                } else {
                    temp_balls.push_back(ball_color);
                }
            }
            
            // If pillar is empty, put back temp balls
            while (!temp_balls.empty()) {
                pillars[p].push(temp_balls.back());
                temp_balls.pop_back();
            }
        }
    }
    
    // Alternative simpler approach
    operations.clear();
    
    // Rebuild pillars
    for (int i = 1; i <= n + 1; i++) {
        while (!pillars[i].empty()) pillars[i].pop();
    }
    
    for (int i = 1; i <= n; i++) {
        vector<int> balls(m);
        for (int j = 0; j < m; j++) {
            cin >> balls[j];
        }
        for (int j = m - 1; j >= 0; j--) {
            pillars[i].push(balls[j]);
        }
    }
    
    // Reset input
    cin.clear();
    cin.seekg(0, ios::beg);
    cin >> n >> m;
    
    for (int i = 1; i <= n + 1; i++) {
        while (!pillars[i].empty()) pillars[i].pop();
    }
    
    for (int i = 1; i <= n; i++) {
        vector<int> balls(m);
        for (int j = 0; j < m; j++) {
            cin >> balls[j];
        }
        for (int j = m - 1; j >= 0; j--) {
            pillars[i].push(balls[j]);
        }
    }
    
    // Simple greedy approach
    operations.clear();
    
    for (int target_color = 1; target_color <= n; target_color++) {
        // Find a pillar to be the target for this color
        int target_pillar = target_color;
        
        // Move all balls of target_color to target_pillar
        bool changed = true;
        while (changed) {
            changed = false;
            for (int p = 1; p <= n + 1; p++) {
                if (p == target_pillar || pillars[p].empty()) continue;
                
                if (pillars[p].top() == target_color && pillars[target_pillar].size() < m) {
                    operations.push_back({p, target_pillar});
                    pillars[target_pillar].push(pillars[p].top());
                    pillars[p].pop();
                    changed = true;
                } else if (pillars[p].top() != target_color && pillars[n + 1].size() < m) {
                    operations.push_back({p, n + 1});
                    pillars[n + 1].push(pillars[p].top());
                    pillars[p].pop();
                    changed = true;
                }
            }
        }
    }
    
    cout << operations.size() << "\n";
    for (auto op : operations) {
        cout << op.first << " " << op.second << "\n";
    }
    
    return 0;
}