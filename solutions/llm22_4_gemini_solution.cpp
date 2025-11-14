#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>

using namespace std;

const int MAXN = 100005;
vector<int> adj[MAXN];
int parent[MAXN];

void dfs_parent(int u, int p) {
    parent[u] = p;
    for (int v : adj[u]) {
        if (v != p) {
            dfs_parent(v, u);
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;
    for (int i = 2; i <= n; ++i) {
        int p;
        cin >> p;
        adj[p].push_back(i);
        adj[i].push_back(p);
    }

    if (n > 1) {
        dfs_parent(1, 0);
    }

    vector<int> leaves;
    for (int i = 2; i <= n; ++i) {
        if (adj[i].size() == 1) {
            leaves.push_back(i);
        }
    }
    
    // Per problem statement, deg(1) >= 2 for N >= 4, so 1 is not a leaf.

    int k = leaves.size();
    int K = n + k;

    cout << K << "\n";

    // Print bags
    cout << "1 1\n";
    for (int i = 2; i <= n; ++i) {
        cout << "2 " << i << " " << parent[i] << "\n";
    }
    for (int i = 0; i < k; ++i) {
        cout << "2 " << leaves[i] << " " << leaves[(i + 1) % k] << "\n";
    }

    // Print new tree edges
    for (int i = 2; i <= n; ++i) {
        cout << i << " " << parent[i] << "\n";
    }
    for (int i = 0; i < k - 1; ++i) {
        cout << n + i + 1 << " " << n + i + 2 << "\n";
    }
    for (int i = 0; i < k; ++i) {
        cout << leaves[i] << " " << n + i + 1 << "\n";
    }

    return 0;
}