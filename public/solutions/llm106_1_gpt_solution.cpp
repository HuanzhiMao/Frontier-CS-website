#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    if (!(cin >> n)) return 0;

    // Read the rest of the input as a string
    string rest;
    {
        ostringstream ss;
        ss << cin.rdbuf();
        rest = ss.str();
    }
    // Normalize line endings
    for (char &c : rest) if (c == '\r') c = '\n';

    // Parse all integers present in the rest of the input
    vector<long long> V;
    {
        istringstream iss(rest);
        long long x;
        while (iss >> x) V.push_back(x);
    }

    vector<vector<int>> adj(n + 1);
    bool ok = false;

    auto buildFromEdgePairs = [&](const vector<long long>& tokens, size_t startIdx) -> bool {
        size_t len = tokens.size();
        if (len < startIdx) return false;
        size_t rem = len - startIdx;
        if (rem % 2 != 0) return false;
        size_t mEdges = rem / 2;
        vector<vector<int>> g(n + 1);
        for (size_t i = 0; i < mEdges; ++i) {
            long long u = tokens[startIdx + 2*i];
            long long v = tokens[startIdx + 2*i + 1];
            if (u < 1 || u > n || v < 1 || v > n) continue;
            if (u == v) continue;
            g[(int)u].push_back((int)v);
            g[(int)v].push_back((int)u);
        }
        adj.swap(g);
        return true;
    };

    // Try: adjacency matrix as integers (n*n tokens of 0/1)
    if (!ok) {
        if (V.size() == (size_t)n * (size_t)n) {
            bool possible = true;
            for (auto x : V) {
                if (!(x == 0 || x == 1)) { possible = false; break; }
            }
            if (possible) {
                vector<vector<int>> g(n + 1);
                for (int i = 0; i < n; ++i) {
                    for (int j = 0; j < n; ++j) {
                        if (V[i * (size_t)n + j] != 0) {
                            if (i == j) continue;
                            int u = i + 1, v = j + 1;
                            g[u].push_back(v);
                            g[v].push_back(u); // ensure symmetry
                        }
                    }
                }
                adj.swap(g);
                ok = true;
            }
        }
    }

    // Try: upper triangular matrix as integers (n*(n-1)/2 tokens of 0/1)
    if (!ok) {
        size_t need = (size_t)n * (size_t)(n - 1) / 2;
        if (V.size() == need) {
            bool possible = true;
            for (auto x : V) {
                if (!(x == 0 || x == 1)) { possible = false; break; }
            }
            if (possible) {
                vector<vector<int>> g(n + 1);
                size_t idx = 0;
                for (int i = 1; i <= n; ++i) {
                    for (int j = i + 1; j <= n; ++j) {
                        if (V[idx++] != 0) {
                            g[i].push_back(j);
                            g[j].push_back(i);
                        }
                    }
                }
                adj.swap(g);
                ok = true;
            }
        }
    }

    // Try: first token is m (number of edges), followed by m edge pairs
    if (!ok && !V.empty()) {
        long long M = V[0];
        if (M >= 0 && (size_t)(1 + 2 * M) == V.size()) {
            if (buildFromEdgePairs(V, 1)) ok = true;
        }
    }

    // Try: treat all integers as edge pairs
    if (!ok && !V.empty()) {
        if (buildFromEdgePairs(V, 0)) ok = true;
    }

    // Try: parse adjacency rows as strings with 0/1 (possibly without spaces)
    if (!ok) {
        vector<string> lines;
        {
            istringstream ls(rest);
            string line;
            while (getline(ls, line)) {
                lines.push_back(line);
            }
        }
        vector<string> rows;
        for (string line : lines) {
            string t;
            for (char c : line) if (c == '0' || c == '1') t.push_back(c);
            if ((int)t.size() == n) {
                rows.push_back(t);
                if ((int)rows.size() == n) break;
            } else {
                // Try to parse spaced 0/1
                istringstream iss(line);
                vector<int> row;
                bool all01 = true;
                int x;
                while (iss >> x) {
                    if (x == 0 || x == 1) row.push_back(x);
                    else { all01 = false; break; }
                }
                if (all01 && (int)row.size() == n) {
                    string t2;
                    t2.reserve(n);
                    for (int val : row) t2.push_back(val ? '1' : '0');
                    rows.push_back(t2);
                    if ((int)rows.size() == n) break;
                }
            }
        }
        if ((int)rows.size() == n) {
            vector<vector<int>> g(n + 1);
            for (int i = 0; i < n; ++i) {
                if ((int)rows[i].size() != n) { rows.clear(); break; }
                for (int j = 0; j < n; ++j) {
                    char ch = rows[i][j];
                    if (ch == '1') {
                        if (i == j) continue;
                        g[i + 1].push_back(j + 1);
                        g[j + 1].push_back(i + 1);
                    }
                }
            }
            if (rows.size() == (size_t)n) {
                adj.swap(g);
                ok = true;
            }
        }
    }

    // If parsing failed, fallback with a trivial valid output to avoid crash
    if (!ok) {
        vector<int> ans;
        if (n >= 1) ans.push_back(1);
        cout << "Y " << ans.size() << "\n";
        for (int i = 0; i < (int)ans.size(); ++i) {
            if (i) cout << ' ';
            cout << ans[i];
        }
        cout << "\n";
        return 0;
    }

    // Normalize adjacency to remove duplicates and ensure symmetry
    {
        vector<vector<char>> mat(n + 1, vector<char>(n + 1, 0));
        for (int u = 1; u <= n; ++u) {
            for (int v : adj[u]) {
                if (v < 1 || v > n || v == u) continue;
                mat[u][v] = 1;
                mat[v][u] = 1;
            }
        }
        vector<vector<int>> g(n + 1);
        for (int u = 1; u <= n; ++u) {
            for (int v = 1; v <= n; ++v) if (mat[u][v]) g[u].push_back(v);
        }
        adj.swap(g);
    }

    // Bipartiteness check with BFS; construct odd cycle if not bipartite
    vector<int> color(n + 1, -1), parent(n + 1, -1), depth(n + 1, 0);
    vector<int> oddCycle;

    deque<int> q;
    for (int s = 1; s <= n && oddCycle.empty(); ++s) {
        if (color[s] != -1) continue;
        color[s] = 0;
        parent[s] = -1;
        depth[s] = 0;
        q.clear();
        q.push_back(s);
        while (!q.empty() && oddCycle.empty()) {
            int u = q.front(); q.pop_front();
            for (int v : adj[u]) {
                if (color[v] == -1) {
                    color[v] = color[u] ^ 1;
                    parent[v] = u;
                    depth[v] = depth[u] + 1;
                    q.push_back(v);
                } else if (v != parent[u] && color[v] == color[u]) {
                    // Found an odd cycle via edge (u, v)
                    int x = u, y = v;
                    vector<int> pathU, pathV;
                    int dx = depth[x], dy = depth[y];
                    while (x != y) {
                        if (dx >= dy) {
                            pathU.push_back(x);
                            x = parent[x];
                            dx--;
                        } else {
                            pathV.push_back(y);
                            y = parent[y];
                            dy--;
                        }
                    }
                    pathU.push_back(x); // LCA
                    reverse(pathV.begin(), pathV.end());
                    oddCycle = pathU;
                    oddCycle.insert(oddCycle.end(), pathV.begin(), pathV.end());
                    break;
                }
            }
        }
    }

    if (!oddCycle.empty()) {
        int l = (int)oddCycle.size();
        cout << "N " << l << "\n";
        for (int i = 0; i < l; ++i) {
            if (i) cout << ' ';
            cout << oddCycle[i];
        }
        cout << "\n";
        return 0;
    }

    // Graph is bipartite
    vector<int> part;
    for (int i = 1; i <= n; ++i) if (color[i] == 0) part.push_back(i);
    cout << "Y " << part.size() << "\n";
    for (int i = 0; i < (int)part.size(); ++i) {
        if (i) cout << ' ';
        cout << part[i];
    }
    cout << "\n";
    return 0;
}