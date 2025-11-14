#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <limits>
#include <numeric>
#include <chrono>

using namespace std;

// A high value to represent infinity
const long long INF_LL = 1e18;

// Global variables for problem parameters
int N, M;
int start_row, start_col;
vector<string> grid;
vector<string> targets;

// Precomputed data
vector<pair<int, int>> char_positions[26];
int overlap_matrix[200][200];
int cost_matrix[200][200];

// Calculates the maximum overlap between the end of s1 and the start of s2
int calculate_overlap(const string& s1, const string& s2) {
    for (int k = min((int)s1.length(), (int)s2.length()) - 1; k > 0; --k) {
        if (s1.substr(s1.length() - k) == s2.substr(0, k)) {
            return k;
        }
    }
    return 0;
}

// Calculates the minimum cost to type a given string S using dynamic programming
pair<long long, vector<pair<int, int>>> calculate_typing_plan(const string& S) {
    if (S.empty()) {
        return {0, {}};
    }
    if (S.length() > 5000) {
        return {INF_LL, {}};
    }

    vector<vector<long long>> dp(S.length());
    vector<vector<int>> back_pointer(S.length());

    // Base case: typing the first character
    char c0 = S[0];
    const auto& p0 = char_positions[c0 - 'A'];
    dp[0].resize(p0.size());
    for (size_t i = 0; i < p0.size(); ++i) {
        dp[0][i] = abs(p0[i].first - start_row) + abs(p0[i].second - start_col) + 1;
    }

    // DP for subsequent characters
    for (size_t k = 1; k < S.length(); ++k) {
        const auto& current_char_pos = char_positions[S[k] - 'A'];
        const auto& prev_char_pos = char_positions[S[k - 1] - 'A'];
        dp[k].resize(current_char_pos.size());
        back_pointer[k].resize(current_char_pos.size());

        for (size_t i = 0; i < current_char_pos.size(); ++i) {
            long long min_cost = INF_LL;
            int best_prev_idx = -1;
            for (size_t j = 0; j < prev_char_pos.size(); ++j) {
                long long move_cost = abs(current_char_pos[i].first - prev_char_pos[j].first) + abs(current_char_pos[i].second - prev_char_pos[j].second);
                long long current_total_cost = dp[k - 1][j] + move_cost + 1;
                if (current_total_cost < min_cost) {
                    min_cost = current_total_cost;
                    best_prev_idx = j;
                }
            }
            dp[k][i] = min_cost;
            back_pointer[k][i] = best_prev_idx;
        }
    }

    // Find the minimum cost after typing the entire string
    long long final_cost = INF_LL;
    int final_idx = -1;
    const auto& last_char_pos = char_positions[S.back() - 'A'];
    for (size_t i = 0; i < last_char_pos.size(); ++i) {
        if (dp[S.length() - 1][i] < final_cost) {
            final_cost = dp[S.length() - 1][i];
            final_idx = i;
        }
    }

    if (final_cost >= INF_LL) {
        return {INF_LL, {}};
    }

    // Reconstruct the path of operations by backtracking
    vector<pair<int, int>> operations(S.length());
    int current_idx = final_idx;
    for (int k = S.length() - 1; k >= 0; --k) {
        operations[k] = char_positions[S[k] - 'A'][current_idx];
        if (k > 0) {
            current_idx = back_pointer[k][current_idx];
        }
    }

    return {final_cost, operations};
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    auto start_clock = chrono::high_resolution_clock::now();

    cin >> N >> M;
    cin >> start_row >> start_col;
    grid.resize(N);
    for (int i = 0; i < N; ++i) cin >> grid[i];
    targets.resize(M);
    for (int i = 0; i < M; ++i) cin >> targets[i];

    // Precomputation
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            char_positions[grid[i][j] - 'A'].push_back({i, j});
        }
    }

    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < M; ++j) {
            if (i == j) continue;
            overlap_matrix[i][j] = calculate_overlap(targets[i], targets[j]);
            cost_matrix[i][j] = targets[j].length() - overlap_matrix[i][j];
        }
    }

    long long min_total_cost = INF_LL;
    vector<pair<int, int>> best_operations;
    
    vector<int> start_nodes(M);
    iota(start_nodes.begin(), start_nodes.end(), 0);

    // Main loop: try forming a superstring starting with each target string
    for (int start_node : start_nodes) {
        auto current_time = chrono::high_resolution_clock::now();
        if (chrono::duration_cast<chrono::milliseconds>(current_time - start_clock).count() > 1950) {
            break;
        }
        
        // Use Nearest Neighbor heuristic to find a path (ordering of strings)
        vector<int> path;
        vector<bool> visited(M, false);
        path.push_back(start_node);
        visited[start_node] = true;
        int current_node = start_node;

        while (path.size() < M) {
            int best_next = -1;
            int min_cost = 1e9;
            for (int next_node = 0; next_node < M; ++next_node) {
                if (!visited[next_node]) {
                    if (cost_matrix[current_node][next_node] < min_cost) {
                        min_cost = cost_matrix[current_node][next_node];
                        best_next = next_node;
                    }
                }
            }
            path.push_back(best_next);
            visited[best_next] = true;
            current_node = best_next;
        }

        // Construct the superstring from the path
        string S = targets[path[0]];
        for (size_t i = 1; i < path.size(); ++i) {
            int prev_idx = path[i - 1];
            int curr_idx = path[i];
            int ov = overlap_matrix[prev_idx][curr_idx];
            S += targets[curr_idx].substr(ov);
        }

        // Calculate optimal typing plan and cost for this superstring
        auto [current_total_cost, current_ops] = calculate_typing_plan(S);

        // Update the best solution found so far
        if (current_total_cost < min_total_cost) {
            min_total_cost = current_total_cost;
            best_operations = current_ops;
        }
    }

    // Output the best sequence of operations
    for (const auto& op : best_operations) {
        cout << op.first << " " << op.second << "\n";
    }

    return 0;
}