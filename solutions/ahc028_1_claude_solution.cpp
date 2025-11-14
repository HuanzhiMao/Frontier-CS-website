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
    string result = "";
    int curI = si, curJ = sj;
    
    // Simple greedy approach: concatenate all target strings
    for (const string& target : targets) {
        for (char c : target) {
            // Find closest position with character c
            int bestI = -1, bestJ = -1;
            int minDist = INT_MAX;
            
            for (auto& pos : charPos[c]) {
                int dist = abs(pos.first - curI) + abs(pos.second - curJ);
                if (dist < minDist) {
                    minDist = dist;
                    bestI = pos.first;
                    bestJ = pos.second;
                }
            }
            
            moves.push_back({bestI, bestJ});
            result += c;
            curI = bestI;
            curJ = bestJ;
        }
    }
    
    for (auto& move : moves) {
        cout << move.first << " " << move.second << "\n";
    }
    
    return 0;
}