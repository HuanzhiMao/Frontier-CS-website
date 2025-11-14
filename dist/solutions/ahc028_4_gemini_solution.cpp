#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <map>

using namespace std;

const int INF = 1e9;

int N_size, M_size;
int start_r, start_c;
vector<string> grid_A;
vector<string> lucky_words;
vector<pair<int, int>> locations[26];
vector<vector<int>> overlap_matrix;

void precompute_overlaps() {
    overlap_matrix.assign(M_size, vector<int>(M_size, 0));
    for (int i = 0; i < M_size; ++i) {
        for (int j = 0; j < M_size; ++j) {
            if (i == j) continue;
            for (int k = 4; k >= 1; --k) {
                if (lucky_words[i].substr(5 - k) == lucky_words[j].substr(0, k)) {
                    overlap_matrix[i][j] = k;
                    break;
                }
            }
        }
    }
}

string build_superstring(const vector<int>& path) {
    if (path.empty()) return "";
    string s = lucky_words[path[0]];
    for (size_t i = 1; i < path.size(); ++i) {
        int prev = path[i - 1];
        int curr = path[i];
        int ov = overlap_matrix[prev][curr];
        s += lucky_words[curr].substr(ov);
    }
    return s;
}

void dist_transform(vector<vector<int>>& grid) {
    for (int i = 0; i < N_size; ++i) {
        for (int j = 1; j < N_size; ++j) grid[i][j] = min(grid[i][j], grid[i][j - 1] + 1);
        for (int j = N_size - 2; j >= 0; --j) grid[i][j] = min(grid[i][j], grid[i][j + 1] + 1);
    }
    for (int j = 0; j < N_size; ++j) {
        for (int i = 1; i < N_size; ++i) grid[i][j] = min(grid[i][j], grid[i - 1][j] + 1);
        for (int i = N_size - 2; i >= 0; --i) grid[i][j] = min(grid[i][j], grid[i + 1][j] + 1);
    }
}

int calculate_typing_cost(const string& s) {
    if (s.empty()) return 0;
    int len = s.length();
    vector<vector<int>> dp(N_size, vector<int>(N_size, INF));

    char first_char = s[0];
    for (const auto& p : locations[first_char - 'A']) {
        dp[p.first][p.second] = abs(p.first - start_r) + abs(p.second - start_c) + 1;
    }

    for (int i = 1; i < len; ++i) {
        vector<vector<int>> prev_dp(N_size, vector<int>(N_size, INF));
        char prev_char = s[i-1];
        for(const auto& p : locations[prev_char - 'A']) {
            prev_dp[p.first][p.second] = dp[p.first][p.second];
        }
        
        dist_transform(prev_dp);
        
        dp.assign(N_size, vector<int>(N_size, INF));
        char current_char = s[i];
        for (const auto& p : locations[current_char - 'A']) {
            if (prev_dp[p.first][p.second] != INF)
                dp[p.first][p.second] = prev_dp[p.first][p.second] + 1;
        }
    }

    int min_cost = INF;
    for (int i = 0; i < N_size; ++i) {
        for (int j = 0; j < N_size; ++j) {
            min_cost = min(min_cost, dp[i][j]);
        }
    }
    return min_cost;
}

vector<pair<int, int>> solve_typing_with_backtrack(const string& s) {
    if (s.empty()) return {};
    int len = s.length();
    vector<vector<vector<int>>> dp(len, vector<vector<int>>(N_size, vector<int>(N_size, INF)));
    vector<vector<vector<pair<uint8_t, uint8_t>>>> parent(len, vector<vector<pair<uint8_t, uint8_t>>>(N_size, vector<pair<uint8_t, uint8_t>>(N_size, {(uint8_t)-1, (uint8_t)-1})));

    char first_char = s[0];
    for (const auto& p : locations[first_char - 'A']) {
        dp[0][p.first][p.second] = abs(p.first - start_r) + abs(p.second - start_c) + 1;
    }

    for (int i = 1; i < len; ++i) {
        char current_char = s[i];
        char prev_char = s[i-1];

        for (const auto& p_curr : locations[current_char - 'A']) {
            int r_curr = p_curr.first;
            int c_curr = p_curr.second;
            int min_prev_cost = INF;
            pair<int, int> best_prev_pos = {-1, -1};
            
            for(const auto& p_prev : locations[prev_char - 'A']) {
                int r_prev = p_prev.first;
                int c_prev = p_prev.second;
                if(dp[i-1][r_prev][c_prev] == INF) continue;

                int current_cost = dp[i-1][r_prev][c_prev] + abs(r_curr - r_prev) + abs(c_curr - c_prev) + 1;
                if(current_cost < min_prev_cost) {
                    min_prev_cost = current_cost;
                    best_prev_pos = {r_prev, c_prev};
                }
            }

            if(best_prev_pos.first != -1) {
                dp[i][r_curr][c_curr] = min_prev_cost;
                parent[i][r_curr][c_curr] = {(uint8_t)best_prev_pos.first, (uint8_t)best_prev_pos.second};
            }
        }
    }

    int min_cost = INF;
    pair<int, int> last_pos = {-1, -1};
    for (int i = 0; i < N_size; ++i) {
        for (int j = 0; j < N_size; ++j) {
            if (dp[len - 1][i][j] < min_cost) {
                min_cost = dp[len - 1][i][j];
                last_pos = {i, j};
            }
        }
    }

    vector<pair<int, int>> result_path;
    pair<int, int> curr_pos = last_pos;
    for (int i = len - 1; i >= 0; --i) {
        result_path.push_back(curr_pos);
        if (i > 0) {
            pair<uint8_t, uint8_t> p = parent[i][curr_pos.first][curr_pos.second];
            curr_pos = {(int)p.first, (int)p.second};
        }
    }
    reverse(result_path.begin(), result_path.end());
    return result_path;
}

vector<int> find_path_nn(int start_node) {
    vector<int> path;
    path.push_back(start_node);
    vector<bool> visited(M_size, false);
    visited[start_node] = true;
    int current_node = start_node;

    for (int i = 0; i < M_size - 1; ++i) {
        int best_next = -1;
        int max_ov = -1;
        for (int j = 0; j < M_size; ++j) {
            if (!visited[j]) {
                if (overlap_matrix[current_node][j] > max_ov) {
                    max_ov = overlap_matrix[current_node][j];
                    best_next = j;
                }
            }
        }
        if (best_next != -1) {
            visited[best_next] = true;
            path.push_back(best_next);
            current_node = best_next;
        } else {
            for (int j = 0; j < M_size; ++j) {
                if(!visited[j]) {
                    visited[j] = true;
                    path.push_back(j);
                    current_node = j;
                    break;
                }
            }
        }
    }
    return path;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> N_size >> M_size;
    cin >> start_r >> start_c;
    grid_A.resize(N_size);
    for (int i = 0; i < N_size; ++i) {
        cin >> grid_A[i];
        for (int j = 0; j < N_size; ++j) {
            locations[grid_A[i][j] - 'A'].push_back({i, j});
        }
    }
    lucky_words.resize(M_size);
    for (int i = 0; i < M_size; ++i) {
        cin >> lucky_words[i];
    }

    precompute_overlaps();

    vector<int> best_path;
    int min_total_cost = INF;
    
    for (int i = 0; i < M_size; ++i) {
        vector<int> path = find_path_nn(i);
        string s = build_superstring(path);
        int current_cost = calculate_typing_cost(s);
        if (current_cost < min_total_cost) {
            min_total_cost = current_cost;
            best_path = path;
        }
    }

    string final_s = build_superstring(best_path);
    vector<pair<int, int>> final_coords = solve_typing_with_backtrack(final_s);

    for (const auto& p : final_coords) {
        cout << p.first << " " << p.second << "\n";
    }

    return 0;
}