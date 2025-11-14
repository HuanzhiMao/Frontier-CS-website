#include <bits/stdc++.h>

using namespace std;

int main() {
    int N;
    cin >> N;
    vector<pair<int, int>> pet_pos(N);
    vector<int> pet_type(N);
    for (int i = 0; i < N; i++) {
        int x, y, t;
        cin >> x >> y >> t;
        pet_pos[i] = {x, y};
        pet_type[i] = t;
    }
    int M;
    cin >> M;
    vector<pair<int, int>> hum_pos(M);
    for (int i = 0; i < M; i++) {
        int x, y;
        cin >> x >> y;
        hum_pos[i] = {x, y};
    }
    vector<vector<bool>> passable(31, vector<bool>(31, false));
    for (int i = 1; i <= 30; i++) for (int j = 1; j <= 30; j++) passable[i][j] = true;
    string block_chars = "udlr";
    string move_chars = "UDLR";
    int dx[4] = {-1, 1, 0, 0};
    int dy[4] = {0, 0, -1, 1};
    for (int turn = 0; turn < 300; turn++) {
        vector<vector<bool>> has_pet(31, vector<bool>(31, false));
        vector<vector<bool>> has_human(31, vector<bool>(31, false));
        for (int i = 0; i < N; i++) {
            auto [x, y] = pet_pos[i];
            has_pet[x][y] = true;
        }
        for (int i = 0; i < M; i++) {
            auto [x, y] = hum_pos[i];
            has_human[x][y] = true;
        }
        string actions(M, '.');
        set<pair<int, int>> to_block;
        for (int i = 0; i < M; i++) {
            auto [x, y] = hum_pos[i];
            bool found_block = false;
            for (int d = 0; d < 4; d++) {
                int sx = x + dx[d], sy = y + dy[d];
                if (sx < 1 || sx > 30 || sy < 1 || sy > 30) continue;
                if (has_pet[sx][sy] || has_human[sx][sy]) continue;
                bool bad = false;
                for (int dd = 0; dd < 4; dd++) {
                    int ax = sx + dx[dd], ay = sy + dy[dd];
                    if (ax >= 1 && ax <= 30 && ay >= 1 && ay <= 30 && has_pet[ax][ay]) bad = true;
                    if (bad) break;
                }
                if (!bad) {
                    actions[i] = block_chars[d];
                    to_block.insert({sx, sy});
                    found_block = true;
                    break;
                }
            }
        }
        for (int i = 0; i < M; i++) {
            if (actions[i] != '.') continue;
            auto [x, y] = hum_pos[i];
            for (int d = 0; d < 4; d++) {
                int tx = x + dx[d], ty = y + dy[d];
                if (tx < 1 || tx > 30 || ty < 1 || ty > 30) continue;
                if (!passable[tx][ty]) continue;
                if (to_block.count({tx, ty})) continue;
                actions[i] = move_chars[d];
                break;
            }
        }
        cout << actions << endl;
        cout.flush();
        for (auto p : to_block) {
            passable[p.first][p.second] = false;
        }
        for (int i = 0; i < M; i++) {
            char act = actions[i];
            if (act == '.' || islower(act)) continue;
            int d = -1;
            if (act == 'U') d = 0;
            else if (act == 'D') d = 1;
            else if (act == 'L') d = 2;
            else if (act == 'R') d = 3;
            int tx = hum_pos[i].first + dx[d];
            int ty = hum_pos[i].second + dy[d];
            hum_pos[i] = {tx, ty};
        }
        for (int i = 0; i < N; i++) {
            string s;
            cin >> s;
            if (s == ".") continue;
            for (char c : s) {
                int d = -1;
                if (c == 'U') d = 0;
                else if (c == 'D') d = 1;
                else if (c == 'L') d = 2;
                else if (c == 'R') d = 3;
                pet_pos[i].first += dx[d];
                pet_pos[i].second += dy[d];
            }
        }
    }
    return 0;
}