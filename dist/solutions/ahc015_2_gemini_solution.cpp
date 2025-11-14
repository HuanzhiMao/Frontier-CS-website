#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <queue>

using namespace std;

const int N = 10;

struct State {
    int grid[N][N];

    State() {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                grid[i][j] = 0;
            }
        }
    }
};

State tilt(const State& s, char dir) {
    State next_s;
    if (dir == 'F') {
        for (int j = 0; j < N; ++j) {
            int current_pos = 0;
            for (int i = 0; i < N; ++i) {
                if (s.grid[i][j] != 0) {
                    next_s.grid[current_pos++][j] = s.grid[i][j];
                }
            }
        }
    } else if (dir == 'B') {
        for (int j = 0; j < N; ++j) {
            int current_pos = N - 1;
            for (int i = N - 1; i >= 0; --i) {
                if (s.grid[i][j] != 0) {
                    next_s.grid[current_pos--][j] = s.grid[i][j];
                }
            }
        }
    } else if (dir == 'L') {
        for (int i = 0; i < N; ++i) {
            int current_pos = 0;
            for (int j = 0; j < N; ++j) {
                if (s.grid[i][j] != 0) {
                    next_s.grid[i][current_pos++] = s.grid[i][j];
                }
            }
        }
    } else if (dir == 'R') {
        for (int i = 0; i < N; ++i) {
            int current_pos = N - 1;
            for (int j = N - 1; j >= 0; --j) {
                if (s.grid[i][j] != 0) {
                    next_s.grid[i][current_pos--] = s.grid[i][j];
                }
            }
        }
    }
    return next_s;
}

long long calculate_sum_sq(const State& s) {
    long long sum_sq = 0;
    bool visited[N][N] = {};
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (s.grid[i][j] != 0 && !visited[i][j]) {
                int flavor = s.grid[i][j];
                int count = 0;
                queue<pair<int, int>> q;

                q.push({i, j});
                visited[i][j] = true;
                
                while (!q.empty()) {
                    pair<int, int> curr = q.front();
                    q.pop();
                    count++;
                    
                    for (int k = 0; k < 4; ++k) {
                        int nr = curr.first + dr[k];
                        int nc = curr.second + dc[k];
                        if (nr >= 0 && nr < N && nc >= 0 && nc < N &&
                            !visited[nr][nc] && s.grid[nr][nc] == flavor) {
                            visited[nr][nc] = true;
                            q.push({nr, nc});
                        }
                    }
                }
                sum_sq += (long long)count * count;
            }
        }
    }
    return sum_sq;
}

const int TARGET_TOP = 0;
const int TARGET_BOTTOM = 1;
const int TARGET_LEFT = 2;

long long calculate_penalty(const State& s, const vector<int>& flavor_to_target) {
    long long penalty = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (s.grid[i][j] != 0) {
                int flavor = s.grid[i][j];
                int target = flavor_to_target[flavor];
                if (target == TARGET_TOP) {
                    penalty += i;
                } else if (target == TARGET_BOTTOM) {
                    penalty += (N - 1 - i);
                } else if (target == TARGET_LEFT) {
                    penalty += j;
                }
            }
        }
    }
    return penalty;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    vector<int> f(100);
    vector<int> d(4, 0);
    for (int i = 0; i < 100; ++i) {
        cin >> f[i];
        d[f[i]]++;
    }

    vector<pair<int, int>> sorted_flavors;
    for (int i = 1; i <= 3; ++i) {
        sorted_flavors.push_back({-d[i], i});
    }
    sort(sorted_flavors.begin(), sorted_flavors.end());

    vector<int> flavor_to_target(4);
    flavor_to_target[sorted_flavors[0].second] = TARGET_TOP;
    flavor_to_target[sorted_flavors[1].second] = TARGET_BOTTOM;
    flavor_to_target[sorted_flavors[2].second] = TARGET_LEFT;

    State current_state;
    const char dirs[] = {'F', 'B', 'L', 'R'};

    for (int t = 1; t <= 100; ++t) {
        int p;
        cin >> p;

        int empty_count = 0;
        int r_place = -1, c_place = -1;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (current_state.grid[i][j] == 0) {
                    empty_count++;
                    if (empty_count == p) {
                        r_place = i;
                        c_place = j;
                        break;
                    }
                }
            }
            if (r_place != -1) break;
        }
        current_state.grid[r_place][c_place] = f[t - 1];
        
        char best_dir = 'F';
        long long max_score = -4e18; 

        for (char dir : dirs) {
            State next_s = tilt(current_state, dir);
            long long sum_sq = calculate_sum_sq(next_s);
            long long penalty = calculate_penalty(next_s, flavor_to_target);

            long long current_score = (long long)t * sum_sq - (long long)(101 - t) * penalty;

            if (current_score > max_score) {
                max_score = current_score;
                best_dir = dir;
            }
        }
        
        cout << best_dir << endl;
        current_state = tilt(current_state, best_dir);
    }

    return 0;
}