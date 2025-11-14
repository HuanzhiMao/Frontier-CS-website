#include <bits/stdc++.h>
using namespace std;

void simulate_tilt(char d, vector<vector<int>>& g) {
    vector<vector<int>> newg(10, vector<int>(10, 0));
    if (d == 'R') {
        for (int r = 0; r < 10; r++) {
            vector<int> cans;
            for (int c = 0; c < 10; c++) if (g[r][c] != 0) cans.push_back(g[r][c]);
            int k = cans.size();
            for (int i = 0; i < k; i++) {
                newg[r][9 - k + 1 + i] = cans[i];
            }
        }
    } else if (d == 'L') {
        for (int r = 0; r < 10; r++) {
            vector<int> cans;
            for (int c = 0; c < 10; c++) if (g[r][c] != 0) cans.push_back(g[r][c]);
            int k = cans.size();
            for (int i = 0; i < k; i++) {
                newg[r][i] = cans[i];
            }
        }
    } else if (d == 'B') {
        for (int c = 0; c < 10; c++) {
            vector<int> cans;
            for (int r = 0; r < 10; r++) if (g[r][c] != 0) cans.push_back(g[r][c]);
            int k = cans.size();
            for (int i = 0; i < k; i++) {
                newg[9 - k + 1 + i][c] = cans[i];
            }
        }
    } else if (d == 'F') {
        for (int c = 0; c < 10; c++) {
            vector<int> cans;
            for (int r = 0; r < 10; r++) if (g[r][c] != 0) cans.push_back(g[r][c]);
            int k = cans.size();
            for (int i = 0; i < k; i++) {
                newg[i][c] = cans[i];
            }
        }
    }
    g = newg;
}

long long compute_sum_sq(const vector<vector<int>>& g) {
    vector<vector<bool>> visited(10, vector<bool>(10, false));
    long long sum_sq = 0;
    for (int r = 0; r < 10; r++) {
        for (int c = 0; c < 10; c++) {
            if (g[r][c] > 0 && !visited[r][c]) {
                int flav = g[r][c];
                int size = 0;
                stack<pair<int, int>> stk;
                stk.push({r, c});
                visited[r][c] = true;
                while (!stk.empty()) {
                    auto [rr, cc] = stk.top(); stk.pop();
                    size++;
                    vector<pair<int, int>> dirs = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
                    for (auto [dr, dc] : dirs) {
                        int nr = rr + dr, nc = cc + dc;
                        if (nr >= 0 && nr < 10 && nc >= 0 && nc < 10 && !visited[nr][nc] && g[nr][nc] == flav) {
                            visited[nr][nc] = true;
                            stk.push({nr, nc});
                        }
                    }
                }
                sum_sq += (long long)size * size;
            }
        }
    }
    return sum_sq;
}

int main() {
    vector<int> f(100);
    for (int i = 0; i < 100; i++) cin >> f[i];
    vector<vector<int>> grid(10, vector<int>(10, 0));
    for (int t = 0; t < 100; t++) {
        int p; cin >> p;
        int count = 0;
        int pr, pc;
        for (int r = 0; r < 10; r++) {
            for (int c = 0; c < 10; c++) {
                if (grid[r][c] == 0) {
                    count++;
                    if (count == p) {
                        pr = r; pc = c;
                        goto placed;
                    }
                }
            }
        }
    placed:
        grid[pr][pc] = f[t];
        long long best = -1;
        char bestd = ' ';
        for (char dd : string("FBLR")) {
            auto temp = grid;
            simulate_tilt(dd, temp);
            long long sc = compute_sum_sq(temp);
            if (sc > best) {
                best = sc;
                bestd = dd;
            }
        }
        cout << bestd << endl;
        simulate_tilt(bestd, grid);
    }
    return 0;
}