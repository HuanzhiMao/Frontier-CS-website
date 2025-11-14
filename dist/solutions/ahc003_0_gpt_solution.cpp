#include <bits/stdc++.h>
using namespace std;

struct Dijkstra {
    static const int H = 30, W = 30, N = H*W;
    vector<double> rowW, colW;
    Dijkstra(const vector<double>& rw, const vector<double>& cw) : rowW(rw), colW(cw) {}

    inline int id(int i, int j) const { return i*W + j; }
    inline pair<int,int> rc(int id) const { return {id / W, id % W}; }

    string solve(int si, int sj, int ti, int tj) {
        const double INF = 1e100;
        vector<double> dist(N, INF);
        vector<int> prev(N, -1);
        vector<char> prevDir(N, 0);
        priority_queue<pair<double,int>, vector<pair<double,int>>, greater<pair<double,int>>> pq;
        int s = id(si, sj), t = id(ti, tj);
        dist[s] = 0.0;
        pq.emplace(0.0, s);

        while (!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();
            if (d != dist[u]) continue;
            if (u == t) break;
            auto [i, j] = rc(u);

            // Up
            if (i > 0) {
                int v = id(i-1, j);
                double w = colW[j];
                if (dist[v] > d + w) {
                    dist[v] = d + w;
                    prev[v] = u;
                    prevDir[v] = 'U';
                    pq.emplace(dist[v], v);
                }
            }
            // Down
            if (i < H-1) {
                int v = id(i+1, j);
                double w = colW[j];
                if (dist[v] > d + w) {
                    dist[v] = d + w;
                    prev[v] = u;
                    prevDir[v] = 'D';
                    pq.emplace(dist[v], v);
                }
            }
            // Left
            if (j > 0) {
                int v = id(i, j-1);
                double w = rowW[i];
                if (dist[v] > d + w) {
                    dist[v] = d + w;
                    prev[v] = u;
                    prevDir[v] = 'L';
                    pq.emplace(dist[v], v);
                }
            }
            // Right
            if (j < W-1) {
                int v = id(i, j+1);
                double w = rowW[i];
                if (dist[v] > d + w) {
                    dist[v] = d + w;
                    prev[v] = u;
                    prevDir[v] = 'R';
                    pq.emplace(dist[v], v);
                }
            }
        }

        // Reconstruct path
        string path;
        int cur = t;
        while (cur != s && cur != -1) {
            path.push_back(prevDir[cur]);
            cur = prev[cur];
        }
        reverse(path.begin(), path.end());
        return path;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const int H = 30, W = 30, DIMS = 60;
    vector<double> rowW(H, 5000.0), colW(W, 5000.0);

    // RLS parameters
    double lambda = 1.0; // forgetting factor (1.0 = no forgetting)
    double initP = 1e6;
    vector<vector<double>> P(DIMS, vector<double>(DIMS, 0.0));
    for (int i = 0; i < DIMS; i++) P[i][i] = initP;

    for (int q = 0; q < 1000; q++) {
        int si, sj, ti, tj;
        if (!(cin >> si >> sj >> ti >> tj)) {
            return 0; // in case of unexpected EOF
        }

        // Compute path using current weights
        Dijkstra dj(rowW, colW);
        string path = dj.solve(si, sj, ti, tj);

        // Output path and flush
        cout << path << '\n' << flush;

        long long feedback_ll;
        if (!(cin >> feedback_ll)) {
            return 0; // in case of unexpected EOF
        }
        double y = (double)feedback_ll;

        // Build feature vector x: counts per row (horizontal moves) and per column (vertical moves)
        vector<double> x(DIMS, 0.0);
        int ci = si, cj = sj;
        for (char c : path) {
            if (c == 'U') {
                // move to (ci-1, cj); vertical edge on column cj
                x[30 + cj] += 1.0;
                ci -= 1;
            } else if (c == 'D') {
                x[30 + cj] += 1.0;
                ci += 1;
            } else if (c == 'L') {
                x[ci] += 1.0;
                cj -= 1;
            } else if (c == 'R') {
                x[ci] += 1.0;
                cj += 1;
            }
        }

        // Predicted length
        double y_pred = 0.0;
        for (int r = 0; r < 30; r++) y_pred += x[r] * rowW[r];
        for (int c = 0; c < 30; c++) y_pred += x[30 + c] * colW[c];

        // RLS update if there was at least some movement
        double normx = 0.0;
        for (int i = 0; i < DIMS; i++) normx += x[i] * x[i];

        if (normx > 0.0) {
            // Compute Px = P * x
            vector<double> Px(DIMS, 0.0);
            for (int i = 0; i < DIMS; i++) {
                double s = 0.0;
                const vector<double>& Pi = P[i];
                for (int j = 0; j < DIMS; j++) s += Pi[j] * x[j];
                Px[i] = s;
            }
            double xTPx = 0.0;
            for (int i = 0; i < DIMS; i++) xTPx += x[i] * Px[i];

            double denom = lambda + xTPx;
            if (denom < 1e-12) denom = 1e-12;

            // Gain K = P x / denom
            vector<double> K(DIMS);
            for (int i = 0; i < DIMS; i++) K[i] = Px[i] / denom;

            // Update weights w = w + K * (y - y_pred)
            double err = y - y_pred;
            for (int i = 0; i < 30; i++) rowW[i] += K[i] * err;
            for (int i = 0; i < 30; i++) colW[i] += K[30 + i] * err;

            // Clamp to plausible range
            for (int i = 0; i < 30; i++) {
                if (rowW[i] < 1000.0) rowW[i] = 1000.0;
                if (rowW[i] > 9000.0) rowW[i] = 9000.0;
            }
            for (int i = 0; i < 30; i++) {
                if (colW[i] < 1000.0) colW[i] = 1000.0;
                if (colW[i] > 9000.0) colW[i] = 9000.0;
            }

            // Update P = (P - K * (x^T P)) / lambda
            // Since x^T P = (P x)^T = Px^T
            // P_new[i][j] = (P[i][j] - K[i] * Px[j]) / lambda
            for (int i = 0; i < DIMS; i++) {
                double Ki = K[i];
                vector<double>& Pi = P[i];
                for (int j = 0; j < DIMS; j++) {
                    Pi[j] = (Pi[j] - Ki * Px[j]) / lambda;
                }
            }
        }
    }

    return 0;
}