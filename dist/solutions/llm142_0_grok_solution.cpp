#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9 + 5;

struct Edge {
    int to, cap, cost, rev;
};

vector<vector<Edge>> G;
vector<int> h, dist, prevv, preve;

void add_edge(int from, int to, int cap, int cost) {
    G[from].push_back({to, cap, cost, (int)G[to].size()});
    G[to].push_back({from, 0, -cost, (int)G[from].size() - 1});
}

int min_cost_flow(int s, int t, int f) {
    int res = 0;
    h = vector<int>(G.size(), 0);
    prevv = vector<int>(G.size());
    preve = vector<int>(G.size());
    while (f > 0) {
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> q;
        dist = vector<int>(G.size(), INF);
        dist[s] = 0;
        q.push({0, s});
        while (!q.empty()) {
            pair<int, int> p = q.top();
            q.pop();
            int v = p.second;
            if (dist[v] < p.first) continue;
            for (int i = 0; i < G[v].size(); ++i) {
                Edge &e = G[v][i];
                if (e.cap > 0 && dist[e.to] > dist[v] + e.cost + h[v] - h[e.to]) {
                    dist[e.to] = dist[v] + e.cost + h[v] - h[e.to];
                    prevv[e.to] = v;
                    preve[e.to] = i;
                    q.push({dist[e.to], e.to});
                }
            }
        }
        if (dist[t] == INF) {
            return -1;
        }
        for (int v = 0; v < G.size(); ++v) if (dist[v] < INF) h[v] += dist[v];
        int d = f;
        for (int v = t; v != s; v = prevv[v]) {
            d = min(d, G[prevv[v]][preve[v]].cap);
        }
        f -= d;
        res += d * h[t];
        for (int v = t; v != s; v = prevv[v]) {
            Edge &e = G[prevv[v]][preve[v]];
            e.cap -= d;
            G[v][e.rev].cap += d;
        }
    }
    return res;
}

int main() {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> state(n + 2);
    for (int p = 1; p <= n; p++) {
        state[p].resize(m);
        for (int j = 0; j < m; j++) {
            cin >> state[p][j];
        }
    }
    vector<vector<int>> count(n + 2, vector<int>(n + 2, 0));
    for (int p = 1; p <= n; p++) {
        for (int col : state[p]) count[p][col]++;
    }
    int N = n;
    int num_nodes = 2 + N + (N + 1);
    G.resize(num_nodes);
    int S = 0;
    int T = num_nodes - 1;
    int color_start = 1;
    int pole_start = 1 + N;
    for (int c = 1; c <= N; c++) {
        add_edge(S, color_start + c - 1, 1, 0);
    }
    for (int p = 1; p <= N + 1; p++) {
        add_edge(pole_start + p - 1, T, 1, 0);
    }
    for (int c = 1; c <= N; c++) {
        for (int p = 1; p <= N + 1; p++) {
            add_edge(color_start + c - 1, pole_start + p - 1, 1, -count[p][c]);
        }
    }
    int mincost = min_cost_flow(S, T, N);
    vector<int> assign(n + 1);
    for (int c = 1; c <= n; c++) {
        int cn = color_start + c - 1;
        for (auto &e : G[cn]) {
            if (e.to >= pole_start && e.to < pole_start + n + 1 && e.cap == 0) {
                int p = e.to - pole_start + 1;
                assign[c] = p;
                break;
            }
        }
    }
    vector<bool> used(n + 2, false);
    for (int c = 1; c <= n; c++) used[assign[c]] = true;
    int extra_pole = 0;
    for (int p = 1; p <= n + 1; p++) if (!used[p]) extra_pole = p;
    vector<int> pole_color(n + 2, 0);
    for (int c = 1; c <= n; c++) pole_color[assign[c]] = c;
    vector<int> wrong(n + 2, 0);
    for (int p = 1; p <= n + 1; p++) {
        if (pole_color[p] > 0) {
            wrong[p] = state[p].size() - count[p][pole_color[p]];
        }
    }
    vector<pair<int, int>> moves_list;
    bool done = false;
    int safety = 0;
    while (!done && safety++ < 3000000) {
        bool changed = false;
        for (int x = 1; x <= n + 1 && !changed; x++) {
            if (state[x].empty()) continue;
            int col = state[x].back();
            int y = assign[col];
            if (x == y) continue;
            if (state[y].size() <= m - 1 && wrong[y] == 0) {
                moves_list.emplace_back(x, y);
                state[x].pop_back();
                state[y].push_back(col);
                if (pole_color[x] > 0) {
                    if (col != pole_color[x]) wrong[x]--;
                }
                if (pole_color[y] > 0) {
                    if (col != pole_color[y]) wrong[y]++;
                }
                changed = true;
            }
        }
        if (changed) continue;
        for (int x = 1; x <= n + 1 && !changed; x++) {
            if (state[x].empty()) continue;
            int col = state[x].back();
            if (pole_color[x] == 0) continue;
            if (col != pole_color[x] && state[extra_pole].size() <= m - 1) {
                int y = extra_pole;
                moves_list.emplace_back(x, y);
                state[x].pop_back();
                state[y].push_back(col);
                if (pole_color[x] > 0) {
                    if (col != pole_color[x]) wrong[x]--;
                }
                changed = true;
            }
        }
        if (changed) continue;
        for (int x = 1; x <= n + 1 && !changed; x++) {
            if (state[x].empty()) continue;
            int col = state[x].back();
            if (wrong[x] == 0) continue;
            if (pole_color[x] == 0) continue;
            if (col == pole_color[x] && state[extra_pole].size() <= m - 1) {
                int y = extra_pole;
                moves_list.emplace_back(x, y);
                state[x].pop_back();
                state[y].push_back(col);
                changed = true;
            }
        }
        if (changed) continue;
        if (!state[extra_pole].empty()) {
            int col = state[extra_pole].back();
            int y = assign[col];
            if (y != extra_pole && state[y].size() <= m - 1) {
                moves_list.emplace_back(extra_pole, y);
                state[extra_pole].pop_back();
                state[y].push_back(col);
                if (pole_color[y] > 0) {
                    if (col != pole_color[y]) wrong[y]++;
                }
                changed = true;
            }
        }
        if (changed) continue;
        done = (state[extra_pole].empty());
        for (int p = 1; p <= n + 1; p++) {
            if (pole_color[p] > 0) {
                if (wrong[p] != 0 || state[p].size() != m) {
                    done = false;
                    break;
                }
            }
        }
        if (!done && !changed) {
            break;
        }
    }
    cout << moves_list.size() << endl;
    for (auto [x, y] : moves_list) {
        cout << x << " " << y << endl;
    }
    return 0;
}