#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <cmath>
#include <iomanip>
#include <numeric>

using namespace std;

// Problem parameters
int si, sj, ti, tj;
double p;
vector<string> h(20);
vector<string> v(19);

// Position struct
struct Pos {
    int r, c;
};

// Function to get next position given a move, considering walls
Pos get_next_pos(int r, int c, char move) {
    if (move == 'U') {
        if (r > 0 && v[r - 1][c] == '0') return {r - 1, c};
    } else if (move == 'D') {
        if (r < 19 && v[r][c] == '0') return {r + 1, c};
    } else if (move == 'L') {
        if (c > 0 && h[r][c - 1] == '0') return {r, c - 1};
    } else if (move == 'R') {
        if (c < 19 && h[r][c] == '0') return {r, c + 1};
    }
    return {r, c};
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Read input
    cin >> si >> sj >> ti >> tj >> p;
    for (int i = 0; i < 20; ++i) cin >> h[i];
    for (int i = 0; i < 19; ++i) cin >> v[i];

    // Precompute shortest path distances from all cells to the target using BFS
    vector<vector<int>> dist(20, vector<int>(20, -1));
    queue<Pos> q;

    dist[ti][tj] = 0;
    q.push({ti, tj});

    while (!q.empty()) {
        Pos curr = q.front();
        q.pop();

        // Check neighbors and add to queue if unvisited and no wall
        // Up neighbor
        if (curr.r > 0 && v[curr.r - 1][curr.c] == '0' && dist[curr.r - 1][curr.c] == -1) {
            dist[curr.r - 1][curr.c] = dist[curr.r][curr.c] + 1;
            q.push({curr.r - 1, curr.c});
        }
        // Down neighbor
        if (curr.r < 19 && v[curr.r][curr.c] == '0' && dist[curr.r + 1][curr.c] == -1) {
            dist[curr.r + 1][curr.c] = dist[curr.r][curr.c] + 1;
            q.push({curr.r + 1, curr.c});
        }
        // Left neighbor
        if (curr.c > 0 && h[curr.r][curr.c - 1] == '0' && dist[curr.r][curr.c - 1] == -1) {
            dist[curr.r][curr.c - 1] = dist[curr.r][curr.c] + 1;
            q.push({curr.r, curr.c - 1});
        }
        // Right neighbor
        if (curr.c < 19 && h[curr.r][curr.c] == '0' && dist[curr.r][curr.c + 1] == -1) {
            dist[curr.r][curr.c + 1] = dist[curr.r][curr.c] + 1;
            q.push({curr.r, curr.c + 1});
        }
    }

    // Main logic: Greedily build the 200-move path
    vector<vector<double>> prob(20, vector<double>(20, 0.0));
    prob[si][sj] = 1.0;

    string ans = "";
    string moves = "DRUL"; // Preferred order due to start/end locations

    for (int t = 0; t < 200; ++t) {
        char best_move = ' ';
        double min_exp_dist = 1e18;
        double min_exp_manhattan = 1e18;

        for (char move : moves) {
            double current_exp_dist = 0;
            double current_exp_manhattan = 0;
            for (int r = 0; r < 20; ++r) {
                for (int c = 0; c < 20; ++c) {
                    if (prob[r][c] > 0) {
                        Pos next_pos = get_next_pos(r, c, move);
                        current_exp_dist += prob[r][c] * dist[next_pos.r][next_pos.c];
                        current_exp_manhattan += prob[r][c] * (abs(next_pos.r - ti) + abs(next_pos.c - tj));
                    }
                }
            }
            
            if (best_move == ' ') { // First move to evaluate
                min_exp_dist = current_exp_dist;
                min_exp_manhattan = current_exp_manhattan;
                best_move = move;
            } else {
                if (current_exp_dist < min_exp_dist - 1e-9) {
                    min_exp_dist = current_exp_dist;
                    min_exp_manhattan = current_exp_manhattan;
                    best_move = move;
                } else if (abs(current_exp_dist - min_exp_dist) < 1e-9 && current_exp_manhattan < min_exp_manhattan - 1e-9) {
                    min_exp_manhattan = current_exp_manhattan;
                    best_move = move;
                }
            }
        }
        ans += best_move;

        // Update probability distribution for the next step
        vector<vector<double>> next_prob(20, vector<double>(20, 0.0));
        for (int r = 0; r < 20; ++r) {
            for (int c = 0; c < 20; ++c) {
                if (prob[r][c] > 0) {
                    // With probability p, stay
                    next_prob[r][c] += p * prob[r][c];
                    // With probability 1-p, move
                    Pos next_pos = get_next_pos(r, c, best_move);
                    if (next_pos.r != ti || next_pos.c != tj) {
                        next_prob[next_pos.r][next_pos.c] += (1.0 - p) * prob[r][c];
                    }
                }
            }
        }
        prob.swap(next_prob);
    }

    cout << ans << endl;

    return 0;
}