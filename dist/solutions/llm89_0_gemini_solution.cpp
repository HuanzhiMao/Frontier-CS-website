#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <map>

using namespace std;

// Function to send a query to the judge and get the result.
bool ask_query(int v, const vector<int>& s) {
    if (s.empty()) {
        return false;
    }
    cout << "? " << s.size() << " " << v;
    for (int si : s) {
        cout << " " << si;
    }
    cout << endl;
    cout.flush();
    int result;
    cin >> result;
    if (result == -1) exit(0);
    return result == 1;
}

vector<pair<int, int>> edges;

// Recursive function to solve for a connected component of nodes.
void solve(vector<int> nodes, int parent_node) {
    if (nodes.empty()) {
        return;
    }

    if (nodes.size() == 1) {
        if (parent_node != 0) {
            edges.push_back({parent_node, nodes[0]});
        }
        return;
    }

    // Pick two endpoints for a path. Using the first two nodes is a simple heuristic.
    int u = nodes[0];
    int v = nodes[1];

    // Find all nodes on the path between u and v within the current component.
    vector<int> path_uv;
    path_uv.push_back(u);
    path_uv.push_back(v);

    for (size_t i = 2; i < nodes.size(); ++i) {
        int w = nodes[i];
        if (ask_query(w, {u, v})) {
            path_uv.push_back(w);
        }
    }

    // Sort the path nodes by their distance from u.
    // p1 is closer to u than p2 iff p1 is on the path between u and p2.
    sort(path_uv.begin(), path_uv.end(), [&](int p1, int p2) {
        if (p1 == u) return true;
        if (p2 == u) return false;
        return ask_query(p1, {u, p2});
    });

    // Connect the root of this component's backbone (u) to its parent from the previous level.
    if (parent_node != 0) {
        edges.push_back({parent_node, path_uv[0]});
    }

    // Add edges forming the backbone path.
    for (size_t i = 0; i < path_uv.size() - 1; ++i) {
        edges.push_back({path_uv[i], path_uv[i+1]});
    }

    // Group remaining nodes by their attachment point on the path.
    map<int, vector<int>> groups;
    vector<bool> is_on_path(1001, false);
    for (int p_node : path_uv) {
        is_on_path[p_node] = true;
    }

    for (int node : nodes) {
        if (is_on_path[node]) continue;

        // Binary search for the attachment point on the path.
        // The attachment point is the node on path (u,v) that is on path (u, node)
        // and is furthest from u.
        int low = 0, high = path_uv.size() - 1;
        int attach_idx = 0;

        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (ask_query(path_uv[mid], {u, node})) {
                attach_idx = mid;
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        groups[path_uv[attach_idx]].push_back(node);
    }
    
    // Recurse on each group of nodes.
    for(auto const& [p_node, sub_nodes] : groups) {
        solve(sub_nodes, p_node);
    }
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    if (n == 1) {
        cout << "!" << endl;
        cout.flush();
        return 0;
    }
    
    vector<int> all_nodes(n);
    iota(all_nodes.begin(), all_nodes.end(), 1);
    
    solve(all_nodes, 0);

    cout << "!" << endl;
    for (const auto& edge : edges) {
        cout << edge.first << " " << edge.second << endl;
    }
    cout.flush();

    return 0;
}