#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <tuple>

using namespace std;

const int GRID_SIZE = 20;
const int MAX_LEN = 200;

int si, sj, ti, tj;
double p;
vector<string> h(GRID_SIZE);
vector<string> v(GRID_SIZE - 1);
int dist[GRID_SIZE][GRID_SIZE];

const int DR[] = {-1, 1, 0, 0};
const int DC[] = {0, 0, -1, 1};
const char DIRS[] = {'U', 'D', 'L', 'R'};

bool is_wall(int r, int c, int move_idx) {
    if (move_idx == 0) { // U
        return r == 0 || v[r - 1][c] == '1';
    } else if (move_idx == 1) { // D
        return r == GRID_SIZE - 1 || v[r][c] == '1';
    } else if (move_idx == 2) { // L
        return c == 0 || h[r][c - 1] == '1';
    } else if (move_idx == 3) { // R
        return c == GRID_SIZE - 1 || h[r][c] == '1';
    }
    return false;
}

void bfs_dist() {
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            dist[i][j] = -1;
        }
    }

    queue<pair<int, int>> q;
    q.push({ti, tj});
    dist[ti][tj] = 0;

    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();

        for (int i = 0; i < 4; ++i) {
            int nr = r + DR[i];
            int nc = c + DC[i];

            if (nr < 0 || nr >= GRID_SIZE || nc < 0 || nc >= GRID_SIZE) continue;
            
            int move_idx = -1;
            if (nr == r - 1 && nc == c) move_idx = 1; // D
            if (nr == r + 1 && nc == c) move_idx = 0; // U
            if (nr == r && nc == c - 1) move_idx = 3; // R
            if (nr == r && nc == c + 1) move_idx = 2; // L
            
            if (!is_wall(nr, nc, move_idx)) {
                if (dist[nr][nc] == -1) {
                    dist[nr][nc] = dist[r][c] + 1;
                    q.push({nr, nc});
                }
            }
        }
    }
}

struct State {
    vector<double> prob;
    int parent_idx;
    char move;
};

struct Candidate {
    double score;
    int parent_idx;
    char move;
    vector<double> prob;

    bool operator<(const Candidate& other) const {
        return score < other.score;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> si >> sj >> ti >> tj >> p;
    for (int i = 0; i < GRID_SIZE; ++i) cin >> h[i];
    for (int i = 0; i < GRID_SIZE - 1; ++i) cin >> v[i];

    bfs_dist();
    
    int BEAM_WIDTH = 100;

    vector<vector<State>> history(MAX_LEN + 1);

    vector<double> initial_prob(GRID_SIZE * GRID_SIZE, 0.0);
    initial_prob[si * GRID_SIZE + sj] = 1.0;
    history[0].push_back({initial_prob, -1, ' '});

    for (int t = 0; t < MAX_LEN; ++t) {
        vector<Candidate> candidates;
        candidates.reserve(history[t].size() * 4);
        for (int i = 0; i < history[t].size(); ++i) {
            const auto& current_state = history[t][i];
            for (int move_idx = 0; move_idx < 4; ++move_idx) {
                char move_char = DIRS[move_idx];
                vector<double> next_prob(GRID_SIZE * GRID_SIZE, 0.0);
                
                for (int r = 0; r < GRID_SIZE; ++r) {
                    for (int c = 0; c < GRID_SIZE; ++c) {
                        int current_pos_idx = r * GRID_SIZE + c;
                        if (current_state.prob[current_pos_idx] == 0.0) continue;

                        next_prob[current_pos_idx] += p * current_state.prob[current_pos_idx];
                        
                        if (is_wall(r, c, move_idx)) {
                            next_prob[current_pos_idx] += (1.0 - p) * current_state.prob[current_pos_idx];
                        } else {
                            int nr = r + DR[move_idx];
                            int nc = c + DC[move_idx];
                            if (nr == ti && nc == tj) {
                                // Reached goal
                            } else {
                                next_prob[nr * GRID_SIZE + nc] += (1.0 - p) * current_state.prob[current_pos_idx];
                            }
                        }
                    }
                }
                
                double score = 0;
                for(int j = 0; j < GRID_SIZE * GRID_SIZE; ++j) {
                    if (next_prob[j] > 0) {
                        score += next_prob[j] * dist[j / GRID_SIZE][j % GRID_SIZE];
                    }
                }
                candidates.push_back({score, i, move_char, next_prob});
            }
        }
        
        sort(candidates.begin(), candidates.end());

        history[t + 1].reserve(BEAM_WIDTH);
        for (int i=0; i < candidates.size() && i < BEAM_WIDTH; ++i) {
             const auto& cand = candidates[i];
             history[t + 1].push_back({cand.prob, cand.parent_idx, cand.move});
        }
    }

    string result = "";
    int current_idx = 0; 
    for (int t = MAX_LEN; t > 0; --t) {
        result += history[t][current_idx].move;
        current_idx = history[t][current_idx].parent_idx;
    }
    reverse(result.begin(), result.end());
    cout << result << endl;

    return 0;
}