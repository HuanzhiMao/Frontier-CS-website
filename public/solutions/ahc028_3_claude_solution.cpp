#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
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
    int cur_i = si, cur_j = sj;
    
    // Simple greedy approach: try to form each target string
    for (const string& target : targets) {
        bool found = false;
        
        // Try to find target as substring in current result
        if (result.find(target) != string::npos) {
            found = true;
        }
        
        if (!found) {
            // Try to extend current string to include this target
            // Look for overlap between end of result and beginning of target
            int max_overlap = 0;
            for (int k = 1; k <= min((int)result.length(), (int)target.length()); k++) {
                if (result.length() >= k && result.substr(result.length() - k) == target.substr(0, k)) {
                    max_overlap = k;
                }
            }
            
            // Add the remaining part of target
            string to_add = target.substr(max_overlap);
            
            for (char c : to_add) {
                if (moves.size() >= 5000) break;
                
                // Find closest position for character c
                int best_dist = INT_MAX;
                int best_i = -1, best_j = -1;
                
                for (auto& pos : char_pos[c - 'A']) {
                    int dist = abs(pos.first - cur_i) + abs(pos.second - cur_j) + 1;
                    if (dist < best_dist) {
                        best_dist = dist;
                        best_i = pos.first;
                        best_j = pos.second;
                    }
                }
                
                if (best_i != -1) {
                    moves.push_back({best_i, best_j});
                    result += c;
                    cur_i = best_i;
                    cur_j = best_j;
                }
            }
        }
        
        if (moves.size() >= 5000) break;
    }
    
    // Output moves
    for (auto& move : moves) {
        cout << move.first << " " << move.second << "\n";
    }
    
    return 0;
}