#include <bits/stdc++.h>
using namespace std;

int main() {
    int N, M;
    cin >> N >> M;
    int c = (N - 1) / 2;
    vector<vector<long long>> ww(N, vector<long long>(N));
    for (int x = 0; x < N; x++) {
        for (int y = 0; y < N; y++) {
            ww[x][y] = 1LL * (x - c) * (x - c) + 1LL * (y - c) * (y - c) + 1;
        }
    }
    vector<vector<bool>> has_dot(N, vector<bool>(N, false));
    for (int i = 0; i < M; i++) {
        int x, y;
        cin >> x >> y;
        has_dot[x][y] = true;
    }
    vector<vector<pair<int, int>>> rect_verts;
    // Axis-aligned 1x1
    for (int i = 0; i < N - 1; i++) {
        for (int j = 0; j < N - 1; j++) {
            vector<pair<int, int>> v = {{i, j}, {i + 1, j}, {i + 1, j + 1}, {i, j + 1}};
            rect_verts.push_back(v);
        }
    }
    // 45-degree small
    for (int minu = 0; minu <= 2 * (N - 1) - 2; minu++) {
        int maxu = minu + 2;
        for (int minv = -(N - 1); minv <= (N - 1) - 2; minv++) {
            int maxv = minv + 2;
            if ((minu % 2) != (minv % 2)) continue;
            auto getxy = [](int uu, int vv) { return make_pair((uu + vv) / 2, (uu - vv) / 2); };
            auto bl = getxy(minu, minv);
            auto br = getxy(minu, maxv);
            auto tr = getxy(maxu, maxv);
            auto tl = getxy(maxu, minv);
            bool inrange = [N](pair<int, int> p) {
                return p.first >= 0 && p.first < N && p.second >= 0 && p.second < N;
            };
            if (!inrange(bl) || !inrange(br) || !inrange(tr) || !inrange(tl)) continue;
            vector<pair<int, int>> v = {bl, br, tr, tl};
            rect_verts.push_back(v);
        }
    }
    int num_rect = rect_verts.size();
    vector<bool> rect_used(num_rect, false);
    vector<vector<bool>> horiz_used(N - 1, vector<bool>(N, false));
    vector<vector<bool>> vert_used(N, vector<bool>(N - 1, false));
    vector<vector<bool>> diag1_used(N - 1, vector<bool>(N - 1, false));
    vector<vector<bool>> diag2_used(N - 1, vector<bool>(N, false));
    auto segments_free = [&](int rid) -> bool {
        auto& v = rect_verts[rid];
        for (int kk = 0; kk < 4; kk++) {
            auto p = v[kk];
            auto q = v[(kk + 1) % 4];
            int dx = q.first - p.first;
            int dy = q.second - p.second;
            if (dx == 0) { // vertical
                int x = p.first;
                int ylow = min(p.second, q.second);
                if (vert_used[x][ylow]) return false;
            } else if (dy == 0) { // horizontal
                int y = p.second;
                int xlow = min(p.first, q.first);
                if (horiz_used[xlow][y]) return false;
            } else {
                assert(abs(dx) == 1 && abs(dy) == 1);
                int xlow = min(p.first, q.first);
                if (dx * dy > 0) { // slope 1
                    int ylow = (p.first == xlow ? p.second : q.second);
                    if (diag1_used[xlow][ylow]) return false;
                } else { // slope -1
                    int yhigh = (p.first == xlow ? p.second : q.second);
                    if (diag2_used[xlow][yhigh]) return false;
                }
            }
        }
        return true;
    };
    auto set_segments_used = [&](int rid) {
        auto& v = rect_verts[rid];
        for (int kk = 0; kk < 4; kk++) {
            auto p = v[kk];
            auto q = v[(kk + 1) % 4];
            int dx = q.first - p.first;
            int dy = q.second - p.second;
            if (dx == 0) { // vertical
                int x = p.first;
                int ylow = min(p.second, q.second);
                vert_used[x][ylow] = true;
            } else if (dy == 0) { // horizontal
                int y = p.second;
                int xlow = min(p.first, q.first);
                horiz_used[xlow][y] = true;
            } else {
                assert(abs(dx) == 1 && abs(dy) == 1);
                int xlow = min(p.first, q.first);
                if (dx * dy > 0) { // slope 1
                    int ylow = (p.first == xlow ? p.second : q.second);
                    diag1_used[xlow][ylow] = true;
                } else { // slope -1
                    int yhigh = (p.first == xlow ? p.second : q.second);
                    diag2_used[xlow][yhigh] = true;
                }
            }
        }
    };
    vector<vector<int>> operations;
    bool changed = true;
    while (changed) {
        changed = false;
        long long best_w = -1;
        int best_rid = -1;
        int best_m = -1;
        for (int rid = 0; rid < num_rect; rid++) {
            if (rect_used[rid]) continue;
            auto& v = rect_verts[rid];
            int count = 0;
            int missing = -1;
            for (int k = 0; k < 4; k++) {
                int xx = v[k].first, yy = v[k].second;
                if (has_dot[xx][yy]) count++;
                else missing = k;
            }
            if (count != 3) continue;
            if (!segments_free(rid)) continue;
            long long cur_w = ww[v[missing].first][v[missing].second];
            if (cur_w > best_w) {
                best_w = cur_w;
                best_rid = rid;
                best_m = missing;
            }
        }
        if (best_rid != -1) {
            changed = true;
            rect_used[best_rid] = true;
            auto& v = rect_verts[best_rid];
            int xx = v[best_m].first, yy = v[best_m].second;
            has_dot[xx][yy] = true;
            set_segments_used(best_rid);
            vector<int> op;
            op.push_back(xx);
            op.push_back(yy);
            for (int kk = 1; kk <= 3; kk++) {
                int next = (best_m + kk) % 4;
                op.push_back(v[next].first);
                op.push_back(v[next].second);
            }
            operations.push_back(op);
        }
    }
    cout << operations.size() << endl;
    for (auto& op : operations) {
        for (int val : op) cout << val << " ";
        cout << endl;
    }
    return 0;
}