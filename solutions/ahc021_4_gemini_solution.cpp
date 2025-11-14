#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <tuple>

const int N = 30;
const int MAX_MOVES = 10000;

int board[N][N];
std::vector<std::tuple<int, int, int, int>> moves;

void do_swap(int r1, int c1, int r2, int c2) {
    if (moves.size() >= MAX_MOVES) {
        return;
    }
    std::swap(board[r1][c1], board[r2][c2]);
    moves.emplace_back(r1, c1, r2, c2);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j <= i; ++j) {
            std::cin >> board[i][j];
        }
    }

    for (int i = 0; i < N; ++i) {
        if (moves.size() >= MAX_MOVES) {
            break;
        }

        size_t moves_before_pass = moves.size();

        if (i % 2 == 0) {
            for (int r = N - 2; r >= 0; --r) {
                if (moves.size() >= MAX_MOVES) break;
                for (int c = 0; c <= r; ++c) {
                    if (moves.size() >= MAX_MOVES) break;
                    
                    int p_val = board[r][c];
                    int c1_val = board[r + 1][c];
                    int c2_val = board[r + 1][c + 1];

                    if (p_val > c1_val || p_val > c2_val) {
                        if (c1_val < c2_val) {
                            do_swap(r, c, r + 1, c);
                        } else {
                            do_swap(r, c, r + 1, c + 1);
                        }
                    }
                }
            }
        } else {
            for (int r = 0; r < N - 1; ++r) {
                if (moves.size() >= MAX_MOVES) break;
                for (int c = 0; c <= r; ++c) {
                    if (moves.size() >= MAX_MOVES) break;
                    
                    int p_val = board[r][c];
                    int c1_val = board[r + 1][c];
                    int c2_val = board[r + 1][c + 1];

                    if (p_val > c1_val || p_val > c2_val) {
                        if (c1_val < c2_val) {
                            do_swap(r, c, r + 1, c);
                        } else {
                            do_swap(r, c, r + 1, c + 1);
                        }
                    }
                }
            }
        }
        
        if (moves.size() == moves_before_pass) {
            break;
        }
    }

    std::cout << moves.size() << "\n";
    for (const auto& move : moves) {
        std::cout << std::get<0>(move) << " " << std::get<1>(move) << " "
                  << std::get<2>(move) << " " << std::get<3>(move) << "\n";
    }

    return 0;
}