#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <iomanip>
#include <set>
#include <tuple>

const int GRID_SIZE = 30;
const int N_QUERIES = 1000;
const double INITIAL_ESTIMATE = 5000.0;
const double MODEL_SELECTION_RATIO = 0.8;

// Estimated edge lengths
double est_h[GRID_SIZE][GRID_SIZE - 1];
double est_v[GRID_SIZE - 1][GRID_SIZE];

// Counts for averaging
double count_h[GRID_SIZE][GRID_SIZE - 1];
double count_v[GRID_SIZE - 1][GRID_SIZE];

// Dijkstra's
long long dist[GRID_SIZE][GRID_SIZE];
std::pair<int, int> parent[GRID_SIZE][GRID_SIZE];

struct State {
    long long d;
    int r, c;

    bool operator>(const State& other) const {
        return d > other.d;
    }
};

void refit_row(int r) {
    std::vector<double> vals(GRID_SIZE - 1);
    std::vector<double> weights(GRID_SIZE - 1);
    for (int j = 0; j < GRID_SIZE - 1; ++j) {
        vals[j] = est_h[r][j];
        weights[j] = count_h[r][j];
    }

    std::vector<double> p_sum_w(GRID_SIZE, 0.0);
    std::vector<double> p_sum_wv(GRID_SIZE, 0.0);
    std::vector<double> p_sum_wv2(GRID_SIZE, 0.0);

    for (int j = 0; j < GRID_SIZE - 1; ++j) {
        p_sum_w[j + 1] = p_sum_w[j] + weights[j];
        p_sum_wv[j + 1] = p_sum_wv[j] + weights[j] * vals[j];
        p_sum_wv2[j + 1] = p_sum_wv2[j] + weights[j] * vals[j] * vals[j];
    }

    auto get_sse = [&](int start, int end) {
        if (start >= end) return 0.0;
        double sw = p_sum_w[end] - p_sum_w[start];
        if (sw < 1e-9) return 0.0;
        double swv = p_sum_wv[end] - p_sum_wv[start];
        double swv2 = p_sum_wv2[end] - p_sum_wv2[start];
        return swv2 - swv * swv / sw;
    };
    
    double sse1 = get_sse(0, GRID_SIZE - 1);

    double best_sse2 = 1e18;
    int best_split = -1;
    for (int j = 1; j < GRID_SIZE - 1; ++j) {
        double current_sse2 = get_sse(0, j) + get_sse(j, GRID_SIZE - 1);
        if (current_sse2 < best_sse2) {
            best_sse2 = current_sse2;
            best_split = j;
        }
    }

    if (best_split != -1 && best_sse2 < sse1 * MODEL_SELECTION_RATIO) {
        double sw0 = p_sum_w[best_split] - p_sum_w[0];
        double mean0 = (sw0 < 1e-9) ? INITIAL_ESTIMATE : (p_sum_wv[best_split] - p_sum_wv[0]) / sw0;
        double sw1 = p_sum_w[GRID_SIZE - 1] - p_sum_w[best_split];
        double mean1 = (sw1 < 1e-9) ? INITIAL_ESTIMATE : (p_sum_wv[GRID_SIZE-1] - p_sum_wv[best_split]) / sw1;

        for (int j = 0; j < best_split; ++j) est_h[r][j] = mean0;
        for (int j = best_split; j < GRID_SIZE - 1; ++j) est_h[r][j] = mean1;
    } else {
        double sw = p_sum_w[GRID_SIZE - 1] - p_sum_w[0];
        double mean = (sw < 1e-9) ? INITIAL_ESTIMATE : (p_sum_wv[GRID_SIZE-1] - p_sum_wv[0]) / sw;
        for (int j = 0; j < GRID_SIZE - 1; ++j) est_h[r][j] = mean;
    }
}

void refit_col(int c) {
    std::vector<double> vals(GRID_SIZE - 1);
    std::vector<double> weights(GRID_SIZE - 1);
    for (int i = 0; i < GRID_SIZE - 1; ++i) {
        vals[i] = est_v[i][c];
        weights[i] = count_v[i][c];
    }
    
    std::vector<double> p_sum_w(GRID_SIZE, 0.0);
    std::vector<double> p_sum_wv(GRID_SIZE, 0.0);
    std::vector<double> p_sum_wv2(GRID_SIZE, 0.0);

    for (int i = 0; i < GRID_SIZE - 1; ++i) {
        p_sum_w[i + 1] = p_sum_w[i] + weights[i];
        p_sum_wv[i + 1] = p_sum_wv[i] + weights[i] * vals[i];
        p_sum_wv2[i + 1] = p_sum_wv2[i] + weights[i] * vals[i] * vals[i];
    }

    auto get_sse = [&](int start, int end) {
        if (start >= end) return 0.0;
        double sw = p_sum_w[end] - p_sum_w[start];
        if (sw < 1e-9) return 0.0;
        double swv = p_sum_wv[end] - p_sum_wv[start];
        double swv2 = p_sum_wv2[end] - p_sum_wv2[start];
        return swv2 - swv * swv / sw;
    };
    
    double sse1 = get_sse(0, GRID_SIZE - 1);

    double best_sse2 = 1e18;
    int best_split = -1;
    for (int i = 1; i < GRID_SIZE - 1; ++i) {
        double current_sse2 = get_sse(0, i) + get_sse(i, GRID_SIZE - 1);
        if (current_sse2 < best_sse2) {
            best_sse2 = current_sse2;
            best_split = i;
        }
    }

    if (best_split != -1 && best_sse2 < sse1 * MODEL_SELECTION_RATIO) {
        double sw0 = p_sum_w[best_split] - p_sum_w[0];
        double mean0 = (sw0 < 1e-9) ? INITIAL_ESTIMATE : (p_sum_wv[best_split] - p_sum_wv[0]) / sw0;
        double sw1 = p_sum_w[GRID_SIZE - 1] - p_sum_w[best_split];
        double mean1 = (sw1 < 1e-9) ? INITIAL_ESTIMATE : (p_sum_wv[GRID_SIZE-1] - p_sum_wv[best_split]) / sw1;
        
        for (int i = 0; i < best_split; ++i) est_v[i][c] = mean0;
        for (int i = best_split; i < GRID_SIZE - 1; ++i) est_v[i][c] = mean1;
    } else {
        double sw = p_sum_w[GRID_SIZE - 1] - p_sum_w[0];
        double mean = (sw < 1e-9) ? INITIAL_ESTIMATE : (p_sum_wv[GRID_SIZE-1] - p_sum_wv[0]) / sw;
        for (int i = 0; i < GRID_SIZE - 1; ++i) est_v[i][c] = mean;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE - 1; ++j) {
            est_h[i][j] = INITIAL_ESTIMATE;
            count_h[i][j] = 1.0;
        }
    }
    for (int i = 0; i < GRID_SIZE - 1; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            est_v[i][j] = INITIAL_ESTIMATE;
            count_v[i][j] = 1.0;
        }
    }

    for (int k = 0; k < N_QUERIES; ++k) {
        int si, sj, ti, tj;
        std::cin >> si >> sj >> ti >> tj;

        for (int i = 0; i < GRID_SIZE; ++i) {
            for (int j = 0; j < GRID_SIZE; ++j) {
                dist[i][j] = -1;
            }
        }

        std::priority_queue<State, std::vector<State>, std::greater<State>> pq;

        dist[si][sj] = 0;
        pq.push({0, si, sj});

        while (!pq.empty()) {
            State current = pq.top();
            pq.pop();

            int r = current.r;
            int c = current.c;

            if (dist[r][c] != -1 && current.d > dist[r][c]) {
                continue;
            }
            if (r == ti && c == tj) break;

            // U
            if (r > 0) {
                int nr = r - 1, nc = c;
                long long new_dist = dist[r][c] + round(est_v[nr][nc]);
                if (dist[nr][nc] == -1 || new_dist < dist[nr][nc]) {
                    dist[nr][nc] = new_dist;
                    parent[nr][nc] = {r, c};
                    pq.push({new_dist, nr, nc});
                }
            }
            // D
            if (r < GRID_SIZE - 1) {
                int nr = r + 1, nc = c;
                long long new_dist = dist[r][c] + round(est_v[r][c]);
                if (dist[nr][nc] == -1 || new_dist < dist[nr][nc]) {
                    dist[nr][nc] = new_dist;
                    parent[nr][nc] = {r, c};
                    pq.push({new_dist, nr, nc});
                }
            }
            // L
            if (c > 0) {
                int nr = r, nc = c - 1;
                long long new_dist = dist[r][c] + round(est_h[r][nc]);
                if (dist[nr][nc] == -1 || new_dist < dist[nr][nc]) {
                    dist[nr][nc] = new_dist;
                    parent[nr][nc] = {r, c};
                    pq.push({new_dist, nr, nc});
                }
            }
            // R
            if (c < GRID_SIZE - 1) {
                int nr = r, nc = c + 1;
                long long new_dist = dist[r][c] + round(est_h[r][c]);
                if (dist[nr][nc] == -1 || new_dist < dist[nr][nc]) {
                    dist[nr][nc] = new_dist;
                    parent[nr][nc] = {r, c};
                    pq.push({new_dist, nr, nc});
                }
            }
        }
        
        std::string path_str = "";
        std::vector<std::tuple<bool, int, int>> path_edges;
        int cur_r = ti, cur_c = tj;

        while (cur_r != si || cur_c != sj) {
            int pr = parent[cur_r][cur_c].first;
            int pc = parent[cur_r][cur_c].second;
            if (cur_r == pr + 1 && cur_c == pc) { // Move D
                path_str += 'D';
                path_edges.emplace_back(false, pr, pc);
            } else if (cur_r == pr - 1 && cur_c == pc) { // Move U
                path_str += 'U';
                path_edges.emplace_back(false, cur_r, pc);
            } else if (cur_c == pc + 1 && cur_r == pr) { // Move R
                path_str += 'R';
                path_edges.emplace_back(true, pr, pc);
            } else { // Move L
                path_str += 'L';
                path_edges.emplace_back(true, pr, cur_c);
            }
            cur_r = pr;
            cur_c = pc;
        }
        std::reverse(path_str.begin(), path_str.end());
        
        std::cout << path_str << std::endl;

        long long measured_len;
        std::cin >> measured_len;
        
        double estimated_len = 0;
        for (const auto& edge : path_edges) {
            bool is_h;
            int r, c;
            std::tie(is_h, r, c) = edge;
            if (is_h) estimated_len += est_h[r][c];
            else estimated_len += est_v[r][c];
        }
        
        if (estimated_len > 1e-9) {
            double ratio = (double)measured_len / estimated_len;
            std::set<int> touched_rows, touched_cols;

            for (const auto& edge : path_edges) {
                bool is_h;
                int r, c;
                std::tie(is_h, r, c) = edge;
                if (is_h) {
                    est_h[r][c] *= (count_h[r][c] + ratio) / (count_h[r][c] + 1.0);
                    count_h[r][c] += 1.0;
                    touched_rows.insert(r);
                } else {
                    est_v[r][c] *= (count_v[r][c] + ratio) / (count_v[r][c] + 1.0);
                    count_v[r][c] += 1.0;
                    touched_cols.insert(c);
                }
            }

            for (int r : touched_rows) refit_row(r);
            for (int c : touched_cols) refit_col(c);
        }
    }

    return 0;
}