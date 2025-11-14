#include <bits/stdc++.h>
using namespace std;

struct OnlineRidge {
    int F;
    double lambda;
    vector<vector<double>> A;
    vector<double> b;
    vector<double> w;
    OnlineRidge(int F_, double lambda_=1.0) : F(F_), lambda(lambda_) {
        A.assign(F, vector<double>(F, 0.0));
        b.assign(F, 0.0);
        w.assign(F, 6000.0); // initial guess
        for (int i = 0; i < F; ++i) A[i][i] = lambda;
    }
    void add_sample(const vector<double>& f, double y) {
        // A += f f^T, b += f*y
        for (int i = 0; i < F; ++i) {
            if (f[i] == 0.0) continue;
            double fi = f[i];
            for (int j = 0; j < F; ++j) {
                double fj = f[j];
                if (fj != 0.0) A[i][j] += fi * fj;
            }
            b[i] += fi * y;
        }
    }
    void solve() {
        // Solve A w = b using Gaussian elimination with partial pivoting
        int n = F;
        vector<vector<double>> M = A;
        vector<double> y = b;
        vector<int> piv(n);
        iota(piv.begin(), piv.end(), 0);
        for (int col = 0; col < n; ++col) {
            int pivot = col;
            double mx = fabs(M[col][col]);
            for (int r = col + 1; r < n; ++r) {
                double val = fabs(M[r][col]);
                if (val > mx) {
                    mx = val;
                    pivot = r;
                }
            }
            if (pivot != col) {
                swap(M[pivot], M[col]);
                swap(y[pivot], y[col]);
            }
            double diag = M[col][col];
            if (fabs(diag) < 1e-9) diag = (diag >= 0 ? 1e-9 : -1e-9);
            double invdiag = 1.0 / diag;
            for (int j = col; j < n; ++j) M[col][j] *= invdiag;
            y[col] *= invdiag;
            for (int r = 0; r < n; ++r) if (r != col) {
                double factor = M[r][col];
                if (factor == 0.0) continue;
                for (int j = col; j < n; ++j) M[r][j] -= factor * M[col][j];
                y[r] -= factor * y[col];
            }
        }
        w = y;
    }
};

struct Solver {
    static const int N = 30;
    int F = 120;
    int cutC = 15; // columns split at j < 15 vs >= 15
    int cutR = 15; // rows split at i < 15 vs >= 15
    OnlineRidge reg;
    int solve_period = 10;
    int Q0 = 60; // exploration queries
    Solver(): reg(F, 1.0) {}

    inline int H_index(int i, int j) const {
        // horizontal edge at row i between j and j+1
        // left side if j < cutC else right side
        return 2 * i + (j >= cutC ? 1 : 0);
    }
    inline int V_index(int i, int j) const {
        // vertical edge between (i,j) and (i+1,j), index by row i (top index)
        // top side if i < cutR else bottom side
        return 60 + 2 * j + (i >= cutR ? 1 : 0);
    }

    inline double getH(int i, int j) const {
        int idx = H_index(i, j);
        double val = reg.w[idx];
        if (val < 500) val = 500;
        if (val > 20000) val = 20000;
        return val;
    }
    inline double getV(int i, int j) const {
        int idx = V_index(i, j);
        double val = reg.w[idx];
        if (val < 500) val = 500;
        if (val > 20000) val = 20000;
        return val;
    }

    string build_path_via_col(int si, int sj, int ti, int tj, int c) const {
        string path;
        int i = si, j = sj;
        if (j < c) path.append(c - j, 'R'), j = c; else if (j > c) path.append(j - c, 'L'), j = c;
        if (i < ti) path.append(ti - i, 'D'), i = ti; else if (i > ti) path.append(i - ti, 'U'), i = ti;
        if (j < tj) path.append(tj - j, 'R'); else if (j > tj) path.append(j - tj, 'L');
        return path;
    }
    string build_path_via_row(int si, int sj, int ti, int tj, int r) const {
        string path;
        int i = si, j = sj;
        if (i < r) path.append(r - i, 'D'), i = r; else if (i > r) path.append(i - r, 'U'), i = r;
        if (j < tj) path.append(tj - j, 'R'), j = tj; else if (j > tj) path.append(j - tj, 'L'), j = tj;
        if (i < ti) path.append(ti - i, 'D'); else if (i > ti) path.append(i - ti, 'U');
        return path;
    }

    double cost_of_path(int si, int sj, const string& path) const {
        int i = si, j = sj;
        double cost = 0.0;
        for (char c : path) {
            if (c == 'L') {
                // edge between (i, j-1) and (i, j), j-1 is index
                cost += getH(i, j - 1);
                j -= 1;
            } else if (c == 'R') {
                // edge between (i, j) and (i, j+1), j is index
                cost += getH(i, j);
                j += 1;
            } else if (c == 'U') {
                // edge between (i-1, j) and (i, j), i-1 is index
                cost += getV(i - 1, j);
                i -= 1;
            } else if (c == 'D') {
                // edge between (i, j) and (i+1, j), i is index
                cost += getV(i, j);
                i += 1;
            }
        }
        return cost;
    }

    string best_pivot_path(int si, int sj, int ti, int tj) const {
        // scan all columns and rows for minimal predicted cost
        string best;
        double bestCost = 1e100;
        // columns
        for (int c = 0; c < N; ++c) {
            string p = build_path_via_col(si, sj, ti, tj, c);
            double cst = cost_of_path(si, sj, p);
            if (cst < bestCost) {
                bestCost = cst;
                best = p;
            }
        }
        // rows
        for (int r = 0; r < N; ++r) {
            string p = build_path_via_row(si, sj, ti, tj, r);
            double cst = cost_of_path(si, sj, p);
            if (cst < bestCost) {
                bestCost = cst;
                best = p;
            }
        }
        return best;
    }

    string dijkstra_path(int si, int sj, int ti, int tj) const {
        int S = si * N + sj;
        int T = ti * N + tj;
        const double INF = 1e100;
        vector<double> dist(N * N, INF);
        vector<int> prev(N * N, -1);
        vector<char> pmove(N * N, 0);
        using P = pair<double,int>;
        priority_queue<P, vector<P>, greater<P>> pq;
        dist[S] = 0.0;
        pq.emplace(0.0, S);
        auto push = [&](int u_i, int u_j, int v_i, int v_j, double w, char mv, int uid){
            int vid = v_i * N + v_j;
            double nd = dist[uid] + w;
            if (nd < dist[vid]) {
                dist[vid] = nd;
                prev[vid] = uid;
                pmove[vid] = mv;
                pq.emplace(nd, vid);
            }
        };
        while (!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();
            if (d != dist[u]) continue;
            if (u == T) break;
            int ui = u / N, uj = u % N;
            if (ui > 0) push(ui, uj, ui - 1, uj, getV(ui - 1, uj), 'U', u);
            if (ui + 1 < N) push(ui, uj, ui + 1, uj, getV(ui, uj), 'D', u);
            if (uj > 0) push(ui, uj, ui, uj - 1, getH(ui, uj - 1), 'L', u);
            if (uj + 1 < N) push(ui, uj, ui, uj + 1, getH(ui, uj), 'R', u);
        }
        string path;
        if (prev[T] == -1) {
            // fallback to simple Manhattan
            int di = ti - si, dj = tj - sj;
            if (dj > 0) path.append(dj, 'R'); else if (dj < 0) path.append(-dj, 'L');
            if (di > 0) path.append(di, 'D'); else if (di < 0) path.append(-di, 'U');
            return path;
        }
        int cur = T;
        while (cur != S) {
            path.push_back(pmove[cur]);
            cur = prev[cur];
        }
        reverse(path.begin(), path.end());
        return path;
    }

    vector<double> features_from_path(int si, int sj, const string& path) const {
        vector<double> f(F, 0.0);
        int i = si, j = sj;
        for (char c : path) {
            if (c == 'L') {
                int jj = j - 1;
                f[H_index(i, jj)] += 1.0;
                j = jj;
            } else if (c == 'R') {
                int jj = j;
                f[H_index(i, jj)] += 1.0;
                j = jj + 1;
            } else if (c == 'U') {
                int ii = i - 1;
                f[V_index(ii, j)] += 1.0;
                i = ii;
            } else if (c == 'D') {
                int ii = i;
                f[V_index(ii, j)] += 1.0;
                i = ii + 1;
            }
        }
        return f;
    }

    string query(int k, int si, int sj, int ti, int tj) {
        string path;
        int man = abs(si - ti) + abs(sj - tj);
        if (k < Q0) {
            if (k % 2 == 0) {
                int c = (k / 2) % N;
                path = build_path_via_col(si, sj, ti, tj, c);
            } else {
                int r = (k / 2) % N;
                path = build_path_via_row(si, sj, ti, tj, r);
            }
        } else {
            string pd = dijkstra_path(si, sj, ti, tj);
            if ((int)pd.size() > man + 20) {
                string pp = best_pivot_path(si, sj, ti, tj);
                path = pp;
            } else {
                path = pd;
            }
        }
        return path;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    Solver solver;
    const int Q = 1000;
    for (int k = 0; k < Q; ++k) {
        int si, sj, ti, tj;
        if (!(cin >> si >> sj >> ti >> tj)) return 0;
        string path = solver.query(k, si, sj, ti, tj);
        cout << path << '\n' << flush;

        int y;
        if (!(cin >> y)) return 0;

        // update model
        vector<double> f = solver.features_from_path(si, sj, path);
        solver.reg.add_sample(f, (double)y);

        // periodically solve
        if ((k + 1) >= solver.Q0 && ((k + 1 - solver.Q0) % solver.solve_period == 0)) {
            solver.reg.solve();
        }
    }
    return 0;
}