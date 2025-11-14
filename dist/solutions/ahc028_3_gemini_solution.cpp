#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <map>

using namespace std;

// Globals for problem params
int N, M;
int start_r, start_c;
vector<string> A;
vector<string> T;

// Precomputed data
vector<pair<int, int>> char_positions[26];
vector<vector<int>> overlap_table;

struct TypingResult {
    vector<pair<int, int>> moves;
    long long cost;
    pair<int, int> end_pos;
    bool computed = false;
};

map<string, int> string_to_id;
int next_string_id = 0;
TypingResult memo[225][1201];

int get_string_id(const string& s) {
    if (string_to_id.find(s) == string_to_id.end()) {
        string_to_id[s] = next_string_id++;
    }
    return string_to_id[s];
}

int manhattan_dist(pair<int, int> p1, pair<int, int> p2) {
    return abs(p1.first - p2.first) + abs(p1.second - p2.second);
}

const TypingResult& generate_moves(pair<int, int> start_pos, const string& s) {
    int pos_id = start_pos.first * N + start_pos.second;
    int s_id = get_string_id(s);

    if (memo[pos_id][s_id].computed) {
        return memo[pos_id][s_id];
    }

    int len = s.length();
    vector<vector<pair<long long, int>>> dp(len);

    int c0_idx = s[0] - 'A';
    dp[0].resize(char_positions[c0_idx].size());
    for (size_t i = 0; i < char_positions[c0_idx].size(); ++i) {
        pair<int, int> p = char_positions[c0_idx][i];
        dp[0][i] = {manhattan_dist(start_pos, p) + 1, -1};
    }

    for (int k = 1; k < len; ++k) {
        int ck_idx = s[k] - 'A';
        int c_prev_idx = s[k - 1] - 'A';
        dp[k].resize(char_positions[ck_idx].size());

        for (size_t i = 0; i < char_positions[ck_idx].size(); ++i) {
            pair<int, int> p_curr = char_positions[ck_idx][i];
            long long min_cost = -1;
            int best_prev_idx = -1;
            for (size_t j = 0; j < char_positions[c_prev_idx].size(); ++j) {
                pair<int, int> p_prev = char_positions[c_prev_idx][j];
                long long current_cost = dp[k - 1][j].first + manhattan_dist(p_prev, p_curr) + 1;
                if (min_cost == -1 || current_cost < min_cost) {
                    min_cost = current_cost;
                    best_prev_idx = j;
                }
            }
            dp[k][i] = {min_cost, best_prev_idx};
        }
    }

    long long final_min_cost = -1;
    int best_final_idx = -1;
    int last_char_idx = s.back() - 'A';
    for (size_t i = 0; i < char_positions[last_char_idx].size(); ++i) {
        if (final_min_cost == -1 || dp[len - 1][i].first < final_min_cost) {
            final_min_cost = dp[len - 1][i].first;
            best_final_idx = i;
        }
    }

    vector<pair<int, int>> moves(len);
    pair<int, int> final_pos = char_positions[last_char_idx][best_final_idx];
    int current_idx = best_final_idx;
    for (int k = len - 1; k >= 0; --k) {
        int char_idx = s[k] - 'A';
        moves[k] = char_positions[char_idx][current_idx];
        if (k > 0) {
            current_idx = dp[k][current_idx].second;
        }
    }
    
    memo[pos_id][s_id] = {moves, final_min_cost, final_pos, true};
    return memo[pos_id][s_id];
}

void precompute_overlaps() {
    overlap_table.assign(M, vector<int>(M, 0));
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < M; ++j) {
            if (i == j) continue;
            for (int k = T[i].length() -1; k >= 1; --k) {
                if (T[i].substr(T[i].length() - k) == T[j].substr(0, k)) {
                    overlap_table[i][j] = k;
                    break;
                }
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    cin >> N >> M;
    cin >> start_r >> start_c;
    A.resize(N);
    for (int i = 0; i < N; ++i) cin >> A[i];
    T.resize(M);
    for (int i = 0; i < M; ++i) cin >> T[i];

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            char_positions[A[i][j] - 'A'].push_back({i, j});
        }
    }
    
    precompute_overlaps();

    for(int i=0; i<M; ++i) {
        for(size_t k=0; k<=T[i].length(); ++k) {
            if (!T[i].substr(k).empty())
                get_string_id(T[i].substr(k));
        }
    }

    TypingResult best_overall_result;
    best_overall_result.cost = -1;

    for (int start_node = 0; start_node < M; ++start_node) {
        vector<bool> used(M, false);
        
        TypingResult current_run_result;
        
        used[start_node] = true;
        current_run_result = generate_moves({start_r, start_c}, T[start_node]);
        int current_idx = start_node;
        pair<int, int> current_pos = current_run_result.end_pos;
        
        for (int step = 1; step < M; ++step) {
            int best_next_j = -1;
            int max_k = -1;
            long long min_cost_at_max_k = -1;
            TypingResult best_append_result;

            for (int j = 0; j < M; ++j) {
                if (!used[j]) {
                    int k = overlap_table[current_idx][j];
                    if (k > max_k) {
                        max_k = k;
                        string suffix = T[j].substr(k);
                        if (suffix.empty()){
                            best_append_result = {{}, 0, current_pos, true};
                        } else {
                            best_append_result = generate_moves(current_pos, suffix);
                        }
                        min_cost_at_max_k = best_append_result.cost;
                        best_next_j = j;
                    } else if (k == max_k) {
                        string suffix = T[j].substr(k);
                        TypingResult append_result;
                        if(suffix.empty()) {
                            append_result = {{}, 0, current_pos, true};
                        } else {
                            append_result = generate_moves(current_pos, suffix);
                        }
                        
                        if (best_next_j == -1 || append_result.cost < min_cost_at_max_k) {
                            min_cost_at_max_k = append_result.cost;
                            best_append_result = append_result;
                            best_next_j = j;
                        }
                    }
                }
            }
            
            used[best_next_j] = true;
            if(!best_append_result.moves.empty()) {
                current_run_result.moves.insert(current_run_result.moves.end(), 
                                                best_append_result.moves.begin(), 
                                                best_append_result.moves.end());
            }
            current_run_result.cost += best_append_result.cost;
            current_pos = best_append_result.end_pos;
            current_idx = best_next_j;
        }

        if (best_overall_result.cost == -1 || current_run_result.cost < best_overall_result.cost) {
            best_overall_result = current_run_result;
        }
    }
    
    for (const auto& move : best_overall_result.moves) {
        cout << move.first << " " << move.second << "\n";
    }

    return 0;
}