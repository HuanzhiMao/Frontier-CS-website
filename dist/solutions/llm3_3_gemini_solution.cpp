#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <map>

int n;
std::vector<int> adj[100005];
bool lit_state[100005] = {false};

// This function is for single, small queries. It is not used in the final solution
// but might be useful for debugging or simpler approaches. It correctly handles
// the state of `lit_state`.
std::vector<int> single_query(const std::vector<int>& ops) {
    if (ops.empty()) {
        return {};
    }
    std::cout << ops.size();
    for (int op : ops) {
        std::cout << " " << op;
    }
    std::cout << std::endl;
    std::vector<int> res(ops.size());
    for (int i = 0; i < ops.size(); ++i) {
        std::cin >> res[i];
        lit_state[ops[i]] = !lit_state[ops[i]];
    }
    return res;
}

// This is the main query function. It does not track state because
// the logic is designed to be stateless (start and end with all lights off).
std::vector<int> query(const std::vector<int>& ops) {
    if (ops.empty()) {
        return {};
    }
    std::cout << ops.size();
    for (int op : ops) {
        std::cout << " " << op;
    }
    std::cout << std::endl;
    std::vector<int> res(ops.size());
    for (int i = 0; i < ops.size(); ++i) {
        std::cin >> res[i];
    }
    return res;
}

void answer(const std::vector<int>& p) {
    std::cout << -1;
    for (int x : p) {
        std::cout << " " << x;
    }
    std::cout << std::endl;
}

// Finds neighbors of u in a set of candidates `cand`, assuming `cand` is an independent set.
// It works by turning on u, then turning on each candidate one by one.
// A change in adjacency status indicates a neighbor has been found.
void find_neighbors_in_is(int u, const std::vector<int>& cand, std::vector<int>& neighbors) {
    if (cand.empty() || adj[u].size() == 2) return;

    std::vector<int> ops;
    ops.push_back(u);
    for (int v : cand) ops.push_back(v);
    
    auto res = query(ops);
    
    int last_res = 0;
    for (size_t i = 0; i < cand.size(); ++i) {
        if (adj[u].size() == 2) break;
        if (res[i + 1] == 1 && last_res == 0) {
            neighbors.push_back(cand[i]);
            adj[u].push_back(cand[i]);
            adj[cand[i]].push_back(u);
        }
        last_res = res[i + 1];
    }

    query(ops); // Cleanup by performing the same operations again
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int subtask;
    std::cin >> subtask >> n;

    int B = static_cast<int>(sqrt(n));
    if (B < 10 && n > 20) B = 10;
    if (n <= 20) B = n;
    
    std::vector<std::vector<int>> groups;
    std::vector<int> p(n);
    std::iota(p.begin(), p.end(), 1);

    for (int i = 0; i < n; i += B) {
        std::vector<int> current_group;
        for (int j = 0; j < B && i + j < n; ++j) {
            current_group.push_back(p[i + j]);
        }
        groups.push_back(current_group);
    }
    int num_groups = groups.size();

    // Phase 1: Find all intra-group edges
    for (const auto& group : groups) {
        if (group.size() <= 1) continue;
        std::vector<int> ops;
        std::vector<std::pair<int, int>> pairs;
        for (size_t i = 0; i < group.size(); ++i) {
            for (size_t j = i + 1; j < group.size(); ++j) {
                int u = group[i], v = group[j];
                ops.push_back(u); ops.push_back(v);
                ops.push_back(u); ops.push_back(v);
                pairs.push_back({u, v});
            }
        }
        auto results = query(ops);
        for (size_t i = 0; i < pairs.size(); ++i) {
            if (results[i * 4 + 1]) {
                adj[pairs[i].first].push_back(pairs[i].second);
                adj[pairs[i].second].push_back(pairs[i].first);
            }
        }
    }

    // Phase 2: Find inter-group edges
    // Decompose each group into two independent sets
    std::vector<std::vector<int>> group_is1(num_groups), group_is2(num_groups);
    std::vector<int> group_id(n + 1);
    for(int i = 0; i < num_groups; ++i) {
        std::map<int, std::vector<int>> group_adj;
        for(int u : groups[i]) {
            group_id[u] = i;
            for(int v : adj[u]) {
                if(std::find(groups[i].begin(), groups[i].end(), v) != groups[i].end()) {
                    group_adj[u].push_back(v);
                }
            }
        }
        
        std::map<int, int> color;
        for(int u : groups[i]) {
            if(color.find(u) == color.end()) {
                std::vector<int> q;
                q.push_back(u);
                color[u] = 1;
                int head = 0;
                while(head < q.size()) {
                    int curr = q[head++];
                    if(color[curr] == 1) group_is1[i].push_back(curr);
                    else group_is2[i].push_back(curr);
                    
                    for(int neighbor : group_adj[curr]) {
                        if(color.find(neighbor) == color.end()) {
                            color[neighbor] = 3 - color[curr];
                            q.push_back(neighbor);
                        }
                    }
                }
            }
        }
    }

    for (int i = 0; i < num_groups; ++i) {
        for (int j = i + 1; j < num_groups; ++j) {
            std::vector<int> g1_cand, g2_cand;
            for(int u : groups[i]) if(adj[u].size() < 2) g1_cand.push_back(u);
            for(int u : groups[j]) if(adj[u].size() < 2) g2_cand.push_back(u);

            for(int u : g1_cand) {
                if (adj[u].size() < 2) {
                    std::vector<int> neighbors;
                    find_neighbors_in_is(u, group_is1[j], neighbors);
                }
                if (adj[u].size() < 2) {
                    std::vector<int> neighbors;
                    find_neighbors_in_is(u, group_is2[j], neighbors);
                }
            }
             for(int u : g2_cand) {
                 if (adj[u].size() < 2) {
                    std::vector<int> neighbors;
                    find_neighbors_in_is(u, group_is1[i], neighbors);
                }
                if (adj[u].size() < 2) {
                    std::vector<int> neighbors;
                    find_neighbors_in_is(u, group_is2[i], neighbors);
                }
            }
        }
    }
    
    // Phase 3: Reconstruct the cycle
    std::vector<int> final_p(n);
    final_p[0] = 1;
    final_p[1] = adj[1][0];
    
    std::map<int, bool> visited;
    visited[1] = true;

    for (int i = 1; i < n - 1; ++i) {
        visited[final_p[i]] = true;
        int u = final_p[i];
        int prev = final_p[i-1];
        for (int v : adj[u]) {
            if (v != prev) {
                final_p[i + 1] = v;
                break;
            }
        }
    }

    answer(final_p);

    return 0;
}