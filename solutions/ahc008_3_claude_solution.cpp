#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    cin >> N;
    
    vector<int> px(N), py(N), pt(N);
    for (int i = 0; i < N; i++) {
        cin >> px[i] >> py[i] >> pt[i];
    }
    
    int M;
    cin >> M;
    
    vector<int> hx(M), hy(M);
    for (int i = 0; i < M; i++) {
        cin >> hx[i] >> hy[i];
    }
    
    vector<vector<bool>> blocked(31, vector<bool>(31, false));
    
    // Mark boundaries as blocked
    for (int i = 0; i <= 30; i++) {
        if (i == 0 || i == 30) {
            for (int j = 0; j <= 30; j++) {
                blocked[i][j] = true;
            }
        } else {
            blocked[i][0] = blocked[i][30] = true;
        }
    }
    
    for (int turn = 0; turn < 300; turn++) {
        string actions(M, '.');
        
        // Simple strategy: try to create walls around pets
        for (int i = 0; i < M; i++) {
            int x = hx[i], y = hy[i];
            
            // Try to place walls in order: up, down, left, right
            vector<pair<int, int>> dirs = {{x-1, y}, {x+1, y}, {x, y-1}, {x, y+1}};
            vector<char> wall_chars = {'u', 'd', 'l', 'r'};
            
            bool placed_wall = false;
            
            for (int d = 0; d < 4; d++) {
                int nx = dirs[d].first;
                int ny = dirs[d].second;
                
                if (nx < 1 || nx > 30 || ny < 1 || ny > 30) continue;
                if (blocked[nx][ny]) continue;
                
                // Check if any pet or human is at this position
                bool occupied = false;
                for (int j = 0; j < N; j++) {
                    if (px[j] == nx && py[j] == ny) {
                        occupied = true;
                        break;
                    }
                }
                if (!occupied) {
                    for (int j = 0; j < M; j++) {
                        if (hx[j] == nx && hy[j] == ny) {
                            occupied = true;
                            break;
                        }
                    }
                }
                
                // Check if any pet is adjacent to this position
                if (!occupied) {
                    vector<pair<int, int>> adj = {{nx-1, ny}, {nx+1, ny}, {nx, ny-1}, {nx, ny+1}};
                    for (auto& adj_pos : adj) {
                        for (int j = 0; j < N; j++) {
                            if (px[j] == adj_pos.first && py[j] == adj_pos.second) {
                                occupied = true;
                                break;
                            }
                        }
                        if (occupied) break;
                    }
                }
                
                if (!occupied) {
                    actions[i] = wall_chars[d];
                    blocked[nx][ny] = true;
                    placed_wall = true;
                    break;
                }
            }
        }
        
        cout << actions << endl;
        cout.flush();
        
        // Read pet movements
        for (int i = 0; i < N; i++) {
            string movement;
            cin >> movement;
            
            // Update pet positions based on movement
            for (char c : movement) {
                if (c == 'U') px[i]--;
                else if (c == 'D') px[i]++;
                else if (c == 'L') py[i]--;
                else if (c == 'R') py[i]++;
            }
        }
    }
    
    return 0;
}