#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <utility>

using namespace std;

int n, m;
vector<vector<int>> stacks;
vector<pair<int, int>> box_pos; // {stack_idx, index_in_vector}
vector<pair<int, int>> operations;

void read_initial_state() {
    cin >> n >> m;
    stacks.resize(m);
    box_pos.resize(n + 1);
    int boxes_per_stack = n / m;
    for (int i = 0; i < m; ++i) {
        stacks[i].resize(boxes_per_stack);
        for (int j = 0; j < boxes_per_stack; ++j) {
            cin >> stacks[i][j];
            box_pos[stacks[i][j]] = {i, j};
        }
    }
}

int choose_destination_stack(int src_stack_idx, int target_box) {
    int best_dest = -1;
    // Score is a pair: {min_box_val > target_box, -stack_size}
    // We want to maximize this score lexicographically.
    // This prioritizes not burying boxes needed soon,
    // and as a tie-breaker, prefers smaller stacks.
    pair<int, int> best_score = {-1, -(n + 1)};

    for (int i = 0; i < m; ++i) {
        if (i == src_stack_idx) {
            continue;
        }

        if (stacks[i].empty()) {
            return i;
        }

        int min_val_above_target = n + 2; // Sentinel for infinity
        for (int box : stacks[i]) {
            if (box > target_box) {
                min_val_above_target = min(min_val_above_target, box);
            }
        }

        pair<int, int> current_score = {min_val_above_target, -(int)stacks[i].size()};

        if (current_score > best_score) {
            best_score = current_score;
            best_dest = i;
        }
    }
    return best_dest;
}

void solve() {
    for (int target_box = 1; target_box <= n; ++target_box) {
        pair<int, int> pos = box_pos[target_box];
        int s_idx = pos.first;
        int h_idx = pos.second;

        int boxes_above_count = stacks[s_idx].size() - (h_idx + 1);

        if (boxes_above_count > 0) {
            int v_to_move = stacks[s_idx][h_idx + 1];
            
            int d_idx = choose_destination_stack(s_idx, target_box);
            
            operations.push_back({v_to_move, d_idx + 1});

            vector<int> block_to_move;
            block_to_move.reserve(boxes_above_count);
            for (size_t i = h_idx + 1; i < stacks[s_idx].size(); ++i) {
                block_to_move.push_back(stacks[s_idx][i]);
            }

            stacks[s_idx].resize(h_idx + 1);

            int dest_start_h = stacks[d_idx].size();
            for (size_t i = 0; i < block_to_move.size(); ++i) {
                int box_v = block_to_move[i];
                stacks[d_idx].push_back(box_v);
                box_pos[box_v] = {d_idx, (int)(dest_start_h + i)};
            }
        }

        operations.push_back({target_box, 0});
        stacks[s_idx].pop_back();
        box_pos[target_box] = {-1, -1};
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    read_initial_state();
    solve();

    for (const auto& op : operations) {
        cout << op.first << " " << op.second << "\n";
    }

    return 0;
}