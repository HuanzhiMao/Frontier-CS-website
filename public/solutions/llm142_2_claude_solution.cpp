#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m;
    cin >> n >> m;
    
    vector<stack<int>> poles(n + 2);
    vector<vector<int>> temp(n + 1);
    
    for (int i = 1; i <= n; i++) {
        for (int j = 0; j < m; j++) {
            int color;
            cin >> color;
            temp[i].push_back(color);
        }
        for (int j = m - 1; j >= 0; j--) {
            poles[i].push(temp[i][j]);
        }
    }
    
    vector<pair<int, int>> operations;
    vector<int> target_pole(n + 1, 0);
    
    // Find target pole for each color
    for (int color = 1; color <= n; color++) {
        for (int pole = 1; pole <= n; pole++) {
            if (target_pole[color] == 0) {
                bool valid = true;
                vector<int> temp_stack;
                stack<int> temp_pole = poles[pole];
                
                while (!temp_pole.empty()) {
                    temp_stack.push_back(temp_pole.top());
                    temp_pole.pop();
                }
                
                int count = 0;
                for (int ball : temp_stack) {
                    if (ball == color) count++;
                }
                
                if (count > 0) {
                    target_pole[color] = pole;
                    break;
                }
            }
        }
    }
    
    // Process each pole
    for (int pole = 1; pole <= n; pole++) {
        while (!poles[pole].empty()) {
            int top_color = poles[pole].top();
            
            if (target_pole[top_color] == pole) {
                // Check if all remaining balls in this pole are of the same color
                bool all_same = true;
                stack<int> temp_stack = poles[pole];
                while (!temp_stack.empty()) {
                    if (temp_stack.top() != top_color) {
                        all_same = false;
                        break;
                    }
                    temp_stack.pop();
                }
                
                if (all_same) {
                    break;
                }
            }
            
            // Move ball to auxiliary pole or target pole
            int dest_pole;
            if (target_pole[top_color] == pole) {
                dest_pole = n + 1;
            } else {
                dest_pole = target_pole[top_color];
                if (poles[dest_pole].size() >= m) {
                    dest_pole = n + 1;
                }
            }
            
            if (poles[dest_pole].size() < m) {
                operations.push_back({pole, dest_pole});
                poles[dest_pole].push(poles[pole].top());
                poles[pole].pop();
            } else {
                // Find any available pole
                for (int p = 1; p <= n + 1; p++) {
                    if (p != pole && poles[p].size() < m) {
                        dest_pole = p;
                        break;
                    }
                }
                operations.push_back({pole, dest_pole});
                poles[dest_pole].push(poles[pole].top());
                poles[pole].pop();
            }
        }
        
        // Move balls back from auxiliary pole if needed
        while (!poles[n + 1].empty()) {
            int top_color = poles[n + 1].top();
            int dest = target_pole[top_color];
            
            if (poles[dest].size() < m) {
                operations.push_back({n + 1, dest});
                poles[dest].push(poles[n + 1].top());
                poles[n + 1].pop();
            } else {
                break;
            }
        }
    }
    
    // Final cleanup
    while (!poles[n + 1].empty()) {
        int top_color = poles[n + 1].top();
        for (int p = 1; p <= n; p++) {
            if (poles[p].size() < m) {
                bool can_place = poles[p].empty() || poles[p].top() == top_color;
                if (can_place) {
                    operations.push_back({n + 1, p});
                    poles[p].push(poles[n + 1].top());
                    poles[n + 1].pop();
                    break;
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