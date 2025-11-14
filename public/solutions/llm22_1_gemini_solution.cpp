#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

const int MAXN = 100005;
const int LOGN = 18;

std::vector<int> adj[MAXN];
int parent[MAXN];
int depth[MAXN];
int up[MAXN][LOGN];
int n;

void dfs(int u, int p, int d) {
    parent[u] = p;
    depth[u] = d;
    up[u][0] = p;
    for (int i = 1; i < LOGN; ++i) {
        if (up[u][i-1] != 0) {
            up[u][i] = up[up[u][i-1]][i-1];
        } else {
            up[u][i] = 0;
        }
    }
    for (int v : adj[u]) {
        if (v != p) {
            dfs(v, u, d + 1);
        }
    }
}

int lca(int u, int v) {
    if (depth[u] < depth[v]) {
        std::swap(u, v);
    }
    for (int i = LOGN - 1; i >= 0; --i) {
        if (depth[u] - (1 << i) >= depth[v]) {
            u = up[u][i];
        }
    }
    if (u == v) {
        return u;
    }
    for (int i = LOGN - 1; i >= 0; --i) {
        if (up[u][i] != 0 && up[v][i] != 0 && up[u][i] != up[v][i]) {
            u = up[u][i];
            v = up[v][i];
        }
    }
    return parent[u];
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::cin >> n;

    for (int i = 2; i <= n; ++i) {
        int p;
        std::cin >> p;
        adj[p].push_back(i);
        adj[i].push_back(p);
    }

    dfs(1, 0, 0);

    std::vector<int> leaves;
    // According to problem, N>=4 and deg(1)>=2.
    // Node 1 is not a leaf. Any other node with degree 1 is a leaf.
    for (int i = 1; i <= n; ++i) {
        if (adj[i].size() == 1 && i != 1) {
            leaves.push_back(i);
        }
    }

    int k = leaves.size();
    int K = n + k;
    std::cout << K << "\n";

    // Print bags
    // Bag for new node 1 (corresponds to original node 1)
    std::cout << 1 << " " << 1 << "\n";
    // Bags for new nodes 2..N
    for (int i = 2; i <= n; ++i) {
        std::cout << 2 << " " << i << " " << parent[i] << "\n";
    }

    // Bags for new nodes N+1 to N+k (for ring edges)
    if (k > 0) {
        for (int i = 0; i < k; ++i) {
            int u = leaves[i];
            int v = leaves[(i + 1) % k];
            int ancestor = lca(u, v);
            std::cout << 3 << " " << u << " " << v << " " << ancestor << "\n";
        }
    }

    // Print new tree edges
    // Edges for the backbone tree (isomorphic to T)
    for (int i = 2; i <= n; ++i) {
        std::cout << i << " " << parent[i] << "\n";
    }
    // Edges connecting ring road bags to the backbone
    if (k > 0) {
        for (int i = 0; i < k; ++i) {
            int u = leaves[i];
            int v = leaves[(i + 1) % k];
            int ancestor = lca(u, v);
            std::cout << n + i + 1 << " " << ancestor << "\n";
        }
    }

    return 0;
}