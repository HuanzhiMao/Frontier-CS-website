#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <random>
#include <chrono>
#include <queue>

using namespace std;

const int N = 50;
const int M = 100;

int n_fixed, m_fixed;
vector<vector<int>> initial_grid;
vector<vector<bool>> adj;

struct Point {
    int r, c;
};

struct PointF {
    double r, c;
};

vector<Point> bresenham(Point p1, Point p2) {
    vector<Point> line;
    int r1 = p1.r, c1 = p1.c;
    int r2 = p2.r, c2 = p2.c;
    int dr = abs(r2 - r1);
    int dc = abs(c2 - c1);
    int sr = (r1 < r2) ? 1 : -1;
    int sc = (c1 < c2) ? 1 : -1;
    int err = dr - dc;

    while (true) {
        line.push_back({r1, c1});
        if (r1 == r2 && c1 == c2) break;
        int e2 = 2 * err;
        if (e2 > -dc) {
            err -= dc;
            r1 += sr;
        }
        if (e2 < dr) {
            err += dr;
            c1 += sc;
        }
    }
    return line;
}

bool is_valid(int r, int c) {
    return r >= 0 && r < N && c >= 0 && c < N;
}

bool check_placement(int r, int c, int color, const vector<vector<int>>& grid) {
    const int dr[] = {-1, 1, 0, 0};
    const int dc[] = {0, 0, -1, 1};
    for (int i = 0; i < 4; ++i) {
        int nr = r + dr[i];
        int nc = c + dc[i];
        if (is_valid(nr, nc)) {
            int neighbor_color = grid[nr][nc];
            if (neighbor_color != 0 && neighbor_color != color && !adj[color][neighbor_color]) {
                return false;
            }
        }
    }
    return true;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n_fixed >> m_fixed;
    initial_grid.assign(N, vector<int>(N));
    adj.assign(M + 1, vector<bool>(M + 1, false));

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cin >> initial_grid[i][j];
        }
    }

    const int dr[] = {-1, 1, 0, 0};
    const int dc[] = {0, 0, -1, 1};

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int c1 = initial_grid[i][j];
            if (i == 0 || i == N - 1 || j == 0 || j == N - 1) {
                adj[c1][0] = adj[0][c1] = true;
            }
            for (int k = 0; k < 2; ++k) { // Check down and right
                int ni = i + dr[k];
                int nj = j + dc[k];
                if (is_valid(ni, nj)) {
                    int c2 = initial_grid[ni][nj];
                    if (c1 != c2) {
                        adj[c1][c2] = adj[c2][c1] = true;
                    }
                }
            }
        }
    }

    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    
    vector<PointF> pos(M + 1);
    uniform_real_distribution<double> dist(0.0, (double)N - 1.0);
    for (int i = 1; i <= M; ++i) {
        pos[i] = {dist(rng), dist(rng)};
    }

    double kr = 20.0;
    double ka = 0.01;
    int iterations = 1000;
    
    for (int iter = 0; iter < iterations; ++iter) {
        vector<PointF> forces(M + 1, {0.0, 0.0});
        for (int i = 1; i <= M; ++i) {
            for (int j = i + 1; j <= M; ++j) {
                double dr_ = pos[i].r - pos[j].r;
                double dc_ = pos[i].c - pos[j].c;
                double dist_sq = dr_ * dr_ + dc_ * dc_;
                dist_sq = max(dist_sq, 0.01);
                double dist_ = sqrt(dist_sq);

                double f_repel = kr / dist_sq;
                forces[i].r += f_repel * dr_ / dist_;
                forces[i].c += f_repel * dc_ / dist_;
                forces[j].r -= f_repel * dr_ / dist_;
                forces[j].c -= f_repel * dc_ / dist_;

                if (adj[i][j]) {
                    double f_attract = ka * dist_;
                    forces[i].r -= f_attract * dr_ / dist_;
                    forces[i].c -= f_attract * dc_ / dist_;
                    forces[j].r += f_attract * dr_ / dist_;
                    forces[j].c += f_attract * dc_ / dist_;
                }
            }
        }

        for (int i = 1; i <= M; ++i) {
            pos[i].r += forces[i].r;
            pos[i].c += forces[i].c;
            pos[i].r = max(0.0, min((double)N - 1, pos[i].r));
            pos[i].c = max(0.0, min((double)N - 1, pos[i].c));
        }
    }
    
    vector<Point> base_points(M + 1);
    vector<vector<bool>> occupied(N, vector<bool>(N, false));
    
    vector<int> p(M);
    iota(p.begin(), p.end(), 1);

    for (int i = 1; i <= M; ++i) {
        int r_start = round(pos[i].r);
        int c_start = round(pos[i].c);

        queue<Point> q;
        if(is_valid(r_start, c_start)) q.push({r_start, c_start});
        else q.push({N/2, N/2});
        
        vector<vector<bool>> visited(N, vector<bool>(N, false));
        if(is_valid(r_start, c_start)) visited[r_start][c_start] = true;
        else visited[N/2][N/2] = true;
        
        bool found = false;
        while(!q.empty()){
            Point curr = q.front();
            q.pop();

            if(!occupied[curr.r][curr.c]){
                base_points[i] = curr;
                occupied[curr.r][curr.c] = true;
                found = true;
                break;
            }

            for(int k=0; k<4; ++k){
                int nr = curr.r + dr[k];
                int nc = curr.c + dc[k];
                if(is_valid(nr, nc) && !visited[nr][nc]){
                    visited[nr][nc] = true;
                    q.push({nr, nc});
                }
            }
        }
    }


    vector<vector<int>> final_grid(N, vector<int>(N, 0));
    for (int i = 1; i <= M; ++i) {
        final_grid[base_points[i].r][base_points[i].c] = i;
    }

    struct Edge {
        int u, v;
        double dist;
    };
    vector<Edge> edges;
    for (int i = 1; i <= M; ++i) {
        for (int j = i + 1; j <= M; ++j) {
            if (adj[i][j]) {
                double dr_ = base_points[i].r - base_points[j].r;
                double dc_ = base_points[i].c - base_points[j].c;
                edges.push_back({i, j, sqrt(dr_ * dr_ + dc_ * dc_)});
            }
        }
    }

    sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.dist < b.dist;
    });

    for (const auto& edge : edges) {
        int u = edge.u, v = edge.v;
        vector<Point> path = bresenham(base_points[u], base_points[v]);
        int mid = path.size() / 2;
        
        for (int i = 0; i <= mid; ++i) {
            const auto& p = path[i];
            if (final_grid[p.r][p.c] != 0 && final_grid[p.r][p.c] != u) break;
            if (!check_placement(p.r, p.c, u, final_grid)) break;
            final_grid[p.r][p.c] = u;
        }
        for (size_t i = path.size() - 1; i > mid; --i) {
            const auto& p = path[i];
            if (final_grid[p.r][p.c] != 0 && final_grid[p.r][p.c] != v) break;
            if (!check_placement(p.r, p.c, v, final_grid)) break;
            final_grid[p.r][p.c] = v;
        }
    }

    for (int i = 1; i <= M; ++i) {
        if (adj[i][0]) {
            Point p = base_points[i];
            Point best_b = {-1,-1};
            int min_dist_sq = 1e9;

            for(int j=0; j<N; ++j){
                Point b_pts[] = {{0, j}, {N-1, j}, {j, 0}, {j, N-1}};
                for(const auto& b : b_pts){
                    int dr_ = p.r - b.r;
                    int dc_ = p.c - b.c;
                    int d_sq = dr_*dr_ + dc_*dc_;
                    if(d_sq < min_dist_sq){
                        min_dist_sq = d_sq;
                        best_b = b;
                    }
                }
            }
            vector<Point> path_to_border = bresenham(p, best_b);
            for (const auto& pp : path_to_border) {
                if(final_grid[pp.r][pp.c] != 0 && final_grid[pp.r][pp.c] != i) break;
                if(!check_placement(pp.r, pp.c, i, final_grid)) break;
                final_grid[pp.r][pp.c] = i;
            }
        }
    }

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << final_grid[i][j] << (j == N - 1 ? "" : " ");
        }
        cout << "\n";
    }

    return 0;
}