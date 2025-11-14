#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

int n, m;
vector<vector<int>> poles;
vector<pair<int, int>> moves;

// Helper function to perform and record a move
void move_ball(int from, int to) {
    if (from == to || poles[from - 1].empty() || poles[to - 1].size() >= m) {
        return;
    }

    int ball = poles[from - 1].back();
    poles[from - 1].pop_back();
    poles[to - 1].push_back(ball);
    moves.push_back({from, to});
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

    // Solve for poles n down to 3
    for (int i = n; i >= 3; --i) {
        int target_pole = i;
        int target_color = i;
        int empty_pole = n + 1;
        int temp_pole_A = 1;
        int temp_pole_B = 2;

        // Count correct balls already on target pole
        int correct_on_target = 0;
        for (int ball : poles[target_pole - 1]) {
            if (ball == target_color) {
                correct_on_target++;
            }
        }
        
        // Move balls from target pole until only correct balls are left
        while (poles[target_pole - 1].size() > correct_on_target) {
            int top_ball = poles[target_pole - 1].back();
            if (top_ball == target_color) {
                move_ball(target_pole, empty_pole); // Move correct balls to empty pole to get them out of the way
            } else {
                // Move incorrect balls to a temp pole. Distribute them to not fill one up.
                if (poles[temp_pole_A - 1].size() < m) {
                    move_ball(target_pole, temp_pole_A);
                } else {
                    move_ball(target_pole, temp_pole_B);
                }
            }
        }
        
        // Gather all target_color balls from other poles (1 to i-1)
        for (int j = 1; j < i; ++j) {
            while (true) {
                bool found_target = false;
                for (int ball : poles[j - 1]) {
                    if (ball == target_color) {
                        found_target = true;
                        break;
                    }
                }
                if (!found_target) break;

                int top_ball = poles[j - 1].back();
                if (top_ball == target_color) {
                    move_ball(j, target_pole);
                } else {
                    int temp_dest = (j == temp_pole_A) ? temp_pole_B : temp_pole_A;
                    move_ball(j, temp_dest);
                }
            }
        }
        
        // Move correct balls from empty_pole back to target_pole
        while (poles[target_pole - 1].size() < m) {
            if (!poles[empty_pole - 1].empty()) {
                move_ball(empty_pole, target_pole);
            } else {
                break;
            }
        }
    }

    // Solve for poles 1 and 2 using pole n+1 as a buffer
    if (n >= 2) {
        int p1 = 1, p2 = 2, empty_pole = n + 1;
        
        // Move all balls from pole 1 to empty_pole
        while (!poles[p1 - 1].empty()) {
            move_ball(p1, empty_pole);
        }

        // Sift balls from pole 2: move color 1 to p1, color 2 to empty_pole
        while (!poles[p2 - 1].empty()) {
            if (poles[p2 - 1].back() == p1) {
                move_ball(p2, p1);
            } else {
                move_ball(p2, empty_pole);
            }
        }

        // Sift balls from empty_pole to their correct destinations
        while (!poles[empty_pole - 1].empty()) {
            if (poles[empty_pole - 1].back() == p1) {
                move_ball(empty_pole, p1);
            } else {
                move_ball(empty_pole, p2);
            }
        }
    }

    cout << moves.size() << "\n";
    for (const auto& p : moves) {
        cout << p.first << " " << p.second << "\n";
    }

    return 0;
}