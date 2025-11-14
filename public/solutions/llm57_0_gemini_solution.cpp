#include <iostream>
#include <vector>
#include <numeric>
#include <queue>
#include <cmath>

void solve() {
    int n;
    std::cin >> n;
    std::vector<std::vector<int>> adj(n + 1);
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        std::cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    std::vector<long long> s(n + 1);
    for (int i = 1; i <= n; ++i) {
        std::cout << "? 1 1 " << i << std::endl;
        std::cin >> s[i];
    }

    std::vector<int> color(n + 1, -1);
    std::vector<int> p0, p1;
    
    std::vector<int> q_vec;
    q_vec.push_back(1);
    color[1] = 0;
    int head = 0;

    while(head < q_vec.size()){
        int u = q_vec[head++];
        if(color[u] == 0) p0.push_back(u);
        else p1.push_back(u);

        for(int v : adj[u]){
            if(color[v] == -1){
                color[v] = 1 - color[u];
                q_vec.push_back(v);
            }
        }
    }

    std::vector<int>* root_partition;
    // s[u] % 2 == (d_R(u)+1)%2
    // For u=1, d_1(1)=0 (color 0).
    // If s[1] is odd, d_R(1) must be even. R is in same partition as 1 (p0).
    // If s[1] is even, d_R(1) must be odd. R is in different partition from 1 (p1).
    if ((s[1] % 2 + 2) % 2 != 0) { // s[1] is odd
        root_partition = &p0;
    } else { // s[1] is even
        root_partition = &p1;
    }

    int root = -1;
    for (int u : *root_partition) {
        if (std::abs(s[u]) == 1) {
            root = u;
            break;
        }
    }

    std::vector<long long> val(n + 1);
    std::vector<int> parent(n + 1, 0);
    std::vector<bool> visited(n + 1, false);
    
    q_vec.clear();
    q_vec.push_back(root);
    visited[root] = true;
    head = 0;

    while(head < q_vec.size()){
        int u = q_vec[head++];
        for(int v : adj[u]){
            if(!visited[v]){
                visited[v] = true;
                parent[v] = u;
                q_vec.push_back(v);
            }
        }
    }

    val[root] = s[root];
    for (size_t i = 0; i < q_vec.size(); ++i) {
        int u = q_vec[i];
        if (u == root) continue;
        val[u] = s[u] - s[parent[u]];
    }

    std::cout << "! ";
    for (int i = 1; i <= n; ++i) {
        std::cout << val[i] << (i == n ? "" : " ");
    }
    std::cout << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int t;
    std::cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}