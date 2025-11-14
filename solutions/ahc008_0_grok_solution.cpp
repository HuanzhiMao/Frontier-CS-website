#include <bits/stdc++.h>
using namespace std;

int main() {
    int N;
    cin >> N;
    vector<int> px(N), py(N), pt(N);
    for (int i = 0; i < N; i++) {
        cin >> px[i] >> py[i] >> pt[i];
    }
    int M;
    cin >> M;
    vector<int> hx(M), hy(M);
    for (int i = 0; i < M; i++) {
        cin >> hx[i] >> hy[i];
    }
    bool wall[31][31] = {};
    int dx[4] = {-1, 1, 0, 0};
    int dy[4] = {0, 0, -1, 1};
    char wch[4] = {'u', 'd', 'l', 'r'};
    char mch[4] = {'U', 'D', 'L', 'R'};
    for (int turn = 0; turn < 300; turn++) {
        vector<char> acts(M);
        set<pair<int, int>> planned_walls;
        for (int i = 0; i < M; i++) {
            bool placed = false;
            for (int dd = 0; dd < 4; dd++) {
                int d = (i % 4 + dd) % 4;
                int tx = hx[i] + dx[d];
                int ty = hy[i] + dy[d];
                bool can = (tx >= 1 && tx <= 30 && ty >= 1 && ty <= 30 && !wall[tx][ty]);
                if (can) {
                    bool has = false;
                    for (int j = 0; j < N; j++) if (px[j] == tx && py[j] == ty) has = true;
                    for (int j = 0; j < M; j++) if (hx[j] == tx && hy[j] == ty) has = true;
                    if (!has) {
                        bool adj = false;
                        for (int dd2 = 0; dd2 < 4; dd2++) {
                            int ntx = tx + dx[dd2];
                            int nty = ty + dy[dd2];
                            if (ntx >= 1 && ntx <= 30 && nty >= 1 && nty <= 30) {
                                for (int j = 0; j < N; j++) {
                                    if (px[j] == ntx && py[j] == nty) {
                                        adj = true;
                                        break;
                                    }
                                }
                            }
                            if (adj) break;
                        }
                        if (!adj) {
                            acts[i] = wch[d];
                            planned_walls.insert({tx, ty});
                            placed = true;
                            break;
                        }
                    }
                }
            }
            if (!placed) {
                bool moved = false;
                for (int dd = 0; dd < 4; dd++) {
                    int d = (i % 4 + dd) % 4;
                    int tx = hx[i] + dx[d];
                    int ty = hy[i] + dy[d];
                    bool can = (tx >= 1 && tx <= 30 && ty >= 1 && ty <= 30 && !wall[tx][ty]);
                    if (can) {
                        acts[i] = mch[d];
                        moved = true;
                        break;
                    }
                }
                if (!moved) {
                    acts[i] = '.';
                }
            }
        }
        for (int i = 0; i < M; i++) {
            char act = acts[i];
            if (act == 'U' || act == 'D' || act == 'L' || act == 'R') {
                int d;
                if (act == 'U') d = 0;
                else if (act == 'D') d = 1;
                else if (act == 'L') d = 2;
                else d = 3;
                int tx = hx[i] + dx[d];
                int ty = hy[i] + dy[d];
                if (planned_walls.count({tx, ty})) {
                    acts[i] = '.';
                }
            }
        }
        string actions_str;
        for (char c : acts) actions_str += c;
        cout << actions_str << endl;
        cout.flush();
        for (int i = 0; i < M; i++) {
            char act = acts[i];
            if (act == 'u' || act == 'd' || act == 'l' || act == 'r') {
                int d;
                if (act == 'u') d = 0;
                else if (act == 'd') d = 1;
                else if (act == 'l') d = 2;
                else d = 3;
                int tx = hx[i] + dx[d];
                int ty = hy[i] + dy[d];
                if (tx >= 1 && tx <= 30 && ty >= 1 && ty <= 30 && !wall[tx][ty]) {
                    wall[tx][ty] = true;
                }
            }
        }
        for (int i = 0; i < M; i++) {
            char act = acts[i];
            if (act == 'U') hx[i] += dx[0];
            else if (act == 'D') hx[i] += dx[1];
            else if (act == 'L') hy[i] += dy[2];
            else if (act == 'R') hy[i] += dy[3];
        }
        for (int i = 0; i < N; i++) {
            string s;
            cin >> s;
            int x = px[i], y = py[i];
            if (s != ".") {
                for (char c : s) {
                    if (c == 'U') x += dx[0];
                    else if (c == 'D') x += dx[1];
                    else if (c == 'L') y += dy[2];
                    else if (c == 'R') y += dy[3];
                }
            }
            px[i] = x;
            py[i] = y;
        }
    }
    return 0;
}