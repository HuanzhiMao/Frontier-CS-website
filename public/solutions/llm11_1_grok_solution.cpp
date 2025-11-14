#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;
    vector<string> grid(n);
    for (int i = 0; i < n; i++) cin >> grid[i];
    int sr, sc, er, ec;
    cin >> sr >> sc >> er >> ec;
    sr--; sc--; er--; ec--;
    vector<vector<int>> cell_id(n, vector<int>(m, -1));
    vector<pair<int, int>> cells;
    vector<vector<bool>> reach(n, vector<bool>(m, false));
    queue<pair<int, int>> q;
    q.push({sr, sc});
    reach[sr][sc] = true;
    while (!q.empty()) {
        auto [x, y] = q.front(); q.pop();
        cells.emplace_back(x, y);
        int dx[] = {0, 0, -1, 1};
        int dy[] = {-1, 1, 0, 0};
        for (int d = 0; d < 4; d++) {
            int nx = x + dx[d], ny = y + dy[d];
            if (nx >= 0 && nx < n && ny >= 0 && ny < m && grid[nx][ny] == '1' && !reach[nx][ny]) {
                reach[nx][ny] = true;
                q.push({nx, ny});
            }
        }
    }
    int total_ones = 0;
    for (int i = 0; i < n; i++) for (int j = 0; j < m; j++) if (grid[i][j] == '1') total_ones++;
    int V = cells.size();
    if (V < total_ones) {
        cout << -1 << endl;
        return 0;
    }
    for (int i = 0; i < V; i++) {
        auto [x, y] = cells[i];
        cell_id[x][y] = i;
    }
    int start_id = cell_id[sr][sc];
    int end_id = cell_id[er][ec];
    auto get_f = [&](int cid, int dir) -> int {
        auto [x, y] = cells[cid];
        int dx[] = {0, 0, -1, 1};
        int dy[] = {-1, 1, 0, 0};
        int nx = x + dx[dir], ny = y + dy[dir];
        if (nx >= 0 && nx < n && ny >= 0 && ny < m && grid[nx][ny] == '1') return cell_id[nx][ny];
        return cid;
    };
    int revd[4] = {1, 0, 3, 2};
    auto get_preds = [&](int cid, int dir) -> vector<int> {
        auto [x, y] = cells[cid];
        int dx[] = {0, 0, -1, 1};
        int dy[] = {-1, 1, 0, 0};
        vector<int> res;
        int rdir = revd[dir];
        int px = x + dx[rdir], py = y + dy[rdir];
        if (px >= 0 && px < n && py >= 0 && py < m && grid[px][py] == '1') res.push_back(cell_id[px][py]);
        int nx = x + dx[dir], ny = y + dy[dir];
        bool is_stay = !(nx >= 0 && nx < n && ny >= 0 && ny < m && grid[nx][ny] == '1');
        if (is_stay) res.push_back(cid);
        return res;
    };
    if (start_id == end_id && V == 1) {
        cout << endl;
        return 0;
    }
    vector<vector<bool>> vstd(V, vector<bool>(V, false));
    vector<vector<pair<int, int>>> prnt(V, vector<pair<int, int>>(V, {-1, -1}));
    vector<vector<char>> ch_d(V, vector<char>(V, 0));
    vector<vector<int>> ch_pr(V, vector<int>(V, -1));
    queue<pair<int, int>> qq;
    qq.push({start_id, end_id});
    vstd[start_id][end_id] = true;
    string MOVES = "LRUD";
    auto build_and_check = [&](pair<int, int> state, char mid, bool is_odd) -> string {
        vector<char> prefix;
        pair<int, int> curr = state;
        while (curr.first != start_id || curr.second != end_id) {
            prefix.push_back(ch_d[curr.first][curr.second]);
            curr = prnt[curr.first][curr.second];
        }
        vector<char> half;
        for (int i = prefix.size() - 1; i >= 0; i--) half.push_back(prefix[i]);
        string S;
        for (char c : half) S += c;
        if (is_odd) S += mid;
        for (int i = half.size() - 1; i >= 0; i--) S += half[i];
        bitset<1024> vis(0);
        int cur_pos = start_id;
        vis[cur_pos] = 1;
        for (char c : S) {
            int d = MOVES.find(c);
            cur_pos = get_f(cur_pos, d);
            vis[cur_pos] = 1;
        }
        if (vis.count() == V && cur_pos == end_id) return S;
        return "";
    };
    string ans = "";
    while (!qq.empty() && ans.empty()) {
        auto [cs, ce] = qq.front(); qq.pop();
        // check even
        if (cs == ce) {
            string cand = build_and_check({cs, ce}, 0, false);
            if (!cand.empty()) {
                ans = cand;
                break;
            }
        }
        // check odd
        for (int k = 0; k < 4; k++) {
            int ns = get_f(cs, k);
            auto preds = get_preds(ce, k);
            for (int pr = 0; pr < preds.size(); pr++) {
                int ne = preds[pr];
                if (ns == ne) {
                    string cand = build_and_check({cs, ce}, MOVES[k], true);
                    if (!cand.empty()) {
                        ans = cand;
                        goto found;
                    }
                }
            }
        }
        // add pairs
        for (int k = 0; k < 4; k++) {
            int ns = get_f(cs, k);
            auto preds = get_preds(ce, k);
            for (int pr = 0; pr < preds.size(); pr++) {
                int ne = preds[pr];
                if (!vstd[ns][ne]) {
                    vstd[ns][ne] = true;
                    prnt[ns][ne] = {cs, ce};
                    ch_d[ns][ne] = MOVES[k];
                    ch_pr[ns][ne] = pr;
                    qq.push({ns, ne});
                }
            }
        }
    }
found:
    if (ans.empty()) cout << -1 << endl;
    else cout << ans << endl;
    return 0;
}