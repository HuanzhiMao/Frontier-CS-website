#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

int n, m;
vector<vector<int>> poles;
vector<pair<int, int>> moves;
vector<bool> solved_pole;
vector<int> target_color_for_pole;
vector<int> target_pole_for_color;
int solved_count = 0;

void move_ball(int from, int to) {
    int ball = poles[from - 1].back();
    poles[from - 1].pop_back();
    poles[to - 1].push_back(ball);
    moves.push_back({from, to});

    if (solved_pole[from - 1]) {
        solved_pole[from - 1] = false;
        solved_count--;
    }

    if (!solved_pole[to - 1] && poles[to-1].size() == m) {
        bool is_solved = true;
        int color = target_color_for_pole[to];
        for (int ball_color : poles[to-1]) {
            if (ball_color != color) {
                is_solved = false;
                break;
            }
        }
        if (is_solved) {
            solved_pole[to-1] = true;
            solved_count++;
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n >> m;
    poles.resize(n + 1);
    solved_pole.resize(n, false);
    target_color_for_pole.resize(n + 2);
    target_pole_for_color.resize(n + 1);

    for (int i = 0; i < n; ++i) {
        poles[i].resize(m);
        for (int j = 0; j < m; ++j) {
            cin >> poles[i][j];
        }
    }

    // Fixed assignment: pole i for color i
    for (int i = 1; i <= n; ++i) {
        target_color_for_pole[i] = i;
        target_pole_for_color[i] = i;
    }

    for (int i = 0; i < n; i++) {
        if (poles[i].size() == m) {
            bool is_solved = true;
            int color = target_color_for_pole[i + 1];
            for (int ball_color : poles[i]) {
                if (ball_color != color) {
                    is_solved = false;
                    break;
                }
            }
            if (is_solved) {
                solved_pole[i] = true;
                solved_count++;
            }
        }
    }

    while (solved_count < n) {
        bool moved = false;

        // Priority 1: Move a ball from the buffer pole to its target pole
        if (!poles[n].empty()) {
            int top_ball_color = poles[n].back();
            int dest_pole = target_pole_for_color[top_ball_color];
            
            if (poles[dest_pole - 1].size() < m) {
                move_ball(n + 1, dest_pole);
                moved = true;
                continue;
            }
        }
        
        // Priority 2: Move a ball from a non-target pole to its target pole
        for (int i = 1; i <= n; ++i) {
            if (solved_pole[i - 1] || poles[i - 1].empty()) continue;
            
            int top_ball_color = poles[i - 1].back();
            int dest_pole = target_pole_for_color[top_ball_color];

            if (i != dest_pole && poles[dest_pole - 1].size() < m) {
                move_ball(i, dest_pole);
                moved = true;
                break;
            }
        }
        if (moved) continue;
        
        // Priority 3: No direct moves possible, create space.
        int best_pole_to_free = -1;
        // Prefer moving a ball that is on the wrong pole.
        for (int i = 1; i <= n; ++i) {
            if (!solved_pole[i - 1] && !poles[i - 1].empty()) {
                if (poles[i - 1].back() != target_color_for_pole[i]) {
                    best_pole_to_free = i;
                    break;
                }
            }
        }
        // If all non-solved poles have a correct ball on top, just pick any non-solved one.
        if (best_pole_to_free == -1) {
            for (int i = 1; i <= n; ++i) {
                if (!solved_pole[i-1] && !poles[i-1].empty()){
                    best_pole_to_free = i;
                    break;
                }
            }
        }
        
        if (best_pole_to_free != -1) {
            move_ball(best_pole_to_free, n + 1);
        }
    }

    cout << moves.size() << "\n";
    for (const auto& p : moves) {
        cout << p.first << " " << p.second << "\n";
    }

    return 0;
}