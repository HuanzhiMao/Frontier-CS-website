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
bool visited[30][30][4];

int getLoopLength(int si, int sj, int sd) {
    int i = si, j = sj, d = sd;
    int length = 0;
    
    while (true) {
        int current_tile = (tiles[i][j] + rotation[i][j]) % 8;
        if (current_tile < 4) current_tile = (current_tile + rotation[i][j]) % 4;
        else if (current_tile < 6) current_tile = 4 + (rotation[i][j] % 2);
        else current_tile = 6 + (rotation[i][j] % 2);
        
        int d2 = to[current_tile][d];
        if (d2 == -1) return 0;
        
        int ni = i + di[d2];
        int nj = j + dj[d2];
        if (ni < 0 || ni >= 30 || nj < 0 || nj >= 30) return 0;
        
        i = ni;
        j = nj;
        d = (d2 + 2) % 4;
        length++;
        
        if (i == si && j == sj && d == sd) return length;
    }
}

vector<int> findAllLoops() {
    vector<int> loops;
    fill(visited[0][0], visited[0][0] + 30*30*4, false);
    
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 30; j++) {
            for (int d = 0; d < 4; d++) {
                if (visited[i][j][d]) continue;
                
                int current_tile = (tiles[i][j] + rotation[i][j]) % 8;
                if (current_tile < 4) current_tile = (current_tile + rotation[i][j]) % 4;
                else if (current_tile < 6) current_tile = 4 + (rotation[i][j] % 2);
                else current_tile = 6 + (rotation[i][j] % 2);
                
                if (to[current_tile][d] == -1) continue;
                
                int len = getLoopLength(i, j, d);
                if (len > 0) {
                    loops.push_back(len);
                    
                    int ti = i, tj = j, td = d;
                    for (int step = 0; step < len; step++) {
                        visited[ti][tj][td] = true;
                        
                        int curr_tile = (tiles[ti][tj] + rotation[ti][tj]) % 8;
                        if (curr_tile < 4) curr_tile = (curr_tile + rotation[ti][tj]) % 4;
                        else if (curr_tile < 6) curr_tile = 4 + (rotation[ti][tj] % 2);
                        else curr_tile = 6 + (rotation[ti][tj] % 2);
                        
                        int d2 = to[curr_tile][td];
                        ti += di[d2];
                        tj += dj[d2];
                        td = (d2 + 2) % 4;
                    }
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
    
    long long best_score = 0;
    string best_answer;
    
    for (int iter = 0; iter < 10000; iter++) {
        vector<int> loops = findAllLoops();
        
        long long score = 0;
        if (loops.size() >= 2) {
            score = (long long)loops[0] * loops[1];
        }
        
        if (score > best_score) {
            best_score = score;
            best_answer = "";
            for (int i = 0; i < 30; i++) {
                for (int j = 0; j < 30; j++) {
                    best_answer += to_string(rotation[i][j]);
                }
            }
        }
        
        int ri = rand() % 30;
        int rj = rand() % 30;
        int old_rot = rotation[ri][rj];
        rotation[ri][rj] = rand() % 4;
        
        vector<int> new_loops = findAllLoops();
        long long new_score = 0;
        if (new_loops.size() >= 2) {
            new_score = (long long)new_loops[0] * new_loops[1];
        }
        
        if (new_score <= score) {
            rotation[ri][rj] = old_rot;
        }
    }
    
    cout << best_answer << endl;
    return 0;
}