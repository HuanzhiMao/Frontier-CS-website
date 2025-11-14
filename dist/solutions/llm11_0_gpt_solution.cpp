#include <bits/stdc++.h>
using namespace std;

struct PalindromePath {
    int n, m;
    vector<string> grid;
    int sr, sc, er, ec; // 0-indexed
    int B; // number of blank cells
    vector<vector<int>> idOf;
    vector<pair<int,int>> cellOf;
    vector<array<int,4>> nextId; // nextId[id][dir]
    int startId, exitId;

    // directions: 0=L, 1=R, 2=U, 3=D
    int dx[4] = {0, 0, -1, 1};
    int dy[4] = {-1, 1, 0, 0};
    char ch[4] = {'L', 'R', 'U', 'D'};
    int opp[4] = {1, 0, 3, 2};

    PalindromePath(int n, int m): n(n), m(m) {
        grid.resize(n);
        idOf.assign(n, vector<int>(m, -1));
    }

    bool inb(int r, int c) { return r>=0 && r<n && c>=0 && c<m; }

    // BFS connectivity
    bool check_connectivity() {
        int total = 0;
        for(int i=0;i<n;i++)
            for(int j=0;j<m;j++)
                if(grid[i][j] == '1') total++;
        B = total;
        if(B == 0) return false;
        vector<vector<int>> vis(n, vector<int>(m, 0));
        queue<pair<int,int>> q;
        if(grid[sr][sc] != '1') return false;
        vis[sr][sc] = 1;
        q.push({sr, sc});
        int cnt = 0;
        while(!q.empty()) {
            auto [r,c] = q.front(); q.pop();
            cnt++;
            for(int d=0; d<4; d++) {
                int nr = r + dx[d], nc = c + dy[d];
                if(inb(nr,nc) && !vis[nr][nc] && grid[nr][nc]=='1') {
                    vis[nr][nc]=1;
                    q.push({nr,nc});
                }
            }
        }
        return cnt == total && grid[er][ec]=='1';
    }

    void build_ids() {
        int id=0;
        cellOf.clear();
        for(int i=0;i<n;i++)
            for(int j=0;j<m;j++)
                if(grid[i][j]=='1') {
                    idOf[i][j]=id++;
                    cellOf.push_back({i,j});
                }
        B=id;
        startId = idOf[sr][sc];
        exitId = idOf[er][ec];
    }

    void build_next() {
        nextId.assign(B, {0,0,0,0});
        for(int id=0; id<B; id++) {
            auto [r,c] = cellOf[id];
            for(int d=0; d<4; d++) {
                int nr = r + dx[d], nc = c + dy[d];
                if(inb(nr,nc) && grid[nr][nc]=='1') {
                    nextId[id][d] = idOf[nr][nc];
                } else {
                    nextId[id][d] = id; // stay
                }
            }
        }
    }

    // DFS to build a closed walk returning to start and visiting all nodes (tree edges twice)
    void dfs_build(int r, int c, vector<vector<int>>& vis, const array<int,4>& order, string& out) {
        vis[r][c]=1;
        for(int t=0;t<4;t++) {
            int d = order[t];
            int nr = r + dx[d], nc = c + dy[d];
            if(inb(nr,nc) && grid[nr][nc]=='1' && !vis[nr][nc]) {
                out.push_back(ch[d]);
                dfs_build(nr, nc, vis, order, out);
                out.push_back(ch[opp[d]]);
            }
        }
    }

    int apply_word(const string& s, int idStart) {
        int cur = idStart;
        for(char c: s) {
            int d;
            if(c=='L') d=0;
            else if(c=='R') d=1;
            else if(c=='U') d=2;
            else d=3;
            cur = nextId[cur][d];
        }
        return cur;
    }

    bool find_Q_for_S0(const string& H0, string& Qout) {
        // S0 = H0 + reverse(H0)
        string S0 = H0;
        string revH = H0;
        reverse(revH.begin(), revH.end());
        S0 += revH;

        // Precompute F(x) = apply S0 starting from x
        vector<int> F(B);
        for(int id=0; id<B; id++) {
            F[id] = apply_word(S0, id);
        }

        // Try P of increasing length up to Lmax
        const int Lmax = 11; // adjustable
        vector<int> arr; arr.reserve(Lmax);
        bool found = false;
        string Qres;

        // Precompute early success with Q empty
        int a0 = startId;
        int a_afterQ = a0; // Q empty
        int b = F[a_afterQ];
        int final0 = b; // Q empty applied at end
        if(final0 == exitId) {
            Qout.clear();
            return true;
        }

        function<bool(int,int,const vector<int>&)> dfsP = [&](int depth, int psId, const vector<int>& seq) -> bool {
            if(found) return true;
            if((int)seq.size() == depth) {
                int posA = psId;
                // apply reverse(P) to psId to get a = Q(s)
                for(int i=depth-1; i>=0; --i) {
                    int d = seq[i];
                    posA = nextId[posA][d];
                }
                int b = F[posA];
                // apply Q to b: P then reverse(P)
                int posB = b;
                for(int i=0;i<depth;i++) {
                    int d = seq[i];
                    posB = nextId[posB][d];
                }
                for(int i=depth-1; i>=0; --i) {
                    int d = seq[i];
                    posB = nextId[posB][d];
                }
                if(posB == exitId) {
                    // Build Q string from seq
                    string Pstr; Pstr.reserve(depth);
                    for(int i=0;i<depth;i++) Pstr.push_back(ch[seq[i]]);
                    string Q = Pstr;
                    for(int i=depth-1;i>=0;i--) Q.push_back(ch[seq[i]]);
                    Qres = Q;
                    found = true;
                    return true;
                }
                return false;
            }
            // try extend
            for(int d=0; d<4; d++) {
                int nxt = nextId[psId][d];
                vector<int> seq2 = seq;
                seq2.push_back(d);
                if(dfsP(depth, nxt, seq2)) return true;
                if(found) return true;
            }
            return false;
        };

        for(int len=1; len<=Lmax && !found; len++) {
            vector<int> seq;
            seq.reserve(len);
            // optimize recursion by iterative building
            function<bool(int,int)> rec = [&](int i, int psId)->bool {
                if(found) return true;
                if(i==len) {
                    // Evaluate
                    int posA = psId;
                    for(int j=len-1;j>=0;j--) {
                        int d = arr[j];
                        posA = nextId[posA][d];
                    }
                    int b = F[posA];
                    int posB = b;
                    for(int j=0;j<len;j++) {
                        int d = arr[j];
                        posB = nextId[posB][d];
                    }
                    for(int j=len-1;j>=0;j--) {
                        int d = arr[j];
                        posB = nextId[posB][d];
                    }
                    if(posB == exitId) {
                        string Pstr; Pstr.reserve(len);
                        for(int j=0;j<len;j++) Pstr.push_back(ch[arr[j]]);
                        string Q = Pstr;
                        for(int j=len-1;j>=0;j--) Q.push_back(ch[arr[j]]);
                        Qres = Q;
                        found = true;
                        return true;
                    }
                    return false;
                } else {
                    for(int d=0; d<4; d++) {
                        arr.push_back(d);
                        int nxt = nextId[psId][d];
                        if(rec(i+1, nxt)) return true;
                        arr.pop_back();
                        if(found) return true;
                    }
                    return false;
                }
            };
            rec(0, startId);
        }

        if(found) {
            Qout = Qres;
            return true;
        }
        return false;
    }

    bool solve_and_output() {
        if(!check_connectivity()) {
            cout << -1 << '\n';
            return true;
        }
        build_ids();
        build_next();

        // Prepare different DFS neighbor orders to try
        vector<array<int,4>> orders;
        // base orders: 8 variants (rotations + reversed)
        array<int,4> base = {0,1,2,3};
        for(int k=0;k<4;k++) {
            array<int,4> ord;
            for(int i=0;i<4;i++) ord[i] = base[(i+k)%4];
            orders.push_back(ord);
        }
        array<int,4> baseRev = {3,2,1,0};
        for(int k=0;k<4;k++) {
            array<int,4> ord;
            for(int i=0;i<4;i++) ord[i] = baseRev[(i+k)%4];
            orders.push_back(ord);
        }

        // Additionally, a couple of hand-crafted orders to diversify
        vector<array<int,4>> extra = {
            array<int,4>{0,2,1,3}, // L U R D
            array<int,4>{1,3,0,2}, // R D L U
            array<int,4>{2,0,3,1}, // U L D R
            array<int,4>{3,1,2,0}  // D R U L
        };
        for(auto &o: extra) orders.push_back(o);

        // Try orders
        for(auto &ord : orders) {
            // build H0 via DFS
            vector<vector<int>> vis(n, vector<int>(m, 0));
            string H0;
            dfs_build(sr, sc, vis, ord, H0);

            // Verify coverage from DFS
            int viscnt=0;
            for(int i=0;i<n;i++)
                for(int j=0;j<m;j++)
                    if(grid[i][j]=='1' && vis[i][j]) viscnt++;
            if(viscnt != B) continue; // fallback (shouldn't happen if connected)

            // Try to find Q for this S0
            string Q;
            if(find_Q_for_S0(H0, Q)) {
                // Build S = Q + (H0 + reverse(H0)) + Q
                string S;
                S.reserve(Q.size()*2 + H0.size()*2);

                S += Q;
                S += H0;
                string revH = H0;
                reverse(revH.begin(), revH.end());
                S += revH;
                S += Q;

                if((int)S.size() > 1000000) {
                    // As a fallback, if too long (should not happen), print -1
                    cout << -1 << '\n';
                    return true;
                }
                cout << S << '\n';
                return true;
            }
        }

        // If not found with the determined orders, attempt a few randomized neighbor orders with shorter search
        mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
        for(int attempt=0; attempt<30; attempt++) {
            array<int,4> ord = {0,1,2,3};
            shuffle(ord.begin(), ord.end(), rng);

            vector<vector<int>> vis(n, vector<int>(m, 0));
            string H0;
            dfs_build(sr, sc, vis, ord, H0);
            int viscnt=0;
            for(int i=0;i<n;i++)
                for(int j=0;j<m;j++)
                    if(grid[i][j]=='1' && vis[i][j]) viscnt++;
            if(viscnt != B) continue;

            string Q;
            if(find_Q_for_S0(H0, Q)) {
                string S;
                S.reserve(Q.size()*2 + H0.size()*2);
                S += Q;
                S += H0;
                string revH = H0;
                reverse(revH.begin(), revH.end());
                S += revH;
                S += Q;

                if((int)S.size() > 1000000) {
                    cout << -1 << '\n';
                    return true;
                }
                cout << S << '\n';
                return true;
            }
        }

        // If still not found
        cout << -1 << '\n';
        return true;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    if(!(cin >> n >> m)) return 0;
    PalindromePath solver(n, m);
    for(int i=0;i<n;i++) {
        string s; cin >> s;
        solver.grid[i] = s;
    }
    cin >> solver.sr >> solver.sc >> solver.er >> solver.ec;
    solver.sr--; solver.sc--; solver.er--; solver.ec--;
    solver.solve_and_output();
    return 0;
}