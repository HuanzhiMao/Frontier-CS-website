#include <bits/stdc++.h>
using namespace std;

int di[] = {0, -1, 0, 1};
int dj[] = {-1, 0, 1, 0};

int to[8][4] = {
    {1, 0, -1, -1},
    {3, -1, -1, 0},
    {-1, -1, 3, 2},
    {-1, 2, 1, -1},
    {1, 0, 3, 2},
    {3, 2, 1, 0},
    {2, -1, 0, -1},
    {-1, 3, -1, 1}
};

int tiles[30][30];
int rotations[30][30];
bool visited[30][30][4];

int calculateLoopLength(int si, int sj, int sd) {
    int i = si, j = sj, d = sd;
    int length = 0;
    
    while (true) {
        int currentTile = (tiles[i][j] + rotations[i][j]) % 8;
        int d2 = to[currentTile][d];
        
        if (d2 == -1) return 0;
        
        i += di[d2];
        j += dj[d2];
        
        if (i < 0 || i >= 30 || j < 0 || j >= 30) return 0;
        
        d = (d2 + 2) % 4;
        length++;
        
        if (i == si && j == sj && d == sd) return length;
        if (length > 2000) return 0; // Prevent infinite loops
    }
}

vector<int> findAllLoopLengths() {
    memset(visited, false, sizeof(visited));
    vector<int> loops;
    
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 30; j++) {
            for (int d = 0; d < 4; d++) {
                if (!visited[i][j][d]) {
                    int currentTile = (tiles[i][j] + rotations[i][j]) % 8;
                    if (to[currentTile][d] != -1) {
                        int length = calculateLoopLength(i, j, d);
                        if (length > 0) {
                            loops.push_back(length);
                            
                            // Mark all positions in this loop as visited
                            int vi = i, vj = j, vd = d;
                            for (int step = 0; step < length; step++) {
                                visited[vi][vj][vd] = true;
                                int currentTileV = (tiles[vi][vj] + rotations[vi][vj]) % 8;
                                int d2 = to[currentTileV][vd];
                                vi += di[d2];
                                vj += dj[d2];
                                vd = (d2 + 2) % 4;
                            }
                        }
                    }
                }
            }
        }
    }
    
    return loops;
}

long long calculateScore() {
    vector<int> loops = findAllLoopLengths();
    if (loops.size() < 2) return 0;
    
    sort(loops.rbegin(), loops.rend());
    return (long long)loops[0] * loops[1];
}

int main() {
    for (int i = 0; i < 30; i++) {
        string row;
        cin >> row;
        for (int j = 0; j < 30; j++) {
            tiles[i][j] = row[j] - '0';
        }
    }
    
    memset(rotations, 0, sizeof(rotations));
    
    long long bestScore = calculateScore();
    
    // Simple hill climbing with random restarts
    for (int restart = 0; restart < 100; restart++) {
        // Random initialization
        for (int i = 0; i < 30; i++) {
            for (int j = 0; j < 30; j++) {
                rotations[i][j] = rand() % 4;
            }
        }
        
        long long currentScore = calculateScore();
        if (currentScore > bestScore) {
            bestScore = currentScore;
        }
        
        // Hill climbing
        bool improved = true;
        while (improved) {
            improved = false;
            for (int i = 0; i < 30; i++) {
                for (int j = 0; j < 30; j++) {
                    int originalRotation = rotations[i][j];
                    
                    for (int newRotation = 0; newRotation < 4; newRotation++) {
                        if (newRotation != originalRotation) {
                            rotations[i][j] = newRotation;
                            long long newScore = calculateScore();
                            
                            if (newScore > currentScore) {
                                currentScore = newScore;
                                improved = true;
                                if (currentScore > bestScore) {
                                    bestScore = currentScore;
                                }
                                break;
                            }
                        }
                    }
                    
                    if (!improved) {
                        rotations[i][j] = originalRotation;
                    }
                }
                if (improved) break;
            }
        }
    }
    
    // Output best solution found so far
    string result = "";
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 30; j++) {
            result += to_string(rotations[i][j]);
        }
    }
    cout << result << endl;
    
    return 0;
}