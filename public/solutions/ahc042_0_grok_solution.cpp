#include <bits/stdc++.h>
using namespace std;

struct Action {
    char type; // 'R' row, 'C' col
    int idx;
    char dir; // 'L','R','U','D'
    int k;
    vector<int> covered;
};

int main() {
    int N;
    cin >> N;
    vector<string> grid(N);
    for (auto& s : grid) cin >> s;
    vector<pair<int, int>> onis;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (grid[i][j] == 'x') onis.emplace_back(i, j);
        }
    }
    int M = onis.size();
    vector<Action> actions;
    // Rows
    for (int r = 0; r < N; r++) {
        // Left
        int maxk = 0;
        for (int j = 0; j < N; j++) {
            if (grid[r][j] == 'o') break;
            maxk = j + 1;
        }
        if (maxk > 0) {
            Action a;
            a.type = 'R'; a.idx = r; a.dir = 'L'; a.k = maxk;
            for (int p = 0; p < M; p++) {
                auto [ii, jj] = onis[p];
                if (ii == r && jj < maxk) a.covered.push_back(p);
            }
            actions.push_back(a);
        }
        // Right
        int maxk_r = 0;
        for (int j = N - 1; j >= 0; j--) {
            if (grid[r][j] == 'o') break;
            maxk_r++;
        }
        if (maxk_r > 0) {
            Action a;
            a.type = 'R'; a.idx = r; a.dir = 'R'; a.k = maxk_r;
            int minj = N - maxk_r;
            for (int p = 0; p < M; p++) {
                auto [ii, jj] = onis[p];
                if (ii == r && jj >= minj) a.covered.push_back(p);
            }
            actions.push_back(a);
        }
    }
    // Columns
    for (int c = 0; c < N; c++) {
        // Up
        int maxk = 0;
        for (int i = 0; i < N; i++) {
            if (grid[i][c] == 'o') break;
            maxk = i + 1;
        }
        if (maxk > 0) {
            Action a;
            a.type = 'C'; a.idx = c; a.dir = 'U'; a.k = maxk;
            for (int p = 0; p < M; p++) {
                auto [ii, jj] = onis[p];
                if (jj == c && ii < maxk) a.covered.push_back(p);
            }
            actions.push_back(a);
        }
        // Down
        int maxk_d = 0;
        for (int i = N - 1; i >= 0; i--) {
            if (grid[i][c] == 'o') break;
            maxk_d++;
        }
        if (maxk_d > 0) {
            Action a;
            a.type = 'C'; a.idx = c; a.dir = 'D'; a.k = maxk_d;
            int mini = N - maxk_d;
            for (int p = 0; p < M; p++) {
                auto [ii, jj] = onis[p];
                if (jj == c && ii >= mini) a.covered.push_back(p);
            }
            actions.push_back(a);
        }
    }
    vector<bool> is_removed(M, false);
    int num_left = M;
    vector<pair<char, int>> move_list;
    while (num_left > 0) {
        double best_ratio = -1.0;
        int best_id = -1;
        int best_num = 0;
        int best_cost = 0;
        for (int id = 0; id < actions.size(); id++) {
            auto& aa = actions[id];
            int num = 0;
            for (int p : aa.covered) {
                if (!is_removed[p]) num++;
            }
            if (num > 0) {
                double ratio = (double)num / (2.0 * aa.k);
                int cost = 2 * aa.k;
                bool better = false;
                if (ratio > best_ratio + 1e-9) better = true;
                else if (abs(ratio - best_ratio) < 1e-9 && num > best_num) better = true;
                else if (abs(ratio - best_ratio) < 1e-9 && num == best_num && cost < best_cost) better = true;
                if (better) {
                    best_ratio = ratio;
                    best_id = id;
                    best_num = num;
                    best_cost = cost;
                }
            }
        }
        if (best_id == -1) break; // should not happen
        auto& best_a = actions[best_id];
        char d = best_a.dir;
        int p = best_a.idx;
        char opp;
        if (d == 'L') opp = 'R';
        else if (d == 'R') opp = 'L';
        else if (d == 'U') opp = 'D';
        else opp = 'U';
        for (int t = 0; t < best_a.k; t++) move_list.emplace_back(d, p);
        for (int t = 0; t < best_a.k; t++) move_list.emplace_back(opp, p);
        for (int pp : best_a.covered) {
            if (!is_removed[pp]) {
                is_removed[pp] = true;
                num_left--;
            }
        }
    }
    for (auto [dd, pp] : move_list) {
        cout << dd << " " << pp << endl;
    }
    return 0;
}