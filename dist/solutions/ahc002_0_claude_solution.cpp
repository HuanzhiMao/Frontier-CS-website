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
    
    set<int> used_tiles;
    used_tiles.insert(t[si][sj]);
    
    string result = "";
    int ci = si, cj = sj;
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    char moves[] = {'U', 'D', 'L', 'R'};
    
    while (true) {
        int best_score = -1;
        int best_dir = -1;
        
        for (int dir = 0; dir < 4; dir++) {
            int ni = ci + dx[dir];
            int nj = cj + dy[dir];
            
            if (ni >= 0 && ni < 50 && nj >= 0 && nj < 50) {
                int tile_id = t[ni][nj];
                if (used_tiles.find(tile_id) == used_tiles.end()) {
                    if (p[ni][nj] > best_score) {
                        best_score = p[ni][nj];
                        best_dir = dir;
                    }
                }
            }
        }
        
        if (best_dir == -1) break;
        
        ci += dx[best_dir];
        cj += dy[best_dir];
        used_tiles.insert(t[ci][cj]);
        result += moves[best_dir];
    }
    
    cout << result << endl;
    return 0;
}