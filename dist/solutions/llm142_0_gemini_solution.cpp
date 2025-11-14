#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

int n, m;
vector<vector<int>> poles;
vector<pair<int, int>> moves;

// Function to perform and record a move
void move_ball(int from, int to) {
    if (from == to) return;
    if (poles[from-1].empty()) return;
    if (poles[to-1].size() >= m) return;

    int ball = poles[from-1].back();
    poles[from-1].pop_back();
    poles[to-1].push_back(ball);
    moves.push_back({from, to});
}

// Finds a pole with available space, excluding a specific pole.
// It prioritizes pole n+1 to consolidate misplaced balls.
int find_space(int exclude_pole_idx) {
    if ((n + 1) != exclude_pole_idx && poles[n].size() < m) {
        return n + 1;
    }
    for (int i = 1; i <= n + 1; ++i) {
        if (i == exclude_pole_idx) continue;
        if (poles[i-1].size() < m) {
            return i;
        }
    }
    return -1; // Should not be reached given the problem constraints
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n >> m;
    poles.resize(n + 1);
    for (int i = 0; i < n; ++i) {
        poles[i].resize(m);
        for (int j = 0; j < m; ++j) {
            cin >> poles[i][j];
        }
    }

    // Phase 1: Segregate balls. For each pole i, move out any ball that is not of color i,
    // or is of color i but is stacked on top of a non-i-colored ball.
    for (int i = 1; i <= n; ++i) {
        int target_color = i;
        while (true) {
            int lowest_intruder_idx = -1;
            for (int j = 0; j < poles[i-1].size(); ++j) {
                if (poles[i-1][j] != target_color) {
                    lowest_intruder_idx = j;
                    break;
                }
            }

            if (lowest_intruder_idx == -1) {
                // Pole i is now "clean" (only contains balls of its target color)
                break;
            }

            int num_to_move = poles[i-1].size() - lowest_intruder_idx;
            for (int k = 0; k < num_to_move; ++k) {
                int dest_pole = find_space(i);
                move_ball(i, dest_pole);
            }
        }
    }

    // Phase 2: Consolidate. Fill each pole i with all balls of color i.
    for (int i = 1; i <= n; ++i) {
        int target_color = i;
        while (poles[i-1].size() < m) {
            // First, try to find an accessible ball of the target color on top of another pole.
            bool found_on_top = false;
            for (int j = 1; j <= n + 1; ++j) {
                if (i == j) continue;
                if (!poles[j-1].empty() && poles[j-1].back() == target_color) {
                    move_ball(j, i);
                    found_on_top = true;
                    break;
                }
            }

            if (found_on_top) {
                continue;
            }

            // If no target ball is on top, we need to "dig" for one.
            // Find the pole with a target ball closest to the top.
            int best_pole = -1;
            int min_dist = m + 1;
            for (int j = 1; j <= n + 1; ++j) {
                if (i == j || poles[j-1].empty()) continue;
                for (int k = poles[j-1].size() - 1; k >= 0; --k) {
                    if (poles[j-1][k] == target_color) {
                        int dist = poles[j-1].size() - 1 - k;
                        if (dist < min_dist) {
                            min_dist = dist;
                            best_pole = j;
                        }
                        break; // Found the highest one in this pole
                    }
                }
            }

            if (best_pole != -1) {
                // Dig for the ball by moving obstructing balls away.
                for (int k = 0; k < min_dist; ++k) {
                    int dest_pole = find_space(best_pole);
                    move_ball(best_pole, dest_pole);
                }
                // Move the uncovered target ball.
                move_ball(best_pole, i);
            } else {
                // This implies all balls of target_color are already on pole i.
                break;
            }
        }
    }

    cout << moves.size() << "\n";
    for (const auto& p : moves) {
        cout << p.first << " " << p.second << "\n";
    }

    return 0;
}