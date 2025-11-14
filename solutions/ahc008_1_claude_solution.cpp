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
char move_chars[] = {'U', 'D', 'L', 'R'};
char wall_chars[] = {'u', 'd', 'l', 'r'};

bool isValid(int x, int y) {
    return x >= 1 && x <= 30 && y >= 1 && y <= 30;
}

vector<vector<bool>> passable;
vector<Pet> pets;
vector<Human> humans;
int N, M;

bool canMakeWall(int x, int y, int turn) {
    if (!isValid(x, y)) return false;
    
    // Check if any pet is at this position
    for (auto& pet : pets) {
        if (pet.x == x && pet.y == y) return false;
    }
    
    // Check if any human is at this position
    for (auto& human : humans) {
        if (human.x == x && human.y == y) return false;
    }
    
    // Check if any pet is adjacent
    for (int d = 0; d < 4; d++) {
        int nx = x + dx[d];
        int ny = y + dy[d];
        for (auto& pet : pets) {
            if (pet.x == nx && pet.y == ny) return false;
        }
    }
    
    return true;
}

bool canMove(int x, int y) {
    if (!isValid(x, y)) return false;
    return passable[x][y];
}

int main() {
    passable.assign(31, vector<bool>(31, true));
    
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
        string actions(M, '.');
        
        // Simple strategy: build walls around starting positions early, then move to create separate areas
        for (int i = 0; i < M; i++) {
            bool acted = false;
            
            // Early turns: focus on building walls
            if (turn < 150) {
                // Try to build walls around current position
                for (int d = 0; d < 4 && !acted; d++) {
                    int wx = humans[i].x + dx[d];
                    int wy = humans[i].y + dy[d];
                    if (canMakeWall(wx, wy, turn)) {
                        actions[i] = wall_chars[d];
                        passable[wx][wy] = false;
                        acted = true;
                    }
                }
            }
            
            // If couldn't build wall or later turns, try to move away from pets
            if (!acted) {
                int bestMove = -1;
                int maxDist = -1;
                
                for (int d = 0; d < 4; d++) {
                    int nx = humans[i].x + dx[d];
                    int ny = humans[i].y + dy[d];
                    if (canMove(nx, ny)) {
                        int minPetDist = 1000;
                        for (auto& pet : pets) {
                            int dist = abs(nx - pet.x) + abs(ny - pet.y);
                            minPetDist = min(minPetDist, dist);
                        }
                        if (minPetDist > maxDist) {
                            maxDist = minPetDist;
                            bestMove = d;
                        }
                    }
                }
                
                if (bestMove != -1) {
                    actions[i] = move_chars[bestMove];
                    humans[i].x += dx[bestMove];
                    humans[i].y += dy[bestMove];
                }
            }
        }
        
        cout << actions << endl;
        cout.flush();
        
        // Read pet movements
        for (int i = 0; i < N; i++) {
            string petMove;
            cin >> petMove;
            
            if (petMove != ".") {
                for (char c : petMove) {
                    int d = -1;
                    if (c == 'U') d = 0;
                    else if (c == 'D') d = 1;
                    else if (c == 'L') d = 2;
                    else if (c == 'R') d = 3;
                    
                    if (d != -1) {
                        int nx = pets[i].x + dx[d];
                        int ny = pets[i].y + dy[d];
                        if (isValid(nx, ny) && passable[nx][ny]) {
                            pets[i].x = nx;
                            pets[i].y = ny;
                        }
                    }
                }
            }
        }
    }
    
    return 0;
}