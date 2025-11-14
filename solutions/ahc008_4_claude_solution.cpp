#include <bits/stdc++.h>
using namespace std;

struct Pet {
    int x, y, type;
};

struct Human {
    int x, y;
};

int N, M;
vector<Pet> pets;
vector<Human> humans;
bool passable[31][31];
int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};

bool isValid(int x, int y) {
    return x >= 1 && x <= 30 && y >= 1 && y <= 30;
}

bool canBuild(int x, int y) {
    if (!isValid(x, y)) return false;
    if (!passable[x][y]) return false;
    
    // Check if any human or pet is at this position
    for (int i = 0; i < N; i++) {
        if (pets[i].x == x && pets[i].y == y) return false;
    }
    for (int i = 0; i < M; i++) {
        if (humans[i].x == x && humans[i].y == y) return false;
    }
    
    // Check if any adjacent square contains a pet
    for (int d = 0; d < 4; d++) {
        int nx = x + dx[d];
        int ny = y + dy[d];
        if (isValid(nx, ny)) {
            for (int i = 0; i < N; i++) {
                if (pets[i].x == nx && pets[i].y == ny) return false;
            }
        }
    }
    
    return true;
}

bool canMove(int x, int y) {
    return isValid(x, y) && passable[x][y];
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    // Initialize passable grid
    for (int i = 1; i <= 30; i++) {
        for (int j = 1; j <= 30; j++) {
            passable[i][j] = true;
        }
    }
    
    cin >> N;
    pets.resize(N);
    for (int i = 0; i < N; i++) {
        cin >> pets[i].x >> pets[i].y >> pets[i].type;
    }
    
    cin >> M;
    humans.resize(M);
    for (int i = 0; i < M; i++) {
        cin >> humans[i].x >> humans[i].y;
    }
    
    for (int turn = 0; turn < 300; turn++) {
        string actions = "";
        
        for (int i = 0; i < M; i++) {
            int bestAction = 0; // 0: stay, 1-4: build, 5-8: move
            int bestScore = -1000000;
            
            // Try all possible actions
            for (int action = 0; action < 9; action++) {
                int score = 0;
                
                if (action == 0) {
                    // Stay
                    score = 0;
                } else if (action <= 4) {
                    // Build wall
                    int buildX = humans[i].x + dx[action-1];
                    int buildY = humans[i].y + dy[action-1];
                    
                    if (canBuild(buildX, buildY)) {
                        // Score based on how close pets are
                        for (int j = 0; j < N; j++) {
                            int dist = abs(pets[j].x - buildX) + abs(pets[j].y - buildY);
                            if (dist <= 3) score += 10;
                        }
                    } else {
                        score = -1000000;
                    }
                } else {
                    // Move
                    int moveX = humans[i].x + dx[action-5];
                    int moveY = humans[i].y + dy[action-5];
                    
                    if (canMove(moveX, moveY)) {
                        // Score based on distance from pets
                        for (int j = 0; j < N; j++) {
                            int dist = abs(pets[j].x - moveX) + abs(pets[j].y - moveY);
                            score += dist;
                        }
                    } else {
                        score = -1000000;
                    }
                }
                
                if (score > bestScore) {
                    bestScore = score;
                    bestAction = action;
                }
            }
            
            if (bestAction == 0) {
                actions += '.';
            } else if (bestAction <= 4) {
                char buildActions[] = {'u', 'd', 'l', 'r'};
                actions += buildActions[bestAction-1];
                
                int buildX = humans[i].x + dx[bestAction-1];
                int buildY = humans[i].y + dy[bestAction-1];
                if (canBuild(buildX, buildY)) {
                    passable[buildX][buildY] = false;
                }
            } else {
                char moveActions[] = {'U', 'D', 'L', 'R'};
                actions += moveActions[bestAction-5];
                
                int moveX = humans[i].x + dx[bestAction-5];
                int moveY = humans[i].y + dy[bestAction-5];
                if (canMove(moveX, moveY)) {
                    humans[i].x = moveX;
                    humans[i].y = moveY;
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