#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m;
    cin >> n >> m;
    
    vector<stack<int>> poles(n + 2);
    
    for (int i = 1; i <= n; i++) {
        for (int j = 0; j < m; j++) {
            int color;
            cin >> color;
            poles[i].push(color);
        }
    }
    
    vector<pair<int, int>> moves;
    vector<int> target_pole(n + 1, -1);
    
    // Find target pole for each color
    for (int color = 1; color <= n; color++) {
        for (int pole = 1; pole <= n; pole++) {
            if (target_pole[color] != -1) break;
            
            // Count how many balls of this color are at the bottom
            stack<int> temp = poles[pole];
            vector<int> balls;
            while (!temp.empty()) {
                balls.push_back(temp.top());
                temp.pop();
            }
            reverse(balls.begin(), balls.end());
            
            int bottom_count = 0;
            for (int i = 0; i < balls.size() && balls[i] == color; i++) {
                bottom_count++;
            }
            
            if (bottom_count > 0) {
                target_pole[color] = pole;
            }
        }
        
        // If no pole found, assign first available
        if (target_pole[color] == -1) {
            for (int pole = 1; pole <= n; pole++) {
                if (target_pole[color] == -1) {
                    bool used = false;
                    for (int c = 1; c < color; c++) {
                        if (target_pole[c] == pole) {
                            used = true;
                            break;
                        }
                    }
                    if (!used) {
                        target_pole[color] = pole;
                    }
                }
            }
        }
    }
    
    // Process each pole
    for (int pole = 1; pole <= n; pole++) {
        while (!poles[pole].empty()) {
            int top_color = poles[pole].top();
            
            if (target_pole[top_color] == pole) {
                // Check if all balls below are the same color
                stack<int> temp = poles[pole];
                bool all_same = true;
                while (!temp.empty()) {
                    if (temp.top() != top_color) {
                        all_same = false;
                        break;
                    }
                    temp.pop();
                }
                if (all_same) break;
            }
            
            // Move to temporary pole or target pole
            int dest = n + 1;
            if (target_pole[top_color] != pole && poles[target_pole[top_color]].size() < m) {
                // Check if target pole only has same color balls
                bool can_move_to_target = true;
                if (!poles[target_pole[top_color]].empty()) {
                    stack<int> temp = poles[target_pole[top_color]];
                    while (!temp.empty()) {
                        if (temp.top() != top_color) {
                            can_move_to_target = false;
                            break;
                        }
                        temp.pop();
                    }
                }
                if (can_move_to_target) {
                    dest = target_pole[top_color];
                }
            }
            
            if (dest == n + 1 && poles[n + 1].size() >= m) {
                // Need to move some balls from temp pole
                while (poles[n + 1].size() >= m) {
                    int temp_color = poles[n + 1].top();
                    poles[n + 1].pop();
                    poles[target_pole[temp_color]].push(temp_color);
                    moves.push_back({n + 1, target_pole[temp_color]});
                }
            }
            
            poles[pole].pop();
            poles[dest].push(top_color);
            moves.push_back({pole, dest});
        }
    }
    
    // Move remaining balls from temp pole
    while (!poles[n + 1].empty()) {
        int color = poles[n + 1].top();
        poles[n + 1].pop();
        poles[target_pole[color]].push(color);
        moves.push_back({n + 1, target_pole[color]});
    }
    
    cout << moves.size() << "\n";
    for (auto move : moves) {
        cout << move.first << " " << move.second << "\n";
    }
    
    return 0;
}