#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <array>
#include <algorithm>
#include <functional>
#include <cmath>

// Constants for the problem
const int N = 20;
const int MAX_LEN = 200;
const int BEAM_WIDTH = 250; // A tunable hyperparameter for beam search

// Type alias for the DP table for cleaner code
using DPTable = std::array<std::array<double, N>, N>;

// Grid and problem parameters
int si, sj, ti, tj;
double p;
std::vector<std::string> h(N);
std::vector<std::string> v(N - 1);

// State struct for beam search
struct State {
    std::string path;
    double score;
    DPTable dp;

    // We need a min-heap for the top-K algorithm, so we define operator>
    bool operator>(const State& other) const {
        return score > other.score;
    }
};

void solve() {
    // Read input
    std::cin >> si >> sj >> ti >> tj >> p;
    for (int i = 0; i < N; ++i) std::cin >> h[i];
    for (int i = 0; i < N - 1; ++i) std::cin >> v[i];

    // Initialize beam with the starting state
    std::vector<State> beam;
    {
        State initial_state;
        initial_state.path = "";
        initial_state.score = 0.0;
        for (int i = 0; i < N; ++i) {
            initial_state.dp[i].fill(0.0);
        }
        initial_state.dp[si][sj] = 1.0;
        beam.push_back(initial_state);
    }

    // Track the best state found so far across all path lengths
    State best_overall_state = beam[0];
    best_overall_state.score = -1.0;

    const char moves[] = {'U', 'D', 'L', 'R'};

    // Main beam search loop
    for (int t = 1; t <= MAX_LEN; ++t) {
        std::priority_queue<State, std::vector<State>, std::greater<State>> pq;

        for (const auto& current_state : beam) {
            // Optimization: if all probability has reached the target, no need to expand.
            // Just keep it as a candidate for the next beam.
            if (current_state.dp[ti][tj] > 1.0 - 1e-12) {
                if (pq.size() < BEAM_WIDTH) {
                    pq.push(current_state);
                } else if (current_state.score > pq.top().score) {
                    pq.pop();
                    pq.push(current_state);
                }
                continue;
            }

            for (char move_char : moves) {
                State next_state;
                next_state.path = current_state.path + move_char;
                
                DPTable& next_dp = next_state.dp;
                // Initialize next DP table with zeros
                for (int i = 0; i < N; ++i) next_dp[i].fill(0.0);
                const DPTable& current_dp = current_state.dp;
                
                // Probability mass already at the target stays there.
                next_dp[ti][tj] = current_dp[ti][tj];
                double prob_arrived_this_step = 0.0;

                // Iterate over the grid to update probabilities
                for (int i = 0; i < N; ++i) {
                    for (int j = 0; j < N; ++j) {
                        if (i == ti && j == tj) continue;
                        if (current_dp[i][j] < 1e-24) continue;

                        // Case 1: Forget the move and stay put
                        next_dp[i][j] += current_dp[i][j] * p;

                        // Case 2: Remember the move
                        double prob_to_move = current_dp[i][j] * (1.0 - p);
                        int ni = i, nj = j;
                        bool wall = true;
                        if (move_char == 'U' && i > 0 && v[i - 1][j] == '0') { ni = i - 1; nj = j; wall = false; }
                        else if (move_char == 'D' && i < N - 1 && v[i][j] == '0') { ni = i + 1; nj = j; wall = false; }
                        else if (move_char == 'L' && j > 0 && h[i][j - 1] == '0') { ni = i; nj = j - 1; wall = false; }
                        else if (move_char == 'R' && j < N - 1 && h[i][j] == '0') { ni = i; nj = j + 1; wall = false; }

                        if (wall) {
                            next_dp[i][j] += prob_to_move;
                        } else {
                            next_dp[ni][nj] += prob_to_move;
                            if (ni == ti && nj == tj) {
                                prob_arrived_this_step += prob_to_move;
                            }
                        }
                    }
                }
                
                next_state.score = current_state.score + prob_arrived_this_step * (401 - t);
                next_state.dp = next_dp;

                // Add to priority queue, maintaining top BEAM_WIDTH states
                if (pq.size() < BEAM_WIDTH) {
                    pq.push(next_state);
                } else if (next_state.score > pq.top().score) {
                    pq.pop();
                    pq.push(next_state);
                }
            }
        }
        
        // Update beam for the next iteration
        beam.clear();
        while (!pq.empty()) {
            beam.push_back(pq.top());
            pq.pop();
        }
        std::reverse(beam.begin(), beam.end()); // pq gives smallest score first

        // Update the overall best state if the current best is better
        if (!beam.empty() && beam[0].score > best_overall_state.score) {
            best_overall_state = beam[0];
        }
    }

    std::cout << best_overall_state.path << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    solve();
    return 0;
}