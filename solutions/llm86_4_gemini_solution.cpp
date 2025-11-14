#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <map>
#include <queue>

using namespace std;

// Function to interact with the judge
int do_query(int u, int v, int w) {
    cout << "0 " << u << " " << v << " " << w << endl;
    int median;
    cin >> median;
    if (median == -1) exit(0); // Exit on error
    return median;
}

// Helper function to calculate distance in the currently known tree
int get_dist(int start, int end, int n, const map<int, vector<int>>& adj) {
    if (start == end) return 0;
    queue<pair<int, int>> q;
    q.push({start, 0});
    map<int, int> dist;
    dist[start] = 0;
    
    while (!q.empty()) {
        pair<int, int> curr = q.front();
        q.pop();
        
        if (curr.first == end) {
            return curr.second;
        }

        if (adj.count(curr.first)) {
            for (int neighbor : adj.at(curr.first)) {
                if (dist.find(neighbor) == dist.end()) {
                    dist[neighbor] = curr.second + 1;
                    q.push({neighbor, curr.second + 1});
                }
            }
        }
    }
    return n; // Should not be reached in a connected graph
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    // The core idea is to build the tree incrementally.
    // We start with a tree of two nodes {1, 2} and add nodes 3, 4, ..., n one by one.
    // For each new node `i`, we must find its parent in the already constructed tree.
    // The parent of `i` is the node `p` in the current tree that is closest to `i`.
    
    // We can find this parent `p` using a tournament-style selection.
    // Let's maintain a candidate for the parent, say `best_p`.
    // For every other node `v` in the current tree, we compare `best_p` and `v`
    // to see which one is closer to `i`.
    
    // The query `m = query(u, v, i)` gives us the median. The key property is that
    // `d(u, i) - d(v, i) = d_T(u, m) - d_T(v, m)`, where `d` is the distance in the
    // hidden tree and `d_T` is the distance in our constructed tree.
    // Thus, `v` is closer to `i` than `u` if and only if `d_T(v, m) < d_T(u, m)`.

    vector<pair<int, int>> edges;
    map<int, vector<int>> adj;
    
    // Start with an edge between nodes 1 and 2.
    edges.push_back({1, 2});
    adj[1].push_back(2);
    adj[2].push_back(1);
    vector<int> nodes_in_tree = {1, 2};

    for (int i = 3; i <= n; ++i) {
        // Find the parent of node `i` in the tree formed by nodes {1, ..., i-1}.
        int best_parent = nodes_in_tree[0];
        
        for (size_t j = 1; j < nodes_in_tree.size(); ++j) {
            int candidate_node = nodes_in_tree[j];
            int m = do_query(best_parent, candidate_node, i);
            
            // If m is one of the candidates, it is closer to i.
            if (m == best_parent) {
                // best_parent remains the better candidate.
            } else if (m == candidate_node) {
                best_parent = candidate_node;
            } else {
                // m is on the path between best_parent and candidate_node.
                // The one closer to m is closer to i.
                int dist_best_m = get_dist(best_parent, m, n, adj);
                int dist_cand_m = get_dist(candidate_node, m, n, adj);
                if (dist_cand_m < dist_best_m) {
                    best_parent = candidate_node;
                }
            }
        }
        
        // We found the parent. Add the edge.
        edges.push_back({best_parent, i});
        adj[best_parent].push_back(i);
        adj[i].push_back(best_parent);
        nodes_in_tree.push_back(i);
    }

    cout << "1";
    for (const auto& edge : edges) {
        cout << " " << edge.first << " " << edge.second;
    }
    cout << endl;

    return 0;
}