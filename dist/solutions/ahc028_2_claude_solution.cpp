#include <bits/stdc++.h>
using namespace std;

struct State {
    int i, j, cost;
};

int main() {
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
    
    // Find positions of each character
    vector<vector<pair<int, int>>> pos(26);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            pos[grid[i][j] - 'A'].push_back({i, j});
        }
    }
    
    vector<pair<int, int>> moves;
    string result = "";
    int curI = si, curJ = sj;
    
    // Simple greedy approach: type each target string
    for (const string& target : targets) {
        // Check if target is already in result
        if (result.find(target) != string::npos) continue;
        
        // Find best position to start typing this target
        int bestCost = INT_MAX;
        vector<pair<int, int>> bestMoves;
        
        // Try starting from current position
        function<void(int, int, int, string, vector<pair<int, int>>&)> dfs = 
            [&](int pos_in_target, int ci, int cj, string curr, vector<pair<int, int>>& path) {
            if (pos_in_target == target.length()) {
                // Check if this helps us complete the target
                string newResult = result + curr;
                if (newResult.find(target) != string::npos) {
                    int totalCost = 0;
                    int ti = curI, tj = curJ;
                    for (auto [ni, nj] : path) {
                        totalCost += abs(ni - ti) + abs(nj - tj) + 1;
                        ti = ni;
                        tj = nj;
                    }
                    if (totalCost < bestCost) {
                        bestCost = totalCost;
                        bestMoves = path;
                    }
                }
                return;
            }
            
            char needed = target[pos_in_target];
            for (auto [ni, nj] : pos[needed - 'A']) {
                if (path.size() < 25) { // Limit search depth
                    path.push_back({ni, nj});
                    dfs(pos_in_target + 1, ni, nj, curr + needed, path);
                    path.pop_back();
                }
            }
        };
        
        vector<pair<int, int>> path;
        dfs(0, curI, curJ, "", path);
        
        if (!bestMoves.empty()) {
            for (auto [ni, nj] : bestMoves) {
                moves.push_back({ni, nj});
                result += grid[ni][nj];
                curI = ni;
                curJ = nj;
            }
        } else {
            // Fallback: just type the target character by character
            for (char c : target) {
                // Find closest position for this character
                int bestDist = INT_MAX;
                int bestI = -1, bestJ = -1;
                for (auto [ni, nj] : pos[c - 'A']) {
                    int dist = abs(ni - curI) + abs(nj - curJ);
                    if (dist < bestDist) {
                        bestDist = dist;
                        bestI = ni;
                        bestJ = nj;
                    }
                }
                moves.push_back({bestI, bestJ});
                result += c;
                curI = bestI;
                curJ = bestJ;
            }
        }
        
        if (moves.size() >= 4500) break; // Stay under limit
    }
    
    // Output moves
    for (auto [i, j] : moves) {
        cout << i << " " << j << "\n";
    }
    
    return 0;
}