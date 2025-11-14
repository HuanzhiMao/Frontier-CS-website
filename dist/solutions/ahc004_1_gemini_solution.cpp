#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <algorithm>
#include <cmath>

using namespace std;

const int N = 20;
int M;
vector<string> S;

struct Placement {
    int r, c, dir; // dir 0: horizontal, 1: vertical
};

// State for SA
vector<Placement> placements;
int board_counts[N][N][8];
bool is_conflicting[N][N];
int total_conflicts;

// Best state found
vector<Placement> best_placements;
int min_conflicts;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

void apply_placement(int s_idx, const Placement& p) {
    const string& s = S[s_idx];
    for (int i = 0; i < s.length(); ++i) {
        int r = p.r, c = p.c;
        if (p.dir == 0) c = (c + i) % N;
        else r = (r + i) % N;
        board_counts[r][c][s[i] - 'A']++;
    }
}

void unapply_placement(int s_idx, const Placement& p) {
    const string& s = S[s_idx];
    for (int i = 0; i < s.length(); ++i) {
        int r = p.r, c = p.c;
        if (p.dir == 0) c = (c + i) % N;
        else r = (r + i) % N;
        board_counts[r][c][s[i] - 'A']--;
    }
}

int update_conflict_status(int r, int c) {
    int old_status = is_conflicting[r][c];
    int distinct_chars = 0;
    for (int i = 0; i < 8; ++i) {
        if (board_counts[r][c][i] > 0) {
            distinct_chars++;
        }
    }
    is_conflicting[r][c] = (distinct_chars > 1);
    return is_conflicting[r][c] - old_status;
}

void initial_placement() {
    vector<vector<char>> grid(N, vector<char>(N, '.'));
    vector<int> p(M);
    for(int i=0; i<M; ++i) p[i] = i;
    sort(p.begin(), p.end(), [&](int a, int b){
        return S[a].length() > S[b].length();
    });

    placements.resize(M);
    vector<bool> placed(M, false);

    for (int i = 0; i < M; ++i) {
        int s_idx = p[i];
        int best_r = -1, best_c = -1, best_dir = -1;
        int max_score = -1e9;

        for (int dir = 0; dir < 2; ++dir) {
            for (int r = 0; r < N; ++r) {
                for (int c = 0; c < N; ++c) {
                    int score = 0;
                    for (int k = 0; k < S[s_idx].length(); ++k) {
                        int cur_r = r, cur_c = c;
                        if (dir == 0) cur_c = (c + k) % N;
                        else cur_r = (r + k) % N;

                        if (grid[cur_r][cur_c] != '.' && grid[cur_r][cur_c] != S[s_idx][k]) {
                            score -= 1000;
                        } else if (grid[cur_r][cur_c] == S[s_idx][k]) {
                            score += 10;
                        }
                    }
                    if (score > max_score) {
                        max_score = score;
                        best_r = r;
                        best_c = c;
                        best_dir = dir;
                    }
                }
            }
        }
        
        placements[s_idx] = {best_r, best_c, best_dir};
        placed[s_idx] = true;

        const string& s = S[s_idx];
        for (int k = 0; k < s.length(); ++k) {
            int cur_r = best_r, cur_c = best_c;
            if (best_dir == 0) cur_c = (best_c + k) % N;
            else cur_r = (best_r + k) % N;
            grid[cur_r][cur_c] = s[k];
        }
    }

    // Initialize SA state
    for(int i=0; i<N; ++i) for(int j=0; j<N; ++j) for(int k=0; k<8; ++k) board_counts[i][j][k] = 0;
    for(int i=0; i<N; ++i) for(int j=0; j<N; ++j) is_conflicting[i][j] = false;
    total_conflicts = 0;

    for (int i = 0; i < M; ++i) {
        apply_placement(i, placements[i]);
    }
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            total_conflicts += update_conflict_status(i, j);
        }
    }
    min_conflicts = total_conflicts;
    best_placements = placements;
}


void solve() {
    auto start_time = chrono::steady_clock::now();
    initial_placement();
    
    double start_temp = 10.0, end_temp = 0.1;
    double time_limit = 1.8; // seconds

    int iter = 0;
    while(true) {
        iter++;
        auto now = chrono::steady_clock::now();
        double time_elapsed = chrono::duration_cast<chrono::milliseconds>(now - start_time).count() / 1000.0;
        if (time_elapsed > time_limit) break;

        double progress = time_elapsed / time_limit;
        double temp = start_temp * pow(end_temp / start_temp, progress);

        int s_idx = uniform_int_distribution<int>(0, M - 1)(rng);
        Placement old_p = placements[s_idx];
        Placement new_p = {uniform_int_distribution<int>(0, N - 1)(rng), uniform_int_distribution<int>(0, N - 1)(rng), uniform_int_distribution<int>(0, 1)(rng)};
        
        vector<pair<int, int>> affected_coords;
        const string& s = S[s_idx];
        for (int i = 0; i < s.length(); ++i) {
            int r = old_p.r, c = old_p.c;
            if (old_p.dir == 0) c = (c + i) % N; else r = (r + i) % N;
            affected_coords.push_back({r, c});
        }
        for (int i = 0; i < s.length(); ++i) {
            int r = new_p.r, c = new_p.c;
            if (new_p.dir == 0) c = (c + i) % N; else r = (r + i) % N;
            affected_coords.push_back({r, c});
        }
        sort(affected_coords.begin(), affected_coords.end());
        affected_coords.erase(unique(affected_coords.begin(), affected_coords.end()), affected_coords.end());

        int delta_conflicts = 0;
        for (auto p : affected_coords) {
            delta_conflicts -= is_conflicting[p.first][p.second];
        }

        unapply_placement(s_idx, old_p);
        apply_placement(s_idx, new_p);

        for (auto p : affected_coords) {
            delta_conflicts += update_conflict_status(p.first, p.second);
        }
        
        if (delta_conflicts <= 0 || (uniform_real_distribution<double>(0.0, 1.0)(rng) < exp(-delta_conflicts / temp))) {
            placements[s_idx] = new_p;
            total_conflicts += delta_conflicts;

            if (total_conflicts < min_conflicts) {
                min_conflicts = total_conflicts;
                best_placements = placements;
                if (min_conflicts == 0) {
                     time_limit = time_elapsed + 0.15; // shorten time if solution found
                }
            }
        } else {
            unapply_placement(s_idx, new_p);
            apply_placement(s_idx, old_p);
            for (auto p : affected_coords) {
                update_conflict_status(p.first, p.second);
            }
        }
    }

    if (min_conflicts == 0) {
        auto now = chrono::steady_clock::now();
        double time_elapsed = chrono::duration_cast<chrono::milliseconds>(now - start_time).count() / 1000.0;
        double compaction_time_limit = 1.95;

        while(true) {
            now = chrono::steady_clock::now();
            time_elapsed = chrono::duration_cast<chrono::milliseconds>(now - start_time).count() / 1000.0;
            if (time_elapsed > compaction_time_limit) break;

            int s_idx = uniform_int_distribution<int>(0, M - 1)(rng);
            
            vector<vector<char>> grid_others(N, vector<char>(N, '.'));
            for(int i=0; i<M; ++i) {
                if(i == s_idx) continue;
                const Placement& p = best_placements[i];
                const string& s = S[i];
                for(int k=0; k<s.length(); ++k) {
                    int r = p.r, c = p.c;
                    if(p.dir == 0) c = (c+k)%N; else r = (r+k)%N;
                    grid_others[r][c] = s[k];
                }
            }

            Placement best_new_p = best_placements[s_idx];
            int current_new_cells = 0;
            const string& s_curr = S[s_idx];
            for(int k=0; k<s_curr.length(); ++k) {
                int cur_r=best_new_p.r, cur_c=best_new_p.c;
                if(best_new_p.dir == 0) cur_c = (cur_c+k)%N; else cur_r = (cur_r+k)%N;
                if(grid_others[cur_r][cur_c] == '.') {
                    current_new_cells++;
                }
            }

            int min_new_cells = current_new_cells;

            for(int dir=0; dir<2; ++dir) {
                for(int r=0; r<N; ++r) {
                    for(int c=0; c<N; ++c) {
                        bool ok = true;
                        int new_cells = 0;
                        for(int k=0; k<s_curr.length(); ++k) {
                            int cur_r=r, cur_c=c;
                            if(dir == 0) cur_c = (c+k)%N; else cur_r = (r+k)%N;
                            if(grid_others[cur_r][cur_c] != '.' && grid_others[cur_r][cur_c] != s_curr[k]) {
                                ok = false;
                                break;
                            }
                            if(grid_others[cur_r][cur_c] == '.') {
                                new_cells++;
                            }
                        }
                        if(ok && new_cells < min_new_cells) {
                            min_new_cells = new_cells;
                            best_new_p = {r, c, dir};
                        }
                    }
                }
            }
            best_placements[s_idx] = best_new_p;
        }
    }
    
    // Output
    vector<vector<char>> final_grid(N, vector<char>(N, '.'));
    if (min_conflicts == 0) {
        for (int i = 0; i < M; ++i) {
            const Placement& p = best_placements[i];
            const string& s = S[i];
            for (int k = 0; k < s.length(); ++k) {
                int r = p.r, c = p.c;
                if (p.dir == 0) c = (c + k) % N; else r = (r + k) % N;
                final_grid[r][c] = s[k];
            }
        }
    } else {
        // Fallback: majority vote
        for(int r=0; r<N; ++r) for(int c=0; c<N; ++c) for(int k=0; k<8; ++k) board_counts[r][c][k] = 0;
        for(int i=0; i<M; ++i) apply_placement(i, best_placements[i]);

        for(int r=0; r<N; ++r) {
            for(int c=0; c<N; ++c) {
                int max_cnt = 0;
                char best_char = '.';
                for(int k=0; k<8; ++k) {
                    if(board_counts[r][c][k] > max_cnt) {
                        max_cnt = board_counts[r][c][k];
                        best_char = 'A' + k;
                    }
                }
                if (max_cnt > 0) final_grid[r][c] = best_char;
            }
        }
    }

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << final_grid[i][j];
        }
        cout << endl;
    }
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N_dummy;
    cin >> N_dummy >> M;
    S.resize(M);
    for (int i = 0; i < M; ++i) {
        cin >> S[i];
    }

    solve();

    return 0;
}