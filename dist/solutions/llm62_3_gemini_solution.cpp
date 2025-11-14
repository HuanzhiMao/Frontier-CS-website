#include <iostream>
#include <vector>
#include <deque>
#include <numeric>

using namespace std;

int n, m;
vector<deque<int>> pillars;
vector<pair<int, int>> moves;

// Function to perform a move and record it
void do_move(int from, int to) {
    if (from == to || pillars[from].empty() || pillars[to].size() >= m) {
        // This should not happen with the current logic
        return;
    }
    int ball = pillars[from].back();
    pillars[from].pop_back();
    pillars[to].push_back(ball);
    moves.push_back({from, to});
}

// Checks if a pillar is fully sorted with m balls of its color
bool is_finished(int p_idx) {
    if (p_idx < 1 || p_idx > n) return true; // Non-target pillars are ignored
    if (pillars[p_idx].size() != m) return false;
    for (int ball : pillars[p_idx]) {
        if (ball != p_idx) return false;
    }
    return true;
}

// Checks if all target pillars (1 to n) are finished
bool all_finished() {
    for (int i = 1; i <= n; ++i) {
        if (!is_finished(i)) return false;
    }
    return true;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n >> m;
    pillars.resize(n + 2);
    for (int i = 1; i <= n; ++i) {
        for (int j = 0; j < m; ++j) {
            int color;
            cin >> color;
            pillars[i].push_back(color);
        }
    }

    // The strategy is a greedy one that prioritizes "good" moves.
    // We designate pillar `i` as the final destination for color `i`.
    // Pillar `n+1` is used as a temporary buffer.
    // The loop continues until all pillars `1..n` are sorted.
    while (!all_finished()) {
        bool moved_this_turn = false;

        // Priority 1: Move a ball from the buffer (pillar n+1) to its correct destination pillar.
        // This is a "good" move if the destination pillar is empty or has the same color on top.
        if (!pillars[n + 1].empty()) {
            int ball_color = pillars[n + 1].back();
            if (ball_color >= 1 && ball_color <= n) {
                if (pillars[ball_color].size() < m && (pillars[ball_color].empty() || pillars[ball_color].back() == ball_color)) {
                    do_move(n + 1, ball_color);
                    moved_this_turn = true;
                }
            }
        }
        if (moved_this_turn) continue;

        // Priority 2: Find a misplaced ball on a pillar `i` (i.e., ball color != i) and move it to its destination `c`.
        // This is only done if the destination pillar `c` is ready to receive it (empty or same color on top).
        for (int i = 1; i <= n; ++i) {
            if (!pillars[i].empty()) {
                int ball_color = pillars[i].back();
                if (ball_color != i) {
                    if (pillars[ball_color].size() < m && (pillars[ball_color].empty() || pillars[ball_color].back() == ball_color)) {
                        do_move(i, ball_color);
                        moved_this_turn = true;
                        break;
                    }
                }
            }
        }
        if (moved_this_turn) continue;

        // Priority 3: If no "good" moves are available, we are in a deadlock/cycle.
        // Example: top(p1)=2, top(p2)=1. We need to use the buffer to break the cycle.
        
        // Case A: The buffer is full. We must move a ball out of it to make space.
        // Let the top ball of the buffer be color `c`. Its destination `c` must be blocked.
        // We find another pillar `j` with space and move the blocking ball from `c` to `j`.
        // This frees up pillar `c` to potentially receive a ball.
        if (pillars[n + 1].size() == m) {
            int ball_color_in_buffer = pillars[n + 1].back();
            int blocked_pillar = ball_color_in_buffer;
            
            int move_to = -1;
            for (int j = 1; j <= n; ++j) {
                if (j != blocked_pillar && pillars[j].size() < m) {
                    move_to = j;
                    break;
                }
            }
            if (move_to != -1) {
                do_move(blocked_pillar, move_to);
            }
            continue; // Let the main loop find the next best move
        }

        // Case B: The buffer has space. We break a cycle by moving a "blocking" ball to the buffer.
        // A simple heuristic is to find any pillar that is not yet sorted and move its top ball to the buffer.
        // This changes the state and will eventually resolve the deadlock.
        int move_from = -1;
        for (int i = 1; i <= n; ++i) {
            if (!is_finished(i) && !pillars[i].empty()) {
                move_from = i;
                break;
            }
        }
        if (move_from != -1) {
            do_move(move_from, n + 1);
        } else if (!pillars[n+1].empty()){
            // This case occurs when pillars 1..n are all either finished or empty,
            // but some balls remain in the buffer. We just need to move them out.
            int ball_color = pillars[n+1].back();
            if (ball_color > 0 && ball_color <=n && pillars[ball_color].size() < m) {
                do_move(n+1, ball_color);
            }
        }
    }

    cout << moves.size() << endl;
    for (const auto& p : moves) {
        cout << p.first << " " << p.second << endl;
    }

    return 0;
}