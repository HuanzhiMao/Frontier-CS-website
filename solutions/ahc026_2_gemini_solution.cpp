#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

// Problem constraints are fixed
const int N = 200;
const int M = 10;
const int K = N / M;

// Struct to store the location of each box for O(1) lookup.
// s_id: stack ID (1-based), pos: index in the stack's vector (0-based).
struct BoxInfo {
    int s_id;
    int pos;
};

// Global state variables
std::vector<int> stacks[M + 1];
BoxInfo box_info[N + 1];
std::vector<std::pair<int, int>> actions;
int current_v = 1;

// Function to perform a move operation.
// It moves the block starting with v_to_move to dest_s_id,
// records the action, and updates the state (stacks and box_info).
void perform_move(int v_to_move, int dest_s_id) {
    actions.push_back({v_to_move, dest_s_id});
    
    int src_s_id = box_info[v_to_move].s_id;
    int src_pos = box_info[v_to_move].pos;

    int num_to_move = stacks[src_s_id].size() - src_pos;
    int dest_start_pos = stacks[dest_s_id].size();

    // Update box_info for all boxes being moved.
    for (int i = 0; i < num_to_move; ++i) {
        int moved_box = stacks[src_s_id][src_pos + i];
        box_info[moved_box].s_id = dest_s_id;
        box_info[moved_box].pos = dest_start_pos + i;
    }
    
    // Efficiently move the block of boxes between stack vectors.
    auto start_it = stacks[src_s_id].begin() + src_pos;
    stacks[dest_s_id].insert(stacks[dest_s_id].end(), start_it, stacks[src_s_id].end());
    stacks[src_s_id].erase(start_it, stacks[src_s_id].end());
}

// Function to perform a carry-out operation for the current target box.
// Records the action and updates state.
void carry_out() {
    actions.push_back({current_v, 0});
    
    int s_id = box_info[current_v].s_id;
    stacks[s_id].pop_back();
    box_info[current_v].s_id = 0; // Mark as carried
    current_v++;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    
    // Read problem dimensions (though they are fixed)
    int n_dummy, m_dummy;
    std::cin >> n_dummy >> m_dummy;
    
    // Read the initial state of the stacks
    for (int i = 1; i <= M; ++i) {
        stacks[i].resize(K);
        for (int j = 0; j < K; ++j) {
            std::cin >> stacks[i][j];
            box_info[stacks[i][j]] = {i, j};
        }
    }
    
    // Main loop: process boxes from 1 to N
    while (current_v <= N) {
        int s_id = box_info[current_v].s_id;
        int pos = box_info[current_v].pos;
        
        // If the target box is already at the top of a stack, carry it out.
        if (pos == stacks[s_id].size() - 1) {
            carry_out();
        } else {
            // Otherwise, move the boxes on top of it.
            int v_above = stacks[s_id][pos + 1];
            
            // Heuristic to find the best destination stack:
            // 1. Primary criterion: Choose the stack where the minimum box number
            //    is as large as possible. This postpones needing to access that stack.
            // 2. Tie-breaker: Choose the shortest stack among those tied. This
            //    minimizes the number of boxes buried by the move.
            int best_dest = -1;
            int max_min_box = -1;
            size_t min_height_at_max = N + 2;
            
            for (int dest = 1; dest <= M; ++dest) {
                if (dest == s_id) continue;
                
                int min_box_in_dest = N + 1; // Represents infinity
                if (!stacks[dest].empty()) {
                    // Since all boxes < current_v are gone, the minimum element
                    // in any stack is the next one we'll need from it.
                    min_box_in_dest = *std::min_element(stacks[dest].begin(), stacks[dest].end());
                }
                
                if (min_box_in_dest > max_min_box) {
                    max_min_box = min_box_in_dest;
                    min_height_at_max = stacks[dest].size();
                    best_dest = dest;
                } else if (min_box_in_dest == max_min_box) {
                    if (stacks[dest].size() < min_height_at_max) {
                        min_height_at_max = stacks[dest].size();
                        best_dest = dest;
                    }
                }
            }
            
            perform_move(v_above, best_dest);
        }
    }
    
    // Output the sequence of actions.
    for (const auto& action : actions) {
        std::cout << action.first << " " << action.second << "\n";
    }
    
    return 0;
}