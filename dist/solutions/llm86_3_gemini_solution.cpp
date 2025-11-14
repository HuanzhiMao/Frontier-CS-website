#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <map>
#include <random>
#include <chrono>
#include <set>

// Function to ask a query
int ask_query(int u, int v, int w) {
    std::cout << "0 " << u << " " << v << " " << w << std::endl;
    int median;
    std::cin >> median;
    if (median == 0) exit(0);
    return median;
}

// Global vector to store edges
std::vector<std::pair<int, int>> edges;
// Random number generator
static std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

void solve(const std::vector<int>& S, int r) {
    if (S.empty()) {
        return;
    }
    if (S.size() == 1) {
        edges.push_back({r, S[0]});
        return;
    }

    std::uniform_int_distribution<int> dist(0, S.size() - 1);
    int p = S[dist(rng)];
    
    std::vector<int> S_p, S_other;
    S_p.push_back(p);

    for (int u : S) {
        if (u == p) continue;
        if (ask_query(p, u, r) != r) {
            S_p.push_back(u);
        } else {
            S_other.push_back(u);
        }
    }

    int c_p = p;
    if (S_p.size() > 1) {
        int current_closest = S_p[0];
        for (size_t i = 1; i < S_p.size(); ++i) {
            int u = S_p[i];
            if (ask_query(current_closest, u, r) == u) {
                current_closest = u;
            }
        }
        c_p = current_closest;
    }
    
    edges.push_back({r, c_p});

    std::vector<int> S_p_rec;
    for(int node : S_p) {
        if(node != c_p) {
            S_p_rec.push_back(node);
        }
    }

    solve(S_p_rec, c_p);
    solve(S_other, r);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    if (n == 3) {
        int m = ask_query(1, 2, 3);
        if (m == 1) {
            edges.push_back({1, 2});
            edges.push_back({1, 3});
        } else if (m == 2) {
            edges.push_back({2, 1});
            edges.push_back({2, 3});
        } else {
            edges.push_back({3, 1});
            edges.push_back({3, 2});
        }
    } else {
        int u1 = 1, u2 = 2;
        std::map<int, std::vector<int>> subtrees;
        std::set<int> path_nodes_set;

        path_nodes_set.insert(u1);
        path_nodes_set.insert(u2);
        
        for (int i = 3; i <= n; ++i) {
            int median = ask_query(u1, u2, i);
            subtrees[median].push_back(i);
            path_nodes_set.insert(median);
        }
        
        std::vector<int> path_nodes(path_nodes_set.begin(), path_nodes_set.end());
        
        std::sort(path_nodes.begin(), path_nodes.end(), [&](int a, int b) {
            if (a == u1) return true;
            if (b == u1) return false;
            if (a == b) return false;
            return ask_query(u1, a, b) == a;
        });

        for (size_t i = 0; i < path_nodes.size() - 1; ++i) {
            edges.push_back({path_nodes[i], path_nodes[i + 1]});
        }

        for (int node_on_path : path_nodes) {
            if (subtrees.count(node_on_path)) {
                solve(subtrees[node_on_path], node_on_path);
            }
        }
    }

    std::cout << "1";
    for (const auto& edge : edges) {
        std::cout << " " << edge.first << " " << edge.second;
    }
    std::cout << std::endl;

    return 0;
}