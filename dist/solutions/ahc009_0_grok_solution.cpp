#include <bits/stdc++.h>
using namespace std;

using arr = array<double, 400>;

struct BeamItem {
    double score;
    double cur_p;
    arr dist;
    int prev;
    char added;
};

pair<int, int> get_move(int x, int y, char c, const vector<vector<int>>& horiz, const vector<vector<int>>& vert) {
    int dx = 0, dy = 0;
    if (c == 'U') dx = -1;
    else if (c == 'D') dx = 1;
    else if (c == 'L') dy = -1;
    else if (c == 'R') dy = 1;
    int nx = x + dx, ny = y + dy;
    if (nx < 0 || nx > 19 || ny < 0 || ny > 19) return {x, y};
    int wall = 1;
    if (dx == -1) {
        wall = vert[nx][ny];
    } else if (dx == 1) {
        wall = vert[x][y];
    } else if (dy == -1) {
        wall = horiz[x][ny];
    } else if (dy == 1) {
        wall = horiz[x][y];
    }
    if (wall == 0) return {nx, ny};
    return {x, y};
}

arr apply(const arr& prev, char dir, double pp, int target_id, const vector<vector<int>>& horiz, const vector<vector<int>>& vert) {
    arr next{};
    for (int id = 0; id < 400; ++id) {
        double pr = prev[id];
        if (pr < 1e-12) continue;
        int x = id / 20, y = id % 20;
        bool is_t = (id == target_id);
        if (is_t) {
            next[id] += pr;
        } else {
            next[id] += pr * pp;
            auto [nx, ny] = get_move(x, y, dir, horiz, vert);
            int nid = nx * 20 + ny;
            next[nid] += pr * (1 - pp);
        }
    }
    return next;
}

int main() {
    int si, sj, ti, tj;
    double p;
    cin >> si >> sj >> ti >> tj >> p;
    vector<vector<int>> horiz(20, vector<int>(19));
    for (int i = 0; i < 20; ++i) {
        string s;
        cin >> s;
        for (int j = 0; j < 19; ++j) {
            horiz[i][j] = s[j] - '0';
        }
    }
    vector<vector<int>> vert(19, vector<int>(20));
    for (int i = 0; i < 19; ++i) {
        string s;
        cin >> s;
        for (int j = 0; j < 20; ++j) {
            vert[i][j] = s[j] - '0';
        }
    }
    int sid = si * 20 + sj;
    int tid = ti * 20 + tj;
    double init_P = (sid == tid ? 1.0 : 0.0);
    arr init_dist{};
    init_dist[sid] = 1.0;
    const int BEAM = 200;
    const int MAX_L = 200;
    vector<vector<BeamItem>> beams(MAX_L + 1);
    BeamItem init;
    init.score = 0.0;
    init.cur_p = init_P;
    init.dist = init_dist;
    init.prev = -1;
    init.added = ' ';
    beams[0].push_back(init);
    for (int len = 1; len <= MAX_L; ++len) {
        vector<pair<double, int>> candidates;
        vector<BeamItem> temp_items;
        const auto& prevb = beams[len - 1];
        for (int i = 0; i < (int)prevb.size(); ++i) {
            const auto& old = prevb[i];
            for (char d : {'U', 'D', 'L', 'R'}) {
                arr new_d = apply(old.dist, d, p, tid, horiz, vert);
                double new_p = new_d[tid];
                double exact = new_p - old.cur_p;
                double new_E = old.score + exact * (401.0 - len);
                BeamItem bi;
                bi.score = new_E;
                bi.cur_p = new_p;
                bi.dist = new_d;
                bi.prev = i;
                bi.added = d;
                temp_items.push_back(bi);
                candidates.push_back({-new_E, (int)temp_items.size() - 1});
            }
        }
        sort(candidates.begin(), candidates.end());
        vector<BeamItem> nextb;
        int take = min((int)candidates.size(), BEAM);
        for (int j = 0; j < take; ++j) {
            int idx = candidates[j].second;
            nextb.push_back(temp_items[idx]);
        }
        beams[len] = move(nextb);
    }
    double best_E = -1;
    int best_idx = -1;
    for (int i = 0; i < (int)beams[MAX_L].size(); ++i) {
        if (beams[MAX_L][i].score > best_E) {
            best_E = beams[MAX_L][i].score;
            best_idx = i;
        }
    }
    string s = "";
    int current_len = MAX_L;
    int current_idx = best_idx;
    while (current_len >= 1) {
        const auto& st = beams[current_len][current_idx];
        s = st.added + s;
        current_idx = st.prev;
        --current_len;
    }
    cout << s << endl;
    return 0;
}