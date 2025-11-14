#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <cmath>
#include <iomanip>
#include <limits>

using namespace std;

const int N = 20;
int si, sj, ti, tj;
double p;
vector<string> h(N);
vector<string> v(N - 1);

bool can_move[N][N][4]; // 0:U, 1:D, 2:L, 3:R
int dist[N][N];

int dr[] = {-1, 1, 0, 0};
int dc[] = {0, 0, -1, 1};
char move_char[] = {'U', 'D', 'L', 'R'};

void build_can_move() {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            // Up
            if (i > 0 && v[i - 1][j] == '0') can_move[i][j][0] = true;
            // Down
            if (i < N - 1 && v[i][j] == '0') can_move[i][j][1] = true;
            // Left
            if (j > 0 && h[i][j - 1] == '0') can_move[i][j][2] = true;
            // Right
            if (j < N - 1 && h[i][j] == '0') can_move[i][j][3] = true;
        }
    }
}

void bfs_dist() {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            dist[i][j] = -1;
        }
    }

    queue<pair<int, int>> q;
    dist[ti][tj] = 0;
    q.push({ti, tj});

    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();

        for (int i = 0; i < 4; ++i) {
            // A neighbor (prev_r, prev_c) can reach (r, c)
            int prev_r = r + dr[i];
            int prev_c = c + dc[i];

            if (prev_r < 0 || prev_r >= N || prev_c < 0 || prev_c >= N || dist[prev_r][prev_c] != -1) continue;

            // The move from (prev_r, prev_c) to (r,c) is direction i^1
            int move_dir_from_prev = i ^ 1;
            if (can_move[prev_r][prev_c][move_dir_from_prev]) {
                dist[prev_r][prev_c] = dist[r][c] + 1;
                q.push({prev_r, prev_c});
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

    build_can_move();
    bfs_dist();

    vector<vector<double>> prob(N, vector<double>(N, 0.0));
    prob[si][sj] = 1.0;

    string ans = "";

    for (int t = 0; t < 200; ++t) {
        char best_move = ' ';
        double min_expected_dist = numeric_limits<double>::max();

        for (int move_idx = 0; move_idx < 4; ++move_idx) {
            vector<vector<double>> next_prob(N, vector<double>(N, 0.0));
            
            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < N; ++j) {
                    if (prob[i][j] < 1e-18) continue;
                    
                    // Stays with probability p
                    next_prob[i][j] += p * prob[i][j];

                    // Moves with probability 1-p
                    int ni = i, nj = j;
                    if (can_move[i][j][move_idx]) {
                        ni += dr[move_idx];
                        nj += dc[move_idx];
                    }
                    next_prob[ni][nj] += (1.0 - p) * prob[i][j];
                }
            }

            double current_expected_dist = 0;
            double total_prob_on_grid = 0;
            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < N; ++j) {
                    if (i == ti && j == tj) continue;
                    if (dist[i][j] > 0) { // Should always be true for non-target cells
                        current_expected_dist += next_prob[i][j] * dist[i][j];
                    }
                    total_prob_on_grid += next_prob[i][j];
                }
            }
            
            if (total_prob_on_grid > 1e-18) {
                current_expected_dist /= total_prob_on_grid;
            } else {
                current_expected_dist = 0;
            }

            if (current_expected_dist < min_expected_dist) {
                min_expected_dist = current_expected_dist;
                best_move = move_char[move_idx];
            }
        }
        
        if (best_move == ' ') { 
           best_move = 'U'; // Default move if all prob reached target
        }
        ans += best_move;

        int best_move_idx = 0;
        for(int i=0; i<4; ++i) if(move_char[i] == best_move) best_move_idx = i;

        vector<vector<double>> new_prob(N, vector<double>(N, 0.0));
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (prob[i][j] < 1e-18) continue;

                new_prob[i][j] += p * prob[i][j];

                int ni = i, nj = j;
                if (can_move[i][j][best_move_idx]) {
                    ni += dr[best_move_idx];
                    nj += dc[best_move_idx];
                }
                new_prob[ni][nj] += (1.0 - p) * prob[i][j];
            }
        }
        
        new_prob[ti][tj] = 0;
        prob = new_prob;
    }

    cout << ans << endl;

    return 0;
}