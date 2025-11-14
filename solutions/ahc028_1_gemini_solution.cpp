#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <tuple>

using namespace std;

// Constants from problem statement
const int GRID_N = 15;
const int NUM_STRINGS_M = 200;

// Global variables to hold input data
int start_r, start_c;
char grid[GRID_N][GRID_N];
string targets[NUM_STRINGS_M];
vector<pair<int, int>> char_pos[26];
int overlaps[NUM_STRINGS_M][NUM_STRINGS_M];

// Calculates the maximum overlap of a suffix of s1 with a prefix of s2
int calculate_overlap(const string& s1, const string& s2) {
    for (int k = min((int)s1.length(), (int)s2.length()) - 1; k >= 1; --k) {
        if (s1.substr(s1.length() - k) == s2.substr(0, k)) {
            return k;
        }
    }
    return 0;
}

// DSU data structure to keep track of merged string components
struct DSU {
    vector<int> parent;
    vector<int> head;
    vector<int> tail;
    DSU(int n) {
        parent.resize(n);
        iota(parent.begin(), parent.end(), 0);
        head.resize(n);
        iota(head.begin(), head.end(), 0);
        tail.resize(n);
        iota(tail.begin(), tail.end(), 0);
    }

    int find(int i) {
        if (parent[i] == i) {
            return i;
        }
        return parent[i] = find(parent[i]);
    }

    void unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j) {
            parent[root_j] = root_i;
            tail[root_i] = tail[root_j];
        }
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n_dummy, m_dummy;
    cin >> n_dummy >> m_dummy;
    cin >> start_r >> start_c;

    for (int i = 0; i < GRID_N; ++i) {
        string row;
        cin >> row;
        for (int j = 0; j < GRID_N; ++j) {
            grid[i][j] = row[j];
            char_pos[grid[i][j] - 'A'].push_back({i, j});
        }
    }

    for (int i = 0; i < NUM_STRINGS_M; ++i) {
        cin >> targets[i];
    }

    // --- Shortest Common Superstring part ---

    // 1. Pre-calculate all pairwise overlaps
    for (int i = 0; i < NUM_STRINGS_M; ++i) {
        for (int j = 0; j < NUM_STRINGS_M; ++j) {
            if (i == j) continue;
            overlaps[i][j] = calculate_overlap(targets[i], targets[j]);
        }
    }
    
    // 2. Create edges for all possible merges, sorted by overlap size
    vector<tuple<int, int, int>> edges;
    for (int i = 0; i < NUM_STRINGS_M; ++i) {
        for (int j = 0; j < NUM_STRINGS_M; ++j) {
            if (i == j) continue;
            edges.emplace_back(overlaps[i][j], i, j);
        }
    }
    sort(edges.rbegin(), edges.rend());

    // 3. Greedily merge components with the largest overlaps
    DSU dsu(NUM_STRINGS_M);
    vector<int> next_node(NUM_STRINGS_M, -1);
    int merges_done = 0;

    for (const auto& edge : edges) {
        int ov, u, v;
        tie(ov, u, v) = edge;

        int root_u = dsu.find(u);
        int root_v = dsu.find(v);

        // Merge if they are in different components and the edge connects tail to head
        if (root_u != root_v && dsu.tail[root_u] == u && dsu.head[root_v] == v) {
            dsu.unite(u, v);
            next_node[u] = v;
            merges_done++;
            if (merges_done == NUM_STRINGS_M - 1) break;
        }
    }

    // 4. Reconstruct the path (order of strings)
    int start_node_idx = -1;
    vector<bool> is_not_head(NUM_STRINGS_M, false);
    for (int i = 0; i < NUM_STRINGS_M; ++i) {
        if (next_node[i] != -1) {
            is_not_head[next_node[i]] = true;
        }
    }
    for (int i = 0; i < NUM_STRINGS_M; ++i) {
        if (!is_not_head[i]) {
            start_node_idx = i;
            break;
        }
    }
    
    vector<int> path;
    int current_node_idx = start_node_idx;
    while(current_node_idx != -1) {
        path.push_back(current_node_idx);
        current_node_idx = next_node[current_node_idx];
    }
    
    // Handle potentially disconnected components by appending unvisited strings
    if (path.size() < NUM_STRINGS_M) {
        vector<bool> visited(NUM_STRINGS_M, false);
        for(int idx : path) visited[idx] = true;
        for(int i = 0; i < NUM_STRINGS_M; ++i) {
            if (!visited[i]) path.push_back(i);
        }
    }


    // 5. Build the final superstring S
    string S = targets[path[0]];
    for (size_t i = 1; i < path.size(); ++i) {
        int prev_idx = path[i-1];
        int curr_idx = path[i];
        int ov = overlaps[prev_idx][curr_idx];
        S += targets[curr_idx].substr(ov);
    }
    
    // --- Typing part ---
    // Greedily type the superstring S by always moving to the closest key
    pair<int, int> current_pos = {start_r, start_c};
    for (char c : S) {
        pair<int, int> best_next_pos = {-1, -1};
        int min_dist = 1e9;
        
        for (const auto& p : char_pos[c - 'A']) {
            int dist = abs(p.first - current_pos.first) + abs(p.second - current_pos.second);
            if (dist < min_dist) {
                min_dist = dist;
                best_next_pos = p;
            }
        }
        cout << best_next_pos.first << " " << best_next_pos.second << "\n";
        current_pos = best_next_pos;
    }

    return 0;
}