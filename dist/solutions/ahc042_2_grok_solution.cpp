#include <bits/stdc++.h>
using namespace std;

int main() {
    int N = 20;
    vector<string> board(N);
    for (auto& s : board) cin >> s;
    vector<pair<int, int>> onis;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i][j] == 'x') onis.emplace_back(i, j);
        }
    }
    int num_onis = onis.size();
    vector<vector<int>> oni_options(num_onis, vector<int>(4, -1));
    for (int k = 0; k < num_onis; k++) {
        auto [i, j] = onis[k];
        // up
        bool ok = true;
        for (int a = 0; a < i; a++) if (board[a][j] == 'o') ok = false;
        if (ok) oni_options[k][0] = i + 1;
        // down
        ok = true;
        for (int a = i + 1; a < N; a++) if (board[a][j] == 'o') ok = false;
        if (ok) oni_options[k][1] = N - i;
        // left
        ok = true;
        for (int b = 0; b < j; b++) if (board[i][b] == 'o') ok = false;
        if (ok) oni_options[k][2] = j + 1;
        // right
        ok = true;
        for (int b = j + 1; b < N; b++) if (board[i][b] == 'o') ok = false;
        if (ok) oni_options[k][3] = N - j;
    }
    vector<string> current = board;
    vector<pair<char, int>> all_moves;
    while (true) {
        // count before_x
        int before_x = 0;
        for (int x = 0; x < N; x++) for (int y = 0; y < N; y++) if (current[x][y] == 'x') before_x++;
        if (before_x == 0) break;
        double max_score = -1.0;
        int best_oni = -1;
        int best_type = -1;
        int best_num = -1;
        int best_dist = -1;
        for (int k = 0; k < num_onis; k++) {
            auto [i, j] = onis[k];
            if (current[i][j] != 'x') continue;
            for (int type = 0; type < 4; type++) {
                if (oni_options[k][type] == -1) continue;
                int dist = oni_options[k][type];
                vector<string> temp = current;
                // perform block on temp
                char shift_d, opp_d;
                int p;
                if (type == 0) { // up
                    shift_d = 'U';
                    opp_d = 'D';
                    p = j;
                } else if (type == 1) { // down
                    shift_d = 'D';
                    opp_d = 'U';
                    p = j;
                } else if (type == 2) { // left
                    shift_d = 'L';
                    opp_d = 'R';
                    p = i;
                } else { // right
                    shift_d = 'R';
                    opp_d = 'L';
                    p = i;
                }
                // shift dist times
                for (int t = 0; t < dist; t++) {
                    if (shift_d == 'U') {
                        for (int a = 1; a < N; a++) temp[a - 1][p] = temp[a][p];
                        temp[N - 1][p] = '.';
                    } else if (shift_d == 'D') {
                        for (int a = N - 2; a >= 0; a--) temp[a + 1][p] = temp[a][p];
                        temp[0][p] = '.';
                    } else if (shift_d == 'L') {
                        for (int b = 1; b < N; b++) temp[p][b - 1] = temp[p][b];
                        temp[p][N - 1] = '.';
                    } else if (shift_d == 'R') {
                        for (int b = N - 2; b >= 0; b--) temp[p][b + 1] = temp[p][b];
                        temp[p][0] = '.';
                    }
                }
                // opposite dist times
                for (int t = 0; t < dist; t++) {
                    if (opp_d == 'U') {
                        for (int a = 1; a < N; a++) temp[a - 1][p] = temp[a][p];
                        temp[N - 1][p] = '.';
                    } else if (opp_d == 'D') {
                        for (int a = N - 2; a >= 0; a--) temp[a + 1][p] = temp[a][p];
                        temp[0][p] = '.';
                    } else if (opp_d == 'L') {
                        for (int b = 1; b < N; b++) temp[p][b - 1] = temp[p][b];
                        temp[p][N - 1] = '.';
                    } else if (opp_d == 'R') {
                        for (int b = N - 2; b >= 0; b--) temp[p][b + 1] = temp[p][b];
                        temp[p][0] = '.';
                    }
                }
                // count after_x
                int after_x = 0;
                for (int x = 0; x < N; x++) for (int y = 0; y < N; y++) if (temp[x][y] == 'x') after_x++;
                int num = before_x - after_x;
                double score = (num == 0) ? 0.0 : (double)num / (2.0 * dist);
                if (score > max_score ||
                    (score == max_score && num > best_num) ||
                    (score == max_score && num == best_num && dist < best_dist)) {
                    max_score = score;
                    best_num = num;
                    best_dist = dist;
                    best_oni = k;
                    best_type = type;
                }
            }
        }
        if (best_oni == -1) break;
        // perform on current
        auto [i, j] = onis[best_oni];
        int type = best_type;
        int dist = oni_options[best_oni][type];
        char shift_d, opp_d;
        int p;
        if (type == 0) {
            shift_d = 'U';
            opp_d = 'D';
            p = j;
        } else if (type == 1) {
            shift_d = 'D';
            opp_d = 'U';
            p = j;
        } else if (type == 2) {
            shift_d = 'L';
            opp_d = 'R';
            p = i;
        } else {
            shift_d = 'R';
            opp_d = 'L';
            p = i;
        }
        // shift dist times
        for (int t = 0; t < dist; t++) {
            all_moves.emplace_back(shift_d, p);
            if (shift_d == 'U') {
                for (int a = 1; a < N; a++) current[a - 1][p] = current[a][p];
                current[N - 1][p] = '.';
            } else if (shift_d == 'D') {
                for (int a = N - 2; a >= 0; a--) current[a + 1][p] = current[a][p];
                current[0][p] = '.';
            } else if (shift_d == 'L') {
                for (int b = 1; b < N; b++) current[p][b - 1] = current[p][b];
                current[p][N - 1] = '.';
            } else if (shift_d == 'R') {
                for (int b = N - 2; b >= 0; b--) current[p][b + 1] = current[p][b];
                current[p][0] = '.';
            }
        }
        // opposite
        for (int t = 0; t < dist; t++) {
            all_moves.emplace_back(opp_d, p);
            if (opp_d == 'U') {
                for (int a = 1; a < N; a++) current[a - 1][p] = current[a][p];
                current[N - 1][p] = '.';
            } else if (opp_d == 'D') {
                for (int a = N - 2; a >= 0; a--) current[a + 1][p] = current[a][p];
                current[0][p] = '.';
            } else if (opp_d == 'L') {
                for (int b = 1; b < N; b++) current[p][b - 1] = current[p][b];
                current[p][N - 1] = '.';
            } else if (opp_d == 'R') {
                for (int b = N - 2; b >= 0; b--) current[p][b + 1] = current[p][b];
                current[p][0] = '.';
            }
        }
    }
    for (auto [d, p] : all_moves) {
        cout << d << " " << p << endl;
    }
    return 0;
}