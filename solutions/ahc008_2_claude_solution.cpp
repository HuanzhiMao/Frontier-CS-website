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
char dirChar[] = {'U', 'D', 'L', 'R'};
char buildChar[] = {'u', 'd', 'l', 'r'};

bool isValid(int x, int y) {
    return x >= 1 && x <= 30 && y >= 1 && y <= 30;
}

int main() {
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
        string actions = "";
        
        for (int i = 0; i < M; i++) {
            bool found = false;
            
            // Try to build walls around pets
            for (int p = 0; p < N; p++) {
                int px = pets[p].x, py = pets[p].y;
                
                // Check adjacent squares to pet
                for (int d = 0; d < 4; d++) {
                    int nx = px + dx[d];
                    int ny = py + dy[d];
                    
                    if (!isValid(nx, ny) || blocked[nx][ny]) continue;
                    
                    // Check if human can build here
                    int hx = humans[i].x, hy = humans[i].y;
                    for (int hd = 0; hd < 4; hd++) {
                        int bx = hx + dx[hd];
                        int by = hy + dy[hd];
                        
                        if (bx == nx && by == ny) {
                            // Check if this square is adjacent to any pet
                            bool adjacentToPet = false;
                            for (int pp = 0; pp < N; pp++) {
                                if (abs(pets[pp].x - bx) + abs(pets[pp].y - by) == 1) {
                                    adjacentToPet = true;
                                    break;
                                }
                            }
                            
                            if (!adjacentToPet) {
                                // Check if any human is on this square
                                bool humanOnSquare = false;
                                for (int h = 0; h < M; h++) {
                                    if (humans[h].x == bx && humans[h].y == by) {
                                        humanOnSquare = true;
                                        break;
                                    }
                                }
                                
                                if (!humanOnSquare) {
                                    actions += buildChar[hd];
                                    blocked[bx][by] = true;
                                    found = true;
                                    break;
                                }
                            }
                        }
                    }
                    if (found) break;
                }
                if (found) break;
            }
            
            if (!found) {
                // Try to move away from pets
                int bestDir = -1;
                int maxDist = -1;
                
                for (int d = 0; d < 4; d++) {
                    int nx = humans[i].x + dx[d];
                    int ny = humans[i].y + dy[d];
                    
                    if (!isValid(nx, ny) || blocked[nx][ny]) continue;
                    
                    int minDistToPet = 1000;
                    for (int p = 0; p < N; p++) {
                        int dist = abs(pets[p].x - nx) + abs(pets[p].y - ny);
                        minDistToPet = min(minDistToPet, dist);
                    }
                    
                    if (minDistToPet > maxDist) {
                        maxDist = minDistToPet;
                        bestDir = d;
                    }
                }
                
                if (bestDir != -1) {
                    actions += dirChar[bestDir];
                    humans[i].x += dx[bestDir];
                    humans[i].y += dy[bestDir];
                } else {
                    actions += '.';
                }
            }
        }
        
        cout << actions << endl;
        cout.flush();
        
        // Read pet movements
        for (int i = 0; i < N; i++) {
            string moves;
            cin >> moves;
            
            for (char c : moves) {
                if (c == 'U') pets[i].x--;
                else if (c == 'D') pets[i].x++;
                else if (c == 'L') pets[i].y--;
                else if (c == 'R') pets[i].y++;
            }
        }
    }
    
    return 0;
}