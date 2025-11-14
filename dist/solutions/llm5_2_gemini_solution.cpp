#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>

// Fast I/O
void fast_io() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
}

// Graph representation and problem parameters
int n, m;
std::vector<std::vector<int>> adj;
std::vector<int> a;

// Tarjan's algorithm variables for finding SCCs
std::vector<int> ids, low;
std::vector<bool> onStack;
std::stack<int> st;
int timer;
int scc_count;
std::vector<int> scc_id;

// DFS for Tarjan's algorithm
void tarjan_dfs(int u) {
    st.push(u);
    onStack[u] = true;
    ids[u] = low[u] = ++timer;

    for (int v : adj[u]) {
        if (ids[v] == 0) {
            tarjan_dfs(v);
            low[u] = std::min(low[u], low[v]);
        } else if (onStack[v]) {
            low[u] = std::min(low[u], ids[v]);
        }
    }

    if (low[u] == ids[u]) {
        ++scc_count;
        while (true) {
            int node = st.top();
            st.pop();
            onStack[node] = false;
            scc_id[node] = scc_count;
            if (node == u) break;
        }
    }
}

// Main function to run Tarjan's algorithm
void find_sccs() {
    ids.assign(n + 1, 0);
    low.assign(n + 1, 0);
    onStack.assign(n + 1, false);
    scc_id.assign(n + 1, 0);
    timer = 0;
    scc_count = 0;
    while(!st.empty()) st.pop();

    for (int i = 1; i <= n; ++i) {
        if (ids[i] == 0) {
            tarjan_dfs(i);
        }
    }
}

int main() {
    fast_io();

    std::cin >> n >> m;
    a.resize(10);
    for (int i = 0; i < 10; ++i) std::cin >> a[i];

    adj.resize(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v;
        std::cin >> u >> v;
        adj[u].push_back(v);
    }

    // 1. Find SCCs using Tarjan's algorithm.
    find_sccs();
    
    // Tarjan's finds SCCs in reverse topological order.
    // We re-number the SCC IDs to get a forward topological sort order (1, 2, ..., scc_count).
    std::vector<int> scc_id_topo(n + 1);
    for (int i = 1; i <= n; ++i) {
        scc_id_topo[i] = scc_count - scc_id[i] + 1;
    }
    
    // Group vertices by their new topological SCC IDs.
    std::vector<std::vector<int>> sccs(scc_count + 1);
    std::vector<int> scc_size(scc_count + 1, 0);
    for (int i = 1; i <= n; ++i) {
        sccs[scc_id_topo[i]].push_back(i);
        scc_size[scc_id_topo[i]]++;
    }

    // 2. Construct the Hamiltonian path.
    std::vector<int> path;
    path.reserve(n);
    std::vector<bool> visited(n + 1, false);
    
    // Start from the first SCC in topological order (ID 1).
    // The guarantee implies we can start at any vertex in this SCC.
    // A deterministic choice like the minimum ID vertex is simple.
    int current_v = n + 1;
    if (n > 0) {
        if (scc_count > 0) {
            for (int node : sccs[1]) {
                current_v = std::min(current_v, node);
            }
        } else {
            current_v = 1;
        }
    } else {
        std::cout << 0 << "\n\n";
        return 0;
    }
    
    // Traverse through SCCs in topological order.
    for (int sid = 1; sid <= scc_count; ++sid) {
        int nodes_in_scc_visited = 0;
        
        // Traverse within the current SCC.
        while (nodes_in_scc_visited < scc_size[sid]) {
            path.push_back(current_v);
            visited[current_v] = true;
            nodes_in_scc_visited++;
            
            if (nodes_in_scc_visited == scc_size[sid]) break;
            
            // Find the next unvisited vertex in the same SCC.
            // The guarantee implies one must exist and any choice works.
            int next_v_in_scc = -1;
            for (int neighbor : adj[current_v]) {
                if (scc_id_topo[neighbor] == sid && !visited[neighbor]) {
                    next_v_in_scc = neighbor;
                    break;
                }
            }
            current_v = next_v_in_scc;
        }
        
        // After visiting all nodes, find the transition to the next SCC.
        if (sid < scc_count) {
            int next_sid = sid + 1;
            int next_v_in_path = -1;
            // The guarantee implies there is an edge to the next SCC.
            for (int neighbor : adj[current_v]) {
                if (scc_id_topo[neighbor] == next_sid) {
                    next_v_in_path = neighbor;
                    break;
                }
            }
            current_v = next_v_in_path;
        }
    }

    // 3. Output the resulting path.
    std::cout << path.size() << "\n";
    for (size_t i = 0; i < path.size(); ++i) {
        std::cout << path[i] << (i == path.size() - 1 ? "" : " ");
    }
    std::cout << "\n";

    return 0;
}