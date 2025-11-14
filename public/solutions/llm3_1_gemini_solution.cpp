#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>

// Using global variables for convenience in competitive programming
int subtask, n;
std::vector<int> adj[100005];

// Function to perform a query
std::vector<int> do_query(const std::vector<int>& q) {
    if (q.empty()) {
        return {};
    }
    std::cout << q.size();
    for (int x : q) {
        std::cout << " " << x;
    }
    std::cout << std::endl;
    std::vector<int> res(q.size());
    for (size_t i = 0; i < q.size(); ++i) {
        std::cin >> res[i];
    }
    return res;
}

// Main logic
void solve() {
    std::cin >> subtask >> n;

    if (n <= 2) {
        std::cout << -1;
        for (int i = 1; i <= n; ++i) std::cout << " " << i;
        std::cout << std::endl;
        return;
    }

    // Step 1: Find an independent set I
    int B = std::min((int)n, (int)(2 * sqrt(n) + 20));
    if (n <= 1000) B = n; 
    
    std::vector<int> initial_block;
    for (int i = 1; i <= B && i <= n; ++i) {
        initial_block.push_back(i);
    }

    std::vector<int> res = do_query(initial_block);
    
    std::vector<int> I;
    int first_one = -1;
    for (size_t i = 0; i < res.size(); ++i) {
        if (res[i] == 1) {
            first_one = i + 1;
            break;
        }
    }

    if (first_one == -1) {
        I = initial_block;
    } else {
        for (int i = 1; i < first_one; ++i) {
            I.push_back(i);
        }
    }
    // To ensure S is empty for the next batch query
    if(first_one != -1) {
        std::vector<int> cleanup_q;
        for(int i=1; i<=first_one; ++i) cleanup_q.push_back(i);
        do_query(cleanup_q);
    } else {
        do_query(initial_block);
    }


    std::vector<bool> is_in_I(n + 1, false);
    for (int u : I) {
        is_in_I[u] = true;
    }

    std::vector<int> C;
    for (int i = 1; i <= n; ++i) {
        if (!is_in_I[i]) {
            C.push_back(i);
        }
    }

    // Step 2: Find I-C edges
    std::vector<int> query_ic;
    std::vector<std::pair<int, int>> ic_pairs;
    for (int u : I) {
        for (int v : C) {
            query_ic.push_back(u); query_ic.push_back(v);
            query_ic.push_back(u); query_ic.push_back(v);
            ic_pairs.push_back({u, v});
        }
    }
    if (!query_ic.empty()) {
        std::vector<int> res_ic = do_query(query_ic);
        for (size_t i = 0; i < ic_pairs.size(); ++i) {
            if (res_ic[i * 4 + 1] == 1) {
                adj[ic_pairs[i].first].push_back(ic_pairs[i].second);
                adj[ic_pairs[i].second].push_back(ic_pairs[i].first);
            }
        }
    }

    std::vector<int> C0, C1;
    for (int v : C) {
        if (adj[v].size() == 0) C0.push_back(v);
        else if (adj[v].size() == 1) C1.push_back(v);
    }
    
    std::vector<int> C01 = C0;
    C01.insert(C01.end(), C1.begin(), C1.end());
    std::sort(C01.begin(), C01.end());

    // Step 3: Find C1 - C01 edges
    std::vector<int> query_c1;
    std::vector<std::pair<int, int>> c1_pairs;

    for (int u : C1) {
        for (int v : C01) {
            if (u < v) {
                query_c1.push_back(u); query_c1.push_back(v);
                query_c1.push_back(u); query_c1.push_back(v);
                c1_pairs.push_back({u, v});
            }
        }
    }
    if (!query_c1.empty()) {
        std::vector<int> res_c1 = do_query(query_c1);
        for (size_t i = 0; i < c1_pairs.size(); ++i) {
            if (res_c1[i * 4 + 1] == 1) {
                adj[c1_pairs[i].first].push_back(c1_pairs[i].second);
                adj[c1_pairs[i].second].push_back(c1_pairs[i].first);
            }
        }
    }

    // Step 4: Find C0-C0 edges
    std::vector<int> C0_unsolved;
    for(int u : C0) {
        if (adj[u].size() < 2) {
            C0_unsolved.push_back(u);
        }
    }

    for (int u : C0_unsolved) {
        if (adj[u].size() < 2) {
            std::vector<int> query_u;
            std::vector<int> candidates;
            for (int v : C0_unsolved) {
                if (u >= v) continue;
                if (adj[v].size() < 2) {
                    bool already_adj = false;
                    for (int neighbor : adj[u]) if (neighbor == v) already_adj = true;
                    if (already_adj) continue;

                    query_u.push_back(u); query_u.push_back(v);
                    query_u.push_back(u); query_u.push_back(v);
                    candidates.push_back(v);
                }
            }

            if (!query_u.empty()) {
                std::vector<int> res_u = do_query(query_u);
                for(size_t i=0; i<candidates.size(); ++i) {
                    if (adj[u].size() == 2) break;
                    if(res_u[i*4 + 1] == 1) {
                        adj[u].push_back(candidates[i]);
                        adj[candidates[i]].push_back(u);
                    }
                }
            }
        }
    }

    // Step 5: Reconstruct the cycle
    std::vector<int> p(n);
    int start_node = 1;
    
    int curr = start_node;
    int prev = -1;
    for (int i = 0; i < n; ++i) {
        p[i] = curr;
        int next_node = -1;
        for (int neighbor : adj[curr]) {
            if (neighbor != prev) {
                next_node = neighbor;
                break;
            }
        }
        prev = curr;
        curr = next_node;
    }

    std::cout << -1;
    for (int i = 0; i < n; ++i) {
        std::cout << " " << p[i];
    }
    std::cout << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    solve();
    return 0;
}