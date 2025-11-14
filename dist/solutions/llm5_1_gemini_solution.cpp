#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

// Fast I/O
void fast_io() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
}

const int MAXN = 500005;
int n, m;
std::vector<int> adj[MAXN];
int a[10];

// For Tarjan's algorithm
int dfn[MAXN], low[MAXN], scc_id[MAXN];
bool on_stack[MAXN];
std::vector<int> st;
int timer, scc_count;
std::vector<std::vector<int>> sccs;

void tarjan(int u) {
    dfn[u] = low[u] = ++timer;
    st.push_back(u);
    on_stack[u] = true;

    for (int v : adj[u]) {
        if (dfn[v] == 0) {
            tarjan(v);
            low[u] = std::min(low[u], low[v]);
        } else if (on_stack[v]) {
            low[u] = std::min(low[u], dfn[v]);
        }
    }

    if (low[u] == dfn[u]) {
        scc_count++;
        std::vector<int> current_scc;
        while (true) {
            int node = st.back();
            st.pop_back();
            on_stack[node] = false;
            scc_id[node] = scc_count;
            current_scc.push_back(node);
            if (node == u) break;
        }
        sccs.push_back(current_scc);
    }
}

// For condensation graph and topological sort
std::vector<int> scc_adj[MAXN];
int scc_in_degree[MAXN];

int main() {
    fast_io();

    std::cin >> n >> m;
    for (int i = 0; i < 10; ++i) {
        std::cin >> a[i];
    }
    for (int i = 0; i < m; ++i) {
        int u, v;
        std::cin >> u >> v;
        adj[u].push_back(v);
    }

    // 1. Find SCCs
    for (int i = 1; i <= n; ++i) {
        if (dfn[i] == 0) {
            tarjan(i);
        }
    }
    // sccs vector is 0-indexed, scc_id is 1-indexed. Let's make sccs 1-indexed for convenience.
    sccs.insert(sccs.begin(), std::vector<int>());

    // 2. Build condensation graph
    for (int u = 1; u <= n; ++u) {
        for (int v : adj[u]) {
            if (scc_id[u] != scc_id[v]) {
                scc_adj[scc_id[u]].push_back(scc_id[v]);
            }
        }
    }
    
    // Remove duplicate edges in condensation graph and count in-degrees
    for (int i = 1; i <= scc_count; ++i) {
        std::sort(scc_adj[i].begin(), scc_adj[i].end());
        scc_adj[i].erase(std::unique(scc_adj[i].begin(), scc_adj[i].end()), scc_adj[i].end());
        for (int v_scc : scc_adj[i]) {
            scc_in_degree[v_scc]++;
        }
    }

    // 3. Topologically sort SCCs (Kahn's algorithm)
    std::vector<int> q;
    for (int i = 1; i <= scc_count; ++i) {
        if (scc_in_degree[i] == 0) {
            q.push_back(i);
        }
    }

    std::vector<int> scc_order;
    int head = 0;
    while(head < q.size()){
        int u_scc = q[head++];
        scc_order.push_back(u_scc);
        for(int v_scc : scc_adj[u_scc]){
            scc_in_degree[v_scc]--;
            if(scc_in_degree[v_scc] == 0){
                q.push_back(v_scc);
            }
        }
    }
    
    // 4. Construct the path
    std::vector<int> path;
    int last_node = -1;
    std::vector<bool> visited(n + 1, false);

    for (int s_id : scc_order) {
        std::vector<int>& current_scc_nodes = sccs[s_id];
        
        int start_node = -1;
        if (last_node == -1) { // First SCC
            start_node = current_scc_nodes[0];
        } else {
            for (int u : adj[last_node]) {
                if (scc_id[u] == s_id) {
                    start_node = u;
                    break;
                }
            }
        }
        
        // This case should not happen given the problem constraints
        if (start_node == -1) {
            start_node = current_scc_nodes[0];
        }

        int current_node = start_node;
        for (size_t i = 0; i < current_scc_nodes.size(); ++i) {
            path.push_back(current_node);
            visited[current_node] = true;
            last_node = current_node;

            int next_node = -1;
            for (int v : adj[current_node]) {
                if (scc_id[v] == s_id && !visited[v]) {
                    next_node = v;
                    break;
                }
            }
            if (next_node != -1) {
                current_node = next_node;
            }
        }
    }

    // Output
    std::cout << path.size() << "\n";
    for (size_t i = 0; i < path.size(); ++i) {
        std::cout << path[i] << (i == path.size() - 1 ? "" : " ");
    }
    std::cout << "\n";

    return 0;
}