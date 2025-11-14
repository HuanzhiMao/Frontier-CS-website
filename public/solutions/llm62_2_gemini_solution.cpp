#include <iostream>
#include <vector>
#include <numeric>

const int MAXN_Pillars = 55;

int n, m;
std::vector<int> pillars[MAXN_Pillars];
std::vector<std::pair<int, int>> operations;
bool is_finished[MAXN_Pillars];
int finished_count = 0;

void perform_move(int from, int to) {
    operations.push_back({from, to});
    pillars[to].push_back(pillars[from].back());
    pillars[from].pop_back();
}

void check_and_update_finished_status(int pillar_idx) {
    if (pillar_idx > n || is_finished[pillar_idx]) {
        return;
    }
    if (pillars[pillar_idx].size() == m) {
        bool all_correct_color = true;
        for (int ball_color : pillars[pillar_idx]) {
            if (ball_color != pillar_idx) {
                all_correct_color = false;
                break;
            }
        }
        if (all_correct_color) {
            is_finished[pillar_idx] = true;
            finished_count++;
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::cin >> n >> m;
    for (int i = 1; i <= n; ++i) {
        for (int j = 0; j < m; ++j) {
            int color;
            std::cin >> color;
            pillars[i].push_back(color);
        }
    }

    for (int i = 1; i <= n; ++i) {
        check_and_update_finished_status(i);
    }

    int empty_pillar_idx = n + 1;

    while (finished_count < n) {
        // Priority 1: Move a ball from the temporary pillar to its final destination if possible.
        if (!pillars[empty_pillar_idx].empty()) {
            int top_ball_color = pillars[empty_pillar_idx].back();
            if (!is_finished[top_ball_color] && pillars[top_ball_color].size() < m) {
                perform_move(empty_pillar_idx, top_ball_color);
                check_and_update_finished_status(top_ball_color);
                continue;
            }
        }

        // Priority 2: Find a misplaced ball on a pillar and move it to its destination if there's space.
        bool move_made = false;
        for (int i = 1; i <= n; ++i) {
            if (pillars[i].empty() || is_finished[i]) {
                continue;
            }
            int top_ball_color = pillars[i].back();
            if (top_ball_color == i) { // Heuristic: don't move a correctly placed top ball unless necessary.
                continue;
            }
            
            if (!is_finished[top_ball_color] && pillars[top_ball_color].size() < m) {
                perform_move(i, top_ball_color);
                check_and_update_finished_status(top_ball_color);
                move_made = true;
                break;
            }
        }
        if (move_made) {
            continue;
        }

        // Priority 3: If a misplaced ball's destination is full, move the top ball from the destination to the empty pillar to make space.
        move_made = false;
        for (int i = 1; i <= n; ++i) {
            if (pillars[i].empty() || is_finished[i]) {
                continue;
            }
            int top_ball_color = pillars[i].back();
            if (top_ball_color == i) {
                continue;
            }
            
            if (!is_finished[top_ball_color] && pillars[top_ball_color].size() == m && pillars[empty_pillar_idx].size() < m) {
                perform_move(top_ball_color, empty_pillar_idx);
                move_made = true;
                break;
            }
        }
        if (move_made) {
            continue;
        }
        
        // Priority 4: If no other move is possible, unblock a pillar. Move a ball from any non-finished pillar to the empty one.
        // This is necessary if all misplaced balls have full destinations, or all top balls are on their correct (but not finished) pillars.
        if (pillars[empty_pillar_idx].size() < m) {
            int pillar_to_unblock = -1;
            for (int i = 1; i <= n; ++i) {
                if (!pillars[i].empty() && !is_finished[i]) {
                    pillar_to_unblock = i;
                    break;
                }
            }

            if(pillar_to_unblock != -1) {
                perform_move(pillar_to_unblock, empty_pillar_idx);
                continue;
            }
        }
    }

    std::cout << operations.size() << "\n";
    for (const auto& op : operations) {
        std::cout << op.first << " " << op.second << "\n";
    }

    return 0;
}