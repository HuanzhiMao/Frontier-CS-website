#include <bits/stdc++.h>
using namespace std;

struct State {
    int r, c;
    vector<vector<bool>> blocks;
};

int N, M;
vector<pair<int, int>> targets;

bool isValid(int r, int c) {
    return r >= 0 && r < N && c >= 0 && c < N;
}

pair<int, int> getDirection(char d) {
    if (d == 'U') return {-1, 0};
    if (d == 'D') return {1, 0};
    if (d == 'L') return {0, -1};
    if (d == 'R') return {0, 1};
    return {0, 0};
}

vector<pair<char, char>> findPath(State state, int targetIdx) {
    vector<pair<char, char>> result;
    int tr = targets[targetIdx].first;
    int tc = targets[targetIdx].second;
    
    // Simple greedy approach - try to reach target directly
    while (state.r != tr || state.c != tc) {
        char bestDir = 'U';
        int bestDist = INT_MAX;
        
        // Try all 4 directions for move
        for (char dir : {'U', 'D', 'L', 'R'}) {
            auto [dr, dc] = getDirection(dir);
            int nr = state.r + dr;
            int nc = state.c + dc;
            
            if (isValid(nr, nc) && !state.blocks[nr][nc]) {
                int dist = abs(nr - tr) + abs(nc - tc);
                if (dist < bestDist) {
                    bestDist = dist;
                    bestDir = dir;
                }
            }
        }
        
        // If we can't move anywhere, try to alter blocks
        if (bestDist == INT_MAX) {
            for (char dir : {'U', 'D', 'L', 'R'}) {
                auto [dr, dc] = getDirection(dir);
                int nr = state.r + dr;
                int nc = state.c + dc;
                
                if (isValid(nr, nc) && state.blocks[nr][nc]) {
                    result.push_back({'A', dir});
                    state.blocks[nr][nc] = false;
                    bestDir = dir;
                    bestDist = abs(nr - tr) + abs(nc - tc);
                    break;
                }
            }
        }
        
        if (bestDist == INT_MAX) break;
        
        auto [dr, dc] = getDirection(bestDir);
        state.r += dr;
        state.c += dc;
        result.push_back({'M', bestDir});
        
        if (result.size() > 2 * N * M) break;
    }
    
    return result;
}

int main() {
    cin >> N >> M;
    
    targets.resize(M);
    for (int i = 0; i < M; i++) {
        cin >> targets[i].first >> targets[i].second;
    }
    
    State state;
    state.r = targets[0].first;
    state.c = targets[0].second;
    state.blocks.assign(N, vector<bool>(N, false));
    
    // Add boundary blocks
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i == 0 || i == N-1 || j == 0 || j == N-1) {
                // Boundary handling is done by isValid function
            }
        }
    }
    
    vector<pair<char, char>> allMoves;
    
    for (int i = 1; i < M; i++) {
        auto moves = findPath(state, i);
        for (auto move : moves) {
            allMoves.push_back(move);
        }
        state.r = targets[i].first;
        state.c = targets[i].second;
        
        if (allMoves.size() > 2 * N * M) break;
    }
    
    for (auto [action, dir] : allMoves) {
        cout << action << " " << dir << "\n";
    }
    
    return 0;
}