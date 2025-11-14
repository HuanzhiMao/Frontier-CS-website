#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

// Function to ask a query
int query(int u, int v, int w) {
    std::cout << "0 " << u << " " << v << " " << w << std::endl;
    int result;
    std::cin >> result;
    if (result == -1) exit(0); // Exit on error in judge
    return result;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    // Step 1: Find a leaf node to use as a root.
    // A leaf is an endpoint of some path. We find two endpoints of a path.
    int u1 = 1, u2 = 2;
    for (int i = 3; i <= n; ++i) {
        int m = query(u1, u2, i);
        if (m == u1) {
            // u1 is on the path between u2 and i. Path is u2 ... u1 ... i.
            // The new endpoints of the path covering {u1, u2, i} are u2 and i.
            u1 = i;
        } else if (m == u2) {
            // u2 is on the path between u1 and i. Path is u1 ... u2 ... i.
            // The new endpoints are u1 and i.
            u2 = i;
        }
        // If m == i, i is on the path between u1 and u2. Endpoints remain u1, u2.
    }
    
    // u1 is a leaf. Let's use it as the root.
    int root = u1;

    std::vector<int> parents(n + 1, 0);

    // Step 2: For each node, find its parent relative to the root.
    for (int i = 1; i <= n; ++i) {
        if (i == root) continue;

        // An ancestor of i is a node j on the path from root to i.
        // This means query(root, i, j) will return j.
        std::vector<int> ancestors;
        for (int j = 1; j <= n; ++j) {
            if (i == j || j == root) continue;
            if (query(root, i, j) == j) {
                ancestors.push_back(j);
            }
        }

        if (ancestors.empty()) {
            // No node is between root and i, so root is the parent.
            parents[i] = root;
        } else {
            // The parent is the ancestor that is farthest from the root.
            // Among two ancestors p and q, if query(root, p, q) == p,
            // p is closer to root, so q is farther.
            int parent_candidate = ancestors[0];
            for (size_t k = 1; k < ancestors.size(); ++k) {
                if (query(root, parent_candidate, ancestors[k]) == parent_candidate) {
                    parent_candidate = ancestors[k];
                }
            }
            parents[i] = parent_candidate;
        }
    }

    // Step 3: Output the edges.
    std::cout << "1";
    for (int i = 1; i <= n; ++i) {
        if (i == root) continue;
        std::cout << " " << i << " " << parents[i];
    }
    std::cout << std::endl;

    return 0;
}