#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <list>
#include <random>
#include <chrono>

// Fast I/O
void fast_io() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
}

class Partitioner {
private:
    int n;
    int k;
    double eps;
    std::vector<std::vector<int>> adj;
    std::vector<int> partition_map;
    long long max_part_size;
    int max_degree = 0;

    std::mt19937 rng;

public:
    Partitioner() {
        rng.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    }

    void read_and_build_graph() {
        int m;
        std::cin >> n >> m >> k >> eps;
        partition_map.resize(n + 1);

        std::vector<std::pair<int, int>> edge_list;
        edge_list.reserve(m);
        for (int i = 0; i < m; ++i) {
            int u, v;
            std::cin >> u >> v;
            if (u != v) {
                if (u > v) std::swap(u, v);
                edge_list.push_back({u, v});
            }
        }

        std::sort(edge_list.begin(), edge_list.end());
        edge_list.erase(std::unique(edge_list.begin(), edge_list.end()), edge_list.end());

        adj.resize(n + 1);
        std::vector<int> degree(n + 1, 0);
        for (const auto& edge : edge_list) {
            adj[edge.first].push_back(edge.second);
            adj[edge.second].push_back(edge.first);
            degree[edge.first]++;
            degree[edge.second]++;
        }

        for(int i = 1; i <= n; ++i) {
            if (degree[i] > max_degree) {
                max_degree = degree[i];
            }
        }
    }

    void solve() {
        long double ideal_size = std::ceil((long double)n / k);
        max_part_size = std::floor((1.0 + eps) * ideal_size);
        if (max_part_size == 0 && n > 0) max_part_size = 1;

        std::vector<int> initial_vertices(n);
        std::iota(initial_vertices.begin(), initial_vertices.end(), 1);
        
        recursive_partition(initial_vertices, k, 1);
    }

    void print_partition() {
        for (int i = 1; i <= n; ++i) {
            std::cout << partition_map[i] << (i == n ? "" : " ");
        }
        std::cout << std::endl;
    }

private:
    void recursive_partition(const std::vector<int>& vertices, int current_k, int part_offset) {
        if (current_k == 1) {
            for (int v : vertices) {
                partition_map[v] = part_offset;
            }
            return;
        }

        if (vertices.empty()) return;

        auto [part1, part2] = bisect(vertices, current_k);
        
        int k1 = current_k / 2;
        int k2 = current_k - k1;

        recursive_partition(part1, k1, part_offset);
        recursive_partition(part2, k2, part_offset + k1);
    }
    
    std::pair<std::vector<int>, std::vector<int>> bisect(const std::vector<int>& vertices, int current_k) {
        int num_vertices = vertices.size();
        if (num_vertices <= 1) {
            std::vector<int> p1, p2;
            if(num_vertices == 1) p1.push_back(vertices[0]);
            return {p1, p2};
        }
        
        std::vector<int> node_to_idx(n + 1, -1);
        std::vector<int> idx_to_node(num_vertices);
        for(int i = 0; i < num_vertices; ++i) {
            node_to_idx[vertices[i]] = i;
            idx_to_node[i] = vertices[i];
        }

        std::vector<int> current_part(num_vertices);
        long long part_0_size = 0;
        
        std::uniform_int_distribution<int> dist(0, num_vertices - 1);
        int start_node_idx = dist(rng);
        
        std::vector<int> q;
        q.push_back(start_node_idx);
        std::vector<bool> visited(num_vertices, false);
        visited[start_node_idx] = true;
        current_part[start_node_idx] = 0;
        part_0_size = 1;
        
        int head = 0;
        while(head < q.size() && part_0_size < num_vertices / 2){
            int u_idx = q[head++];
            int u_node = idx_to_node[u_idx];
            
            for(int v_node : adj[u_node]){
                if(node_to_idx[v_node] != -1){
                    int v_idx = node_to_idx[v_node];
                    if(!visited[v_idx]){
                        visited[v_idx] = true;
                        current_part[v_idx] = 0;
                        part_0_size++;
                        q.push_back(v_idx);
                        if (part_0_size >= num_vertices / 2) break;
                    }
                }
            }
        }
        for(int i=0; i<num_vertices; ++i) if (!visited[i]) current_part[i] = 1;

        long long k_A = current_k / 2;
        long long k_B = current_k - k_A;
        long long max_A_size = k_A * max_part_size;
        long long max_B_size = k_B * max_part_size;

        const int num_passes = 5;
        for(int pass_idx = 0; pass_idx < num_passes; ++pass_idx) {
            std::vector<long> gains(num_vertices);
            
            std::vector<std::list<int>> buckets(2 * max_degree + 1);
            std::vector<std::list<int>::iterator> bucket_iters(num_vertices);
            
            part_0_size = 0;
            for(int i=0; i<num_vertices; ++i) if(current_part[i] == 0) part_0_size++;

            for(int i=0; i<num_vertices; ++i) {
                int u_node = idx_to_node[i];
                long external_degree = 0, internal_degree = 0;
                for(int v_node : adj[u_node]) {
                    if (node_to_idx[v_node] != -1) {
                        if(current_part[i] == current_part[node_to_idx[v_node]]) internal_degree++;
                        else external_degree++;
                    }
                }
                gains[i] = external_degree - internal_degree;
                buckets[gains[i] + max_degree].push_front(i);
                bucket_iters[i] = buckets[gains[i] + max_degree].begin();
            }

            std::vector<bool> locked(num_vertices, false);
            std::vector<int> move_history; move_history.reserve(num_vertices);
            long long gain_sum = 0, max_gain_sum = 0;
            int best_k_moves = 0;
            
            for(int k_move=0; k_move < num_vertices; ++k_move) {
                int best_node_to_move = -1;

                int best_node_0 = -1;
                if ((num_vertices - part_0_size) + 1 <= max_B_size) {
                    for (int g = 2 * max_degree; g >= 0; --g) {
                        for (int u_idx : buckets[g]) { if (!locked[u_idx] && current_part[u_idx] == 0) { best_node_0 = u_idx; goto found_best_0; } }
                    }
                }
                found_best_0:;

                int best_node_1 = -1;
                if (part_0_size + 1 <= max_A_size) {
                    for (int g = 2 * max_degree; g >= 0; --g) {
                        for (int u_idx : buckets[g]) { if (!locked[u_idx] && current_part[u_idx] == 1) { best_node_1 = u_idx; goto found_best_1; } }
                    }
                }
                found_best_1:;

                if (best_node_0 == -1 && best_node_1 == -1) break;

                long gain0 = (best_node_0 == -1) ? -2e9 : gains[best_node_0];
                long gain1 = (best_node_1 == -1) ? -2e9 : gains[best_node_1];

                if (gain0 > gain1) best_node_to_move = best_node_0;
                else if (gain1 > gain0) best_node_to_move = best_node_1;
                else {
                    if (part_0_size > (num_vertices - part_0_size)) best_node_to_move = best_node_0 != -1 ? best_node_0 : best_node_1;
                    else best_node_to_move = best_node_1 != -1 ? best_node_1 : best_node_0;
                }
                if (best_node_to_move == -1) break;

                int u_idx = best_node_to_move;
                int u_node = idx_to_node[u_idx];
                int from_part = current_part[u_idx];
                
                gain_sum += gains[u_idx];
                if (gain_sum > max_gain_sum) {
                    max_gain_sum = gain_sum;
                    best_k_moves = k_move + 1;
                }
                
                locked[u_idx] = true;
                move_history.push_back(u_idx);
                current_part[u_idx] = 1 - from_part;
                if (from_part == 0) part_0_size--; else part_0_size++;

                for(int v_node : adj[u_node]) {
                    if(node_to_idx[v_node] != -1) {
                        int v_idx = node_to_idx[v_node];
                        if(locked[v_idx]) continue;
                        
                        long old_gain = gains[v_idx];
                        buckets[old_gain + max_degree].erase(bucket_iters[v_idx]);
                        
                        long new_gain = old_gain + (current_part[v_idx] == from_part ? 2 : -2);
                        gains[v_idx] = new_gain;
                        buckets[new_gain + max_degree].push_front(v_idx);
                        bucket_iters[v_idx] = buckets[new_gain + max_degree].begin();
                    }
                }
            }
            
            if(max_gain_sum > 0) {
                 for(int i = num_vertices - 1; i >= best_k_moves; --i) {
                     int u_idx = move_history[i];
                     current_part[u_idx] = 1 - current_part[u_idx];
                 }
            } else {
                for (int i = move_history.size() - 1; i >= 0; --i) {
                     int u_idx = move_history[i];
                     current_part[u_idx] = 1 - current_part[u_idx];
                }
                break;
            }
        }
        
        std::vector<int> p1, p2;
        p1.reserve(num_vertices); p2.reserve(num_vertices);
        for (int i=0; i<num_vertices; ++i) {
            if (current_part[i] == 0) p1.push_back(idx_to_node[i]);
            else p2.push_back(idx_to_node[i]);
        }
        return {p1, p2};
    }
};

int main() {
    fast_io();
    Partitioner p;
    p.read_and_build_graph();
    p.solve();
    p.print_partition();
    return 0;
}