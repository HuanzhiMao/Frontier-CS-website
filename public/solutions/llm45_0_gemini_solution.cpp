#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <queue>
#include <list>
#include <random>

// Fast I/O
void fast_io() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
}

// Graph and problem parameters
int n, m, k;
double eps;
std::vector<std::vector<int>> adj;
int max_part_size;
int max_degree = 0;

// Partitioning state
std::vector<int> node_indices;
std::vector<int> partition_map;

// Random number generator for reproducibility
std::mt19937 rng(42);

// Helper for finding a pseudo-peripheral node in a subgraph
int find_peripheral_node(int begin, int end, const std::vector<bool>& p_nodes) {
    if (begin == end) return -1;
    if (begin + 1 == end) return node_indices[begin];

    std::uniform_int_distribution<int> dist(begin, end - 1);
    int start_node = node_indices[dist(rng)];

    std::vector<int> q;
    q.reserve(end - begin);
    std::vector<int> d(n + 1, -1);
    
    // BFS from start_node to find farthest node u
    q.push_back(start_node);
    d[start_node] = 0;
    int head = 0;
    int farthest_node = start_node;
    int max_dist = 0;

    while(head < q.size()){
        int u = q[head++];
        if (d[u] > max_dist) {
            max_dist = d[u];
            farthest_node = u;
        }
        for (int v : adj[u]) {
            if (p_nodes[v] && d[v] == -1) {
                d[v] = d[u] + 1;
                q.push_back(v);
            }
        }
    }

    // BFS from farthest_node to find its farthest node
    q.clear();
    head = 0;
    std::fill(d.begin(), d.end(), -1);

    q.push_back(farthest_node);
    d[farthest_node] = 0;
    max_dist = 0;
    int peripheral_node = farthest_node;

    while(head < q.size()){
        int u = q[head++];
        if (d[u] > max_dist) {
            max_dist = d[u];
            peripheral_node = u;
        }
        for (int v : adj[u]) {
            if (p_nodes[v] && d[v] == -1) {
                d[v] = d[u] + 1;
                q.push_back(v);
            }
        }
    }

    return peripheral_node;
}

// Bisection function with FM refinement
int bisect_and_reorder(int begin, int end, int num_parts) {
    int num_sub_nodes = end - begin;
    if (num_sub_nodes <= 1) {
        return num_sub_nodes;
    }

    // Balance constraints for this bisection
    long long k_half = num_parts / 2;
    int size_limit = k_half * max_part_size;
    int min_A_size = num_sub_nodes > size_limit ? num_sub_nodes - size_limit : 0;


    // Data structures for this bisection
    std::vector<bool> p_nodes(n + 1, false);
    for (int i = begin; i < end; ++i) p_nodes[node_indices[i]] = true;

    std::vector<int> part(n + 1, 0);
    int part_A_size = 0;

    // 1. Initial Partition using BFS
    int start_node = find_peripheral_node(begin, end, p_nodes);
    if (start_node == -1) return 0;

    std::vector<bool> visited(n + 1, false);
    
    int target_A_size = num_sub_nodes / 2;
    if (target_A_size < min_A_size) target_A_size = min_A_size;
    if (target_A_size > size_limit) target_A_size = size_limit;

    std::vector<int> bfs_nodes;
    bfs_nodes.reserve(num_sub_nodes);
    bfs_nodes.push_back(start_node);
    visited[start_node] = true;
    int head = 0;
    while(head < bfs_nodes.size()){
        int u = bfs_nodes[head++];
        for(int v : adj[u]){
            if(p_nodes[v] && !visited[v]){
                visited[v] = true;
                bfs_nodes.push_back(v);
            }
        }
    }
    
    for(int i=0; i < num_sub_nodes; ++i) {
        if(i < target_A_size) {
            part[bfs_nodes[i]] = 0;
        } else {
            part[bfs_nodes[i]] = 1;
        }
    }
    part_A_size = target_A_size;
    int part_B_size = num_sub_nodes - part_A_size;


    // 2. FM Refinement
    int num_passes = 10;
    for (int pass = 0; pass < num_passes; ++pass) {
        std::vector<int> gain(n + 1, 0);
        
        for (int i = begin; i < end; ++i) {
            int u = node_indices[i];
            for (int v : adj[u]) {
                if (p_nodes[v]) {
                    gain[u] += (part[u] == part[v] ? -1 : 1);
                }
            }
        }

        std::vector<list<int>::iterator> bucket_iters(n + 1);
        std::vector<list<int>> buckets(2 * max_degree + 1);
        int max_gain_val = -max_degree -1;

        for (int i = begin; i < end; ++i) {
            int u = node_indices[i];
            buckets[gain[u] + max_degree].push_front(u);
            bucket_iters[u] = buckets[gain[u] + max_degree].begin();
            if(gain[u] > max_gain_val) max_gain_val = gain[u];
        }

        std::vector<bool> locked(n + 1, false);
        std::vector<int> moves;
        moves.reserve(num_sub_nodes);
        
        long long max_prefix_gain = 0;
        long long current_prefix_gain = 0;
        int best_step = -1;

        for (int step = 0; step < num_sub_nodes; ++step) {
            int best_node = -1;
            int max_g = -max_degree - 1;
            
            while(max_gain_val >= -max_degree && buckets[max_gain_val + max_degree].empty()) {
                max_gain_val--;
            }
            if(max_gain_val < -max_degree) break;
            int g = max_gain_val;

            for (int u : buckets[g + max_degree]) {
                if (part[u] == 0) { // Moving from A to B
                    if (part_A_size - 1 >= min_A_size) {
                        best_node = u; break;
                    }
                } else { // Moving from B to A
                    if (part_B_size - 1 >= min_A_size) {
                        best_node = u; break;
                    }
                }
            }

            if (best_node == -1) {
                 for(g = max_gain_val - 1; g >= -max_degree; --g) {
                    bool found = false;
                    for(int u : buckets[g + max_degree]) {
                        if (part[u] == 0) { if (part_A_size - 1 >= min_A_size) {best_node = u; found=true; break;}}
                        else { if (part_B_size - 1 >= min_A_size) {best_node = u; found=true; break;}}
                    }
                    if(found) break;
                 }
                 if(best_node == -1) break;
            }

            max_g = g;
            
            int u_part = part[best_node];
            buckets[max_g + max_degree].erase(bucket_iters[best_node]);
            
            current_prefix_gain += max_g;
            if (current_prefix_gain > max_prefix_gain) {
                max_prefix_gain = current_prefix_gain;
                best_step = step;
            }
            
            moves.push_back(best_node);
            locked[best_node] = true;
            part[best_node] = 1 - u_part;
            if(u_part == 0) { part_A_size--; part_B_size++; }
            else { part_B_size--; part_A_size++; }

            for (int v : adj[best_node]) {
                if (p_nodes[v] && !locked[v]) {
                    buckets[gain[v] + max_degree].erase(bucket_iters[v]);
                    int factor = (part[v] == u_part) ? 2 : -2;
                    gain[v] += factor;
                    buckets[gain[v] + max_degree].push_front(v);
                    bucket_iters[v] = buckets[gain[v] + max_degree].begin();
                    if(gain[v] > max_gain_val) max_gain_val = gain[v];
                }
            }
        }
        
        // Rollback to best state
        for (int i = moves.size() - 1; i > best_step; --i) {
            int u = moves[i];
            part[u] = 1 - part[u];
        }
        
        if (max_prefix_gain <= 0) break;
    }

    // Reorder node_indices
    auto it = std::partition(node_indices.begin() + begin, node_indices.begin() + end,
                             [&](int u) { return part[u] == 0; });
    
    return std::distance(node_indices.begin() + begin, it);
}

void recursive_partition(int begin, int end, int num_parts, int part_id_offset) {
    if (num_parts == 1 || begin >= end) {
        for (int i = begin; i < end; ++i) {
            partition_map[node_indices[i] - 1] = part_id_offset + 1;
        }
        return;
    }

    int part_A_size = bisect_and_reorder(begin, end, num_parts);
    int mid = begin + part_A_size;
    int next_num_parts = num_parts / 2;
    
    recursive_partition(begin, mid, next_num_parts, part_id_offset);
    recursive_partition(mid, end, next_num_parts, part_id_offset + next_num_parts);
}

int main() {
    fast_io();

    std::cin >> n >> m >> k >> eps;

    adj.resize(n + 1);
    std::vector<std::pair<int, int>> edges;
    edges.reserve(m);
    for (int i = 0; i < m; ++i) {
        int u, v;
        std::cin >> u >> v;
        if (u != v) {
            if (u > v) std::swap(u, v);
            edges.push_back({u, v});
        }
    }

    std::sort(edges.begin(), edges.end());
    edges.erase(std::unique(edges.begin(), edges.end()), edges.end());

    for (const auto& edge : edges) {
        adj[edge.first].push_back(edge.second);
        adj[edge.second].push_back(edge.first);
    }
    
    for(int i = 1; i <= n; ++i) {
        max_degree = std::max(max_degree, (int)adj[i].size());
    }

    max_part_size = floor((1.0 + eps) * std::ceil((double)n / k));
    
    node_indices.resize(n);
    std::iota(node_indices.begin(), node_indices.end(), 1);
    partition_map.resize(n);

    recursive_partition(0, n, k, 0);

    for (int i = 0; i < n; ++i) {
        std::cout << partition_map[i] << (i == n - 1 ? "" : " ");
    }
    std::cout << std::endl;

    return 0;
}