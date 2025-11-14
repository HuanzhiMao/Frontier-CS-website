#include <bits/stdc++.h>
using namespace std;

struct State {
    int r, c;
    vector<vector<bool>> blocks;
};

int dr[] = {-1, 1, 0, 0};
int dc[] = {0, 0, -1, 1};
char dirs[] = {'U', 'D', 'L', 'R'};

bool inBounds(int r, int c, int N) {
    return r >= 0 && r < N && c >= 0 && c < N;
}

pair<int, int> slide(int r, int c, int dir, const vector<vector<bool>>& blocks, int N) {
    while (true) {
        int nr = r + dr[dir];
        int nc = c + dc[dir];
        if (!inBounds(nr, nc, N) || blocks[nr][nc]) {
            break;
        }
        r = nr;
        c = nc;
    }
    return {r, c};
}

vector<pair<char, char>> solve(int N, int M, vector<pair<int, int>>& targets) {
    vector<pair<char, char>> result;
    State state;
    state.r = targets[0].first;
    state.c = targets[0].second;
    state.blocks = vector<vector<bool>>(N, vector<bool>(N, false));
    
    for (int target_idx = 1; target_idx < M && result.size() < 2 * N * M; target_idx++) {
        int target_r = targets[target_idx].first;
        int target_c = targets[target_idx].second;
        
        // Simple greedy approach to reach the target
        while ((state.r != target_r || state.c != target_c) && result.size() < 2 * N * M) {
            bool moved = false;
            
            // Try to move directly towards target
            for (int dir = 0; dir < 4; dir++) {
                int nr = state.r + dr[dir];
                int nc = state.c + dc[dir];
                
                if (!inBounds(nr, nc, N)) continue;
                
                // Calculate Manhattan distance improvement
                int curr_dist = abs(state.r - target_r) + abs(state.c - target_c);
                int new_dist = abs(nr - target_r) + abs(nc - target_c);
                
                if (new_dist < curr_dist) {
                    if (state.blocks[nr][nc]) {
                        // Remove block
                        result.push_back({'A', dirs[dir]});
                        state.blocks[nr][nc] = false;
                    } else {
                        // Move
                        result.push_back({'M', dirs[dir]});
                        state.r = nr;
                        state.c = nc;
                    }
                    moved = true;
                    break;
                }
            }
            
            if (!moved) {
                // Try sliding
                for (int dir = 0; dir < 4; dir++) {
                    auto [slide_r, slide_c] = slide(state.r, state.c, dir, state.blocks, N);
                    if (slide_r != state.r || slide_c != state.c) {
                        int curr_dist = abs(state.r - target_r) + abs(state.c - target_c);
                        int slide_dist = abs(slide_r - target_r) + abs(slide_c - target_c);
                        
                        if (slide_dist < curr_dist) {
                            result.push_back({'S', dirs[dir]});
                            state.r = slide_r;
                            state.c = slide_c;
                            moved = true;
                            break;
                        }
                    }
                }
            }
            
            if (!moved) {
                // Just move in any valid direction
                for (int dir = 0; dir < 4; dir++) {
                    int nr = state.r + dr[dir];
                    int nc = state.c + dc[dir];
                    
                    if (!inBounds(nr, nc, N)) continue;
                    
                    if (state.blocks[nr][nc]) {
                        result.push_back({'A', dirs[dir]});
                        state.blocks[nr][nc] = false;
                    } else {
                        result.push_back({'M', dirs[dir]});
                        state.r = nr;
                        state.c = nc;
                    }
                    moved = true;
                    break;
                }
            }
            
            if (!moved) break;
        }
        
        if (result.size() >= 2 * N * M) break;
    }
    
    return result;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    cin >> N >> M;
    
    vector<pair<int, int>> targets(M);
    for (int i = 0; i < M; i++) {
        cin >> targets[i].first >> targets[i].second;
    }
    
    auto result = solve(N, M, targets);
    
    for (auto& action : result) {
        cout << action.first << " " << action.second << "\n";
    }
    
    return 0;
}