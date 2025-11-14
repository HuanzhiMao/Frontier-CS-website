#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

void solve() {
    int N;
    std::cin >> N;
    std::vector<std::string> board(N);
    int oni_total = 0;
    for (int i = 0; i < N; ++i) {
        std::cin >> board[i];
        for (char c : board[i]) {
            if (c == 'x') {
                oni_total++;
            }
        }
    }

    struct Move {
        char dir;
        int idx;
        int k;
        int benefit;
    };

    std::vector<std::pair<char, int>> solution_moves;

    while (oni_total > 0) {
        Move best_move = {' ', -1, 0, 0};
        long long best_b = 0;
        long long best_k = 1;

        // Find the best move across all rows and columns
        // Rows
        for (int i = 0; i < N; ++i) {
            // Left shifts
            int oni_removed = 0;
            for (int k = 1; k <= N; ++k) {
                if (board[i][k - 1] == 'o') break;
                if (board[i][k - 1] == 'x') oni_removed++;
                if (oni_removed > 0) {
                    long long cur_b = oni_removed;
                    long long cur_k = k;
                    if (cur_b * best_k > best_b * cur_k) {
                        best_b = cur_b; best_k = cur_k;
                        best_move = {'L', i, k, oni_removed};
                    } else if (cur_b * best_k == best_b * cur_k && cur_k < best_k) {
                        best_b = cur_b; best_k = cur_k;
                        best_move = {'L', i, k, oni_removed};
                    }
                }
            }
            // Right shifts
            oni_removed = 0;
            for (int k = 1; k <= N; ++k) {
                if (board[i][N - k] == 'o') break;
                if (board[i][N - k] == 'x') oni_removed++;
                if (oni_removed > 0) {
                    long long cur_b = oni_removed;
                    long long cur_k = k;
                    if (cur_b * best_k > best_b * cur_k) {
                        best_b = cur_b; best_k = cur_k;
                        best_move = {'R', i, k, oni_removed};
                    } else if (cur_b * best_k == best_b * cur_k && cur_k < best_k) {
                        best_b = cur_b; best_k = cur_k;
                        best_move = {'R', i, k, oni_removed};
                    }
                }
            }
        }

        // Columns
        for (int j = 0; j < N; ++j) {
            // Up shifts
            int oni_removed = 0;
            for (int k = 1; k <= N; ++k) {
                if (board[k - 1][j] == 'o') break;
                if (board[k - 1][j] == 'x') oni_removed++;
                if (oni_removed > 0) {
                    long long cur_b = oni_removed;
                    long long cur_k = k;
                    if (cur_b * best_k > best_b * cur_k) {
                        best_b = cur_b; best_k = cur_k;
                        best_move = {'U', j, k, oni_removed};
                    } else if (cur_b * best_k == best_b * cur_k && cur_k < best_k) {
                        best_b = cur_b; best_k = cur_k;
                        best_move = {'U', j, k, oni_removed};
                    }
                }
            }
            // Down shifts
            oni_removed = 0;
            for (int k = 1; k <= N; ++k) {
                if (board[N - k][j] == 'o') break;
                if (board[N - k][j] == 'x') oni_removed++;
                if (oni_removed > 0) {
                    long long cur_b = oni_removed;
                    long long cur_k = k;
                    if (cur_b * best_k > best_b * cur_k) {
                        best_b = cur_b; best_k = cur_k;
                        best_move = {'D', j, k, oni_removed};
                    } else if (cur_b * best_k == best_b * cur_k && cur_k < best_k) {
                        best_b = cur_b; best_k = cur_k;
                        best_move = {'D', j, k, oni_removed};
                    }
                }
            }
        }

        if (best_move.idx == -1) {
            // This case should not be reached due to problem guarantees
            break;
        }

        // Apply the best move
        for (int t = 0; t < best_move.k; ++t) {
            solution_moves.push_back({best_move.dir, best_move.idx});
        }
        oni_total -= best_move.benefit;

        if (best_move.dir == 'L') {
            std::string temp_row = board[best_move.idx];
            for (int j = 0; j < N; ++j) {
                board[best_move.idx][j] = (j < N - best_move.k) ? temp_row[j + best_move.k] : '.';
            }
        } else if (best_move.dir == 'R') {
            std::string temp_row = board[best_move.idx];
            for (int j = 0; j < N; ++j) {
                board[best_move.idx][j] = (j >= best_move.k) ? temp_row[j - best_move.k] : '.';
            }
        } else if (best_move.dir == 'U') {
            std::string temp_col(N, ' ');
            for(int i = 0; i < N; ++i) temp_col[i] = board[i][best_move.idx];
            for(int i = 0; i < N; ++i) {
                board[i][best_move.idx] = (i < N - best_move.k) ? temp_col[i + best_move.k] : '.';
            }
        } else if (best_move.dir == 'D') {
            std::string temp_col(N, ' ');
            for(int i = 0; i < N; ++i) temp_col[i] = board[i][best_move.idx];
            for(int i = 0; i < N; ++i) {
                board[i][best_move.idx] = (i >= best_move.k) ? temp_col[i - best_move.k] : '.';
            }
        }
    }

    for (const auto& move : solution_moves) {
        std::cout << move.first << " " << move.second << '\n';
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    solve();
    return 0;
}