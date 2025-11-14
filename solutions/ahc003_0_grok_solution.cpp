#include <bits/stdc++.h>
using namespace std;

const int N = 30;
const int NN = N * N;
const int PARAMS = 180;
const double LAMBDA = 0.1;
const double PRIOR = 5000.0;

int node(int i, int j) { return i * N + j; }

pair<int, int> coords(int u) { return {u / N, u % N}; }

vector<double> mat_vec_mul(const vector<vector<double>>& mat, const vector<double>& vec) {
    int n = mat.size();
    vector<double> res(n, 0.0);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            res[i] += mat[i][j] * vec[j];
        }
    }
    return res;
}

string find_path(const vector<double>& beta, int si, int sj, int ti, int tj) {
    int start = node(si, sj);
    int end = node(ti, tj);
    vector<double> dist(NN, 1e18);
    vector<int> prev(NN, -1);
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
    dist[start] = 0;
    pq.push({0, start});
    vector<pair<int, double>> adj[NN];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int u = node(i, j);
            if (j + 1 < N) {
                int part = (j < 10 ? 0 : j < 20 ? 1 : 2);
                double w = beta[i * 3 + part];
                int v = node(i, j + 1);
                adj[u].emplace_back(v, w);
                adj[v].emplace_back(u, w);
            }
            if (i + 1 < N) {
                int part = (i < 10 ? 0 : i < 20 ? 1 : 2);
                double w = beta[90 + j * 3 + part];
                int v = node(i + 1, j);
                adj[u].emplace_back(v, w);
                adj[v].emplace_back(u, w);
            }
        }
    }
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d > dist[u]) continue;
        for (auto [v, w] : adj[u]) {
            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                prev[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
    vector<int> positions;
    int cur = end;
    while (cur != start) {
        positions.push_back(cur);
        cur = prev[cur];
    }
    positions.push_back(start);
    reverse(positions.begin(), positions.end());
    string path = "";
    for (int k = 0; k < (int)positions.size() - 1; k++) {
        int u = positions[k], vv = positions[k + 1];
        auto [ui, uj] = coords(u);
        auto [vi, vj] = coords(vv);
        if (vi == ui && vj == uj + 1) path += 'R';
        else if (vi == ui && vj == uj - 1) path += 'L';
        else if (vi == ui - 1 && vj == uj) path += 'U';
        else if (vi == ui + 1 && vj == uj) path += 'D';
    }
    return path;
}

int main() {
    vector<vector<double>> inv(PARAMS, vector<double>(PARAMS, 0.0));
    for (int i = 0; i < PARAMS; i++) {
        inv[i][i] = 1.0 / LAMBDA;
    }
    vector<double> xty(PARAMS, 0.0);
    vector<double> beta(PARAMS, PRIOR);
    for (int q = 0; q < 1000; q++) {
        int si, sj, ti, tj;
        cin >> si >> sj >> ti >> tj;
        string path = find_path(beta, si, sj, ti, tj);
        cout << path << endl;
        cout.flush();
        long long r;
        cin >> r;
        // compute x
        vector<double> x(PARAMS, 0.0);
        int ci = si, cj = sj;
        for (char move : path) {
            if (move == 'R') {
                int j = cj;
                int part = (j < 10 ? 0 : j < 20 ? 1 : 2);
                x[ci * 3 + part] += 1.0;
                cj++;
            } else if (move == 'L') {
                int j = cj - 1;
                int part = (j < 10 ? 0 : j < 20 ? 1 : 2);
                x[ci * 3 + part] += 1.0;
                cj--;
            } else if (move == 'D') {
                int ii = ci;
                int part = (ii < 10 ? 0 : ii < 20 ? 1 : 2);
                x[90 + cj * 3 + part] += 1.0;
                ci++;
            } else if (move == 'U') {
                int ii = ci - 1;
                int part = (ii < 10 ? 0 : ii < 20 ? 1 : 2);
                x[90 + cj * 3 + part] += 1.0;
                ci--;
            }
        }
        // update RLS
        vector<double> tmp = mat_vec_mul(inv, x);
        double denom = 1.0;
        for (int i = 0; i < PARAMS; i++) {
            denom += tmp[i] * x[i];
        }
        if (abs(denom) < 1e-9) denom = 1e-9; // avoid division by zero
        vector<double> gain(PARAMS);
        for (int i = 0; i < PARAMS; i++) {
            gain[i] = tmp[i] / denom;
        }
        vector<double> xt_inv(PARAMS, 0.0);
        for (int j = 0; j < PARAMS; j++) {
            for (int i = 0; i < PARAMS; i++) {
                xt_inv[j] += x[i] * inv[i][j];
            }
        }
        for (int i = 0; i < PARAMS; i++) {
            for (int j = 0; j < PARAMS; j++) {
                inv[i][j] -= gain[i] * xt_inv[j];
            }
        }
        for (int i = 0; i < PARAMS; i++) {
            xty[i] += x[i] * (double)r;
        }
        vector<double> rhs(PARAMS);
        for (int i = 0; i < PARAMS; i++) {
            rhs[i] = xty[i] + LAMBDA * PRIOR;
        }
        beta = mat_vec_mul(inv, rhs);
    }
    return 0;
}