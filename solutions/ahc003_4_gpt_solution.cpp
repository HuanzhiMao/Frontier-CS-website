#include <bits/stdc++.h>
using namespace std;

struct Node {
    double d;
    int idx;
    bool operator<(const Node& other) const {
        return d > other.d;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const int H = 30, W = 30;
    const int N = H * W;

    // Weight estimates for edges
    vector<vector<double>> h(H, vector<double>(W - 1, 6000.0)); // horizontal edges: (i,j)-(i,j+1)
    vector<vector<double>> v(H - 1, vector<double>(W, 6000.0)); // vertical edges: (i,j)-(i+1,j)

    // Usage counts for exploration bonus
    vector<vector<int>> ch(H, vector<int>(W - 1, 0));
    vector<vector<int>> cv(H - 1, vector<int>(W, 0));

    auto idx = [&](int i, int j) { return i * W + j; };
    auto coord = [&](int id) { return pair<int,int>(id / W, id % W); };

    std::mt19937 rng(712367);

    int si, sj, ti, tj;
    for (int q = 0; q < 1000; q++) {
        if (!(cin >> si >> sj >> ti >> tj)) {
            return 0;
        }

        // Exploration factor decreasing over time
        double exploreBase = 200.0 * (1.0 - (double)q / 1200.0);
        if (exploreBase < 10.0) exploreBase = 10.0;

        vector<double> dist(N, 1e100);
        vector<int> prev(N, -1);
        vector<char> prevMove(N, 0);

        priority_queue<Node> pq;

        int s = idx(si, sj);
        int t = idx(ti, tj);
        dist[s] = 0.0;
        pq.push({0.0, s});

        array<int,4> di = {-1, 1, 0, 0};
        array<int,4> dj = {0, 0, -1, 1};
        array<char,4> mv = {'U', 'D', 'L', 'R'};

        // Randomize exploration order a bit to diversify early paths
        array<int,4> order = {0,1,2,3};
        // Shuffle order lightly
        {
            uint32_t r = rng();
            if (r & 1) swap(order[0], order[1]);
            if (r & 2) swap(order[2], order[3]);
            if (r & 4) swap(order[1], order[2]);
        }

        while (!pq.empty()) {
            Node cur = pq.top(); pq.pop();
            if (cur.d != dist[cur.idx]) continue;
            if (cur.idx == t) break;
            auto [ci, cj] = coord(cur.idx);

            for (int kk = 0; kk < 4; kk++) {
                int kdir = order[kk];
                int ni = ci + di[kdir];
                int nj = cj + dj[kdir];
                if (ni < 0 || ni >= H || nj < 0 || nj >= W) continue;

                double w = 0.0;
                if (kdir == 2) { // L: (ci,cj)->(ci,cj-1) uses h[ci][cj-1]
                    int jh = cj - 1;
                    double base = h[ci][jh];
                    int cnt = ch[ci][jh];
                    double bonus = exploreBase / sqrt((double)cnt + 1.0);
                    w = max(1.0, base - bonus);
                } else if (kdir == 3) { // R: (ci,cj)->(ci,cj+1) uses h[ci][cj]
                    int jh = cj;
                    double base = h[ci][jh];
                    int cnt = ch[ci][jh];
                    double bonus = exploreBase / sqrt((double)cnt + 1.0);
                    w = max(1.0, base - bonus);
                } else if (kdir == 0) { // U: (ci,cj)->(ci-1,cj) uses v[ci-1][cj]
                    int ivv = ci - 1;
                    double base = v[ivv][cj];
                    int cnt = cv[ivv][cj];
                    double bonus = exploreBase / sqrt((double)cnt + 1.0);
                    w = max(1.0, base - bonus);
                } else if (kdir == 1) { // D: (ci,cj)->(ci+1,cj) uses v[ci][cj]
                    int ivv = ci;
                    double base = v[ivv][cj];
                    int cnt = cv[ivv][cj];
                    double bonus = exploreBase / sqrt((double)cnt + 1.0);
                    w = max(1.0, base - bonus);
                }

                int nid = idx(ni, nj);
                double nd = cur.d + w;
                if (nd < dist[nid]) {
                    dist[nid] = nd;
                    prev[nid] = cur.idx;
                    prevMove[nid] = mv[kdir];
                    pq.push({nd, nid});
                }
            }
        }

        // Reconstruct path
        string path;
        int cur = t;
        while (cur != s && prev[cur] != -1) {
            path.push_back(prevMove[cur]);
            cur = prev[cur];
        }
        reverse(path.begin(), path.end());

        // As a fallback, if somehow no path was found (shouldn't happen), create Manhattan path
        if (path.empty() && s != t) {
            int ci = si, cj = sj;
            while (ci < ti) { path.push_back('D'); ci++; }
            while (ci > ti) { path.push_back('U'); ci--; }
            while (cj < tj) { path.push_back('R'); cj++; }
            while (cj > tj) { path.push_back('L'); cj--; }
        }

        cout << path << "\n";
        cout.flush();

        int observed;
        if (!(cin >> observed)) {
            return 0;
        }

        // Update weights along the path based on observed length
        // Compute predicted sum (without exploration bonuses)
        double pred = 0.0;
        int ci = si, cj = sj;
        int M = (int)path.size();
        for (char c : path) {
            if (c == 'L') {
                pred += h[ci][cj - 1];
                cj -= 1;
            } else if (c == 'R') {
                pred += h[ci][cj];
                cj += 1;
            } else if (c == 'U') {
                pred += v[ci - 1][cj];
                ci -= 1;
            } else if (c == 'D') {
                pred += v[ci][cj];
                ci += 1;
            }
        }

        if (M > 0) {
            double error = pred - (double)observed;
            // Learning rate schedule
            double lr;
            if (q < 300) lr = 0.35;
            else if (q < 700) lr = 0.25;
            else lr = 0.15;

            double delta = (lr * error) / M;

            ci = si; cj = sj;
            for (char c : path) {
                if (c == 'L') {
                    double &w = h[ci][cj - 1];
                    w -= delta;
                    if (w < 1000.0) w = 1000.0;
                    if (w > 9000.0) w = 9000.0;
                    ch[ci][cj - 1] += 1;
                    cj -= 1;
                } else if (c == 'R') {
                    double &w = h[ci][cj];
                    w -= delta;
                    if (w < 1000.0) w = 1000.0;
                    if (w > 9000.0) w = 9000.0;
                    ch[ci][cj] += 1;
                    cj += 1;
                } else if (c == 'U') {
                    double &w = v[ci - 1][cj];
                    w -= delta;
                    if (w < 1000.0) w = 1000.0;
                    if (w > 9000.0) w = 9000.0;
                    cv[ci - 1][cj] += 1;
                    ci -= 1;
                } else if (c == 'D') {
                    double &w = v[ci][cj];
                    w -= delta;
                    if (w < 1000.0) w = 1000.0;
                    if (w > 9000.0) w = 9000.0;
                    cv[ci][cj] += 1;
                    ci += 1;
                }
            }
        }
    }

    return 0;
}