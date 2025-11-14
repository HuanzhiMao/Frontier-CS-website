#include <bits/stdc++.h>
using namespace std;

struct Vehicle {
    int id;
    bool horiz;
    int fixed;
    int len;
    int max_head;
};

int main() {
    vector<vector<int>> board(6, vector<int>(6));
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            cin >> board[i][j];
        }
    }
    set<int> idset;
    for (int r = 0; r < 6; r++) {
        for (int c = 0; c < 6; c++) {
            if (board[r][c] > 0) idset.insert(board[r][c]);
        }
    }
    int n = idset.size();
    vector<vector<pair<int, int>>> pos_per_id(21);
    for (int r = 0; r < 6; r++) {
        for (int c = 0; c < 6; c++) {
            int idd = board[r][c];
            if (idd > 0) pos_per_id[idd].push_back({r, c});
        }
    }
    vector<Vehicle> vehs;
    int red_index = -1;
    for (int ii : idset) {
        auto& ps = pos_per_id[ii];
        int sz = ps.size();
        bool same_row = true, same_col = true;
        int row0 = ps[0].first, col0 = ps[0].second;
        for (auto& p : ps) {
            if (p.first != row0) same_row = false;
            if (p.second != col0) same_col = false;
        }
        bool h = same_row;
        int fix = h ? row0 : col0;
        if (h) sort(ps.begin(), ps.end(), [](auto a, auto b) { return a.second < b.second; });
        else sort(ps.begin(), ps.end(), [](auto a, auto b) { return a.first < b.first; });
        int len = sz;
        int init_head = h ? ps[0].second : ps[0].first;
        int maxh = 6 - len;
        if (ii == 1) {
            maxh = 6;
            red_index = vehs.size();
        }
        vehs.push_back({ii, h, fix, len, maxh});
    }
    if (red_index != -1 && red_index != 0) {
        swap(vehs[0], vehs[red_index]);
    }
    n = vehs.size();
    vector<int> poss(n);
    for (int i = 0; i < n; i++) {
        poss[i] = vehs[i].max_head + 1;
    }
    vector<long long> cum(n + 1, 1);
    for (int i = 0; i < n; i++) {
        cum[i + 1] = cum[i] * poss[i];
    }
    long long total = cum[n];
    vector<int> initial_pos(n);
    for (int i = 0; i < n; i++) {
        auto& v = vehs[i];
        vector<pair<int, int>> ps = pos_per_id[v.id];
        if (v.horiz) sort(ps.begin(), ps.end(), [](auto a, auto b) { return a.second < b.second; });
        else sort(ps.begin(), ps.end(), [](auto a, auto b) { return a.first < b.first; });
        initial_pos[i] = v.horiz ? ps[0].second : ps[0].first;
    }
    long long init_state = 0;
    for (int i = 0; i < n; i++) {
        init_state += (long long)initial_pos[i] * cum[i];
    }
    // generate goals
    vector<long long> goals;
    bool occ[6][6] = {};
    vector<int> curposg(n);
    function<void(int)> recg = [&](int k) {
        if (k == n) {
            long long state = (long long)6 * cum[0];
            for (int i = 1; i < n; i++) {
                state += (long long)curposg[i] * cum[i];
            }
            goals.push_back(state);
            return;
        }
        if (k == 0) {
            recg(1);
            return;
        }
        for (int p = 0; p < poss[k]; p++) {
            bool ok = true;
            vector<pair<int, int>> tomark;
            auto& v = vehs[k];
            if (v.horiz) {
                int r = v.fixed;
                for (int cc = p; cc < p + v.len; cc++) {
                    if (cc > 5) {
                        ok = false;
                        break;
                    }
                    if (occ[r][cc]) {
                        ok = false;
                        break;
                    }
                    tomark.push_back({r, cc});
                }
            } else {
                int c = v.fixed;
                for (int rr = p; rr < p + v.len; rr++) {
                    if (rr > 5) {
                        ok = false;
                        break;
                    }
                    if (occ[rr][c]) {
                        ok = false;
                        break;
                    }
                    tomark.push_back({rr, c});
                }
            }
            if (ok) {
                for (auto& pr : tomark) occ[pr.first][pr.second] = true;
                curposg[k] = p;
                recg(k + 1);
                for (auto& pr : tomark) occ[pr.first][pr.second] = false;
            }
        }
    };
    recg(0);
    // backwards BFS
    vector<unsigned char> dist(total, 255);
    deque<long long> q;
    for (auto st : goals) {
        if (dist[st] == 255) {
            dist[st] = 0;
            q.push_back(st);
        }
    }
    while (!q.empty()) {
        long long state = q.front();
        q.pop_front();
        vector<int> curpos(n);
        long long temp = state;
        for (int i = 0; i < n; i++) {
            curpos[i] = (temp / cum[i]) % poss[i];
        }
        for (int i = 0; i < n; i++) {
            auto& v = vehs[i];
            for (int dlt : {-1, 1}) {
                int front_r, front_c;
                int head = curpos[i];
                if (v.horiz) {
                    int left = head;
                    int right = left + v.len - 1;
                    int front = (dlt == 1 ? right + 1 : left - 1);
                    front_r = v.fixed;
                    front_c = front;
                } else {
                    int top = head;
                    int bot = top + v.len - 1;
                    int front = (dlt == 1 ? bot + 1 : top - 1);
                    front_r = front;
                    front_c = v.fixed;
                }
                bool is_off = (front_r < 0 || front_r > 5 || front_c < 0 || front_c > 5);
                bool can = false;
                if (is_off) {
                    if (i == 0 && v.horiz && dlt == 1 && v.fixed == 2 && front_c > 5) {
                        can = true;
                    }
                } else {
                    bool empty = true;
                    for (int j = 0; j < n; j++) {
                        if (j == i) continue;
                        auto& vj = vehs[j];
                        int hj = curpos[j];
                        if (vj.horiz) {
                            if (vj.fixed != front_r) continue;
                            int lj = hj, rj = hj + vj.len - 1;
                            if (lj <= front_c && front_c <= rj) {
                                empty = false;
                                break;
                            }
                        } else {
                            if (vj.fixed != front_c) continue;
                            int tj = hj, bj = hj + vj.len - 1;
                            if (tj <= front_r && front_r <= bj) {
                                empty = false;
                                break;
                            }
                        }
                    }
                    if (empty) can = true;
                }
                if (can) {
                    int newhead = head + dlt;
                    if (newhead >= 0 && newhead < poss[i]) {
                        long long newstate = state + (long long)dlt * cum[i];
                        if (dist[newstate] == 255) {
                            dist[newstate] = dist[state] + 1;
                            q.push_back(newstate);
                        }
                    }
                }
            }
        }
    }
    // forward BFS from initial
    vector<unsigned char> reach_dist(total, 255);
    vector<int> prev_st(total, -1);
    vector<char> prev_v(total, 0);
    vector<char> prev_d(total, 0);
    deque<long long> qq;
    qq.push_back(init_state);
    reach_dist[init_state] = 0;
    int max_steps = -1;
    int best_form = INT_MAX;
    long long best_st = -1;
    while (!qq.empty()) {
        long long state = qq.front();
        qq.pop_front();
        int dd = reach_dist[state];
        unsigned char solve = dist[state];
        if (solve == 255) continue;
        if ((int)solve > max_steps || ((int)solve == max_steps && dd < best_form)) {
            max_steps = (int)solve;
            best_form = dd;
            best_st = state;
        }
        vector<int> curpos(n);
        long long temp = state;
        for (int i = 0; i < n; i++) {
            curpos[i] = (temp / cum[i]) % poss[i];
        }
        for (int i = 0; i < n; i++) {
            auto& v = vehs[i];
            for (int dlt : {-1, 1}) {
                int front_r, front_c;
                int head = curpos[i];
                if (v.horiz) {
                    int left = head;
                    int right = left + v.len - 1;
                    int front = (dlt == 1 ? right + 1 : left - 1);
                    front_r = v.fixed;
                    front_c = front;
                } else {
                    int top = head;
                    int bot = top + v.len - 1;
                    int front = (dlt == 1 ? bot + 1 : top - 1);
                    front_r = front;
                    front_c = v.fixed;
                }
                bool is_off = (front_r < 0 || front_r > 5 || front_c < 0 || front_c > 5);
                bool can = false;
                if (is_off) {
                    if (i == 0 && v.horiz && dlt == 1 && v.fixed == 2 && front_c > 5) {
                        can = true;
                    }
                } else {
                    bool empty = true;
                    for (int j = 0; j < n; j++) {
                        if (j == i) continue;
                        auto& vj = vehs[j];
                        int hj = curpos[j];
                        if (vj.horiz) {
                            if (vj.fixed != front_r) continue;
                            int lj = hj, rj = hj + vj.len - 1;
                            if (lj <= front_c && front_c <= rj) {
                                empty = false;
                                break;
                            }
                        } else {
                            if (vj.fixed != front_c) continue;
                            int tj = hj, bj = hj + vj.len - 1;
                            if (tj <= front_r && front_r <= bj) {
                                empty = false;
                                break;
                            }
                        }
                    }
                    if (empty) can = true;
                }
                if (can) {
                    int newhead = head + dlt;
                    if (newhead >= 0 && newhead < poss[i]) {
                        long long newstate = state + (long long)dlt * cum[i];
                        if (reach_dist[newstate] == 255) {
                            reach_dist[newstate] = dd + 1;
                            prev_st[newstate] = state;
                            prev_v[newstate] = v.id;
                            char direc = v.horiz ? (dlt == 1 ? 'R' : 'L') : (dlt == 1 ? 'D' : 'U');
                            prev_d[newstate] = direc;
                            qq.push_back(newstate);
                        }
                    }
                }
            }
        }
    }
    // reconstruct
    vector<pair<int, char>> moves;
    long long current = best_st;
    while (current != init_state) {
        moves.push_back({(int)prev_v[current], prev_d[current]});
        current = prev_st[current];
    }
    reverse(moves.begin(), moves.end());
    cout << max_steps << " " << moves.size() << endl;
    for (auto& mv : moves) {
        cout << mv.first << " " << mv.second << endl;
    }
    return 0;
}