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
int rotation[30][30];
vector<vector<vector<bool>>> visited;

int getLoopLength(int si, int sj, int sd) {
    int i = si, j = sj, d = sd;
    int length = 0;
    
    do {
        int current_tile = (tiles[i][j] + rotation[i][j]) % 8;
        if (tiles[i][j] >= 6) {
            current_tile = tiles[i][j] + (rotation[i][j] % 2) * (tiles[i][j] == 6 ? 1 : -1);
        }
        
        int d2 = to[current_tile][d];
        if (d2 == -1) return 0;
        
        i += di[d2];
        j += dj[d2];
        
        if (i < 0 || i >= 30 || j < 0 || j >= 30) return 0;
        
        d = (d2 + 2) % 4;
        length++;
        
    } while (!(i == si && j == sj && d == sd));
    
    return length;
}

vector<int> findAllLoops() {
    vector<int> loops;
    visited.assign(30, vector<vector<bool>>(30, vector<bool>(4, false)));
    
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 30; j++) {
            for (int d = 0; d < 4; d++) {
                if (visited[i][j][d]) continue;
                
                int current_tile = (tiles[i][j] + rotation[i][j]) % 8;
                if (tiles[i][j] >= 6) {
                    current_tile = tiles[i][j] + (rotation[i][j] % 2) * (tiles[i][j] == 6 ? 1 : -1);
                }
                
                if (to[current_tile][d] == -1) continue;
                
                int len = getLoopLength(i, j, d);
                if (len > 0) {
                    loops.push_back(len);
                    
                    int ti = i, tj = j, td = d;
                    do {
                        visited[ti][tj][td] = true;
                        
                        int tcurrent_tile = (tiles[ti][tj] + rotation[ti][tj]) % 8;
                        if (tiles[ti][tj] >= 6) {
                            tcurrent_tile = tiles[ti][tj] + (rotation[ti][tj] % 2) * (tiles[ti][tj] == 6 ? 1 : -1);
                        }
                        
                        int td2 = to[tcurrent_tile][td];
                        ti += di[td2];
                        tj += dj[td2];
                        td = (td2 + 2) % 4;
                    } while (!(ti == i && tj == j && td == d));
                }
            }
        }
    }
    
    sort(loops.rbegin(), loops.rend());
    return loops;
}

int main() {
    for (int i = 0; i < 30; i++) {
        string s;
        cin >> s;
        for (int j = 0; j < 30; j++) {
            tiles[i][j] = s[j] - '0';
            rotation[i][j] = 0;
        }
    }
    
    int best_score = 0;
    string best_rotation(900, '0');
    
    for (int iter = 0; iter < 10000; iter++) {
        int i = rand() % 30;
        int j = rand() % 30;
        int old_rot = rotation[i][j];
        
        rotation[i][j] = rand() % 4;
        
        vector<int> loops = findAllLoops();
        int score = 0;
        if (loops.size() >= 2) {
            score = loops[0] * loops[1];
        }
        
        if (score > best_score) {
            best_score = score;
            for (int x = 0; x < 30; x++) {
                for (int y = 0; y < 30; y++) {
                    best_rotation[x * 30 + y] = '0' + rotation[x][y];
                }
            }
        } else {
            rotation[i][j] = old_rot;
        }
    }
    
    cout << best_rotation << endl;
    
    return 0;
}