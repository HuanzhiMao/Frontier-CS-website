#include <iostream>
#include <vector>
#include <deque>
#include <utility>
#include <string>
#include <sstream>

using namespace std;

int n, m;
vector<deque<int>> poles;
vector<pair<int, int>> moves;

void move_ball(int from, int to) {
    if (from == to || poles[from].empty() || poles[to].size() >= m) {
        return;
    }

    int ball = poles[from].back();
    poles[from].pop_back();
    poles[to].push_back(ball);
    moves.push_back({from, to});
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n >> m;
    poles.resize(n + 2);
    for (int i = 1; i <= n; ++i) {
        for (int j = 0; j < m; ++j) {
            int color;
            cin >> color;
            poles[i].push_back(color);
        }
    }

    int temp_pole = n + 1;

    for (int i = 1; i <= n; ++i) {
        // Goal: Fill pole i with color i.
        // Poles 1..i-1 are sorted and must not be touched.

        // Phase 1: Gather balls of color i from other unsorted poles (j > i).
        // Move any obstructing balls to the temporary pole.
        for (int j = i + 1; j <= n; ++j) {
            while (true) {
                bool found_i_ball = false;
                for (int ball : poles[j]) {
                    if (ball == i) {
                        found_i_ball = true;
                        break;
                    }
                }
                if (!found_i_ball) {
                    break;
                }
                
                if (poles[j].back() == i) {
                    move_ball(j, i);
                } else {
                    move_ball(j, temp_pole);
                }
            }
        }

        // Phase 2: Move any non-i balls from pole i to the temporary pole.
        while (!poles[i].empty() && poles[i].back() != i) {
            move_ball(i, temp_pole);
        }

        // Phase 3: Sift through the temporary pole.
        // Move i-balls to pole i, and other balls to their own poles.
        while (poles[i].size() < m) {
            if (poles[temp_pole].empty()) {
                // This case can happen if i-balls are on pole i but under other balls.
                // We need to move them to temp_pole to unearth the non-i balls.
                if (poles[i].empty() || poles[i].back() != i) break; 
                move_ball(i, temp_pole);
                while (!poles[i].empty() && poles[i].back() != i) {
                    move_ball(i, temp_pole);
                }
                continue;
            }

            if (poles[temp_pole].back() == i) {
                move_ball(temp_pole, i);
            } else {
                int color = poles[temp_pole].back();
                move_ball(temp_pole, color);
            }
        }
    }

    cout << moves.size() << "\n";
    for (const auto& p : moves) {
        cout << p.first << " " << p.second << "\n";
    }

    return 0;
}