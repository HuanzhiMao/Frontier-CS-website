#include <bits/stdc++.h>
using namespace std;

struct State {
    int i, j;
    vector<vector<bool>> blocks;
    int target_idx;
};

int main() {
    int N, M;
    cin >> N >> M;
    
    vector<pair<int, int>> targets(M);
    for (int i = 0; i < M; i++) {
        cin >> targets[i].first >> targets[i].second;
    }
    
    State state;
    state.i = targets[0].first;
    state.j = targets[0].second;
    state.blocks = vector<vector<bool>>(N, vector<bool>(N, false));
    state.target_idx = 1;
    
    vector<string> actions;
    
    int di[] = {-1, 1, 0, 0};
    int dj[] = {0, 0, -1, 1};
    char dirs[] = {'U', 'D', 'L', 'R'};
    
    while (state.target_idx < M && actions.size() < 2 * N * M) {
        int target_i = targets[state.target_idx].first;
        int target_j = targets[state.target_idx].second;
        
        bool moved = false;
        
        // Try direct move to adjacent target
        for (int d = 0; d < 4; d++) {
            int ni = state.i + di[d];
            int nj = state.j + dj[d];
            
            if (ni >= 0 && ni < N && nj >= 0 && nj < N && 
                ni == target_i && nj == target_j && !state.blocks[ni][nj]) {
                actions.push_back(string("M") + dirs[d]);
                state.i = ni;
                state.j = nj;
                state.target_idx++;
                moved = true;
                break;
            }
        }
        
        if (moved) continue;
        
        // Try sliding to target
        for (int d = 0; d < 4; d++) {
            int ni = state.i;
            int nj = state.j;
            bool can_slide = false;
            bool hits_target = false;
            
            while (true) {
                int next_i = ni + di[d];
                int next_j = nj + dj[d];
                
                if (next_i < 0 || next_i >= N || next_j < 0 || next_j >= N || 
                    state.blocks[next_i][next_j]) {
                    break;
                }
                
                ni = next_i;
                nj = next_j;
                can_slide = true;
                
                if (ni == target_i && nj == target_j) {
                    hits_target = true;
                    break;
                }
            }
            
            if (can_slide && hits_target) {
                actions.push_back(string("S") + dirs[d]);
                state.i = ni;
                state.j = nj;
                state.target_idx++;
                moved = true;
                break;
            }
        }
        
        if (moved) continue;
        
        // Simple pathfinding with moves
        int best_d = -1;
        int best_dist = INT_MAX;
        
        for (int d = 0; d < 4; d++) {
            int ni = state.i + di[d];
            int nj = state.j + dj[d];
            
            if (ni >= 0 && ni < N && nj >= 0 && nj < N && !state.blocks[ni][nj]) {
                int dist = abs(ni - target_i) + abs(nj - target_j);
                if (dist < best_dist) {
                    best_dist = dist;
                    best_d = d;
                }
            }
        }
        
        if (best_d != -1) {
            actions.push_back(string("M") + dirs[best_d]);
            state.i += di[best_d];
            state.j += dj[best_d];
            moved = true;
        }
        
        if (!moved) {
            // Place block to create stopping point
            for (int d = 0; d < 4; d++) {
                int ni = state.i + di[d];
                int nj = state.j + dj[d];
                
                if (ni >= 0 && ni < N && nj >= 0 && nj < N && !state.blocks[ni][nj]) {
                    actions.push_back(string("A") + dirs[d]);
                    state.blocks[ni][nj] = true;
                    moved = true;
                    break;
                }
            }
        }
        
        if (!moved) break;
    }
    
    for (const string& action : actions) {
        cout << action[0] << " " << action[1] << "\n";
    }
    
    return 0;
}