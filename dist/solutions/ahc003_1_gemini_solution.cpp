#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <random>

using namespace std;

const int N = 30;
const int QUERIES = 1000;

// Hyperparameters
const double ALPHA = 0.4;
const double BETA = 0.3;
const int REFINE_INTERVAL = 25;
const double MIN_W = 1000.0;
const double MAX_W = 9000.0;
const double INITIAL_NOISE = 0.2;

// Estimated edge weights
double h_est[N][N - 1];
double v_est[N - 1][N];

// For Dijkstra
struct State {
    double dist;
    int r, c;

    bool operator>(const State& other) const {
        return dist > other.dist;
    }
};

double dist[N][N];
pair<int, int> parent[N][N];

// Randomness
mt19937 rng(42);

void initialize() {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N - 1; ++j) {
            h_est[i][j] = 5000.0;
        }
    }
    for (int i = 0; i < N - 1; ++i) {
        for (int j = 0; j < N; ++j) {
            v_est[i][j] = 5000.0;
        }
    }
}

string solve_path(int si, int sj, int ti, int tj, int k) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            dist[i][j] = 1e18;
            parent[i][j] = {-1, -1};
        }
    }

    priority_queue<State, vector<State>, greater<State>> pq;

    dist[si][sj] = 0;
    pq.push({0.0, si, sj});
    
    double noise_level = INITIAL_NOISE * (QUERIES - k) / QUERIES;
    uniform_real_distribution<double> unif(-noise_level, noise_level);

    while (!pq.empty()) {
        State current = pq.top();
        pq.pop();

        double d = current.dist;
        int r = current.r;
        int c = current.c;

        if (d > dist[r][c]) {
            continue;
        }
        if (r == ti && c == tj) break;

        // Up
        if (r > 0) {
            double weight = v_est[r - 1][c] * (1.0 + unif(rng));
            if (dist[r - 1][c] > d + weight) {
                dist[r - 1][c] = d + weight;
                parent[r - 1][c] = {r, c};
                pq.push({dist[r - 1][c], r - 1, c});
            }
        }
        // Down
        if (r < N - 1) {
            double weight = v_est[r][c] * (1.0 + unif(rng));
            if (dist[r + 1][c] > d + weight) {
                dist[r + 1][c] = d + weight;
                parent[r + 1][c] = {r, c};
                pq.push({dist[r + 1][c], r + 1, c});
            }
        }
        // Left
        if (c > 0) {
            double weight = h_est[r][c - 1] * (1.0 + unif(rng));
            if (dist[r][c - 1] > d + weight) {
                dist[r][c - 1] = d + weight;
                parent[r][c - 1] = {r, c};
                pq.push({dist[r][c - 1], r, c - 1});
            }
        }
        // Right
        if (c < N - 1) {
            double weight = h_est[r][c] * (1.0 + unif(rng));
            if (dist[r][c + 1] > d + weight) {
                dist[r][c + 1] = d + weight;
                parent[r][c + 1] = {r, c};
                pq.push({dist[r][c + 1], r, c + 1});
            }
        }
    }

    string path_str = "";
    int cur_r = ti, cur_c = tj;
    while (cur_r != si || cur_c != sj) {
        int pr = parent[cur_r][cur_c].first;
        int pc = parent[cur_r][cur_c].second;
        if (pr == -1) break;
        if (cur_r == pr + 1) path_str += 'D';
        else if (cur_r == pr - 1) path_str += 'U';
        else if (cur_c == pc + 1) path_str += 'R';
        else if (cur_c == pc - 1) path_str += 'L';
        cur_r = pr;
        cur_c = pc;
    }
    reverse(path_str.begin(), path_str.end());
    return path_str;
}

void update_estimates(int si, int sj, const string& path, long long measured_len) {
    double b_est = 0;
    int r = si, c = sj;
    for (char move : path) {
        if (move == 'U') {
            b_est += v_est[r - 1][c]; r--;
        } else if (move == 'D') {
            b_est += v_est[r][c]; r++;
        } else if (move == 'L') {
            b_est += h_est[r][c - 1]; c--;
        } else if (move == 'R') {
            b_est += h_est[r][c]; c++;
        }
    }

    if (b_est <= 0) return;
    
    double ratio = (double)measured_len / b_est;
    double factor = (ratio - 1) * ALPHA + 1;

    r = si; c = sj;
    for (char move : path) {
        if (move == 'U') {
            v_est[r - 1][c] = max(MIN_W, min(MAX_W, v_est[r - 1][c] * factor)); r--;
        } else if (move == 'D') {
            v_est[r][c] = max(MIN_W, min(MAX_W, v_est[r][c] * factor)); r++;
        } else if (move == 'L') {
            h_est[r][c - 1] = max(MIN_W, min(MAX_W, h_est[r][c - 1] * factor)); c--;
        } else if (move == 'R') {
            h_est[r][c] = max(MIN_W, min(MAX_W, h_est[r][c] * factor)); c++;
        }
    }
}

void refine_estimates() {
    // Refine horizontal edges
    double new_h_est[N][N - 1];
    for (int i = 0; i < N; ++i) {
        vector<double> vals(N - 1);
        for(int j=0; j<N-1; ++j) vals[j] = h_est[i][j];

        vector<double> pref_sum(N, 0.0), pref_sum_sq(N, 0.0);
        for (int j = 0; j < N - 1; ++j) {
            pref_sum[j + 1] = pref_sum[j] + vals[j];
            pref_sum_sq[j + 1] = pref_sum_sq[j] + vals[j] * vals[j];
        }

        double min_total_var = 1e18;
        int best_split = 1;

        for (int x = 1; x < N - 1; ++x) {
            double sum1 = pref_sum[x];
            double sum_sq1 = pref_sum_sq[x];
            int count1 = x;
            double var1 = sum_sq1 - sum1 * sum1 / count1;

            double sum2 = pref_sum[N - 1] - pref_sum[x];
            double sum_sq2 = pref_sum_sq[N - 1] - pref_sum_sq[x];
            int count2 = (N - 1) - x;
            double var2 = sum_sq2 - sum2 * sum2 / count2;

            if (var1 + var2 < min_total_var) {
                min_total_var = var1 + var2;
                best_split = x;
            }
        }

        double mean0 = (pref_sum[best_split]) / best_split;
        double mean1 = (pref_sum[N - 1] - pref_sum[best_split]) / ((N - 1) - best_split);
        
        for (int j = 0; j < N - 1; ++j) {
            double mean = (j < best_split) ? mean0 : mean1;
            new_h_est[i][j] = (1.0 - BETA) * h_est[i][j] + BETA * mean;
            new_h_est[i][j] = max(MIN_W, min(MAX_W, new_h_est[i][j]));
        }
    }
    for(int i=0; i<N; ++i) for(int j=0; j<N-1; ++j) h_est[i][j] = new_h_est[i][j];

    // Refine vertical edges
    double new_v_est[N - 1][N];
    for (int j = 0; j < N; ++j) {
        vector<double> vals(N-1);
        for(int i=0; i<N-1; ++i) vals[i] = v_est[i][j];

        vector<double> pref_sum(N, 0.0), pref_sum_sq(N, 0.0);
        for (int i = 0; i < N - 1; ++i) {
            pref_sum[i + 1] = pref_sum[i] + vals[i];
            pref_sum_sq[i + 1] = pref_sum_sq[i] + vals[i] * vals[i];
        }

        double min_total_var = 1e18;
        int best_split = 1;

        for (int y = 1; y < N - 1; ++y) {
            double sum1 = pref_sum[y];
            double sum_sq1 = pref_sum_sq[y];
            int count1 = y;
            double var1 = sum_sq1 - sum1 * sum1 / count1;

            double sum2 = pref_sum[N - 1] - pref_sum[y];
            double sum_sq2 = pref_sum_sq[N - 1] - pref_sum_sq[y];
            int count2 = (N-1) - y;
            double var2 = sum_sq2 - sum2 * sum2 / count2;

            if (var1 + var2 < min_total_var) {
                min_total_var = var1 + var2;
                best_split = y;
            }
        }

        double mean0 = (pref_sum[best_split]) / best_split;
        double mean1 = (pref_sum[N - 1] - pref_sum[best_split]) / ((N - 1) - best_split);

        for (int i = 0; i < N - 1; ++i) {
            double mean = (i < best_split) ? mean0 : mean1;
            new_v_est[i][j] = (1.0 - BETA) * v_est[i][j] + BETA * mean;
            new_v_est[i][j] = max(MIN_W, min(MAX_W, new_v_est[i][j]));
        }
    }
    for(int i=0; i<N-1; ++i) for(int j=0; j<N; ++j) v_est[i][j] = new_v_est[i][j];
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    initialize();

    for (int k = 0; k < QUERIES; ++k) {
        int si, sj, ti, tj;
        cin >> si >> sj >> ti >> tj;

        string path = solve_path(si, sj, ti, tj, k);
        cout << path << endl;

        long long measured_len;
        cin >> measured_len;

        update_estimates(si, sj, path, measured_len);

        if ((k + 1) % REFINE_INTERVAL == 0 && k > 0) {
            refine_estimates();
        }
    }

    return 0;
}