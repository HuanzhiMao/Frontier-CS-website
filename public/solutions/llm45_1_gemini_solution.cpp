#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <set>
#include <random>
#include <chrono>
#include <unordered_map>

using namespace std;

int n;
int m; // using int for m
int k;
double eps;
vector<vector<int>> adj;
vector<int> partition_map;
int max_part_size;

mt19937 rng;

vector<bool> in_current_group;
vector<int> group_node_to_idx;

// Bisection refinement logic
void refine_bisection(const vector<int>& group, vector<bool>& part_assignment) {
    int part1_size = 0;
    for (size_t i = 0; i < group.size(); ++i) {
        if (part_assignment[i]) {
            part1_size++;
        }
    }
    int part0_size = group.size() - part1_size;
    
    vector<int> shuffled_group = group;

    for (int pass = 0; pass < 5; ++pass) {
        bool changed = false;
        shuffle(shuffled_group.begin(), shuffled_group.end(), rng);
        
        for (int u : shuffled_group) {
            int u_idx = group_node_to_idx[u];
            bool u_part = part_assignment[u_idx];
            
            bool can_move = false;
            if (u_part == 0) { // Moving from 0 to 1
                if (part0_size > part1_size) can_move = true;
            } else { // Moving from 1 to 0
                if (part1_size > part0_size) can_move = true;
            }
            if (part0_size == part1_size) can_move = true;

            if (!can_move) continue;

            int internal_edges = 0;
            int external_edges = 0;
            for (int v : adj[u]) {
                if (in_current_group[v]) {
                    int v_idx = group_node_to_idx[v];
                    if (part_assignment[v_idx] == u_part) {
                        internal_edges++;
                    } else {
                        external_edges++;
                    }
                }
            }

            if (external_edges > internal_edges) {
                part_assignment[u_idx] = !u_part;
                if (u_part == 0) {
                    part0_size--;
                    part1_size++;
                } else {
                    part0_size++;
                    part1_size--;
                }
                changed = true;
            }
        }
        if (!changed) break;
    }
}

// Bisection main logic
void bisect(const vector<int>& group, vector<int>& p1, vector<int>& p2) {
    if (group.empty()) return;
    if (group.size() <= 1) {
        p1 = group;
        return;
    }

    for(size_t i = 0; i < group.size(); ++i) {
        in_current_group[group[i]] = true;
        group_node_to_idx[group[i]] = i;
    }

    vector<bool> part_assignment(group.size(), false); // false for part 0, true for part 1
    
    // BFS for initial partition
    int start_node = group[uniform_int_distribution<int>(0, group.size() - 1)(rng)];
    vector<int> bfs_order;
    vector<int> q;
    q.push_back(start_node);
    
    vector<bool> visited(n + 1, false);
    visited[start_node] = true;
    bfs_order.push_back(start_node);
    
    int head = 0;
    while(head < (int)q.size() && bfs_order.size() < group.size()){
        int u = q[head++];
        for(int v : adj[u]){
            if(in_current_group[v] && !visited[v]){
                visited[v] = true;
                q.push_back(v);
                bfs_order.push_back(v);
            }
        }
    }
    
    if (bfs_order.size() < group.size()) {
        for(int node : group) {
            if(!visited[node]) bfs_order.push_back(node);
        }
    }

    for (size_t i = 0; i < group.size() / 2; ++i) {
        part_assignment[group_node_to_idx[bfs_order[i]]] = false;
    }
    for (size_t i = group.size() / 2; i < group.size(); ++i) {
        part_assignment[group_node_to_idx[bfs_order[i]]] = true;
    }

    refine_bisection(group, part_assignment);

    for (size_t i = 0; i < group.size(); ++i) {
        if (part_assignment[i] == false) {
            p1.push_back(group[i]);
        } else {
            p2.push_back(group[i]);
        }
    }
    
    for(int node : group) {
        in_current_group[node] = false;
    }
}

// Recursive bisection driver
void recursive_bisect(const vector<int>& group, int current_k, int part_id_offset) {
    if (current_k == 1) {
        for (int node : group) {
            partition_map[node] = part_id_offset;
        }
        return;
    }
    if (group.empty()) {
        return;
    }

    vector<int> p1, p2;
    bisect(group, p1, p2);

    recursive_bisect(p1, current_k / 2, part_id_offset);
    recursive_bisect(p2, current_k / 2, part_id_offset + current_k / 2);
}

// Final k-way refinement
void k_way_refine() {
    vector<int> part_sizes(k + 1, 0);
    for(int i = 1; i <= n; ++i) {
        part_sizes[partition_map[i]]++;
    }

    vector<vector<int>> adj_part_counts(n + 1, vector<int>(k + 1, 0));
    for (int i = 1; i <= n; ++i) {
        for (int neighbor : adj[i]) {
            adj_part_counts[i][partition_map[neighbor]]++;
        }
    }

    for (int iter = 0; iter < 2; ++iter) {
        vector<int> nodes(n);
        iota(nodes.begin(), nodes.end(), 1);
        shuffle(nodes.begin(), nodes.end(), rng);

        int moves = 0;
        for (int u : nodes) {
            int old_part = partition_map[u];
            if (part_sizes[old_part] <= 1) continue;

            int best_part = old_part;
            int max_gain = 0;

            int current_internal_edges = adj_part_counts[u][old_part];
            
            unordered_map<int, int> neighbor_parts;
            for(int neighbor : adj[u]) {
                neighbor_parts[partition_map[neighbor]]++;
            }

            for (auto const& [p, count] : neighbor_parts) {
                if (p == old_part) continue;
                if (part_sizes[p] + 1 > max_part_size) continue;
                
                int gain = count - current_internal_edges;
                if (gain > max_gain) {
                    max_gain = gain;
                    best_part = p;
                }
            }
            
            if (best_part != old_part) {
                moves++;
                for (int v : adj[u]) {
                    adj_part_counts[v][old_part]--;
                    adj_part_counts[v][best_part]++;
                }
                part_sizes[old_part]--;
                part_sizes[best_part]++;
                partition_map[u] = best_part;
            }
        }
        if (moves == 0) break;
    }
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    rng.seed(chrono::high_resolution_clock::now().time_since_epoch().count());

    cin >> n >> m >> k >> eps;

    vector<set<int>> adj_set(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        if (u != v) {
            adj_set[u].insert(v);
            adj_set[v].insert(u);
        }
    }

    adj.resize(n + 1);
    for (int i = 1; i <= n; ++i) {
        adj[i].assign(adj_set[i].begin(), adj_set[i].end());
    }

    partition_map.resize(n + 1);
    max_part_size = floor((1.0 + eps) * ceil((double)n / k));
    
    in_current_group.resize(n + 1, false);
    group_node_to_idx.resize(n + 1);

    vector<int> nodes(n);
    iota(nodes.begin(), nodes.end(), 1);

    recursive_bisect(nodes, k, 1);

    k_way_refine();

    for (int i = 1; i <= n; ++i) {
        cout << partition_map[i] << (i == n ? "" : " ");
    }
    cout << endl;

    return 0;
}