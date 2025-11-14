#include <bits/stdc++.h>
using namespace std;

int main() {
    const double INF = 1e18;
    double h[30][29], v[29][30];
    for (int i = 0; i < 30; i++) for (int j = 0; j < 29; j++) h[i][j] = 5000.0;
    for (int i = 0; i < 29; i++) for (int j = 0; j < 30; j++) v[i][j] = 5000.0;
    for (int q = 0; q < 1000; q++) {
        int si, sj, ti, tj;
        cin >> si >> sj >> ti >> tj;
        double dist[30][30];
        pair<int, int> pre[30][30];
        for (int i = 0; i < 30; i++) for (int j = 0; j < 30; j++) {
            dist[i][j] = INF;
            pre[i][j] = {-1, -1};
        }
        dist[si][sj] = 0;
        priority_queue<tuple<double, int, int>, vector<tuple<double, int, int>>, greater<tuple<double, int, int>>> pq;
        pq.push({0, si, sj});
        while (!pq.empty()) {
            auto [cost, i, j] = pq.top(); pq.pop();
            if (cost > dist[i][j]) continue;
            if (i > 0) {
                int ni = i - 1, nj = j;
                double w = v[i - 1][j];
                if (dist[ni][nj] > dist[i][j] + w) {
                    dist[ni][nj] = dist[i][j] + w;
                    pre[ni][nj] = {i, j};
                    pq.push({dist[ni][nj], ni, nj});
                }
            }
            if (i < 29) {
                int ni = i + 1, nj = j;
                double w = v[i][j];
                if (dist[ni][nj] > dist[i][j] + w) {
                    dist[ni][nj] = dist[i][j] + w;
                    pre[ni][nj] = {i, j};
                    pq.push({dist[ni][nj], ni, nj});
                }
            }
            if (j > 0) {
                int ni = i, nj = j - 1;
                double w = h[i][j - 1];
                if (dist[ni][nj] > dist[i][j] + w) {
                    dist[ni][nj] = dist[i][j] + w;
                    pre[ni][nj] = {i, j};
                    pq.push({dist[ni][nj], ni, nj});
                }
            }
            if (j < 29) {
                int ni = i, nj = j + 1;
                double w = h[i][j];
                if (dist[ni][nj] > dist[i][j] + w) {
                    dist[ni][nj] = dist[i][j] + w;
                    pre[ni][nj] = {i, j};
                    pq.push({dist[ni][nj], ni, nj});
                }
            }
        }
        vector<pair<int, int>> pos;
        pair<int, int> cur = {ti, tj};
        while (cur.first != -1) {
            pos.push_back(cur);
            cur = pre[cur.first][cur.second];
        }
        reverse(pos.begin(), pos.end());
        string pathstr = "";
        for (size_t k = 0; k + 1 < pos.size(); k++) {
            auto p1 = pos[k], p2 = pos[k + 1];
            if (p2.first == p1.first - 1) pathstr += 'U';
            else if (p2.first == p1.first + 1) pathstr += 'D';
            else if (p2.second == p1.second - 1) pathstr += 'L';
            else if (p2.second == p1.second + 1) pathstr += 'R';
        }
        cout << pathstr << endl;
        cout.flush();
        long long r;
        cin >> r;
        double predist = dist[ti][tj];
        int L = pathstr.length();
        double increment = ((double)r - predist) / L;
        for (size_t k = 0; k + 1 < pos.size(); k++) {
            auto p1 = pos[k], p2 = pos[k + 1];
            int i = p1.first, j = p1.second;
            int ni = p2.first, nj = p2.second;
            if (ni == i && nj == j + 1) {
                h[i][j] += increment;
                h[i][j] = max(100.0, min(9000.0, h[i][j]));
            } else if (ni == i && nj == j - 1) {
                h[i][nj] += increment;
                h[i][nj] = max(100.0, min(9000.0, h[i][nj]));
            } else if (nj == j && ni == i + 1) {
                v[i][j] += increment;
                v[i][j] = max(100.0, min(9000.0, v[i][j]));
            } else if (nj == j && ni == i - 1) {
                v[ni][j] += increment;
                v[ni][j] = max(100.0, min(9000.0, v[ni][j]));
            }
        }
    }
    return 0;
}