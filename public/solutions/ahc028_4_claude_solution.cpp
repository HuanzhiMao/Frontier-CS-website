#include <bits/stdc++.h>
using namespace std;

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
    
    // Create position map for each character
    map<char, vector<pair<int, int>>> charPos;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            charPos[grid[i][j]].push_back({i, j});
        }
    }
    
    vector<pair<int, int>> moves;
    int curI = si, curJ = sj;
    
    // Simple greedy approach: try to form each target string
    for (const string& target : targets) {
        for (char c : target) {
            // Find closest position with character c
            int bestDist = 1e9;
            int bestI = -1, bestJ = -1;
            
            for (auto [i, j] : charPos[c]) {
                int dist = abs(i - curI) + abs(j - curJ) + 1;
                if (dist < bestDist) {
                    bestDist = dist;
                    bestI = i;
                    bestJ = j;
                }
            }
            
            moves.push_back({bestI, bestJ});
            curI = bestI;
            curJ = bestJ;
            
            if (moves.size() >= 5000) break;
        }
        if (moves.size() >= 5000) break;
    }
    
    for (auto [i, j] : moves) {
        cout << i << " " << j << "\n";
    }
    
    return 0;
}