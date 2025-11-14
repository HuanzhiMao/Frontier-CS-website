#include <bits/stdc++.h>
using namespace std;

struct Pet {
    int x, y, type;
};

struct Human {
    int x, y;
};

int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};
char dirChar[] = {'u', 'd', 'l', 'r'};
char moveChar[] = {'U', 'D', 'L', 'R'};

bool isValid(int x, int y) {
    return x >= 1 && x <= 30 && y >= 1 && y <= 30;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    cin >> N;
    
    vector<Pet> pets(N);
    for (int i = 0; i < N; i++) {
        cin >> pets[i].x >> pets[i].y >> pets[i].type;
    }
    
    int M;
    cin >> M;
    
    vector<Human> humans(M);
    for (int i = 0; i < M; i++) {
        cin >> humans[i].x >> humans[i].y;
    }
    
    vector<vector<bool>> blocked(31, vector<bool>(31, false));
    
    for (int turn = 0; turn < 300; turn++) {
        string actions(M, '.');
        
        // Simple strategy: try to build walls around pets
        for (int h = 0; h < M; h++) {
            int hx = humans[h].x;
            int hy = humans[h].y;
            
            bool foundAction = false;
            
            // Try to block squares near pets
            for (int p = 0; p < N && !foundAction; p++) {
                int px = pets[p].x;
                int py = pets[p].y;
                
                // Check if we can block around this pet
                for (int d = 0; d < 4 && !foundAction; d++) {
                    int nx = px + dx[d];
                    int ny = py + dy[d];
                    
                    if (!isValid(nx, ny)) continue;
                    if (blocked[nx][ny]) continue;
                    
                    // Check if human is adjacent to this position
                    for (int d2 = 0; d2 < 4; d2++) {
                        int bx = hx + dx[d2];
                        int by = hy + dy[d2];
                        
                        if (bx == nx && by == ny) {
                            // Check if we can block this square
                            bool canBlock = true;
                            
                            // Can't block if pets or humans are there
                            for (int p2 = 0; p2 < N; p2++) {
                                if (pets[p2].x == nx && pets[p2].y == ny) {
                                    canBlock = false;
                                    break;
                                }
                            }
                            
                            for (int h2 = 0; h2 < M && canBlock; h2++) {
                                if (humans[h2].x == nx && humans[h2].y == ny) {
                                    canBlock = false;
                                    break;
                                }
                            }
                            
                            // Can't block if adjacent to pets
                            for (int dd = 0; dd < 4 && canBlock; dd++) {
                                int adjx = nx + dx[dd];
                                int adjy = ny + dy[dd];
                                
                                for (int p2 = 0; p2 < N; p2++) {
                                    if (pets[p2].x == adjx && pets[p2].y == adjy) {
                                        canBlock = false;
                                        break;
                                    }
                                }
                            }
                            
                            if (canBlock) {
                                actions[h] = dirChar[d2];
                                foundAction = true;
                                break;
                            }
                        }
                    }
                }
            }
            
            // If no blocking action found, try to move away from pets
            if (!foundAction) {
                int bestScore = -1000;
                int bestDir = -1;
                
                for (int d = 0; d < 4; d++) {
                    int nx = hx + dx[d];
                    int ny = hy + dy[d];
                    
                    if (!isValid(nx, ny) || blocked[nx][ny]) continue;
                    
                    int score = 0;
                    for (int p = 0; p < N; p++) {
                        int dist = abs(pets[p].x - nx) + abs(pets[p].y - ny);
                        score += dist;
                    }
                    
                    if (score > bestScore) {
                        bestScore = score;
                        bestDir = d;
                    }
                }
                
                if (bestDir != -1) {
                    actions[h] = moveChar[bestDir];
                    humans[h].x += dx[bestDir];
                    humans[h].y += dy[bestDir];
                }
            } else {
                // Update blocked array if we placed a wall
                for (int d = 0; d < 4; d++) {
                    if (actions[h] == dirChar[d]) {
                        int nx = hx + dx[d];
                        int ny = hy + dy[d];
                        if (isValid(nx, ny)) {
                            blocked[nx][ny] = true;
                        }
                        break;
                    }
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
                if (c == 'U') pets[i].x--;
                else if (c == 'D') pets[i].x++;
                else if (c == 'L') pets[i].y--;
                else if (c == 'R') pets[i].y++;
            }
        }
    }
    
    return 0;
}