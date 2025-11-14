#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    int N;
    cin >> N;
    vector<int> px(N+1), py(N+1), pt(N+1);
    for(int i=1; i<=N; i++) {
        cin >> px[i] >> py[i] >> pt[i];
    }
    int M;
    cin >> M;
    vector<int> hx(M+1), hy(M+1);
    vector<pair<int,int>> targets(M+1);
    for(int i=1; i<=M; i++) {
        cin >> hx[i] >> hy[i];
        targets[i] = {15, min(30, max(1, i * 3)) };
    }
    bool impass[31][31];
    memset(impass, 0, sizeof(impass));
    map<char, pair<int,int>> ddir = {
        {'U', {-1,0}}, {'D', {1,0}}, {'L', {0,-1}}, {'R', {0,1}},
        {'u', {-1,0}}, {'d', {1,0}}, {'l', {0,-1}}, {'r', {0,1}}
    };
    auto valid_pos = [](int x, int y) -> bool {
        return 1 <= x && x <= 30 && 1 <= y && y <= 30;
    };
    for(int turn = 0; turn < 300; turn++) {
        set<pair<int,int>> pet_pos, human_pos;
        for(int i=1; i<=N; i++) pet_pos.insert({px[i], py[i]});
        for(int i=1; i<=M; i++) human_pos.insert({hx[i], hy[i]});
        vector<char> act(M+1, '.');
        for(int p=1; p<=M; p++) {
            int cx = hx[p], cy = hy[p];
            int tx = targets[p].first, ty = targets[p].second;
            if(cx == tx && cy == ty) {
                vector<char> try_place = {'d', 'r', 'u', 'l'};
                for(char c : try_place) {
                    auto [dx, dy] = ddir[c];
                    int nx = cx + dx, ny = cy + dy;
                    if(!valid_pos(nx, ny)) continue;
                    if(pet_pos.count({nx, ny}) || human_pos.count({nx, ny})) continue;
                    bool adj_pet = false;
                    for(auto [adx, ady] : vector<pair<int,int>>{{-1,0},{1,0},{0,-1},{0,1}}) {
                        int ax = nx + adx, ay = ny + ady;
                        if(valid_pos(ax, ay) && pet_pos.count({ax, ay})) {
                            adj_pet = true;
                            break;
                        }
                    }
                    if(!adj_pet) {
                        act[p] = c;
                        break;
                    }
                }
            } else {
                int dx = tx - cx, dy = ty - cy;
                char move_c = '.';
                char pref = 0, sec = 0;
                if(abs(dx) > abs(dy)) {
                    if(dx != 0) pref = dx > 0 ? 'D' : 'U';
                    if(dy != 0) sec = dy > 0 ? 'R' : 'L';
                } else {
                    if(dy != 0) pref = dy > 0 ? 'R' : 'L';
                    if(dx != 0) sec = dx > 0 ? 'D' : 'U';
                }
                if(pref) {
                    auto [ddx, ddy] = ddir[pref];
                    int nx = cx + ddx, ny = cy + ddy;
                    if(valid_pos(nx, ny) && !impass[nx][ny]) {
                        move_c = pref;
                    }
                }
                if(move_c == '.' && sec) {
                    auto [ddx, ddy] = ddir[sec];
                    int nx = cx + ddx, ny = cy + ddy;
                    if(valid_pos(nx, ny) && !impass[nx][ny]) {
                        move_c = sec;
                    }
                }
                act[p] = move_c;
            }
        }
        set<pair<int,int>> place_targets;
        for(int p=1; p<=M; p++) {
            if(islower(act[p])) {
                char c = act[p];
                auto [dx, dy] = ddir[c];
                int nx = hx[p] + dx, ny = hy[p] + dy;
                place_targets.insert({nx, ny});
            }
        }
        for(int p=1; p<=M; p++) {
            if(isupper(act[p])) {
                char c = act[p];
                auto [dx, dy] = ddir[c];
                int nx = hx[p] + dx, ny = hy[p] + dy;
                if(place_targets.count({nx, ny})) {
                    act[p] = '.';
                }
            }
        }
        string actions = "";
        for(int p=1; p<=M; p++) {
            actions += act[p];
        }
        cout << actions << endl;
        cout.flush();
        for(auto pr : place_targets) {
            impass[pr.first][pr.second] = true;
        }
        for(int p=1; p<=M; p++) {
            if(isupper(act[p])) {
                char c = act[p];
                auto [dx, dy] = ddir[c];
                hx[p] += dx;
                hy[p] += dy;
            }
        }
        for(int i=1; i<=N; i++) {
            string s;
            cin >> s;
            if(s == ".") continue;
            int x = px[i], y = py[i];
            for(char c : s) {
                auto [dx, dy] = ddir[c];
                x += dx;
                y += dy;
            }
            px[i] = x;
            py[i] = y;
        }
    }
    return 0;
}