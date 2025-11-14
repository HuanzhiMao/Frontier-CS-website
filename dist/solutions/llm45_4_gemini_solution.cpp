#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <queue>
#include <list>
#include <random>

// Globals
int n, m, k;
double eps;
std::vector<std::vector<int>> adj;
std::vector<int> final_partition;
std::vector<int> nodes;

// Bisection helpers
std::vector<int> bisect_part;
std::vector<int> subproblem_id;
int current_subproblem_id = 0;

// PRNG
std::mt19937 rng(1337);

// BFS on subgraph to find farthest node
int bfs_farthest(int start_node, int my_subproblem_id) {
    std::vector<int> dist(n + 1, -1);
    dist[start_node] = 0;
    int farthest_node = start_node;
    
    std::vector<int> q_vec;
    q_vec.reserve(n);
    q_vec.push_back(start_node);
    int head = 0;

    while(head < q_vec.size()){
        int u = q_vec[head++];
        farthest_node = u;

        for (int v : adj[u]) {
            if (subproblem_id[v] == my_subproblem_id && dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q_vec.push_back(v);
            }
        }
    }
    return farthest_node;
}

// Fiduccia-Mattheyses refinement
void fm_refine(int begin, int end) {
    int num_nodes_sub = end - begin;
    if (num_nodes_sub <= 1) return;

    int max_deg_sub = 0;
    for (int i = begin; i < end; ++i) {
        int u = nodes[i];
        int current_deg = 0;
        for (int v : adj[u]) {
            if (subproblem_id[v] == current_subproblem_id) {
                current_deg++;
            }
        }
        if (current_deg > max_deg_sub) {
            max_deg_sub = current_deg;
        }
    }
    if (max_deg_sub == 0) return;

    std::vector<int> gains(n + 1);
    std::vector<std::list<int>> buckets(2 * max_deg_sub + 1);
    std::vector<std::list<int>::iterator> node_iters(n + 1);
    int p_max = max_deg_sub;

    for (int pass = 0; pass < 3; ++pass) {
        long long current_cut = 0;
        int max_gain = -num_nodes_sub - 1;
        
        int part_size[2] = {0, 0};
        for (int i = begin; i < end; ++i) {
            part_size[bisect_part[nodes[i]]]++;
        }

        for (int i = 0; i < 2 * p_max + 1; ++i) buckets[i].clear();

        for (int i = begin; i < end; ++i) {
            int u = nodes[i];
            int external_deg = 0;
            int total_deg = 0;
            for (int v : adj[u]) {
                if (subproblem_id[v] == current_subproblem_id) {
                    total_deg++;
                    if (bisect_part[v] != bisect_part[u]) {
                        external_deg++;
                    }
                }
            }
            gains[u] = external_deg - (total_deg - external_deg);
            buckets[gains[u] + p_max].push_front(u);
            node_iters[u] = buckets[gains[u] + p_max].begin();
            
            if (bisect_part[u] == 0) {
                current_cut += external_deg;
            }
        }
        for(int i = 2*p_max; i >= 0; --i) {
            if (!buckets[i].empty()) {
                max_gain = i - p_max;
                break;
            }
        }
        current_cut /= 2;

        std::vector<bool> locked(n + 1, false);
        std::vector<int> move_history;
        std::vector<long long> cut_history;
        long long min_cut = current_cut;
        int best_move_idx = -1;

        int target_size_0 = num_nodes_sub / 2;
        int tol = std::max(1, (int)floor(eps * num_nodes_sub / 2.0));
        
        for (int move_count = 0; move_count < num_nodes_sub; ++move_count) {
            bool can_move_from_0 = (part_size[0] - 1 >= target_size_0 - tol);
            bool can_move_from_1 = (part_size[0] + 1 <= target_size_0 + tol);
            
            int best_node = -1;
            while(max_gain > -p_max - 1) {
                int g_idx = max_gain + p_max;
                int cand0 = -1, cand1 = -1;
                for(int node : buckets[g_idx]) {
                    if(!locked[node]) {
                        if (bisect_part[node] == 0) cand0 = node; else cand1 = node;
                    }
                    if(cand0 != -1 && cand1 != -1) break;
                }
                
                if (can_move_from_0 && can_move_from_1) {
                    if (part_size[0] > part_size[1]) { if (cand0 != -1) best_node = cand0; else if (cand1 != -1) best_node = cand1; } 
                    else { if (cand1 != -1) best_node = cand1; else if (cand0 != -1) best_node = cand0; }
                } else if (can_move_from_0) { best_node = cand0; } 
                else if (can_move_from_1) { best_node = cand1; }

                if (best_node != -1) break;
                max_gain--;
            }
            if (best_node == -1) break;

            int from_part = bisect_part[best_node];
            buckets[gains[best_node] + p_max].erase(node_iters[best_node]);
            
            locked[best_node] = true;
            current_cut -= gains[best_node];
            move_history.push_back(best_node);
            cut_history.push_back(current_cut);
            if (current_cut < min_cut) {
                min_cut = current_cut;
                best_move_idx = move_count;
            }

            part_size[from_part]--;
            part_size[1 - from_part]++;

            for (int neighbor : adj[best_node]) {
                if (subproblem_id[neighbor] == current_subproblem_id && !locked[neighbor]) {
                    buckets[gains[neighbor] + p_max].erase(node_iters[neighbor]);
                    int change = (bisect_part[neighbor] == from_part) ? 2 : -2;
                    gains[neighbor] += change;
                    buckets[gains[neighbor] + p_max].push_front(neighbor);
                    node_iters[neighbor] = buckets[gains[neighbor] + p_max].begin();
                    if (gains[neighbor] > max_gain) max_gain = gains[neighbor];
                }
            }
            bisect_part[best_node] = 1 - from_part;

            while (max_gain > -p_max - 1) {
                bool found_unlocked_in_bucket = false;
                for (int node : buckets[max_gain + p_max]) {
                    if (!locked[node]) {
                        found_unlocked_in_bucket = true;
                        break;
                    }
                }
                if (found_unlocked_in_bucket) break;
                max_gain--;
            }
        }

        for (int i = move_history.size() - 1; i > best_move_idx; --i) {
            int node_to_revert = move_history[i];
            bisect_part[node_to_revert] = 1 - bisect_part[node_to_revert];
        }
        
        if (best_move_idx == -1 && current_cut >= min_cut) break;
    }
}

int bisect(int begin, int end) {
    int num_nodes_sub = end - begin;
    if (num_nodes_sub <= 1) {
        return begin + num_nodes_sub / 2;
    }

    current_subproblem_id++;
    for (int i = begin; i < end; ++i) {
        subproblem_id[nodes[i]] = current_subproblem_id;
    }

    int start_node_bfs = nodes[begin + rng() % num_nodes_sub];
    int s1 = bfs_farthest(start_node_bfs, current_subproblem_id);
    int s2 = bfs_farthest(s1, current_subproblem_id);

    std::queue<int> q;
    q.push(s1); bisect_part[s1] = 0;
    int p1_size = 1;
    
    int target_p1_size = num_nodes_sub / 2;
    if(s1 != s2) bisect_part[s2] = 1;

    while (!q.empty() && p1_size < target_p1_size) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (subproblem_id[v] == current_subproblem_id && bisect_part[v] == -1) {
                bisect_part[v] = 0;
                p1_size++;
                q.push(v);
                if (p1_size >= target_p1_size) break;
            }
        }
    }

    for (int i = begin; i < end; ++i) {
        int u = nodes[i];
        if (bisect_part[u] == -1) {
            bisect_part[u] = 1;
        }
    }

    fm_refine(begin, end);

    for (int i = begin; i < end; ++i) {
        subproblem_id[nodes[i]] = 0;
    }
    
    auto pivot_it = std::partition(nodes.begin() + begin, nodes.begin() + end,
        [&](int v) { return bisect_part[v] == 0; });

    for (int i = begin; i < end; ++i) {
        bisect_part[nodes[i]] = -1;
    }

    return pivot_it - nodes.begin();
}

void recursive_partition(int begin, int end, int current_k, int offset) {
    if (current_k == 1) {
        for (int i = begin; i < end; ++i) {
            final_partition[nodes[i]] = offset;
        }
        return;
    }
    if (begin >= end) {
        return;
    }

    int mid = bisect(begin, end);

    recursive_partition(begin, mid, current_k / 2, offset);
    recursive_partition(mid, end, current_k / 2, offset + current_k / 2);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::cin >> n >> m >> k >> eps;

    adj.resize(n + 1);
    std::vector<std::pair<int, int>> edges;
    edges.reserve(m);
    for (int i = 0; i < m; ++i) {
        int u, v;
        std::cin >> u >> v;
        if (u != v) {
            edges.push_back({std::min(u,v), std::max(u,v)});
        }
    }
    std::sort(edges.begin(), edges.end());
    edges.erase(std::unique(edges.begin(), edges.end()), edges.end());
    
    for(const auto& edge : edges) {
        adj[edge.first].push_back(edge.second);
        adj[edge.second].push_back(edge.first);
    }
    
    final_partition.resize(n + 1);
    nodes.resize(n);
    std::iota(nodes.begin(), nodes.end(), 1);

    bisect_part.assign(n + 1, -1);
    subproblem_id.assign(n + 1, 0);
    
    recursive_partition(0, n, k, 1);

    for (int i = 1; i <= n; ++i) {
        std::cout << final_partition[i] << (i == n ? "" : " ");
    }
    std::cout << "\n";

    return 0;
}