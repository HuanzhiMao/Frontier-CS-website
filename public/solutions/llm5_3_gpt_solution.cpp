#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    if (!(cin >> n >> m)) return 0;
    int a[10];
    for (int i = 0; i < 10; ++i) cin >> a[i]; // scoring parameters, unused
    
    vector<int> U(m), V(m);
    vector<int> indeg(n + 1, 0), outdeg(n + 1, 0);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        U[i] = u;
        V[i] = v;
        outdeg[u]++;
        indeg[v]++;
    }
    
    vector<vector<int>> adj(n + 1), rev(n + 1);
    for (int i = 1; i <= n; ++i) {
        adj[i].reserve(outdeg[i]);
        rev[i].reserve(indeg[i]);
    }
    for (int i = 0; i < m; ++i) {
        int u = U[i], v = V[i];
        adj[u].push_back(v);
        rev[v].push_back(u);
    }
    
    vector<int> order(n);
    for (int i = 0; i < n; ++i) order[i] = i + 1;
    vector<int> pos(n + 1, 0), dp(n + 1, 0), par(n + 1, -1);
    
    mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
    
    vector<int> bestPath;
    int bestLen = 0;
    
    auto start = chrono::steady_clock::now();
    const long long TIME_LIMIT_MS = 950;
    
    int attempts = 0;
    while (true) {
        auto now = chrono::steady_clock::now();
        long long elapsed = chrono::duration_cast<chrono::milliseconds>(now - start).count();
        if (elapsed > TIME_LIMIT_MS) break;
        
        if (attempts == 0) {
            shuffle(order.begin(), order.end(), rng);
        } else if (attempts % 3 == 1 && !bestPath.empty()) {
            // Place bestPath at the beginning, shuffle the rest
            vector<char> inpath(n + 1, 0);
            for (int v : bestPath) inpath[v] = 1;
            vector<int> rest;
            rest.reserve(n - (int)bestPath.size());
            for (int v = 1; v <= n; ++v) if (!inpath[v]) rest.push_back(v);
            shuffle(rest.begin(), rest.end(), rng);
            int idx = 0;
            for (int i = 0; i < (int)bestPath.size(); ++i) order[i] = bestPath[i];
            for (int i = bestPath.size(); i < n; ++i) order[i] = rest[idx++];
        } else {
            shuffle(order.begin(), order.end(), rng);
        }
        
        for (int i = 0; i < n; ++i) pos[order[i]] = i;
        
        int curBestLen = 0, curEnd = -1;
        for (int i = 0; i < n; ++i) {
            int u = order[i];
            int val = 1, parent = -1;
            for (int v : rev[u]) {
                int pv = pos[v];
                if (pv < i) {
                    int cand = dp[v] + 1;
                    if (cand > val) {
                        val = cand;
                        parent = v;
                    }
                }
            }
            dp[u] = val;
            par[u] = parent;
            if (val > curBestLen) {
                curBestLen = val;
                curEnd = u;
            }
        }
        
        if (curBestLen > bestLen) {
            vector<int> path;
            path.reserve(curBestLen);
            int x = curEnd;
            while (x != -1) {
                path.push_back(x);
                x = par[x];
            }
            reverse(path.begin(), path.end());
            bestPath.swap(path);
            bestLen = curBestLen;
        }
        
        attempts++;
    }
    
    if (bestPath.empty()) {
        // Fallback (should not happen as dp ensures length >= 1)
        bestPath.push_back(1);
    }
    
    // Greedy extension at both ends
    vector<char> used(n + 1, 0);
    for (int v : bestPath) used[v] = 1;
    
    // Extend tail with outgoing edges
    while (true) {
        int tail = bestPath.back();
        bool found = false;
        for (int w : adj[tail]) {
            if (!used[w]) {
                bestPath.push_back(w);
                used[w] = 1;
                found = true;
                break;
            }
        }
        if (!found) break;
    }
    
    // Extend head with incoming edges
    vector<int> prefix;
    int headVal = bestPath.front();
    while (true) {
        bool found = false;
        for (int v : rev[headVal]) {
            if (!used[v]) {
                prefix.push_back(v);
                used[v] = 1;
                headVal = v;
                found = true;
                break;
            }
        }
        if (!found) break;
    }
    
    vector<int> finalPath;
    finalPath.reserve(prefix.size() + bestPath.size());
    for (int i = (int)prefix.size() - 1; i >= 0; --i) finalPath.push_back(prefix[i]);
    for (int v : bestPath) finalPath.push_back(v);
    
    cout << finalPath.size() << "\n";
    for (int i = 0; i < (int)finalPath.size(); ++i) {
        if (i) cout << ' ';
        cout << finalPath[i];
    }
    cout << "\n";
    
    return 0;
}