#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <iomanip>
#include <cmath>

using namespace std;

const int N = 20;
int si, sj, ti, tj;
double p;
vector<string> h(N);
vector<string> v(N - 1);

int dist[N][N];
double prob[N][N];
double next_prob[N][N];

// U, D, L, R
int dr[] = {-1, 1, 0, 0};
int dc[] = {0, 0, -1, 1};
char move_char[] = {'U', 'D', 'L', 'R'};

bool is_valid(int r, int c) {
    return r >= 0 && r < N && c >= 0 && c < N;
}

pair<int, int> get_next_pos(int r, int c, int move_idx) {
    int nr = r + dr[move_idx];
    int nc = c + dc[move_idx];

    if (!is_valid(nr, nc)) {
        return {r, c};
    }

    if (move_idx == 0) { // U from (r,c) to (r-1,c), wall is v[r-1][c]
        if (v[r - 1][c] == '1') return {r, c};
    } else if (move_idx == 1) { // D from (r,c) to (r+1,c), wall is v[r][c]
        if (v[r][c] == '1') return {r, c};
    } else if (move_idx == 2) { // L from (r,c) to (r,c-1), wall is h[r][c-1]
        if (h[r][c - 1] == '1') return {r, c};
    } else { // R from (r,c) to (r,c+1), wall is h[r][c]
        if (h[r][c] == '1') return {r, c};
    }
    return {nr, nc};
}

void bfs_dist() {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            dist[i][j] = -1;
        }
    }

    queue<pair<int, int>> q;
    q.push({ti, tj});
    dist[ti][tj] = 0;

    while (!q.empty()) {
        pair<int, int> curr = q.front();
        q.pop();
        int r = curr.first;
        int c = curr.second;

        for (int i = 0; i < 4; ++i) {
            int pr = r - dr[i];
            int pc = c - dc[i];

            if (!is_valid(pr, pc)) continue;
            
            pair<int, int> next_pos = get_next_pos(pr, pc, i);
            if (next_pos.first == r && next_pos.second == c) {
                if (dist[pr][pc] == -1) {
                    dist[pr][pc] = dist[r][c] + 1;
                    q.push({pr, pc});
                }
            }
        }
    }
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> si >> sj >> ti >> tj >> p;
    for (int i = 0; i < N; ++i) cin >> h[i];
    for (int i = 0; i < N - 1; ++i) cin >> v[i];

    bfs_dist();

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            prob[i][j] = 0.0;
        }
    }
    prob[si][sj] = 1.0;

    string path = "";
    
    char move_order[] = {'D', 'R', 'U', 'L'};
    int move_indices[4];
    for(int i = 0; i < 4; ++i) {
        if (move_order[i] == 'U') move_indices[i] = 0;
        else if (move_order[i] == 'D') move_indices[i] = 1;
        else if (move_order[i] == 'L') move_indices[i] = 2;
        else move_indices[i] = 3;
    }

    for (int t = 0; t < 200; ++t) {
        int best_move_idx = -1;
        double min_expected_dist = 1e18;

        for (int i = 0; i < 4; ++i) {
            int move_idx = move_indices[i];
            double current_expected_dist = 0.0;
            for (int r = 0; r < N; ++r) {
                for (int c = 0; c < N; ++c) {
                    if (prob[r][c] > 1e-18) { // Use a small epsilon
                        pair<int, int> next_pos = get_next_pos(r, c, move_idx);
                        int nr = next_pos.first;
                        int nc = next_pos.second;

                        double exp_dist_from_rc = p * dist[r][c] + (1.0 - p) * dist[nr][nc];
                        current_expected_dist += prob[r][c] * exp_dist_from_rc;
                    }
                }
            }
            if (current_expected_dist < min_expected_dist) {
                min_expected_dist = current_expected_dist;
                best_move_idx = move_idx;
            }
        }

        path += move_char[best_move_idx];

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                next_prob[i][j] = 0.0;
            }
        }

        for (int r = 0; r < N; ++r) {
            for (int c = 0; c < N; ++c) {
                if (prob[r][c] > 1e-18) {
                    // Stay due to forget
                    next_prob[r][c] += prob[r][c] * p;

                    // Move
                    pair<int, int> next_pos = get_next_pos(r, c, best_move_idx);
                    int nr = next_pos.first;
                    int nc = next_pos.second;
                    
                    if (nr == ti && nc == tj) {
                        // Reached goal, this probability is removed from simulation
                    } else {
                        next_prob[nr][nc] += prob[r][c] * (1.0 - p);
                    }
                }
            }
        }

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                prob[i][j] = next_prob[i][j];
            }
        }
    }

    cout << path << endl;

    return 0;
}