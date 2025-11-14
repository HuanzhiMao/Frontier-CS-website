#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m;
    cin >> n >> m;
    
    vector<stack<int>> poles(n + 2);
    
    for (int i = 1; i <= n; i++) {
        vector<int> balls(m);
        for (int j = 0; j < m; j++) {
            cin >> balls[j];
        }
        for (int j = 0; j < m; j++) {
            poles[i].push(balls[j]);
        }
    }
    
    vector<pair<int, int>> operations;
    vector<bool> finished(n + 1, false);
    
    while (true) {
        bool changed = false;
        
        // First, try to move balls to their correct poles if possible
        for (int i = 1; i <= n; i++) {
            if (finished[i] || poles[i].empty()) continue;
            
            int topColor = poles[i].top();
            
            // Check if pole i should contain this color
            bool shouldContain = true;
            stack<int> temp = poles[i];
            while (!temp.empty()) {
                if (temp.top() != topColor) {
                    shouldContain = false;
                    break;
                }
                temp.pop();
            }
            
            if (shouldContain && poles[i].size() == m) {
                finished[i] = true;
                continue;
            }
            
            if (!shouldContain) {
                // Try to move to correct pole
                bool moved = false;
                for (int j = 1; j <= n; j++) {
                    if (i == j) continue;
                    
                    // Check if pole j should contain topColor
                    bool canMoveTo = true;
                    if (!poles[j].empty()) {
                        stack<int> temp2 = poles[j];
                        while (!temp2.empty()) {
                            if (temp2.top() != topColor) {
                                canMoveTo = false;
                                break;
                            }
                            temp2.pop();
                        }
                    }
                    
                    if (canMoveTo && poles[j].size() < m) {
                        poles[j].push(poles[i].top());
                        poles[i].pop();
                        operations.push_back({i, j});
                        moved = true;
                        changed = true;
                        break;
                    }
                }
                
                if (!moved && poles[n + 1].size() < m) {
                    poles[n + 1].push(poles[i].top());
                    poles[i].pop();
                    operations.push_back({i, n + 1});
                    changed = true;
                }
            }
        }
        
        // Move from temporary pole to correct poles
        if (!poles[n + 1].empty()) {
            int topColor = poles[n + 1].top();
            bool moved = false;
            
            for (int j = 1; j <= n; j++) {
                bool canMoveTo = true;
                if (!poles[j].empty()) {
                    stack<int> temp = poles[j];
                    while (!temp.empty()) {
                        if (temp.top() != topColor) {
                            canMoveTo = false;
                            break;
                        }
                        temp.pop();
                    }
                }
                
                if (canMoveTo && poles[j].size() < m) {
                    poles[j].push(poles[n + 1].top());
                    poles[n + 1].pop();
                    operations.push_back({n + 1, j});
                    moved = true;
                    changed = true;
                    break;
                }
            }
        }
        
        if (!changed) break;
    }
    
    cout << operations.size() << "\n";
    for (auto op : operations) {
        cout << op.first << " " << op.second << "\n";
    }
    
    return 0;
}