#include <bits/stdc++.h>
using namespace std;

int N = 20, M = 40;
int grid[25][25];
int di[] = {-1, 1, 0, 0};
int dj[] = {0, 0, -1, 1};
char dirs[] = {'U', 'D', 'L', 'R'};

struct State {
    int i, j;
};

bool isValid(int i, int j) {
    return i >= 0 && i < N && j >= 0 && j < N;
}

State slide(State pos, int dir) {
    int ni = pos.i, nj = pos.j;
    while (true) {
        int nni = ni + di[dir];
        int nnj = nj + dj[dir];
        if (!isValid(nni, nnj) || grid[nni][nnj] == 1) {
            break;
        }
        ni = nni;
        nj = nnj;
    }
    return {ni, nj};
}

vector<string> solve() {
    vector<pair<int, int>> targets(M);
    cin >> N >> M;
    
    int si, sj;
    cin >> si >> sj;
    
    for (int i = 0; i < M; i++) {
        cin >> targets[i].first >> targets[i].second;
    }
    
    memset(grid, 0, sizeof(grid));
    
    vector<string> actions;
    State current = {si, sj};
    
    for (int targetIdx = 0; targetIdx < M; targetIdx++) {
        int ti = targets[targetIdx].first;
        int tj = targets[targetIdx].second;
        
        while (current.i != ti || current.j != tj) {
            if (actions.size() >= 2 * N * M) break;
            
            // Try to move directly first
            bool moved = false;
            for (int d = 0; d < 4; d++) {
                int ni = current.i + di[d];
                int nj = current.j + dj[d];
                
                if (isValid(ni, nj) && grid[ni][nj] == 0) {
                    if (ni == ti && nj == tj) {
                        // Direct move to target
                        actions.push_back(string("M ") + dirs[d]);
                        current = {ni, nj};
                        moved = true;
                        break;
                    }
                }
            }
            
            if (moved) continue;
            
            // Try sliding to target
            moved = false;
            for (int d = 0; d < 4; d++) {
                State newPos = slide(current, d);
                if (newPos.i == ti && newPos.j == tj && (newPos.i != current.i || newPos.j != current.j)) {
                    actions.push_back(string("S ") + dirs[d]);
                    current = newPos;
                    moved = true;
                    break;
                }
            }
            
            if (moved) continue;
            
            // Try to get closer by moving
            int bestDir = -1;
            int minDist = INT_MAX;
            
            for (int d = 0; d < 4; d++) {
                int ni = current.i + di[d];
                int nj = current.j + dj[d];
                
                if (isValid(ni, nj) && grid[ni][nj] == 0) {
                    int dist = abs(ni - ti) + abs(nj - tj);
                    if (dist < minDist) {
                        minDist = dist;
                        bestDir = d;
                    }
                }
            }
            
            if (bestDir != -1) {
                actions.push_back(string("M ") + dirs[bestDir]);
                current.i += di[bestDir];
                current.j += dj[bestDir];
                continue;
            }
            
            // Try to slide to get closer
            bestDir = -1;
            minDist = INT_MAX;
            
            for (int d = 0; d < 4; d++) {
                State newPos = slide(current, d);
                if (newPos.i != current.i || newPos.j != current.j) {
                    int dist = abs(newPos.i - ti) + abs(newPos.j - tj);
                    if (dist < minDist) {
                        minDist = dist;
                        bestDir = d;
                    }
                }
            }
            
            if (bestDir != -1) {
                actions.push_back(string("S ") + dirs[bestDir]);
                current = slide(current, bestDir);
                continue;
            }
            
            // Place a block to enable sliding
            for (int d = 0; d < 4; d++) {
                int ni = current.i + di[d];
                int nj = current.j + dj[d];
                
                if (isValid(ni, nj) && grid[ni][nj] == 0) {
                    actions.push_back(string("A ") + dirs[d]);
                    grid[ni][nj] = 1;
                    break;
                }
            }
        }
    }
    
    return actions;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    vector<string> result = solve();
    
    for (const string& action : result) {
        cout << action << "\n";
    }
    
    return 0;
}