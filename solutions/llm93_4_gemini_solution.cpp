#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>

/**
 * @brief Performs a query to the black box.
 * 
 * @param nodes A vector of distinct node labels for the query.
 * @return The integer result from the black box.
 */
int do_query(const std::vector<int>& nodes) {
    if (nodes.empty()) {
        return 0;
    }
    std::cout << "? " << nodes.size();
    for (int node : nodes) {
        std::cout << " " << node;
    }
    std::cout << std::endl;
    int result;
    std::cin >> result;
    return result;
}

/**
 * @brief Main logic to solve the problem.
 */
void solve() {
    int n, ty;
    std::cin >> n >> ty;

    std::vector<int> parent(n + 1);
    std::vector<int> depth(n + 1);

    // Step 1: Topologically sort nodes based on a value related to depth and subtree size.
    std::vector<std::pair<int, int>> sorted_nodes_pairs;
    for (int i = 1; i <= n; ++i) {
        std::vector<int> query_vec;
        query_vec.reserve(n);
        query_vec.push_back(i);
        for (int j = 1; j <= n; ++j) {
            if (i != j) {
                query_vec.push_back(j);
            }
        }
        int f_val = do_query(query_vec);
        sorted_nodes_pairs.push_back({f_val, i});
    }
    std::sort(sorted_nodes_pairs.begin(), sorted_nodes_pairs.end());

    std::vector<int> u;
    u.reserve(n);
    for (const auto& p : sorted_nodes_pairs) {
        u.push_back(p.second);
    }

    int root = u[0];
    parent[root] = 0;
    depth[root] = 0;

    // Step 2: Determine depths of all nodes using the topological sort.
    std::vector<int> prefix;
    prefix.reserve(n);
    prefix.push_back(u[0]);
    for (int i = 1; i < n; ++i) {
        int curr_node = u[i];
        std::vector<int> query_vec;
        query_vec.reserve(i + 1);
        query_vec.push_back(curr_node);
        query_vec.insert(query_vec.end(), prefix.begin(), prefix.end());
        int res = do_query(query_vec);
        depth[curr_node] = (i + 1) - res;
        prefix.push_back(curr_node);
    }
    
    // Step 3: Find parents by binary searching on candidates from the topological sort.
    for (int i = 1; i < n; ++i) {
        int curr_node = u[i];
        int d = depth[curr_node];
        
        std::vector<int> cands;
        for (int j = 0; j < i; ++j) {
            if (depth[u[j]] == d - 1) {
                cands.push_back(u[j]);
            }
        }

        if (cands.size() == 1) {
            parent[curr_node] = cands[0];
            continue;
        }
        if (cands.empty()) {
            // This case should not be reached in a valid tree.
            continue;
        }

        int low = 0, high = cands.size() - 1;
        while (low < high) {
            int mid = low + (high - low) / 2;
            std::vector<int> query_vec;
            query_vec.reserve(mid - low + 2);
            query_vec.push_back(curr_node);
            for (int k = low; k <= mid; ++k) {
                query_vec.push_back(cands[k]);
            }
            int group_size = mid - low + 1;
            int res = do_query(query_vec);

            if (res < group_size + 1) { // Parent is in this range [low, mid]
                high = mid;
            } else { // Parent is not in this range, so it's in [mid+1, high]
                low = mid + 1;
            }
        }
        parent[curr_node] = cands[low];
    }

    // Output the final answer.
    std::cout << "!";
    for (int i = 1; i <= n; ++i) {
        std::cout << " " << parent[i];
    }
    std::cout << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    solve();
    return 0;
}