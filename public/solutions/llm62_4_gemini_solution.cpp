#include <iostream>
#include <vector>
#include <deque>
#include <numeric>
#include <algorithm>

using namespace std;

int n, m;
vector<deque<int>> pillars;
vector<pair<int, int>> moves;

// Function to perform a move and record it
void move_ball(int from, int to) {
    if (from == to) return;
    int ball = pillars[from - 1].back();
    pillars[from - 1].pop_back();
    pillars[to - 1].push_back(ball);
    moves.push_back({from, to});
}

// Finds the shallowest ball of a given color among a set of pillars
pair<int, int> find_shallowest_ball(int color, const vector<int>& from_pillars) {
    pair<int, int> best_pos = {-1, m + 2}; // {pillar, depth from top}
    for (int i : from_pillars) {
        for (int j = pillars[i - 1].size() - 1; j >= 0; --j) {
            if (pillars[i - 1][j] == color) {
                int depth = pillars[i - 1].size() - j;
                if (depth < best_pos.second) {
                    best_pos = {i, depth};
                }
                break; 
            }
        }
    }
    return best_pos;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n >> m;
    pillars.resize(n + 1);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            int color;
            cin >> color;
            pillars[i].push_back(color);
        }
    }

    int buffer_p = n + 1;

    for (int c = 1; c <= n; ++c) {
        // Phase 1: Move all balls that are not color c from pillar c
        while (true) {
            int wrong_ball_depth = -1;
            // Find the topmost ball on pillar c that is not of color c
            for (int i = 0; i < pillars[c - 1].size(); ++i) {
                if (pillars[c - 1][i] != c) {
                    wrong_ball_depth = pillars[c - 1].size() - i;
                    break;
                }
            }

            if (wrong_ball_depth == -1) break; // All balls on pillar c are correct

            // Move the wrong ball and everything above it to the buffer
            for (int i = 0; i < wrong_ball_depth; ++i) {
                move_ball(c, buffer_p);
            }
        }
        
        // Phase 2: Gather remaining balls of color c to pillar c
        while (pillars[c - 1].size() < m) {
            vector<int> search_pillars;
            for(int i = 1; i <= n + 1; ++i) {
                if(i != c) search_pillars.push_back(i);
            }
            
            pair<int, int> pos = find_shallowest_ball(c, search_pillars);
            int p = pos.first;
            int depth = pos.second;

            if (p == -1) break; // No more balls of color c found

            // Make space in buffer_p if needed.
            // The pillar `p` will have space after we move balls from it,
            // so we can use it as a temporary dump for the buffer.
            while (pillars[buffer_p - 1].size() > m - depth) {
                int dump_p = -1;
                // Find any pillar that is not c or p to dump buffer contents.
                // If p is the buffer, we can dump to any pillar other than c.
                if (p == buffer_p) {
                    for (int i = 1; i <= n; ++i) {
                        if (i != c) { dump_p = i; break; }
                    }
                } else {
                    dump_p = p;
                }
                move_ball(buffer_p, dump_p);
            }
            
            // Unearth the ball by moving balls on top to the buffer
            for (int i = 0; i < depth - 1; ++i) {
                move_ball(p, buffer_p);
            }
            // Move the target ball to pillar c
            move_ball(p, c);
        }
    }

    cout << moves.size() << endl;
    for (const auto& move : moves) {
        cout << move.first << " " << move.second << endl;
    }

    return 0;
}