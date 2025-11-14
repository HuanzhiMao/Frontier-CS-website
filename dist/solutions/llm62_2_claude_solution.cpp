#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m;
    cin >> n >> m;
    
    vector<vector<int>> pillars(n + 2);
    
    for (int i = 1; i <= n; i++) {
        pillars[i].resize(m);
        for (int j = 0; j < m; j++) {
            cin >> pillars[i][j];
        }
    }
    
    vector<pair<int, int>> operations;
    
    // For each color 1 to n, gather all balls of that color to pillar corresponding to that color
    for (int color = 1; color <= n; color++) {
        // First, move all balls from pillar 'color' to temporary pillar (n+1)
        while (!pillars[color].empty()) {
            int top = pillars[color].back();
            pillars[color].pop_back();
            pillars[n + 1].push_back(top);
            operations.push_back({color, n + 1});
        }
        
        // Now collect all balls of current color from all pillars to pillar 'color'
        for (int p = 1; p <= n + 1; p++) {
            if (p == color) continue;
            
            vector<int> temp;
            // Remove balls from pillar p, keeping track of balls that are not of current color
            while (!pillars[p].empty()) {
                int top = pillars[p].back();
                pillars[p].pop_back();
                if (top == color) {
                    // Move this ball to target pillar
                    pillars[color].push_back(top);
                    operations.push_back({p, color});
                } else {
                    // Store temporarily
                    temp.push_back(top);
                    if (p != n + 1) {
                        pillars[n + 1].push_back(top);
                        operations.push_back({p, n + 1});
                    }
                }
            }
            
            if (p != n + 1) {
                // Put back non-target colored balls from temp pillar to original pillar
                while (!temp.empty() && pillars[p].size() < m) {
                    int ball = pillars[n + 1].back();
                    pillars[n + 1].pop_back();
                    pillars[p].push_back(ball);
                    operations.push_back({n + 1, p});
                    temp.pop_back();
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