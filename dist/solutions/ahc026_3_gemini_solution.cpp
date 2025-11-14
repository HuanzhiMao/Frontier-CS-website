#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

const int INF = 1e9 + 7;

int n, m;
std::vector<std::vector<int>> stacks;
std::vector<std::pair<int, int>> pos;
std::vector<std::pair<int, int>> history;
std::vector<int> min_val_in_stack;

void apply_move_and_update(int v_to_move, int target_stack_idx) {
    history.emplace_back(v_to_move, target_stack_idx + 1);

    int s_idx = pos[v_to_move].first;
    int v_idx = pos[v_to_move].second;

    std::vector<int> block_to_move;
    block_to_move.reserve(stacks[s_idx].size() - v_idx);
    for(size_t i = v_idx; i < stacks[s_idx].size(); ++i) {
        block_to_move.push_back(stacks[s_idx][i]);
    }
    
    stacks[s_idx].resize(v_idx);
    
    size_t dest_initial_size = stacks[target_stack_idx].size();
    stacks[target_stack_idx].reserve(dest_initial_size + block_to_move.size());
    for(size_t i = 0; i < block_to_move.size(); ++i) {
        int box = block_to_move[i];
        stacks[target_stack_idx].push_back(box);
        pos[box] = {target_stack_idx, (int)(dest_initial_size + i)};
    }
}

void apply_carry_out_and_update(int v) {
    history.emplace_back(v, 0);
    int s_idx = pos[v].first;
    stacks[s_idx].pop_back();
}

int compute_min_val(int stack_idx) {
    if (stacks[stack_idx].empty()) {
        return INF;
    }
    int min_v = INF;
    for (int box : stacks[stack_idx]) {
        min_v = std::min(min_v, box);
    }
    return min_v;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::cin >> n >> m;
    stacks.resize(m);
    pos.resize(n + 1);
    int k = n / m;
    for (int i = 0; i < m; ++i) {
        stacks[i].resize(k);
        for (int j = 0; j < k; ++j) {
            std::cin >> stacks[i][j];
            pos[stacks[i][j]] = {i, j};
        }
    }

    min_val_in_stack.resize(m);
    for(int i = 0; i < m; ++i) {
        min_val_in_stack[i] = compute_min_val(i);
    }


    for (int v = 1; v <= n; ++v) {
        int s_idx = pos[v].first;
        int v_idx = pos[v].second;

        if ((size_t)v_idx == stacks[s_idx].size() - 1) {
            apply_carry_out_and_update(v);
            if (v == min_val_in_stack[s_idx]) {
                min_val_in_stack[s_idx] = compute_min_val(s_idx);
            }
        } else {
            int v_above = stacks[s_idx][v_idx + 1];
            
            int min_in_block = INF;
            for(size_t i = v_idx + 1; i < stacks[s_idx].size(); ++i) {
                min_in_block = std::min(min_in_block, stacks[s_idx][i]);
            }
            
            std::vector<int> good_dests, bad_dests;
            for (int i = 0; i < m; ++i) {
                if (i == s_idx) continue;
                if (stacks[i].empty() || min_val_in_stack[i] > min_in_block) {
                    good_dests.push_back(i);
                } else {
                    bad_dests.push_back(i);
                }
            }

            int best_d = -1;
            auto find_best = [&](const std::vector<int>& dests) {
                int target = -1;
                int min_h = INF;
                int max_top = -2;

                for (int d_idx : dests) {
                    int h = stacks[d_idx].size();
                    if (h < min_h) {
                        min_h = h;
                        target = d_idx;
                        max_top = stacks[d_idx].empty() ? -1 : stacks[d_idx].back();
                    } else if (h == min_h) {
                        int current_top = stacks[d_idx].empty() ? -1 : stacks[d_idx].back();
                        if (current_top > max_top) {
                            target = d_idx;
                            max_top = current_top;
                        }
                    }
                }
                return target;
            };

            if (!good_dests.empty()) {
                best_d = find_best(good_dests);
            } else {
                best_d = find_best(bad_dests);
            }
            
            int old_min_s = min_val_in_stack[s_idx];
            bool old_min_s_is_moved = (pos[old_min_s].second > v_idx);
            int old_min_d = min_val_in_stack[best_d];

            apply_move_and_update(v_above, best_d);

            min_val_in_stack[best_d] = std::min(old_min_d, min_in_block);
            if (old_min_s_is_moved) {
                min_val_in_stack[s_idx] = compute_min_val(s_idx);
            }

            apply_carry_out_and_update(v);
            if (v == min_val_in_stack[s_idx]) {
                 min_val_in_stack[s_idx] = compute_min_val(s_idx);
            }
        }
    }

    for (const auto& op : history) {
        std::cout << op.first << " " << op.second << "\n";
    }

    return 0;
}