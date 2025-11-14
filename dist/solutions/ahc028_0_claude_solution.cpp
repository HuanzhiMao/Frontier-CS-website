#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    cin >> N >> M;
    
    int si, sj;
    cin >> si >> sj;
    
    vector<string> grid(N);
    for (int i = 0; i < N; i++) {
        cin >> grid[i];
    }
    
    vector<string> targets(M);
    for (int i = 0; i < M; i++) {
        cin >> targets[i];
    }
    
    // Create position map for each character
    vector<vector<pair<int, int>>> char_pos(26);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            char_pos[grid[i][j] - 'A'].push_back({i, j});
        }
    }
    
    vector<pair<int, int>> moves;
    string result = "";
    int curr_i = si, curr_j = sj;
    
    // For each target string, find the shortest path to type it
    for (const string& target : targets) {
        // Check if target is already a substring
        if (result.find(target) != string::npos) {
            continue;
        }
        
        // Find best position to start typing this target
        int best_cost = INT_MAX;
        vector<pair<int, int>> best_moves;
        
        // Try starting from current position
        for (auto start_pos : char_pos[target[0] - 'A']) {
            vector<pair<int, int>> temp_moves;
            int cost = abs(start_pos.first - curr_i) + abs(start_pos.second - curr_j) + 1;
            temp_moves.push_back(start_pos);
            
            int temp_i = start_pos.first, temp_j = start_pos.second;
            
            // Type the rest of the target
            for (int k = 1; k < target.length(); k++) {
                char next_char = target[k];
                
                // Find closest position for next character
                int min_dist = INT_MAX;
                pair<int, int> best_next = {0, 0};
                
                for (auto pos : char_pos[next_char - 'A']) {
                    int dist = abs(pos.first - temp_i) + abs(pos.second - temp_j) + 1;
                    if (dist < min_dist) {
                        min_dist = dist;
                        best_next = pos;
                    }
                }
                
                cost += min_dist;
                temp_moves.push_back(best_next);
                temp_i = best_next.first;
                temp_j = best_next.second;
            }
            
            if (cost < best_cost) {
                best_cost = cost;
                best_moves = temp_moves;
            }
        }
        
        // Add best moves
        for (auto move : best_moves) {
            moves.push_back(move);
            result += grid[move.first][move.second];
            curr_i = move.first;
            curr_j = move.second;
        }
        
        if (moves.size() > 4000) break;
    }
    
    // Output moves
    for (auto move : moves) {
        cout << move.first << " " << move.second << "\n";
    }
    
    return 0;
}