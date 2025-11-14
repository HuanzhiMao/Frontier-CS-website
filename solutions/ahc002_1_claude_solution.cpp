#include <bits/stdc++.h>
using namespace std;

int main() {
    int si, sj;
    cin >> si >> sj;
    
    vector<vector<int>> t(50, vector<int>(50));
    vector<vector<int>> p(50, vector<int>(50));
    
    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 50; j++) {
            cin >> t[i][j];
        }
    }
    
    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 50; j++) {
            cin >> p[i][j];
        }
    }
    
    set<int> visitedTiles;
    visitedTiles.insert(t[si][sj]);
    
    string path = "";
    int ci = si, cj = sj;
    int totalScore = p[si][sj];
    
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    string moves[] = {"U", "D", "L", "R"};
    
    while (true) {
        int bestScore = -1;
        int bestMove = -1;
        
        for (int dir = 0; dir < 4; dir++) {
            int ni = ci + dx[dir];
            int nj = cj + dy[dir];
            
            if (ni >= 0 && ni < 50 && nj >= 0 && nj < 50) {
                int tileId = t[ni][nj];
                if (visitedTiles.find(tileId) == visitedTiles.end()) {
                    if (p[ni][nj] > bestScore) {
                        bestScore = p[ni][nj];
                        bestMove = dir;
                    }
                }
            }
        }
        
        if (bestMove == -1) break;
        
        ci += dx[bestMove];
        cj += dy[bestMove];
        visitedTiles.insert(t[ci][cj]);
        totalScore += p[ci][cj];
        path += moves[bestMove];
    }
    
    cout << path << endl;
    
    return 0;
}