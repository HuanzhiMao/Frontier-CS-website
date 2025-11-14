#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <map>

// Function to make a query
int ask(const std::vector<int>& q) {
    if (q.empty()) {
        return 0;
    }
    std::cout << "? " << q.size();
    for (int x : q) {
        std::cout << " " << x;
    }
    std::cout << std::endl;
    int result;
    std::cin >> result;
    return result;
}

// Function to submit the answer
void answer(const std::vector<int>& p) {
    std::cout << "!";
    for (size_t i = 1; i < p.size(); ++i) {
        std::cout << " " << p[i];
    }
    std::cout << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n, ty;
    std::cin >> n >> ty;

    if (n == 1) {
        std::vector<int> p(n + 1, 0);
        answer(p);
        return 0;
    }

    std::vector<std::pair<int, int>> depth_proxy(n);
    std::vector<int> all_nodes;
    for (int i = 1; i <= n; ++i) {
        all_nodes.push_back(i);
    }

    for (int i = 1; i <= n; ++i) {
        std::vector<int> q_vec;
        q_vec.push_back(i);
        for (int j : all_nodes) {
            if (i == j) continue;
            q_vec.push_back(j);
        }
        int res = ask(q_vec);
        depth_proxy[i-1] = {res, i};
    }

    std::sort(depth_proxy.begin(), depth_proxy.end());

    std::vector<std::vector<int>> nodes_by_depth;
    if (n > 0) {
        nodes_by_depth.push_back({depth_proxy[0].second});
    }
    for (size_t i = 1; i < n; ++i) {
        if (depth_proxy[i].first > depth_proxy[i-1].first) {
            nodes_by_depth.push_back({});
        }
        nodes_by_depth.back().push_back(depth_proxy[i].second);
    }
    
    int root = nodes_by_depth[0][0];
    std::vector<int> par(n + 1, 0);
    par[root] = 0;

    for (size_t d = 1; d < nodes_by_depth.size(); ++d) {
        std::vector<int>& parents_level = nodes_by_depth[d - 1];
        std::vector<int>& children_level = nodes_by_depth[d];

        for (int child : children_level) {
            std::vector<int> cands = parents_level;
            
            while (cands.size() > 1) {
                int mid = cands.size() / 2;
                std::vector<int> left_half(cands.begin(), cands.begin() + mid);
                
                std::vector<int> q_vec;
                q_vec.push_back(child);
                q_vec.insert(q_vec.end(), left_half.begin(), left_half.end());
                
                int res = ask(q_vec);
                int num_related = left_half.size() + 1 - res;
                
                if (num_related > 0) {
                    cands = left_half;
                } else {
                    cands.assign(cands.begin() + mid, cands.end());
                }
            }
            par[child] = cands[0];
        }
    }

    answer(par);

    return 0;
}