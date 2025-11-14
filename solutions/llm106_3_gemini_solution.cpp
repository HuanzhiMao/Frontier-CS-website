#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <map>

int n;
std::map<std::vector<int>, int> cache;

// Query function with caching
int ask(std::vector<int> s) {
    if (s.empty()) {
        return 0;
    }
    std::sort(s.begin(), s.end());
    if (cache.count(s)) {
        return cache[s];
    }

    std::cout << "? " << s.size() << std::endl;
    for (size_t i = 0; i < s.size(); ++i) {
        std::cout << s[i] << (i == s.size() - 1 ? "" : " ");
    }
    std::cout << std::endl;

    int m;
    std::cin >> m;
    if (m == -1) exit(0);
    return cache[s] = m;
}

std::vector<int> P[2];
std::vector<int> color;
std::vector<int> parent;

// Helper to combine a vector and a single element
std::vector<int> combine(const std::vector<int>& v1, int element) {
    std::vector<int> res = v1;
    res.push_back(element);
    return res;
}
// Helper to combine two vectors
std::vector<int> combine(const std::vector<int>& v1, const std::vector<int>& v2) {
    std::vector<int> res = v1;
    res.insert(res.end(), v2.begin(), v2.end());
    return res;
}

// Function to find one neighbor of v in a set S
int find_neighbor(int v, const std::vector<int>& S) {
    if (S.empty()) return -1;
    int low = 0, high = S.size() - 1;
    while(low < high) {
        int mid = low + (high - low) / 2;
        std::vector<int> prefix;
        for(int i = low; i <= mid; ++i) prefix.push_back(S[i]);
        
        if (ask(combine(prefix, v)) > ask(prefix)) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    return S[low];
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::cin >> n;
    
    color.assign(n + 1, -1);
    parent.assign(n + 1, 0);

    P[0].push_back(1);
    color[1] = 0;
    
    std::vector<int> uncolored;
    for (int i = 2; i <= n; ++i) {
        uncolored.push_back(i);
    }
    
    while (!uncolored.empty()) {
        std::vector<int> C = combine(P[0], P[1]);
        int C_edges = ask(C);
        
        int low = 0, high = uncolored.size() - 1;
        while(low < high) {
            int mid = low + (high - low) / 2;
            std::vector<int> prefix;
            for(int j = low; j <= mid; ++j) prefix.push_back(uncolored[j]);
            
            int edges_between = ask(combine(C, prefix)) - C_edges - ask(prefix);
            if(edges_between > 0) {
                high = mid;
            } else {
                low = mid + 1;
            }
        }
        
        int v_idx = low;
        int v = uncolored[v_idx];
        
        uncolored.erase(uncolored.begin() + v_idx);

        bool conn0 = (ask(combine(P[0], v)) > ask(P[0]));
        bool conn1 = (ask(combine(P[1], v)) > ask(P[1]));
        
        if (conn0 && conn1) {
            int u0 = find_neighbor(v, P[0]);
            int u1 = find_neighbor(v, P[1]);
            
            std::vector<int> path0, path1;
            int curr = u0;
            while(curr != 0) {
                path0.push_back(curr);
                if (curr == 1) break;
                curr = parent[curr];
            }
            curr = u1;
            while(curr != 0) {
                path1.push_back(curr);
                if (curr == 1) break;
                curr = parent[curr];
            }
            std::reverse(path0.begin(), path0.end());
            std::reverse(path1.begin(), path1.end());

            size_t lca_depth = 0;
            while(lca_depth < path0.size() && lca_depth < path1.size() && path0[lca_depth] == path1[lca_depth]) {
                lca_depth++;
            }
            lca_depth--;

            std::vector<int> cycle;
            cycle.push_back(v);
            for(int j = path0.size() - 1; j >= (int)lca_depth; --j) {
                cycle.push_back(path0[j]);
            }
            for(size_t j = lca_depth + 1; j < path1.size(); ++j) {
                cycle.push_back(path1[j]);
            }

            std::cout << "N " << cycle.size() << std::endl;
            for(size_t j = 0; j < cycle.size(); ++j) {
                std::cout << cycle[j] << (j == cycle.size() - 1 ? "" : " ");
            }
            std::cout << std::endl;
            return 0;
        }
        
        if (conn0) {
            color[v] = 1;
            parent[v] = find_neighbor(v, P[0]);
            P[1].push_back(v);
        } else {
            color[v] = 0;
            parent[v] = find_neighbor(v, P[1]);
            P[0].push_back(v);
        }
    }
    
    std::cout << "Y " << P[0].size() << std::endl;
    for(size_t j = 0; j < P[0].size(); ++j) {
        std::cout << P[0][j] << (j == P[0].size() - 1 ? "" : " ");
    }
    std::cout << std::endl;

    return 0;
}