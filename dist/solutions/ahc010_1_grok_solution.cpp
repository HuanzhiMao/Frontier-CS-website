#include <bits/stdc++.h>
using namespace std;

const int N = 30;
const int SZ = N * N * 4;
int di[4] = {0, -1, 0, 1};
int dj[-1] = {-1, 0, 1, 0};
int to_arr[8][4] = {
    {1, 0, -1, -1},
    {3, -1, -1, 0},
    {-1, -1, 3, 2},
    {-1, 2, 1, -1},
    {1, 0, 3, 2},
    {3, 2, 1, 0},
    {2, -1, 0, -1},
    {-1, 3, -1, 1}
};
int rot_map[8] = {1, 2, 3, 0, 5, 4, 7, 6};
bool vis[SZ];
int pos_arr[SZ];
int stamp_arr[SZ];

void get_ijd(int id, int& i, int& j, int& d) {
    d = id % 4;
    int temp = id / 4;
    j = temp % N;
    i = temp / N;
}

int main() {
    srand(time(0));
    vector<vector<int>> grid(N, vector<int>(N));
    for (int i = 0; i < N; i++) {
        string line;
        cin >> line;
        for (int j = 0; j < N; j++) {
            grid[i][j] = line[j] - '0';
        }
    }
    vector<vector<int>> rots(N, vector<int>(N));
    vector<vector<int>> best_rots = rots;
    long long best_score = 0;
    auto compute_cycles = [&](const vector<vector<int>>& r) -> vector<int> {
        int t[N][N];
        for (int x = 0; x < N; x++) {
            for (int y = 0; y < N; y++) {
                int e = grid[x][y];
                for (int k = 0; k < r[x][y]; k++) e = rot_map[e];
                t[x][y] = e;
            }
        }
        memset(vis, 0, sizeof(vis));
        vector<int> cl;
        int ts = 0;
        for (int start = 0; start < SZ; start++) {
            if (vis[start]) continue;
            int i, j, d;
            get_ijd(start, i, j, d);
            if (to_arr[t[i][j]][d] == -1) {
                vis[start] = true;
                continue;
            }
            ts++;
            int idx = 0;
            int u = start;
            while (true) {
                if (vis[u]) break;
                vis[u] = true;
                pos_arr[u] = idx;
                stamp_arr[u] = ts;
                idx++;
                int ci, cj, cd;
                get_ijd(u, ci, cj, cd);
                int d2 = to_arr[t[ci][cj]][cd];
                if (d2 == -1) break;
                int ni = ci + di[d2];
                int nj = cj + dj[d2];
                if (ni < 0 || ni >= N || nj < 0 || nj >= N) break;
                int nd = (d2 + 2) % 4;
                int nu = ((ni * N + nj) * 4 + nd);
                u = nu;
                if (stamp_arr[u] == ts) {
                    int clen = idx - pos_arr[u];
                    cl.push_back(clen);
                    break;
                }
            }
        }
        return cl;
    };
    auto get_score = [&](const vector<vector<int>>& r) -> long long {
        vector<int> cl = compute_cycles(r);
        if (cl.size() <= 1) return 0;
        sort(cl.rbegin(), cl.rend());
        return (long long)cl[0] * cl[1];
    };
    for (int trial = 0; trial < 10; trial++) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                rots[i][j] = rand() % 4;
            }
        }
        long long sc = get_score(rots);
        if (sc > best_score) {
            best_score = sc;
            best_rots = rots;
        }
    }
    rots = best_rots;
    long long current_score = best_score;
    for (int iter = 0; iter < 100000; iter++) {
        int i = rand() % N;
        int j = rand() % N;
        int old_r = rots[i][j];
        int new_r = rand() % 4;
        if (new_r == old_r) continue;
        rots[i][j] = new_r;
        long long new_score = get_score(rots);
        if (new_score > current_score) {
            current_score = new_score;
        } else {
            rots[i][j] = old_r;
        }
    }
    string ans = "";
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            ans += '0' + rots[i][j];
        }
    }
    cout << ans << endl;
    return 0;
}