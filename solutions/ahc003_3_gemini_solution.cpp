#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <iomanip>

constexpr int N = 30;
constexpr int N_QUERIES = 1000;

struct Pos {
    int r, c;
};

// Estimated individual edge weights and update counts
double h_est[N][N - 1];
double v_est[N - 1][N];
long long h_cnt[N][N - 1];
long long v_cnt[N - 1][N];

// Structured model parameters
bool is_M2_h[N], is_M2_v[N];
int split_h[N], split_v_cols[N];
double H_avg[N][2], V_avg[N][2];

// Edge weights derived from the structured model
double h_model[N][N - 1];
double v_model[N - 1][N];

void init() {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N - 1; ++j) {
            h_est[i][j] = 5000.0;
            h_model[i][j] = 5000.0;
            h_cnt[i][j] = 0;
        }
    }
    for (int i = 0; i < N - 1; ++i) {
        for (int j = 0; j < N; ++j) {
            v_est[i][j] = 5000.0;
            v_model[i][j] = 5000.0;
            v_cnt[i][j] = 0;
        }
    }

    for (int i = 0; i < N; ++i) {
        is_M2_h[i] = false;
        is_M2_v[i] = false;
        H_avg[i][0] = H_avg[i][1] = 5000.0;
        V_avg[i][0] = V_avg[i][1] = 5000.0;
    }
}

void fit_model() {
    // Fit horizontal edges
    for (int i = 0; i < N; ++i) {
        long long total_updates = 0;
        for (int j = 0; j < N - 1; ++j) {
            total_updates += h_cnt[i][j];
        }
        if (total_updates < (N - 1) * 5) continue;

        double sum = 0, sq_sum = 0;
        for (int j = 0; j < N - 1; ++j) {
            sum += h_est[i][j];
            sq_sum += h_est[i][j] * h_est[i][j];
        }

        double cost_m1 = sq_sum - sum * sum / (N - 1);
        
        double min_cost_m2 = 1e18;
        int best_split = -1;
        double means[2];

        double current_sum_1 = 0, current_sq_sum_1 = 0;
        for (int j = 1; j < N - 1; ++j) {
            current_sum_1 += h_est[i][j - 1];
            current_sq_sum_1 += h_est[i][j - 1] * h_est[i][j-1];
            double sum2 = sum - current_sum_1;
            double sq_sum2 = sq_sum - current_sq_sum_1;

            double cost1 = current_sq_sum_1 - current_sum_1 * current_sum_1 / j;
            double cost2 = sq_sum2 - sum2 * sum2 / (N - 1 - j);
            if (cost1 + cost2 < min_cost_m2) {
                min_cost_m2 = cost1 + cost2;
                best_split = j;
                means[0] = current_sum_1 / j;
                means[1] = sum2 / (N - 1 - j);
            }
        }
        
        if (min_cost_m2 < cost_m1 * 0.6) {
            is_M2_h[i] = true;
            split_h[i] = best_split;
            H_avg[i][0] = means[0];
            H_avg[i][1] = means[1];
        } else {
            is_M2_h[i] = false;
            H_avg[i][0] = sum / (N - 1);
        }
    }

    // Fit vertical edges
    for (int j = 0; j < N; ++j) {
        long long total_updates = 0;
        for (int i = 0; i < N - 1; ++i) {
            total_updates += v_cnt[i][j];
        }
        if (total_updates < (N-1) * 5) continue;

        double sum = 0, sq_sum = 0;
        for (int i = 0; i < N - 1; ++i) {
            sum += v_est[i][j];
            sq_sum += v_est[i][j] * v_est[i][j];
        }

        double cost_m1 = sq_sum - sum * sum / (N - 1);
        
        double min_cost_m2 = 1e18;
        int best_split = -1;
        double means[2];

        double current_sum_1 = 0, current_sq_sum_1 = 0;
        for (int i = 1; i < N - 1; ++i) {
            current_sum_1 += v_est[i-1][j];
            current_sq_sum_1 += v_est[i-1][j] * v_est[i-1][j];
            double sum2 = sum - current_sum_1;
            double sq_sum2 = sq_sum - current_sq_sum_1;
            
            double cost1 = current_sq_sum_1 - current_sum_1 * current_sum_1 / i;
            double cost2 = sq_sum2 - sum2 * sum2 / (N - 1 - i);
            if (cost1 + cost2 < min_cost_m2) {
                min_cost_m2 = cost1 + cost2;
                best_split = i;
                means[0] = current_sum_1 / i;
                means[1] = sum2 / (N - 1 - i);
            }
        }
        
        if (min_cost_m2 < cost_m1 * 0.6) {
            is_M2_v[j] = true;
            split_v_cols[j] = best_split;
            V_avg[j][0] = means[0];
            V_avg[j][1] = means[1];
        } else {
            is_M2_v[j] = false;
            V_avg[j][0] = sum / (N - 1);
        }
    }

    // Update model weights
    for (int i = 0; i < N; ++i) {
        if (is_M2_h[i]) {
            for (int j = 0; j < N - 1; ++j) {
                h_model[i][j] = (j < split_h[i]) ? H_avg[i][0] : H_avg[i][1];
            }
        } else {
            for (int j = 0; j < N - 1; ++j) {
                h_model[i][j] = H_avg[i][0];
            }
        }
    }
    for (int j = 0; j < N; ++j) {
        if (is_M2_v[j]) {
            for (int i = 0; i < N - 1; ++i) {
                v_model[i][j] = (i < split_v_cols[j]) ? V_avg[j][0] : V_avg[j][1];
            }
        } else {
            for (int i = 0; i < N - 1; ++i) {
                v_model[i][j] = V_avg[j][0];
            }
        }
    }
}

struct State {
    double cost;
    int r, c;

    bool operator>(const State& other) const {
        return cost > other.cost;
    }
};

std::string solve(int k, int si, int sj, int ti, int tj) {
    double lambda = std::max(0.0, 1.0 - k / 600.0);
    
    double dist[N][N];
    Pos parent[N][N];
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            dist[i][j] = 1e18;
        }
    }

    dist[si][sj] = 0;
    std::priority_queue<State, std::vector<State>, std::greater<State>> pq;
    pq.push({0.0, si, sj});

    while (!pq.empty()) {
        State current = pq.top();
        pq.pop();

        if (current.cost > dist[current.r][current.c]) {
            continue;
        }
        if (current.r == ti && current.c == tj) {
            break;
        }

        int r = current.r;
        int c = current.c;

        // U
        if (r > 0) {
            double weight = lambda * v_est[r-1][c] + (1.0-lambda) * v_model[r-1][c];
            if (dist[r][c] + weight < dist[r-1][c]) {
                dist[r-1][c] = dist[r][c] + weight;
                parent[r-1][c] = {r, c};
                pq.push({dist[r-1][c], r-1, c});
            }
        }
        // D
        if (r < N - 1) {
            double weight = lambda * v_est[r][c] + (1.0-lambda) * v_model[r][c];
            if (dist[r][c] + weight < dist[r+1][c]) {
                dist[r+1][c] = dist[r][c] + weight;
                parent[r+1][c] = {r, c};
                pq.push({dist[r+1][c], r+1, c});
            }
        }
        // L
        if (c > 0) {
            double weight = lambda * h_est[r][c-1] + (1.0-lambda) * h_model[r][c-1];
            if (dist[r][c] + weight < dist[r][c-1]) {
                dist[r][c-1] = dist[r][c] + weight;
                parent[r][c-1] = {r, c};
                pq.push({dist[r][c-1], r, c-1});
            }
        }
        // R
        if (c < N - 1) {
            double weight = lambda * h_est[r][c] + (1.0-lambda) * h_model[r][c];
            if (dist[r][c] + weight < dist[r][c+1]) {
                dist[r][c+1] = dist[r][c] + weight;
                parent[r][c+1] = {r, c};
                pq.push({dist[r][c+1], r, c+1});
            }
        }
    }
    
    std::string path_rev = "";
    int cur_r = ti, cur_c = tj;
    while (cur_r != si || cur_c != sj) {
        Pos p = parent[cur_r][cur_c];
        int pr = p.r;
        int pc = p.c;
        if (cur_r == pr - 1 && cur_c == pc) path_rev += 'U';
        else if (cur_r == pr + 1 && cur_c == pc) path_rev += 'D';
        else if (cur_c == pc - 1 && cur_r == pr) path_rev += 'L';
        else path_rev += 'R';
        cur_r = pr;
        cur_c = pc;
    }
    std::reverse(path_rev.begin(), path_rev.end());
    return path_rev;
}

void update_estimates(int si, int sj, const std::string& path, double actual_path_len) {
    double estimated_path_len = 0;
    int r = si, c = sj;
    for (char move : path) {
        if (move == 'U') {
            estimated_path_len += v_est[r - 1][c];
            r--;
        } else if (move == 'D') {
            estimated_path_len += v_est[r][c];
            r++;
        } else if (move == 'L') {
            estimated_path_len += h_est[r][c - 1];
            c--;
        } else { // R
            estimated_path_len += h_est[r][c];
            c++;
        }
    }

    if (estimated_path_len < 1e-9) return;

    double ratio = actual_path_len / estimated_path_len;
    
    r = si, c = sj;
    for (char move : path) {
        if (move == 'U') {
            double alpha = 1.0 / (v_cnt[r - 1][c] + 10.0);
            v_est[r - 1][c] *= (1.0 - alpha + alpha * ratio);
            v_cnt[r - 1][c]++;
            r--;
        } else if (move == 'D') {
            double alpha = 1.0 / (v_cnt[r][c] + 10.0);
            v_est[r][c] *= (1.0 - alpha + alpha * ratio);
            v_cnt[r][c]++;
            r++;
        } else if (move == 'L') {
            double alpha = 1.0 / (h_cnt[r][c - 1] + 10.0);
            h_est[r][c - 1] *= (1.0 - alpha + alpha * ratio);
            h_cnt[r][c - 1]++;
            c--;
        } else { // R
            double alpha = 1.0 / (h_cnt[r][c] + 10.0);
            h_est[r][c] *= (1.0 - alpha + alpha * ratio);
            h_cnt[r][c]++;
            c++;
        }
    }
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    
    init();

    for (int k = 0; k < N_QUERIES; ++k) {
        if (k > 0 && k % 50 == 0) {
            fit_model();
        }

        int si, sj, ti, tj;
        std::cin >> si >> sj >> ti >> tj;

        std::string path = solve(k, si, sj, ti, tj);

        std::cout << path << std::endl;

        double actual_path_len;
        std::cin >> actual_path_len;
        update_estimates(si, sj, path, actual_path_len);
    }

    return 0;
}