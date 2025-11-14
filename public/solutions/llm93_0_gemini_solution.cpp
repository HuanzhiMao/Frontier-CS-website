#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <functional>

int n, ty;
std::vector<std::vector<int>> memo_anc;

int do_query(const std::vector<int>& vec) {
    std::cout << "? " << vec.size();
    for (int x : vec) {
        std::cout << " " << x;
    }
    std::cout << std::endl;
    int result;
    std::cin >> result;
    return result;
}

// Returns 1 if u is ancestor of v, -1 if v is ancestor of u, 0 if unrelated
int get_relation(int u, int v) {
    if (u == v) return 0;
    if (memo_anc[u][v] != -2) {
        return memo_anc[u][v];
    }

    if (do_query({u, v}) == 2) {
        return memo_anc[u][v] = memo_anc[v][u] = 0;
    }

    std::vector<int> others;
    others.reserve(n - 2);
    for (int i = 1; i <= n; ++i) {
        if (i != u && i != v) {
            others.push_back(i);
        }
    }

    std::vector<int> q_uv = {u, v};
    q_uv.insert(q_uv.end(), others.begin(), others.end());
    int res_uv = do_query(q_uv);

    std::vector<int> q_vu = {v, u};
    q_vu.insert(q_vu.end(), others.begin(), others.end());
    int res_vu = do_query(q_vu);
    
    // Given that any non-leaf has at least 2 children, for an ancestor-descendant pair u, v,
    // the set of nodes related to u is strictly larger than the set of nodes related to v.
    // This implies that the number of nodes unrelated to u is strictly smaller.
    // The greedy selection on these unrelated nodes should also result in a smaller set.
    // Thus, res_uv should be strictly different from res_vu.
    if (res_uv < res_vu) {
        memo_anc[u][v] = 1;
        memo_anc[v][u] = -1;
    } else {
        memo_anc[u][v] = -1;
        memo_anc[v][u] = 1;
    }
    return memo_anc[u][v];
}

bool is_ancestor(int u, int v) {
    return get_relation(u, v) == 1;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::cin >> n >> ty;

    memo_anc.assign(n + 1, std::vector<int>(n + 1, -2));
    
    std::vector<int> p(n);
    std::iota(p.begin(), p.end(), 1);
    
    std::vector<int> val(n + 1);
    for (int i = 1; i <= n; ++i) {
        std::vector<int> q_vec;
        q_vec.push_back(i);
        for (int j = 1; j <= n; ++j) {
            if (i == j) continue;
            q_vec.push_back(j);
        }
        val[i] = do_query(q_vec);
    }
    
    std::sort(p.begin(), p.end(), [&](int i, int j) {
        if (val[i] != val[j]) {
            return val[i] < val[j];
        }
        return i < j;
    });

    std::vector<int> par(n + 1, 0);
    par[p[0]] = 0;

    for (size_t i = 1; i < p.size(); ++i) {
        int u = p[i];
        
        int low = 0, high = i - 1;
        int parent_idx = 0; 
        
        if (!is_ancestor(p[0], u)) {
             // This case suggests p[0] is not the root of the component containing u.
             // This can happen if the val-sort is not a perfect topological sort and multiple roots of components exist.
             // In a single tree, p[0] must be the root and an ancestor to all.
             // We can find an ancestor to start the binary search.
             bool found_ancestor = false;
             for(int j=0; j<i; ++j) {
                 if (is_ancestor(p[j], u)) {
                     low = j;
                     parent_idx = j;
                     found_ancestor = true;
                     break;
                 }
             }
             // If no ancestor found among p_0...p_{i-1}, u must be a child of the true root.
             // This situation is unlikely if val-sort works well.
             if(!found_ancestor) {
                 par[u] = p[0];
                 continue;
             }
        }
        
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (is_ancestor(p[mid], u)) {
                parent_idx = mid;
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        par[u] = p[parent_idx];
    }

    std::cout << "!";
    for (int i = 1; i <= n; ++i) {
        std::cout << " " << par[i];
    }
    std::cout << std::endl;

    return 0;
}