#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <set>
#include <map>

// A cache for queries to avoid re-querying the same triple
std::map<std::vector<int>, int> query_cache;

int query(int u, int v, int w) {
    if (u == v || u == w || v == w) {
        if (u == v) return u;
        if (u == w) return u;
        return v;
    }
    std::vector<int> q_nodes = {u, v, w};
    std::sort(q_nodes.begin(), q_nodes.end());
    if (query_cache.count(q_nodes)) {
        return query_cache[q_nodes];
    }
    std::cout << "0 " << u << " " << v << " " << w << std::endl;
    int median;
    std::cin >> median;
    query_cache[q_nodes] = median;
    return median;
}

std::vector<std::pair<int, int>> edges;

void solve(const std::vector<int>& nodes, int p) {
    if (nodes.empty()) {
        return;
    }
    if (nodes.size() == 1) {
        edges.push_back({p, nodes[0]});
        return;
    }

    int u = nodes[0];
    std::map<int, int> attachment;
    std::set<int> path_nodes_set;
    path_nodes_set.insert(p);
    path_nodes_set.insert(u);

    for (size_t i = 1; i < nodes.size(); ++i) {
        int w = nodes[i];
        int m = query(p, u, w);
        attachment[w] = m;
        path_nodes_set.insert(m);
    }

    std::vector<int> path_nodes;
    for (int node : path_nodes_set) {
        path_nodes.push_back(node);
    }
    
    std::sort(path_nodes.begin(), path_nodes.end(), [&](int n1, int n2) {
        if (n1 == p) return true;
        if (n2 == p) return false;
        return query(p, n1, n2) == n1;
    });

    for (size_t i = 0; i < path_nodes.size() - 1; ++i) {
        edges.push_back({path_nodes[i], path_nodes[i+1]});
    }

    std::map<int, std::vector<int>> groups;
    for (size_t i = 1; i < nodes.size(); ++i) {
        int w = nodes[i];
        if (path_nodes_set.find(w) == path_nodes_set.end()) {
            groups[attachment[w]].push_back(w);
        }
    }

    for (auto const& [attach_node, sub_nodes] : groups) {
        solve(sub_nodes, attach_node);
    }
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    if (n == 3) {
        int m = query(1, 2, 3);
        std::cout << "1";
        if (m == 1) {
            std::cout << " 1 2 1 3" << std::endl;
        } else if (m == 2) {
            std::cout << " 2 1 2 3" << std::endl;
        } else {
            std::cout << " 3 1 3 2" << std::endl;
        }
        return 0;
    }

    int u1 = 1, u2 = 2;
    
    std::map<int, int> initial_attachment;
    std::set<int> path12_nodes_set;
    path12_nodes_set.insert(u1);
    path12_nodes_set.insert(u2);
    
    std::vector<int> other_nodes;
    for (int i = 3; i <= n; ++i) {
        other_nodes.push_back(i);
    }

    for (int node : other_nodes) {
        int m = query(u1, u2, node);
        initial_attachment[node] = m;
        path12_nodes_set.insert(m);
    }

    std::vector<int> path12_nodes;
    for (int node : path12_nodes_set) {
        path12_nodes.push_back(node);
    }

    std::sort(path12_nodes.begin(), path12_nodes.end(), [&](int n1, int n2) {
        if (n1 == u1) return true;
        if (n2 == u1) return false;
        return query(u1, n1, n2) == n1;
    });

    for (size_t i = 0; i < path12_nodes.size() - 1; ++i) {
        edges.push_back({path12_nodes[i], path12_nodes[i+1]});
    }

    std::map<int, std::vector<int>> groups;
    for (int node : other_nodes) {
        if (path12_nodes_set.find(node) == path12_nodes_set.end()) {
             groups[initial_attachment[node]].push_back(node);
        }
    }

    for (auto const& [attach_node, sub_nodes] : groups) {
        solve(sub_nodes, attach_node);
    }

    std::cout << "1";
    for (const auto& edge : edges) {
        std::cout << " " << edge.first << " " << edge.second;
    }
    std::cout << std::endl;

    return 0;
}