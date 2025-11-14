#include <iostream>
#include <vector>
#include <deque>
#include <utility>

int n, m;
std::vector<std::deque<int>> pillars;
std::vector<std::pair<int, int>> moves;

void move_ball(int from, int to) {
    if (from == to || pillars[from].empty() || pillars[to].size() >= m) {
        return;
    }
    int ball = pillars[from].back();
    pillars[from].pop_back();
    pillars[to].push_back(ball);
    moves.push_back({from, to});
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::cin >> n >> m;
    pillars.resize(n + 2);
    for (int i = 1; i <= n; ++i) {
        pillars[i].resize(m);
        for (int j = 0; j < m; ++j) {
            std::cin >> pillars[i][j];
        }
    }

    int empty_pillar = n + 1;

    for (int c = 1; c <= n; ++c) {
        // Phase 1: Move balls that are not color `c` from pillar `c`
        std::vector<int> correct_balls_on_top;
        while (!pillars[c].empty() && pillars[c].back() == c) {
            correct_balls_on_top.push_back(c);
            move_ball(c, empty_pillar);
        }

        std::vector<int> other_balls_to_return;
        while (!pillars[c].empty()) {
            if (pillars[c].back() == c) {
                // This is a correct ball underneath wrong ones. Move it to a temporary safe spot.
                int safe_pillar = -1;
                for (int p = 1; p <= n; ++p) {
                    if (p != c && pillars[p].size() < m) {
                        safe_pillar = p;
                        break;
                    }
                }
                if (safe_pillar == -1) { // Should not happen often, but as a fallback
                    if (pillars[empty_pillar].size() < m) {
                        safe_pillar = empty_pillar;
                    }
                }
                move_ball(c, safe_pillar);
                other_balls_to_return.push_back(safe_pillar);
            } else {
                move_ball(c, empty_pillar);
            }
        }

        for (int p : other_balls_to_return) {
            move_ball(p, c);
        }
        
        while (!correct_balls_on_top.empty()) {
            correct_balls_on_top.pop_back();
            // To move back from empty_pillar, find the ball and bring it to top
            int pos = -1;
            for(int i = pillars[empty_pillar].size() - 1; i >= 0; --i) {
                if (pillars[empty_pillar][i] == c) {
                    pos = i;
                    break;
                }
            }
            int balls_above = pillars[empty_pillar].size() - 1 - pos;
            for(int i = 0; i < balls_above; ++i) {
                move_ball(empty_pillar, c);
            }
            move_ball(empty_pillar, c);
            for(int i = 0; i < balls_above; ++i) {
                move_ball(c, empty_pillar);
            }
        }


        // Phase 2: Gather all balls of color `c` to pillar `c`.
        while (pillars[c].size() < m) {
            int src_p = -1, src_pos = -1;
            int best_dist = m + 1;

            // Find the ball of color 'c' that is easiest to get (closest to top)
            for (int p = 1; p <= n + 1; ++p) {
                if (p == c) continue;
                for (int i = pillars[p].size() - 1; i >= 0; --i) {
                    if (pillars[p][i] == c) {
                        int dist = pillars[p].size() - 1 - i;
                        if (dist < best_dist) {
                            best_dist = dist;
                            src_p = p;
                            src_pos = i;
                        }
                        break; 
                    }
                }
            }
            if (src_p == -1) break; 

            int balls_to_uncover = pillars[src_p].size() - 1 - src_pos;
            for (int i = 0; i < balls_to_uncover; ++i) {
                if (pillars[empty_pillar].size() == m) {
                    move_ball(empty_pillar, src_p);
                }
                move_ball(src_p, empty_pillar);
            }
            move_ball(src_p, c);
            for (int i = 0; i < balls_to_uncover; ++i) {
                if (pillars[src_p].size() == m) {
                    move_ball(src_p, empty_pillar);
                }
                move_ball(empty_pillar, src_p);
            }
        }
    }

    std::cout << moves.size() << "\n";
    for (const auto& p : moves) {
        std::cout << p.first << " " << p.second << "\n";
    }

    return 0;
}