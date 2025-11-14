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

int getLoopLength(int si, int sj, int sd) {
    int i = si, j = sj, d = sd;
    int length = 0;
    
    while (true) {
        int t = (tiles[i][j] + rotations[i][j]) % 8;
        if (t >= 4) t = 4 + (t + 4) % 2;
        
        int d2 = to[t][d];
        if (d2 == -1) return 0;
        
        i += di[d2];
        j += dj[d2];
        if (i < 0 || i >= 30 || j < 0 || j >= 30) return 0;
        
        d = (d2 + 2) % 4;
        length++;
        
        if (i == si && j == sj && d == sd) return length;
        if (length > 3600) return 0;
    }
}

vector<int> findAllLoops() {
    fill(visited[0][0], visited[0][0] + 30 * 30 * 4, false);
    vector<int> loops;
    
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 30; j++) {
            for (int d = 0; d < 4; d++) {
                if (!visited[i][j][d]) {
                    int len = getLoopLength(i, j, d);
                    if (len > 0) {
                        loops.push_back(len);
                        // Mark all positions in this loop as visited
                        int ci = i, cj = j, cd = d;
                        for (int step = 0; step < len; step++) {
                            visited[ci][cj][cd] = true;
                            int t = (tiles[ci][cj] + rotations[ci][cj]) % 8;
                            if (t >= 4) t = 4 + (t + 4) % 2;
                            int d2 = to[t][cd];
                            ci += di[d2];
                            cj += dj[d2];
                            cd = (d2 + 2) % 4;
                        }
                    }
                }
            }
        }
    }
    
    sort(loops.rbegin(), loops.rend());
    return loops;
}

long long getScore() {
    vector<int> loops = findAllLoops();
    if (loops.size() < 2) return 0;
    return (long long)loops[0] * loops[1];
}

int main() {
    for (int i = 0; i < 30; i++) {
        string s;
        cin >> s;
        for (int j = 0; j < 30; j++) {
            tiles[i][j] = s[j] - '0';
            rotations[i][j] = 0;
        }
    }
    
    long long bestScore = getScore();
    
    // Simple hill climbing
    for (int iter = 0; iter < 100000; iter++) {
        int i = rand() % 30;
        int j = rand() % 30;
        int oldRot = rotations[i][j];
        
        rotations[i][j] = rand() % 4;
        
        long long newScore = getScore();
        if (newScore <= bestScore) {
            rotations[i][j] = oldRot;
        } else {
            bestScore = newScore;
        }
    }
    
    string result = "";
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 30; j++) {
            result += char('0' + rotations[i][j]);
        }
    }
    
    cout << result << endl;
    return 0;
}