#include <bits/stdc++.h>
using namespace std;

int main() {
    int si, sj;
    cin >> si >> sj;
    
    vector<vector<int>> tiles(50, vector<int>(50));
    vector<vector<int>> points(50, vector<int>(50));
    
    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 50; j++) {
            cin >> tiles[i][j];
        }
    }
    
    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 50; j++) {
            cin >> points[i][j];
        }
    }
    
    set<int> visited_tiles;
    visited_tiles.insert(tiles[si][sj]);
    
    int ci = si, cj = sj;
    int total_score = points[si][sj];
    string path = "";
    
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    string moves = "UDLR";
    
    while (true) {
        int best_score = -1;
        int best_move = -1;
        
        for (int d = 0; d < 4; d++) {
            int ni = ci + dx[d];
            int nj = cj + dy[d];
            
            if (ni >= 0 && ni < 50 && nj >= 0 && nj < 50) {
                int tile_id = tiles[ni][nj];
                if (visited_tiles.find(tile_id) == visited_tiles.end()) {
                    if (points[ni][nj] > best_score) {
                        best_score = points[ni][nj];
                        best_move = d;
                    }
                }
            }
        }
        
        if (best_move == -1) break;
        
        ci += dx[best_move];
        cj += dy[best_move];
        visited_tiles.insert(tiles[ci][cj]);
        total_score += points[ci][cj];
        path += moves[best_move];
    }
    
    cout << path << endl;
    
    return 0;
}