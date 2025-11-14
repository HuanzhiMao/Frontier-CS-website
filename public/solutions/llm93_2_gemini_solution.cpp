#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

// Function to ask a query. Handles printing and reading.
int ask_query(const std::vector<int>& nodes) {
    std::cout << "? " << nodes.size();
    for (int node : nodes) {
        std::cout << " " << node;
    }
    std::cout << std::endl;
    int result;
    std::cin >> result;
    return result;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    int ty; // ty is unused
    std::cin >> n >> ty;

    if (n == 1) {
        std::cout << "! 0" << std::endl;
        return 0;
    }

    // Step 1: Calculate a "value" for each node.
    // This value, val[i], is effectively n - |Ancestors(i)| - |Descendants(i)|.
    // A smaller value means the node is closer to the root. Due to the tree properties,
    // for any node u and its descendant v, val[u] < val[v].
    std::vector<int> val(n + 1);
    std::vector<int> all_nodes(n);
    std::iota(all_nodes.begin(), all_nodes.end(), 1);
    for (int i = 1; i <= n; ++i) {
        std::vector<int> query_vec;
        query_vec.reserve(n);
        query_vec.push_back(i);
        for (int node : all_nodes) {
            if (node != i) {
                query_vec.push_back(node);
            }
        }
        val[i] = ask_query(query_vec);
    }

    // Step 2 & 3: Create a topologically sorted list of nodes based on their val.
    // The node with the smallest val is the root.
    std::vector<int> sorted_nodes(n);
    std::iota(sorted_nodes.begin(), sorted_nodes.end(), 1);
    std::sort(sorted_nodes.begin(), sorted_nodes.end(), [&](int a, int b) {
        return val[a] < val[b];
    });

    // Step 4, 5, 6: Determine parent for each node.
    std::vector<int> par(n + 1);
    // The first node in the sorted list is the root.
    par[sorted_nodes[0]] = 0;

    // For each other node, its parent is its "deepest" ancestor.
    // In our sorted list, this corresponds to the ancestor with the largest index.
    for (int i = 1; i < n; ++i) {
        int u = sorted_nodes[i];
        
        // Binary search for the parent's index in the prefix of the sorted list.
        int low = 0, high = i - 1;
        int p_idx = 0; // The parent's index in sorted_nodes

        while (low <= high) {
            int mid = low + (high - low) / 2;
            int v = sorted_nodes[mid];
            
            // Query if u and v are on the same path (one is an ancestor of the other).
            // Since val[v] < val[u], if they are related, v must be an ancestor of u.
            if (ask_query({u, v}) == 1) {
                // v is an ancestor. It's a candidate for the parent.
                // Try to find an ancestor with a larger index (deeper in the tree).
                p_idx = mid;
                low = mid + 1;
            } else {
                // v is not an ancestor. The parent must have been seen earlier
                // (have a smaller index in sorted_nodes).
                high = mid - 1;
            }
        }
        par[u] = sorted_nodes[p_idx];
    }

    // Step 7: Print the final answer.
    std::cout << "!";
    for (int i = 1; i <= n; ++i) {
        std::cout << " " << par[i];
    }
    std::cout << std::endl;

    return 0;
}