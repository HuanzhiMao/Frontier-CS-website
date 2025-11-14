#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

// Constants for this problem, as specified in the statement
const int N_val = 200;
const int M_val = 10;

// Data structures to maintain the state of the boxes and stacks
std::vector<int> stacks[M_val + 1];
std::pair<int, int> box_pos[N_val + 1];
std::vector<std::pair<int, int>> history;

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n, m;
    // Reading n and m although they are fixed, for code generality
    std::cin >> n >> m;

    // Initial setup of stacks from input and populate box_pos for quick lookups
    for (int i = 1; i <= m; ++i) {
        int num_boxes_in_stack = n / m;
        stacks[i].resize(num_boxes_in_stack);
        for (int j = 0; j < num_boxes_in_stack; ++j) {
            std::cin >> stacks[i][j];
            box_pos[stacks[i][j]] = {i, j};
        }
    }

    // Main loop to carry out boxes from 1 to n in ascending order
    for (int target_v = 1; target_v <= n; ++target_v) {
        int s_src = box_pos[target_v].first;
        int h_src = box_pos[target_v].second;

        // Check if the target box is already at the top of its stack
        if (h_src == (int)stacks[s_src].size() - 1) {
            history.push_back({target_v, 0});
            stacks[s_src].pop_back();
        } else {
            // Box is covered, we need to move the boxes on top of it
            int best_s_dst = -1;
            
            // Priority 1: An empty stack is the best destination as it obstructs no boxes.
            for (int i = 1; i <= m; ++i) {
                if (i == s_src) continue;
                if (stacks[i].empty()) {
                    best_s_dst = i;
                    break;
                }
            }

            // If no empty stack, find the one that minimizes future costs (badness)
            if (best_s_dst == -1) {
                double min_badness = 1e18; // A large value for initialization
                for (int i = 1; i <= m; ++i) {
                    if (i == s_src) continue;
                    
                    double current_badness = 0;
                    for (int v : stacks[i]) {
                        // Heuristic: badness is the sum of inverse of "time until needed".
                        // A stack with higher numbered boxes will have lower badness.
                        current_badness += 1.0 / (v - target_v + 1);
                    }
                    if (current_badness < min_badness) {
                        min_badness = current_badness;
                        best_s_dst = i;
                    }
                }
            }

            // Perform the move operation
            int v_to_move = stacks[s_src][h_src + 1];
            history.push_back({v_to_move, best_s_dst});
            
            size_t orig_dst_size = stacks[best_s_dst].size();
            for (size_t i = h_src + 1; i < stacks[s_src].size(); ++i) {
                int v = stacks[s_src][i];
                stacks[best_s_dst].push_back(v);
                box_pos[v] = {best_s_dst, (int)(orig_dst_size + (i - (h_src + 1)))};
            }
            stacks[s_src].resize(h_src + 1);
            
            // Now the target box is exposed, carry it out
            history.push_back({target_v, 0});
            stacks[s_src].pop_back();
        }
    }

    // Print all the recorded operations
    for (const auto& op : history) {
        std::cout << op.first << " " << op.second << "\n";
    }

    return 0;
}