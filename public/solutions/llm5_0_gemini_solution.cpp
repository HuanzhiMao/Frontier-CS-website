#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>

const int MAXN = 500005;
std::vector<int> adj[MAXN], rev_adj[MAXN];
bool visited[MAXN];
std::stack<int> order;
int scc_id[MAXN];
int n, m;

void dfs1(int u) {
    visited[u] = true;
    for (int v : adj[u]) {
        if (!visited[v]) {
            dfs1(v);
        }
    }
    order.push(u);
}

void dfs2(int u, int current_scc) {
    scc_id[u] = current_scc;
    for (int v : rev_adj[u]) {
        if (scc_id[v] == -1) {
            dfs2(v, current_scc);
        }
    }
}

void find_sccs() {
    for (int i = 1; i <= n; ++i) {
        visited[i] = false;
        scc_id[i] = -1;
    }
    for (int i = 1; i <= n; ++i) {
        if (!visited[i]) {
            dfs1(i);
        }
    }
    int current_scc = 0;
    while (!order.empty()) {
        int u = order.top();
        order.pop();
        if (scc_id[u] == -1) {
            dfs2(u, current_scc++);
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::cin >> n >> m;
    for (int i = 0; i < 10; ++i) {
        int a;
        std::cin >> a;
    }

    for (int i = 0; i < m; ++i) {
        int u, v;
        std::cin >> u >> v;
        adj[u].push_back(v);
        rev_adj[v].push_back(u);
    }
    
    find_sccs();
    
    int max_scc_id = -1;
    for (int i = 1; i <= n; ++i) {
        if (scc_id[i] > max_scc_id) {
            max_scc_id = scc_id[i];
        }
    }
    
    int start_node = -1;
    for (int i = 1; i <= n; ++i) {
        if (scc_id[i] == max_scc_id) {
            if (start_node == -1 || i < start_node) {
                start_node = i;
            }
        }
    }
    
    for (int i = 1; i <= n; ++i) {
        visited[i] = false;
    }
    
    std::vector<int> path;
    int current_node = start_node;
    while (current_node != -1) {
        visited[current_node] = true;
        path.push_back(current_node);

        int best_neighbor = -1;
        
        // Prioritize staying in the same SCC
        int min_id_same_scc = -1;
        for (int v : adj[current_node]) {
            if (!visited[v] && scc_id[v] == scc_id[current_node]) {
                if (min_id_same_scc == -1 || v < min_id_same_scc) {
                    min_id_same_scc = v;
                }
            }
        }
        
        if (min_id_same_scc != -1) {
            best_neighbor = min_id_same_scc;
        } else {
            // Move to a new SCC, choosing the "next" one in topological order
            int best_next_scc_id = -1;
            int min_id_next_scc = -1;
            for (int v : adj[current_node]) {
                if (!visited[v] && scc_id[v] < scc_id[current_node]) {
                    if (scc_id[v] > best_next_scc_id) {
                        best_next_scc_id = scc_id[v];
                        min_id_next_scc = v;
                    } else if (scc_id[v] == best_next_scc_id) {
                        if (min_id_next_scc == -1 || v < min_id_next_scc) {
                            min_id_next_scc = v;
                        }
                    }
                }
            }
            best_neighbor = min_id_next_scc;
        }
        current_node = best_neighbor;
    }

    std::cout << path.size() << "\n";
    for (size_t i = 0; i < path.size(); ++i) {
        std::cout << path[i] << (i == path.size() - 1 ? "" : " ");
    }
    std::cout << "\n";

    return 0;
}