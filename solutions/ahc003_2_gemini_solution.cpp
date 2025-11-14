#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <cmath>
#include <numeric>
#include <algorithm>

const int N = 30;
const int K = 1000;
const double MIN_COST = 1000.0;
const double MAX_COST = 9000.0;

double h_costs[N][N - 1];
double v_costs[N - 1][N];
int h_counts[N][N - 1];
int v_counts[N - 1][N];

struct State {
    int r, c;
    double d;

    bool operator>(const State& other) const {
        return d > other.d;
    }
};

void solve() {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N - 1; ++j) {
            h_costs[i][j] = (MIN_COST + MAX_COST) / 2.0;
            h_counts[i][j] = 0;
        }
    }
    for (int i = 0; i < N - 1; ++i) {
        for (int j = 0; j < N; ++j) {
            v_costs[i][j] = (MIN_COST + MAX_COST) / 2.0;
            v_counts[i][j] = 0;
        }
    }

    for (int k = 0; k < K; ++k) {
        int si, sj, ti, tj;
        std::cin >> si >> sj >> ti >> tj;

        std::vector<std::vector<double>> dist(N, std::vector<double>(N, 1e18));
        std::vector<std::vector<std::pair<int, int>>> parent(N, std::vector<std::pair<int, int>>(N, {-1, -1}));
        std::priority_queue<State, std::vector<State>, std::greater<State>> pq;

        dist[si][sj] = 0;
        pq.push({si, sj, 0.0});

        while (!pq.empty()) {
            State curr = pq.top();
            pq.pop();

            if (curr.d > dist[curr.r][curr.c]) {
                continue;
            }
            if (curr.r == ti && curr.c == tj) {
                break;
            }

            int r = curr.r, c = curr.c;

            if (r > 0) {
                int nr = r - 1, nc = c;
                double w = v_costs[nr][nc];
                if (dist[r][c] + w < dist[nr][nc]) {
                    dist[nr][nc] = dist[r][c] + w;
                    parent[nr][nc] = {r, c};
                    pq.push({nr, nc, dist[nr][nc]});
                }
            }
            if (r < N - 1) {
                int nr = r + 1, nc = c;
                double w = v_costs[r][c];
                if (dist[r][c] + w < dist[nr][nc]) {
                    dist[nr][nc] = dist[r][c] + w;
                    parent[nr][nc] = {r, c};
                    pq.push({nr, nc, dist[nr][nc]});
                }
            }
            if (c > 0) {
                int nr = r, nc = c - 1;
                double w = h_costs[nr][nc];
                if (dist[r][c] + w < dist[nr][nc]) {
                    dist[nr][nc] = dist[r][c] + w;
                    parent[nr][nc] = {r, c};
                    pq.push({nr, nc, dist[nr][nc]});
                }
            }
            if (c < N - 1) {
                int nr = r, nc = c + 1;
                double w = h_costs[r][c];
                if (dist[r][c] + w < dist[nr][nc]) {
                    dist[nr][nc] = dist[r][c] + w;
                    parent[nr][nc] = {r, c};
                    pq.push({nr, nc, dist[nr][nc]});
                }
            }
        }
        
        std::string path_str = "";
        int cur_r = ti, cur_c = tj;
        while (cur_r != si || cur_c != sj) {
            int pr = parent[cur_r][cur_c].first;
            int pc = parent[cur_r][cur_c].second;
            if (pr == cur_r + 1) path_str += 'U';
            else if (pr == cur_r - 1) path_str += 'D';
            else if (pc == cur_c + 1) path_str += 'L';
            else if (pc == cur_c - 1) path_str += 'R';
            cur_r = pr;
            cur_c = pc;
        }
        std::reverse(path_str.begin(), path_str.end());

        std::cout << path_str << std::endl;

        long long measured_b;
        std::cin >> measured_b;

        double estimated_b = 0;
        cur_r = si; cur_c = sj;
        for (char move : path_str) {
            if (move == 'U') { estimated_b += v_costs[cur_r - 1][cur_c]; cur_r--; }
            else if (move == 'D') { estimated_b += v_costs[cur_r][cur_c]; cur_r++; }
            else if (move == 'L') { estimated_b += h_costs[cur_r][cur_c - 1]; cur_c--; }
            else if (move == 'R') { estimated_b += h_costs[cur_r][cur_c]; cur_c++; }
        }
        
        double delta = (static_cast<double>(measured_b) - estimated_b) / path_str.length();
        double alpha = 0.2;

        cur_r = si; cur_c = sj;
        for (char move : path_str) {
            if (move == 'U') {
                v_costs[cur_r - 1][cur_c] += alpha * delta;
                v_costs[cur_r - 1][cur_c] = std::max(MIN_COST, std::min(MAX_COST, v_costs[cur_r - 1][cur_c]));
                v_counts[cur_r - 1][cur_c]++;
                cur_r--;
            } else if (move == 'D') {
                v_costs[cur_r][cur_c] += alpha * delta;
                v_costs[cur_r][cur_c] = std::max(MIN_COST, std::min(MAX_COST, v_costs[cur_r][cur_c]));
                v_counts[cur_r][cur_c]++;
                cur_r++;
            } else if (move == 'L') {
                h_costs[cur_r][cur_c - 1] += alpha * delta;
                h_costs[cur_r][cur_c - 1] = std::max(MIN_COST, std::min(MAX_COST, h_costs[cur_r][cur_c - 1]));
                h_counts[cur_r][cur_c - 1]++;
                cur_c--;
            } else if (move == 'R') {
                h_costs[cur_r][cur_c] += alpha * delta;
                h_costs[cur_r][cur_c] = std::max(MIN_COST, std::min(MAX_COST, h_costs[cur_r][cur_c]));
                h_counts[cur_r][cur_c]++;
                cur_c++;
            }
        }
        
        for (int i = 0; i < N; ++i) {
            std::vector<double> S(N, 0.0);
            for (int j = 0; j < N - 1; ++j) S[j + 1] = S[j] + h_costs[i][j];

            double max_v = -1.0;
            int best_x = (N - 1) / 2;
            for (int x = 1; x < N - 1; ++x) {
                double val = S[x] * S[x] / x + (S[N - 1] - S[x]) * (S[N - 1] - S[x]) / (N - 1 - x);
                if (val > max_v) {
                    max_v = val;
                    best_x = x;
                }
            }
            
            double mean_l = S[best_x] / best_x;
            double mean_r = (S[N - 1] - S[best_x]) / (N - 1 - best_x);

            for (int j = 0; j < N - 1; ++j) {
                double target = (j < best_x) ? mean_l : mean_r;
                double beta = 0.5 / (1.0 + h_counts[i][j]);
                h_costs[i][j] = (1.0 - beta) * h_costs[i][j] + beta * target;
            }
        }
        for (int j = 0; j < N; ++j) {
            std::vector<double> S(N, 0.0);
            for (int i = 0; i < N - 1; ++i) S[i + 1] = S[i] + v_costs[i][j];
            
            double max_v = -1.0;
            int best_y = (N-1)/2;
            for (int y = 1; y < N - 1; ++y) {
                double val = S[y] * S[y] / y + (S[N - 1] - S[y]) * (S[N - 1] - S[y]) / (N - 1 - y);
                if (val > max_v) {
                    max_v = val;
                    best_y = y;
                }
            }

            double mean_u = S[best_y] / best_y;
            double mean_d = (S[N - 1] - S[best_y]) / (N - 1 - best_y);
            
            for (int i = 0; i < N - 1; ++i) {
                double target = (i < best_y) ? mean_u : mean_d;
                double beta = 0.5 / (1.0 + v_counts[i][j]);
                v_costs[i][j] = (1.0 - beta) * v_costs[i][j] + beta * target;
            }
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    solve();
    return 0;
}