#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

// Function to perform a swap operation
void do_swap(int i, int j) {
    if (i == j) return;
    std::cout << "2 " << i << " " << j << std::endl;
    int resp;
    std::cin >> resp;
}

// Function to perform a query operation
int do_ask(int l, int r) {
    if (l > r) return 0;
    std::cout << "1 " << l << " " << r << std::endl;
    int resp;
    std::cin >> resp;
    return resp;
}

// Function to report the final answer
void do_report(const std::vector<int>& p) {
    std::cout << "3";
    for (size_t i = 1; i < p.size(); ++i) {
        std::cout << " " << p[i];
    }
    std::cout << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n, l1, l2;
    std::cin >> n >> l1 >> l2;
    
    if (n == 1) {
        do_report({0, 1});
        return 0;
    }

    // Build an adjacency list of indices. Two indices are connected
    // if their corresponding values in the permutation are adjacent (e.g., v and v+1).
    std::vector<std::vector<int>> adj(n + 1);
    for (int i = 1; i <= n; ++i) {
        for (int j = i + 1; j <= n; ++j) {
            // To check if p[i] and p[j] are value-adjacent,
            // we move them to positions 1 and 2 and check C(1,2).
            do_swap(1, i);
            do_swap(2, j);
            if (do_ask(1, 2) == 3) {
                adj[i].push_back(j);
                adj[j].push_back(i);
            }
            // Swap back to restore the original permutation state for the next check.
            do_swap(2, j);
            do_swap(1, i);
        }
    }

    // A permutation's value-adjacency graph is always a single path.
    // Find an endpoint of this path (a node with degree 1).
    int start_node = -1;
    for (int i = 1; i <= n; ++i) {
        if (adj[i].size() == 1) {
            start_node = i;
            break;
        }
    }
    // For n=2, both nodes have degree 1. For n>2, there are two endpoints.
    // If n>1 and no node has degree 1, something is wrong (e.g., a cycle).
    // This case should not happen for a permutation. We pick 1 as a fallback.
    if (n > 1 && start_node == -1) {
      start_node = 1;
    }

    // Traverse the path to find the ordered chain of indices.
    std::vector<int> chain(n);
    std::vector<bool> visited(n + 1, false);
    int curr = start_node;
    int prev = -1;
    for (int i = 0; i < n; ++i) {
        chain[i] = curr;
        visited[curr] = true;
        for (int neighbor : adj[curr]) {
            if (neighbor != prev) {
                prev = curr;
                curr = neighbor;
                break;
            }
        }
    }

    // We now have the chain of indices. The values could be increasing (1, 2, ..., n)
    // or decreasing (n, n-1, ..., 1) along this chain.
    std::vector<int> p(n + 1);
    for (int i = 0; i < n; ++i) {
        p[chain[i]] = i + 1;
    }
    
    // To determine the correct orientation, we check an endpoint property.
    // A permutation 1, 2, ..., n has n value-contiguous prefixes starting at index 1.
    // A permutation n, n-1, ..., 1 has only one such prefix (the element itself).
    // We can count these prefixes using C(1,n) - C(2,n).
    
    // Move the elements corresponding to values 1 and 2 to positions 1 and 2.
    do_swap(1, chain[0]);
    do_swap(2, chain[1]);
    
    long pref_count1 = do_ask(1, n) - do_ask(2, n);
    
    // Swap back to restore state.
    do_swap(2, chain[1]);
    do_swap(1, chain[0]);
    
    // Now check the reversed sequence. Move elements for n and n-1 to pos 1 and 2.
    do_swap(1, chain[n-1]);
    do_swap(2, chain[n-2]);

    long pref_count2 = do_ask(1, n) - do_ask(2, n);
    
    // Swap back.
    do_swap(2, chain[n-2]);
    do_swap(1, chain[n-1]);

    // If the reversed sequence gives more contiguous prefixes, our initial assumption was wrong.
    if (pref_count2 > pref_count1) {
      for(int i = 1; i <= n; ++i) {
          p[i] = n + 1 - p[i];
      }
    }
    
    do_report(p);

    return 0;
}