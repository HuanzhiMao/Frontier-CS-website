#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <numeric>

using namespace std;

const int N = 30;
const int N_QUERIES = 1000;

double h_est[N][N - 1];
double v_est[N - 1][N];
long long h_count[N][N - 1];
long long v_count[N - 1][N];

struct State {
    double dist;
    int r, c;

    bool operator>(const State& other) const {
        return dist > other.dist;
    }
};

struct Point {
    int r, c;
};

string find_path(int si, int sj, int ti, int tj) {
    vector<vector<double>> dist(N, vector<double>(N, 1e18));
    vector<vector<Point>> parent(N, vector<Point>(N, {-1, -1}));
    priority_queue<State, vector<State>, greater<State>> pq;

    dist[si][sj] = 0;
    pq.push({0.0, si, sj});

    while (!pq.empty()) {
        State current = pq.top();
        pq.pop();

        int r = current.r;
        int c = current.c;

        if (current.dist > dist[r][c]) {
            continue;
        }

        if (r == ti && c == tj) {
            break;
        }

        // U
        if (r > 0) {
            int nr = r - 1;
            double edge_cost = v_est[nr][c];
            if (dist[r][c] + edge_cost < dist[nr][c]) {
                dist[nr][c] = dist[r][c] + edge_cost;
                parent[nr][c] = {r, c};
                pq.push({dist[nr][c], nr, c});
            }
        }
        // D
        if (r < N - 1) {
            int nr = r + 1;
            double edge_cost = v_est[r][c];
            if (dist[r][c] + edge_cost < dist[nr][c]) {
                dist[nr][c] = dist[r][c] + edge_cost;
                parent[nr][c] = {r, c};
                pq.push({dist[nr][c], nr, c});
            }
        }
        // L
        if (c > 0) {
            int nc = c - 1;
            double edge_cost = h_est[r][nc];
            if (dist[r][c] + edge_cost < dist[r][nc]) {
                dist[r][nc] = dist[r][c] + edge_cost;
                parent[r][nc] = {r, c};
                pq.push({dist[r][nc], r, nc});
            }
        }
        // R
        if (c < N - 1) {
            int nc = c + 1;
            double edge_cost = h_est[r][c];
            if (dist[r][c] + edge_cost < dist[r][nc]) {
                dist[r][nc] = dist[r][c] + edge_cost;
                parent[r][nc] = {r, c};
                pq.push({dist[r][nc], r, nc});
            }
        }
    }

    string path = "";
    int cur_r = ti, cur_c = tj;
    while (cur_r != si || cur_c != sj) {
        Point p = parent[cur_r][cur_c];
        int pr = p.r;
        int pc = p.c;
        if (pr == cur_r - 1) path += 'D';
        else if (pr == cur_r + 1) path += 'U';
        else if (pc == cur_c - 1) path += 'R';
        else path += 'L';
        cur_r = pr;
        cur_c = pc;
    }
    reverse(path.begin(), path.end());
    return path;
}

void update_estimates(int si, int sj, const string& path, long long measured_len) {
    double estimated_len = 0;
    int r = si, c = sj;
    for (char move : path) {
        if (move == 'U') {
            estimated_len += v_est[r - 1][c]; r--;
        } else if (move == 'D') {
            estimated_len += v_est[r][c]; r++;
        } else if (move == 'L') {
            estimated_len += h_est[r][c - 1]; c--;
        } else { // 'R'
            estimated_len += h_est[r][c]; c++;
        }
    }

    if (estimated_len < 1e-9) return;
    double ratio = (double)measured_len / estimated_len;
    
    const double alpha = 0.2;

    r = si; c = sj;
    for (char move : path) {
        if (move == 'U') {
            v_count[r - 1][c]++;
            v_est[r - 1][c] *= (1.0 + alpha * (ratio - 1.0));
            v_est[r - 1][c] = clamp(v_est[r - 1][c], 1000.0, 9000.0);
            r--;
        } else if (move == 'D') {
            v_count[r][c]++;
            v_est[r][c] *= (1.0 + alpha * (ratio - 1.0));
            v_est[r][c] = clamp(v_est[r][c], 1000.0, 9000.0);
            r++;
        } else if (move == 'L') {
            h_count[r][c - 1]++;
            h_est[r][c - 1] *= (1.0 + alpha * (ratio - 1.0));
            h_est[r][c - 1] = clamp(h_est[r][c - 1], 1000.0, 9000.0);
            c--;
        } else { // 'R'
            h_count[r][c]++;
            h_est[r][c] *= (1.0 + alpha * (ratio - 1.0));
            h_est[r][c] = clamp(h_est[r][c], 1000.0, 9000.0);
            c++;
        }
    }
}

void fit_model() {
    // Fit horizontal edges
    for (int i = 0; i < N; ++i) {
        double min_error = 1e18;
        int best_split = -1;

        for (int x = 0; x < N; ++x) { // x is the start of the second partition
            double sum_w0 = 0, sum_wh0 = 0, sum_wsq0 = 0;
            double sum_w1 = 0, sum_wh1 = 0, sum_wsq1 = 0;
            
            bool has_data = false;
            for (int j = 0; j < N - 1; ++j) {
                if (h_count[i][j] == 0) continue;
                has_data = true;
                long long w = h_count[i][j];
                double val = h_est[i][j];
                if (j < x) {
                    sum_w0 += w; sum_wh0 += w * val; sum_wsq0 += w * val * val;
                } else {
                    sum_w1 += w; sum_wh1 += w * val; sum_wsq1 += w * val * val;
                }
            }
            if (!has_data) continue;
            
            double error = 0;
            if (sum_w0 > 0) error += sum_wsq0 - sum_wh0 * sum_wh0 / sum_w0;
            if (sum_w1 > 0) error += sum_wsq1 - sum_wh1 * sum_wh1 / sum_w1;
            
            if (error < min_error) {
                min_error = error;
                best_split = x;
            }
        }
        
        if (best_split != -1) {
            double sum_w0 = 0, sum_wh0 = 0;
            double sum_w1 = 0, sum_wh1 = 0;
            for (int j = 0; j < N - 1; ++j) {
                if (h_count[i][j] == 0) continue;
                long long w = h_count[i][j]; double val = h_est[i][j];
                if (j < best_split) { sum_w0 += w; sum_wh0 += w * val; }
                else { sum_w1 += w; sum_wh1 += w * val; }
            }

            double avg0 = (sum_w0 > 0) ? sum_wh0 / sum_w0 : -1.0;
            double avg1 = (sum_w1 > 0) ? sum_wh1 / sum_w1 : -1.0;
            
            if (avg0 > 0 && avg1 > 0) {
                for (int j = 0; j < N - 1; ++j) h_est[i][j] = (j < best_split) ? avg0 : avg1;
            } else if (avg0 > 0) {
                for (int j = 0; j < N - 1; ++j) h_est[i][j] = avg0;
            } else if (avg1 > 0) {
                for (int j = 0; j < N - 1; ++j) h_est[i][j] = avg1;
            }
        }
    }
    
    // Fit vertical edges
    for (int j = 0; j < N; ++j) {
        double min_error = 1e18;
        int best_split = -1;

        for (int y = 0; y < N; ++y) { // y is the start of the second partition
            double sum_w0 = 0, sum_wh0 = 0, sum_wsq0 = 0;
            double sum_w1 = 0, sum_wh1 = 0, sum_wsq1 = 0;
            
            bool has_data = false;
            for (int i = 0; i < N - 1; ++i) {
                if (v_count[i][j] == 0) continue;
                has_data = true;
                long long w = v_count[i][j]; double val = v_est[i][j];
                if (i < y) {
                    sum_w0 += w; sum_wh0 += w * val; sum_wsq0 += w * val * val;
                } else {
                    sum_w1 += w; sum_wh1 += w * val; sum_wsq1 += w * val * val;
                }
            }
            if(!has_data) continue;

            double error = 0;
            if (sum_w0 > 0) error += sum_wsq0 - sum_wh0 * sum_wh0 / sum_w0;
            if (sum_w1 > 0) error += sum_wsq1 - sum_wh1 * sum_wh1 / sum_w1;

            if (error < min_error) {
                min_error = error;
                best_split = y;
            }
        }

        if (best_split != -1) {
            double sum_w0 = 0, sum_wh0 = 0;
            double sum_w1 = 0, sum_wh1 = 0;
            for (int i = 0; i < N - 1; ++i) {
                if (v_count[i][j] == 0) continue;
                long long w = v_count[i][j]; double val = v_est[i][j];
                if (i < best_split) { sum_w0 += w; sum_wh0 += w * val; }
                else { sum_w1 += w; sum_wh1 += w * val; }
            }
            
            double avg0 = (sum_w0 > 0) ? sum_wh0 / sum_w0 : -1.0;
            double avg1 = (sum_w1 > 0) ? sum_wh1 / sum_w1 : -1.0;

            if (avg0 > 0 && avg1 > 0) {
                for (int i = 0; i < N - 1; ++i) v_est[i][j] = (i < best_split) ? avg0 : avg1;
            } else if (avg0 > 0) {
                for (int i = 0; i < N - 1; ++i) v_est[i][j] = avg0;
            } else if (avg1 > 0) {
                for (int i = 0; i < N - 1; ++i) v_est[i][j] = avg1;
            }
        }
    }
}


void init() {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N - 1; ++j) {
            h_est[i][j] = 5000.0;
            h_count[i][j] = 0;
        }
    }
    for (int i = 0; i < N - 1; ++i) {
        for (int j = 0; j < N; ++j) {
            v_est[i][j] = 5000.0;
            v_count[i][j] = 0;
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    init();

    for (int k = 0; k < N_QUERIES; ++k) {
        int si, sj, ti, tj;
        cin >> si >> sj >> ti >> tj;

        string path = find_path(si, sj, ti, tj);
        cout << path << endl;

        long long measured_len;
        cin >> measured_len;
        
        update_estimates(si, sj, path, measured_len);

        if (k >= 99 && (k + 1) % 50 == 0) {
            fit_model();
        }
    }

    return 0;
}