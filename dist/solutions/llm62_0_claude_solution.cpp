#include <iostream>
#include <vector>
#include <stack>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;
    
    vector<stack<int>> pillars(n + 2);
    
    for (int i = 1; i <= n; i++) {
        vector<int> temp(m);
        for (int j = 0; j < m; j++) {
            cin >> temp[j];
        }
        for (int j = 0; j < m; j++) {
            pillars[i].push(temp[j]);
        }
    }
    
    vector<pair<int, int>> operations;
    
    // Process each color from 1 to n
    for (int color = 1; color <= n; color++) {
        // Find target pillar for this color (first pillar that has this color on top or is empty)
        int target = -1;
        for (int i = 1; i <= n + 1; i++) {
            if (pillars[i].empty() || pillars[i].top() == color) {
                target = i;
                break;
            }
        }
        
        // Collect all balls of this color to the target pillar
        for (int collected = 0; collected < m; ) {
            bool found = false;
            
            // Look for this color on top of any pillar
            for (int i = 1; i <= n + 1; i++) {
                if (!pillars[i].empty() && pillars[i].top() == color) {
                    if (i != target) {
                        // Move to target
                        operations.push_back({i, target});
                        pillars[target].push(pillars[i].top());
                        pillars[i].pop();
                        collected++;
                    } else {
                        collected++;
                    }
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                // Need to move other balls out of the way
                for (int i = 1; i <= n + 1; i++) {
                    if (!pillars[i].empty()) {
                        // Find a pillar to temporarily move the top ball
                        for (int j = 1; j <= n + 1; j++) {
                            if (i != j && pillars[j].size() < m) {
                                operations.push_back({i, j});
                                pillars[j].push(pillars[i].top());
                                pillars[i].pop();
                                found = true;
                                break;
                            }
                        }
                        if (found) break;
                    }
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