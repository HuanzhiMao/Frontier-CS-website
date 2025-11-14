#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, si, sj;
    if (!(cin >> N >> si >> sj)) return 0;
    vector<string> grid(N);
    for (int i = 0; i < N; ++i) cin >> grid[i];

    auto inb = [&](int x, int y){ return 0 <= x && x < N && 0 <= y && y < N; };
    auto id = [&](int x, int y){ return x * N + y; };

    vector<int> w(N*N, 0), rx(N*N), ry(N*N);
    vector<char> isroad(N*N, 0);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int v = id(i,j);
            rx[v] = i; ry[v] = j;
            if (grid[i][j] != '#') {
                isroad[v] = 1;
                w[v] = grid[i][j] - '0';
            }
        }
    }

    // Build neighbors with preference to lighter destination weight
    const int dx[4] = {-1, 1, 0, 0};
    const int dy[4] = {0, 0, -1, 1};
    const char dc[4] = {'U', 'D', 'L', 'R'};

    struct Nei { int to; char ch; int ww; };
    vector<vector<Nei>> g(N*N);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int u = id(i,j);
            if (!isroad[u]) continue;
            vector<Nei> tmp;
            for (int d = 0; d < 4; ++d) {
                int ni = i + dx[d], nj = j + dy[d];
                if (!inb(ni, nj)) continue;
                int v = id(ni, nj);
                if (!isroad[v]) continue;
                tmp.push_back({v, dc[d], w[v]});
            }
            sort(tmp.begin(), tmp.end(), [&](const Nei &a, const Nei &b){
                if (a.ww != b.ww) return a.ww < b.ww;
                return a.ch < b.ch;
            });
            g[u] = move(tmp);
        }
    }

    int s = id(si, sj);
    vector<char> vis(N*N, 0);
    vector<int> it(N*N, 0);
    string ans;
    vector<int> st;
    st.reserve(N*N);
    st.push_back(s);
    vis[s] = 1;

    auto moveChar = [&](int from, int to)->char{
        int fi = rx[from], fj = ry[from];
        int ti = rx[to], tj = ry[to];
        if (ti == fi - 1 && tj == fj) return 'U';
        if (ti == fi + 1 && tj == fj) return 'D';
        if (ti == fi && tj == fj - 1) return 'L';
        if (ti == fi && tj == fj + 1) return 'R';
        return 'U'; // should not happen
    };

    while (!st.empty()) {
        int u = st.back();
        auto &neis = g[u];
        bool moved = false;
        for (int &k = it[u]; k < (int)neis.size(); ++k) {
            int v = neis[k].to;
            if (!vis[v]) {
                ans.push_back(neis[k].ch);
                st.push_back(v);
                vis[v] = 1;
                ++k; // next time continue from following neighbor
                moved = true;
                break;
            }
        }
        if (!moved) {
            st.pop_back();
            if (!st.empty()) {
                int p = st.back();
                ans.push_back(moveChar(u, p));
            }
        }
    }

    cout << ans << '\n';
    return 0;
}