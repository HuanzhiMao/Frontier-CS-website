#include <bits/stdc++.h>
using namespace std;

int main() {
    int N, M;
    cin >> N >> M;
    
    vector<pair<int, int>> targets(M);
    for (int i = 0; i < M; i++) {
        cin >> targets[i].first >> targets[i].second;
    }
    
    vector<string> actions;
    int cx = targets[0].first, cy = targets[0].second;
    vector<vector<bool>> blocked(N, vector<bool>(N, false));
    
    // Add boundary blocks
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i == 0 || i == N-1 || j == 0 || j == N-1) {
                blocked[i][j] = true;
            }
        }
    }
    
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    string dirs = "UDLR";
    
    for (int target_idx = 1; target_idx < M; target_idx++) {
        int tx = targets[target_idx].first;
        int ty = targets[target_idx].second;
        
        while (cx != tx || cy != ty) {
            int best_dir = -1;
            int min_dist = INT_MAX;
            
            // Try each direction
            for (int d = 0; d < 4; d++) {
                int nx = cx + dx[d];
                int ny = cy + dy[d];
                
                if (nx >= 0 && nx < N && ny >= 0 && ny < N && !blocked[nx][ny]) {
                    int dist = abs(nx - tx) + abs(ny - ty);
                    if (dist < min_dist) {
                        min_dist = dist;
                        best_dir = d;
                    }
                }
            }
            
            if (best_dir != -1) {
                actions.push_back("M " + string(1, dirs[best_dir]));
                cx += dx[best_dir];
                cy += dy[best_dir];
            } else {
                break;
            }
            
            if (actions.size() >= 2 * N * M) break;
        }
        
        if (actions.size() >= 2 * N * M) break;
    }
    
    for (const string& action : actions) {
        cout << action << "\n";
    }
    
    return 0;
}