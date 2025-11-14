#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, si, sj;
    if (!(cin >> N >> si >> sj)) return 0;
    vector<string> grid(N);
    for (int i = 0; i < N; ++i) cin >> grid[i];

    vector<vector<char>> road(N, vector<char>(N, 0));
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            road[i][j] = (grid[i][j] != '#');

    vector<vector<char>> vis(N, vector<char>(N, 0));

    // Directions: U, D, L, R
    const int di[4] = {-1, 1, 0, 0};
    const int dj[4] = {0, 0, -1, 1};
    const char dc[4] = {'U', 'D', 'L', 'R'};
    const char rc[4] = {'D', 'U', 'R', 'L'};

    struct Frame {
        int i, j, k, from; // k: next direction index to try, from: direction from parent (-1 for root)
    };

    string ans;
    ans.reserve(N * N * 2);

    if (!road[si][sj]) {
        cout << "\n";
        return 0;
    }

    vector<Frame> st;
    st.push_back({si, sj, 0, -1});
    vis[si][sj] = 1;

    while (!st.empty()) {
        Frame &f = st.back();
        if (f.k < 4) {
            int d = f.k++;
            int ni = f.i + di[d];
            int nj = f.j + dj[d];
            if (0 <= ni && ni < N && 0 <= nj && nj < N && road[ni][nj] && !vis[ni][nj]) {
                ans.push_back(dc[d]);
                vis[ni][nj] = 1;
                st.push_back({ni, nj, 0, d});
            }
        } else {
            if (f.from != -1) ans.push_back(rc[f.from]);
            st.pop_back();
        }
    }

    cout << ans << "\n";
    return 0;
}