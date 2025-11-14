#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m;
    cin >> n >> m;
    
    vector<stack<int>> pillars(n + 2);
    vector<pair<int, int>> operations;
    
    for (int i = 1; i <= n; i++) {
        vector<int> balls(m);
        for (int j = 0; j < m; j++) {
            cin >> balls[j];
        }
        for (int j = m - 1; j >= 0; j--) {
            pillars[i].push(balls[j]);
        }
    }
    
    // Find target pillar for each color
    vector<int> target(n + 1);
    int pillar_idx = 1;
    for (int color = 1; color <= n; color++) {
        target[color] = pillar_idx++;
    }
    
    // Process each pillar
    for (int p = 1; p <= n; p++) {
        while (!pillars[p].empty()) {
            int color = pillars[p].top();
            
            if (target[color] == p) {
                // Already in correct position, check if all below are same color
                vector<int> temp;
                bool correct = true;
                
                while (!pillars[p].empty()) {
                    int c = pillars[p].top();
                    pillars[p].pop();
                    temp.push_back(c);
                    if (c != color) {
                        correct = false;
                    }
                }
                
                // Put back in reverse order
                for (int i = temp.size() - 1; i >= 0; i--) {
                    pillars[p].push(temp[i]);
                }
                
                if (correct) {
                    break; // This pillar is done
                } else {
                    // Need to move the top ball
                    pillars[p].pop();
                    
                    if (pillars[target[color]].size() < m) {
                        pillars[target[color]].push(color);
                        operations.push_back({p, target[color]});
                    } else {
                        // Use auxiliary pillar
                        pillars[n + 1].push(color);
                        operations.push_back({p, n + 1});
                    }
                }
            } else {
                // Move to correct pillar or auxiliary
                pillars[p].pop();
                
                if (pillars[target[color]].size() < m) {
                    pillars[target[color]].push(color);
                    operations.push_back({p, target[color]});
                } else {
                    pillars[n + 1].push(color);
                    operations.push_back({p, n + 1});
                }
            }
        }
    }
    
    // Move balls from auxiliary pillar to their correct positions
    while (!pillars[n + 1].empty()) {
        int color = pillars[n + 1].top();
        pillars[n + 1].pop();
        
        if (pillars[target[color]].size() < m) {
            pillars[target[color]].push(color);
            operations.push_back({n + 1, target[color]});
        } else {
            // Find an empty spot or make room
            for (int p = 1; p <= n; p++) {
                if (pillars[p].size() < m) {
                    pillars[p].push(color);
                    operations.push_back({n + 1, p});
                    break;
                }
            }
        }
    }
    
    // Final cleanup - move misplaced balls
    bool changed = true;
    while (changed) {
        changed = false;
        for (int p = 1; p <= n; p++) {
            if (!pillars[p].empty()) {
                int color = pillars[p].top();
                if (target[color] != p && pillars[target[color]].size() < m) {
                    pillars[p].pop();
                    pillars[target[color]].push(color);
                    operations.push_back({p, target[color]});
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