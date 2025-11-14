#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout << fixed << setprecision(10);

    const int ROWS = 30;
    const int COLS = 30;
    const int N = ROWS * COLS;
    double h[30][29];
    double v[29][30];
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 29; j++) {
            h[i][j] = 5000.0;
        }
    }
    for (int i = 0; i < 29; i++) {
        for (int j = 0; j < 30; j++) {
            v[i][j] = 5000.0;
        }
    }

    for (int q = 1; q <= 1000; q++) {
        int si, sj, ti, tj;
        cin >> si >> sj >> ti >> tj;
        int sid = si * 30 + sj;
        int tid = ti * 30 + tj;

        vector<double> dist(N, 1e18);
        vector<int> prevv(N, -1);
        dist[sid] = 0.0;
        priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
        pq.push({0.0, sid});

        while (!pq.empty()) {
            auto [cost, u] = pq.top();
            pq.pop();
            if (cost > dist[u]) continue;
            int i = u / 30;
            int j = u % 30;

            // up
            if (i > 0) {
                int vv = u - 30;
                double wc = dist[u] + v[i - 1][j];
                if (wc < dist[vv]) {
                    dist[vv] = wc;
                    prevv[vv] = u;
                    pq.push({wc, vv});
                }
            }
            // down
            if (i < 29) {
                int vv = u + 30;
                double wc = dist[u] + v[i][j];
                if (wc < dist[vv]) {
                    dist[vv] = wc;
                    prevv[vv] = u;
                    pq.push({wc, vv});
                }
            }
            // left
            if (j > 0) {
                int vv = u - 1;
                double wc = dist[u] + h[i][j - 1];
                if (wc < dist[vv]) {
                    dist[vv] = wc;
                    prevv[vv] = u;
                    pq.push({wc, vv});
                }
            }
            // right
            if (j < 29) {
                int vv = u + 1;
                double wc = dist[u] + h[i][j];
                if (wc < dist[vv]) {
                    dist[vv] = wc;
                    prevv[vv] = u;
                    pq.push({wc, vv});
                }
            }
        }

        // reconstruct path
        vector<pair<int, int>> path_vertices;
        int cur = tid;
        while (cur != sid) {
            path_vertices.push_back({cur / 30, cur % 30});
            cur = prevv[cur];
        }
        path_vertices.push_back({si, sj});
        reverse(path_vertices.begin(), path_vertices.end());

        // generate move string
        string moves = "";
        for (size_t k = 0; k < path_vertices.size() - 1; k++) {
            int i1 = path_vertices[k].first;
            int j1 = path_vertices[k].second;
            int i2 = path_vertices[k + 1].first;
            int j2 = path_vertices[k + 1].second;
            if (i1 == i2) {
                if (j2 > j1) {
                    moves += 'R';
                } else {
                    moves += 'L';
                }
            } else {
                if (i2 > i1) {
                    moves += 'D';
                } else {
                    moves += 'U';
                }
            }
        }

        cout << moves << endl;

        long long r;
        cin >> r;

        // compute best
        double best = 0.0;
        for (size_t k = 0; k < path_vertices.size() - 1; k++) {
            int i1 = path_vertices[k].first;
            int j1 = path_vertices[k].second;
            int i2 = path_vertices[k + 1].first;
            int j2 = path_vertices[k + 1].second;
            if (i1 == i2) {
                int jj = min(j1, j2);
                best += h[i1][jj];
            } else {
                int ii = min(i1, i2);
                best += v[ii][j1];
            }
        }

        // update
        int L = path_vertices.size() - 1;
        if (L == 0) continue;
        double lr = 0.5 / sqrt(q);
        double correction = lr * (r - best) / L;

        for (size_t k = 0; k < path_vertices.size() - 1; k++) {
            int i1 = path_vertices[k].first;
            int j1 = path_vertices[k].second;
            int i2 = path_vertices[k + 1].first;
            int j2 = path_vertices[k + 1].second;
            if (i1 == i2) {
                int jj = min(j1, j2);
                h[i1][jj] += correction;
                if (h[i1][jj] < 100.0) h[i1][jj] = 100.0;
            } else {
                int ii = min(i1, i2);
                v[ii][j1] += correction;
                if (v[ii][j1] < 100.0) v[ii][j1] = 100.0;
            }
        }
    }
    return 0;
}