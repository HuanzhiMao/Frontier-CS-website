#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

// Global variable for the total number of vertices.
int N;
// Stores the reconstructed edges of the tree.
vector<pair<int, int>> edges;

/**
 * @brief Sends a query to the judge to check if vertex v is on Steiner(S).
 * @param v The vertex to check.
 * @param s The set of vertices S.
 * @return True if v is on Steiner(S), false otherwise.
 */
bool query(int v, const vector<int>& s) {
    if (s.empty()) {
        return false;
    }
    cout << "? " << s.size() << " " << v;
    for (int u : s) {
        cout << " " << u;
    }
    cout << endl;
    int result;
    cin >> result;
    if (result == -1) exit(0); // Exit immediately on judge error.
    return result == 1;
}

/**
 * @brief A specialized query to check if a vertex v lies on the simple path between u1 and u2.
 * @param v The vertex to check.
 * @param u1 The first endpoint of the path.
 * @param u2 The second endpoint of the path.
 * @return True if v is on the path between u1 and u2, false otherwise.
 */
bool is_on_path(int v, int u1, int u2) {
    if (v == u1 || v == u2) return true;
    return query(v, {u1, u2});
}

/**
 * @brief Recursively reconstructs the tree structure for a given subtree.
 * @param root The root of the current subtree being considered.
 * @param nodes The set of nodes in the current subtree (excluding the root).
 */
void reconstruct(int root, vector<int> nodes) {
    if (nodes.empty()) {
        return;
    }

    // Partition nodes into groups, where each group belongs to a subtree of a distinct child of 'root'.
    vector<vector<int>> groups;
    if (!nodes.empty()) {
        vector<int> remaining_nodes = nodes;
        while (!remaining_nodes.empty()) {
            int pivot = remaining_nodes[0];
            vector<int> current_group;
            current_group.push_back(pivot);

            vector<int> next_remaining_nodes;
            for (size_t i = 1; i < remaining_nodes.size(); ++i) {
                int node = remaining_nodes[i];
                // 'root' is on the path between pivot and node iff they are in different subtrees.
                if (is_on_path(root, pivot, node)) {
                    next_remaining_nodes.push_back(node);
                } else {
                    current_group.push_back(node);
                }
            }
            groups.push_back(current_group);
            remaining_nodes = next_remaining_nodes;
        }
    }

    // For each group, find the child of 'root' and recurse.
    for (const auto& group : groups) {
        int child_of_root = -1;
        int min_dist = N + 2;

        // The child of 'root' is the node in the group closest to 'root'.
        // Distance is measured by the number of nodes on the path.
        for (int u : group) {
            int dist = 0;
            for (int v = 1; v <= N; ++v) {
                if (is_on_path(v, root, u)) {
                    dist++;
                }
            }
            if (dist < min_dist) {
                min_dist = dist;
                child_of_root = u;
            }
        }
        
        edges.push_back({root, child_of_root});
        
        // Prepare for the recursive call on the rest of the group.
        vector<int> subgroup_nodes;
        for (int node : group) {
            if (node != child_of_root) {
                subgroup_nodes.push_back(node);
            }
        }
        reconstruct(child_of_root, subgroup_nodes);
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> N;

    // Start reconstruction with root 1 and all other nodes.
    vector<int> initial_nodes;
    for (int i = 2; i <= N; ++i) {
        initial_nodes.push_back(i);
    }
    
    reconstruct(1, initial_nodes);

    // Output the final answer.
    cout << "!" << endl;
    for (const auto& edge : edges) {
        cout << edge.first << " " << edge.second << endl;
    }

    return 0;
}