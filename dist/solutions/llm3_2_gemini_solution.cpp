#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include <chrono>

int subtask, n;

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

// Function to submit the answer
void submit_answer(const std::vector<int>& p) {
    std::cout << -1;
    for (int x : p) {
        std::cout << " " << x;
    }
    std::cout << std::endl;
}

void solve_subtask1() {
    std::vector<std::vector<int>> adj(n + 1);
    std::vector<int> current_s;

    for (int i = 1; i <= n; ++i) {
        for (int j = i + 1; j <= n; ++j) {
            std::vector<int> q = {i, j};
            std::vector<int> res = do_query(q);
            if (res[1] == 1) {
                adj[i].push_back(j);
                adj[j].push_back(i);
            }
            do_query(q); // Clear S
        }
    }

    std::vector<int> p;
    std::vector<bool> visited(n + 1, false);
    int curr = 1;
    int prev = -1;
    for (int i = 0; i < n; ++i) {
        p.push_back(curr);
        visited[curr] = true;
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
    submit_answer(p);
}

void solve_subtask2() {
    int k = 200;
    if (n <= 5000) k = 100;

    std::vector<int> p_perm(n);
    std::iota(p_perm.begin(), p_perm.end(), 1);

    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

    std::vector<std::vector<bool>> is_local_min(k, std::vector<bool>(n + 1, false));
    std::vector<std::vector<bool>> is_local_max(k, std::vector<bool>(n + 1, false));
    
    std::vector<int> current_S_vec;

    for (int i = 0; i < k; ++i) {
        std::shuffle(p_perm.begin(), p_perm.end(), rng);
        
        do_query(current_S_vec); 
        current_S_vec.clear();

        auto res1 = do_query(p_perm);
        for(int node : p_perm) current_S_vec.push_back(node);

        for (int j = 0; j < n; ++j) {
            if (res1[j] == 0) {
                is_local_min[i][p_perm[j]] = true;
            }
        }

        std::reverse(p_perm.begin(), p_perm.end());
        
        do_query(current_S_vec); 
        current_S_vec.clear();

        auto res2 = do_query(p_perm);
        for(int node : p_perm) current_S_vec.push_back(node);

        for (int j = 0; j < n; ++j) {
            if (res2[j] == 0) {
                is_local_max[i][p_perm[j]] = true;
            }
        }
    }
    do_query(current_S_vec);
    current_S_vec.clear();

    std::vector<std::vector<int>> adj(n + 1);
    std::vector<std::vector<bool>> possible(n + 1, std::vector<bool>(n + 1, true));

    for (int r = 0; r < k; ++r) {
        std::vector<int> L_r, M_r;
        for (int i = 1; i <= n; ++i) {
            if (is_local_min[r][i]) L_r.push_back(i);
            if (is_local_max[r][i]) M_r.push_back(i);
        }
        for (size_t i = 0; i < L_r.size(); ++i) {
            for (size_t j = i + 1; j < L_r.size(); ++j) {
                possible[L_r[i]][L_r[j]] = false;
            }
        }
        for (size_t i = 0; i < M_r.size(); ++i) {
            for (size_t j = i + 1; j < M_r.size(); ++j) {
                possible[M_r[i]][M_r[j]] = false;
            }
        }
    }
    
    for (int i = 1; i <= n; ++i) {
        std::vector<int> cands;
        for (int j = i + 1; j <= n; ++j) {
            if (possible[i][j]) {
                cands.push_back(j);
            }
        }
        
        if (cands.size() <= 5) { // Threshold for when to switch to exact checking
            for (int cand : cands) {
                 if (adj[i].size() < 2 && adj[cand].size() < 2) {
                    std::vector<int> q = {i, cand};
                    auto res = do_query(q);
                    if (res[1] == 1) {
                        adj[i].push_back(cand);
                        adj[cand].push_back(i);
                    }
                    do_query(q);
                }
            }
        } else { // Hope for the best if too many candidates, likely won't happen
             for (int cand : cands) {
                if (adj[i].size() < 2) adj[i].push_back(cand);
                if (adj[cand].size() < 2) adj[cand].push_back(i);
             }
        }
    }

    std::vector<int> result_p;
    std::vector<bool> visited(n + 1, false);
    int curr = 1;
    int prev = -1;
    for (int i = 0; i < n; ++i) {
        result_p.push_back(curr);
        visited[curr] = true;
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
    submit_answer(result_p);
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    
    std::cin >> subtask >> n;

    if (n <= 2000) {
        solve_subtask1();
    } else {
        solve_subtask2();
    }

    return 0;
}