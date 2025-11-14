#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <utility>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n, m;
    std::cin >> n >> m;

    std::vector<std::vector<int>> stacks(m);
    std::vector<std::pair<int, int>> box_location(n + 1);
    std::vector<std::pair<int, int>> operations;

    for (int i = 0; i < m; ++i) {
        stacks[i].resize(n / m);
        for (int j = 0; j < n / m; ++j) {
            std::cin >> stacks[i][j];
            box_location[stacks[i][j]] = {i, j};
        }
    }

    for (int target_box = 1; target_box <= n; ++target_box) {
        auto [s_idx, h_idx] = box_location[target_box];

        if (h_idx == (int)stacks[s_idx].size() - 1) {
            operations.push_back({target_box, 0});
            stacks[s_idx].pop_back();
            box_location[target_box] = {-1, -1};
        } else {
            int move_box_val = stacks[s_idx][h_idx + 1];

            int best_d = -1;
            std::pair<int, int> max_desire = {-1, -1001};

            for (int i = 0; i < m; ++i) {
                if (i == s_idx) continue;

                int primary_score;
                int secondary_score = -stacks[i].size();

                if (stacks[i].empty()) {
                    primary_score = 10000;
                } else {
                    int top_i = stacks[i].back();
                    if (top_i < move_box_val) {
                        primary_score = 1000 + top_i;
                    } else { 
                        primary_score = top_i;
                    }
                }
                
                std::pair<int, int> current_desire = {primary_score, secondary_score};
                if (current_desire > max_desire) {
                    max_desire = current_desire;
                    best_d = i;
                }
            }
            int d_idx = best_d;

            operations.push_back({move_box_val, d_idx + 1});

            std::vector<int> block_to_move;
            for (size_t i = h_idx + 1; i < stacks[s_idx].size(); ++i) {
                block_to_move.push_back(stacks[s_idx][i]);
            }
            
            int original_d_size = stacks[d_idx].size();
            stacks[d_idx].insert(stacks[d_idx].end(), block_to_move.begin(), block_to_move.end());
            
            stacks[s_idx].resize(h_idx + 1);

            for (size_t i = 0; i < block_to_move.size(); ++i) {
                int box_val = block_to_move[i];
                box_location[box_val] = {d_idx, original_d_size + (int)i};
            }

            operations.push_back({target_box, 0});
            stacks[s_idx].pop_back();
            box_location[target_box] = {-1, -1};
        }
    }

    for (const auto& op : operations) {
        std::cout << op.first << " " << op.second << "\n";
    }

    return 0;
}