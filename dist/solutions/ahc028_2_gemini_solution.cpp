#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <set>

using namespace std;

const int INF = 1e9;

struct Pos {
    int r, c;
};

int dist(Pos p1, Pos p2) {
    return abs(p1.r - p2.r) + abs(p1.c - p2.c);
}

struct PathInfo {
    int cost;
    Pos start_pos;
    Pos end_pos;
    vector<Pos> path;
};

int N, M;
Pos start_finger_pos;
vector<string> grid;
vector<string> t;
vector<vector<Pos>> pos_of_char(26);

unordered_map<string, unordered_map<int, PathInfo>> suffix_infos;
unordered_map<string, PathInfo> best_paths;


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> N >> M;
    cin >> start_finger_pos.r >> start_finger_pos.c;
    grid.resize(N);
    for (int i = 0; i < N; ++i) {
        cin >> grid[i];
        for (int j = 0; j < N; ++j) {
            pos_of_char[grid[i][j] - 'A'].push_back({i, j});
        }
    }
    t.resize(M);
    set<string> suffixes;
    for (int i = 0; i < M; ++i) {
        cin >> t[i];
        for (int j = 0; j < t[i].length(); ++j) {
            suffixes.insert(t[i].substr(j));
        }
    }

    vector<string> sorted_suffixes(suffixes.begin(), suffixes.end());
    sort(sorted_suffixes.begin(), sorted_suffixes.end(), [](const string& a, const string& b) {
        return a.length() < b.length();
    });

    for (const auto& s : sorted_suffixes) {
        char first_char = s[0];
        const auto& start_positions = pos_of_char[first_char - 'A'];
        
        for (const auto& p_start : start_positions) {
            PathInfo pi;
            pi.start_pos = p_start;
            if (s.length() == 1) {
                pi.cost = 1;
                pi.end_pos = p_start;
                pi.path = {p_start};
            } else {
                string tail = s.substr(1);
                char second_char = tail[0];
                const auto& tail_start_positions = pos_of_char[second_char - 'A'];
                int min_tail_cost = INF;
                Pos best_tail_start_pos = {-1, -1};
                
                const auto& tail_infos_map = suffix_infos.at(tail);

                for (const auto& p_tail_start : tail_start_positions) {
                    int key = p_tail_start.r * N + p_tail_start.c;
                    const auto& tail_info = tail_infos_map.at(key);
                    int current_cost = dist(p_start, p_tail_start) + 1 + tail_info.cost;
                    if (current_cost < min_tail_cost) {
                        min_tail_cost = current_cost;
                        best_tail_start_pos = p_tail_start;
                    }
                }
                pi.cost = min_tail_cost;
                int key = best_tail_start_pos.r * N + best_tail_start_pos.c;
                const auto& best_tail_info = tail_infos_map.at(key);
                pi.end_pos = best_tail_info.end_pos;
                pi.path = best_tail_info.path;
                pi.path.insert(pi.path.begin(), p_start);
            }
            suffix_infos[s][p_start.r * N + p_start.c] = pi;
        }
    }
    
    for (const auto& s_ : sorted_suffixes) {
      if (best_paths.count(s_)) continue;
      int min_c = INF;
      PathInfo best_pi;
      const auto& infos = suffix_infos.at(s_);
      for (const auto& pair : infos) {
        if(pair.second.cost < min_c) {
          min_c = pair.second.cost;
          best_pi = pair.second;
        }
      }
      best_paths[s_] = best_pi;
    }

    vector<vector<int>> cost_matrix(M, vector<int>(M));
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < M; ++j) {
            if (i == j) continue;
            string s1 = t[i];
            string s2 = t[j];
            int max_overlap = 0;
            for (int k = min((int)s1.length(), (int)s2.length()) -1; k >= 1; --k) {
                if (s1.substr(s1.length() - k) == s2.substr(0, k)) {
                    max_overlap = k;
                    break;
                }
            }
            if (max_overlap >= 5) {
                cost_matrix[i][j] = 0;
            } else {
                string suffix = s2.substr(max_overlap);
                const auto& s1_path = best_paths.at(s1);
                const auto& suffix_path = best_paths.at(suffix);
                cost_matrix[i][j] = dist(s1_path.end_pos, suffix_path.start_pos) + suffix_path.cost;
            }
        }
    }
    
    vector<int> best_tour;
    int min_tour_cost = INF;

    for (int start_node = 0; start_node < M; ++start_node) {
        vector<int> tour;
        tour.push_back(start_node);
        vector<bool> visited(M, false);
        visited[start_node] = true;
        int current_cost = dist(start_finger_pos, best_paths.at(t[start_node]).start_pos) + best_paths.at(t[start_node]).cost;
        
        int last_node = start_node;
        while (tour.size() < M) {
            int next_node = -1;
            int min_edge_cost = INF;
            for (int i = 0; i < M; ++i) {
                if (!visited[i]) {
                    if (cost_matrix[last_node][i] < min_edge_cost) {
                        min_edge_cost = cost_matrix[last_node][i];
                        next_node = i;
                    }
                }
            }
            tour.push_back(next_node);
            visited[next_node] = true;
            current_cost += min_edge_cost;
            last_node = next_node;
        }
        
        if (current_cost < min_tour_cost) {
            min_tour_cost = current_cost;
            best_tour = tour;
        }
    }
    
    bool improved = true;
    while(improved) {
        improved = false;
        for (int i = 0; i < M - 1; ++i) {
            for (int j = i + 2; j < M -1; ++j) {
                int p_i = best_tour[i], p_i1 = best_tour[i+1];
                int p_j = best_tour[j], p_j1 = best_tour[j+1];
                
                int old_cost_part = cost_matrix[p_i][p_i1] + cost_matrix[p_j][p_j1];
                int new_cost_part = cost_matrix[p_i][p_j] + cost_matrix[p_i1][p_j1];

                if (new_cost_part < old_cost_part) {
                    reverse(best_tour.begin() + i + 1, best_tour.begin() + j + 1);
                    improved = true;
                }
            }
        }
    }


    int first_node = best_tour[0];
    const auto& first_path_info = best_paths.at(t[first_node]);
    for(const auto& p : first_path_info.path) {
        cout << p.r << " " << p.c << '\n';
    }

    for (size_t i = 0; i < best_tour.size() - 1; ++i) {
        int u_idx = best_tour[i];
        int v_idx = best_tour[i+1];
        string s1 = t[u_idx];
        string s2 = t[v_idx];
        int max_overlap = 0;
        for (int k = min((int)s1.length(), (int)s2.length()) - 1; k >= 1; --k) {
            if (s1.substr(s1.length() - k) == s2.substr(0, k)) {
                max_overlap = k;
                break;
            }
        }
        
        if (max_overlap < 5) {
            string suffix = s2.substr(max_overlap);
            const auto& suffix_path_info = best_paths.at(suffix);
            for(const auto& p : suffix_path_info.path) {
                cout << p.r << " " << p.c << '\n';
            }
        }
    }

    return 0;
}