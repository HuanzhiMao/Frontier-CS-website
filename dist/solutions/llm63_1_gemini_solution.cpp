#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

int N, M;
std::vector<std::pair<int, int>> edges;

// Asks a query and returns the result
int ask_query(const std::vector<int>& directions) {
    std::cout << 0;
    for (int d : directions) {
        std::cout << " " << d;
    }
    std::cout << std::endl;
    int result;
    std::cin >> result;
    if (result == -1) exit(0);
    return result;
}

// Submits the final answer
void submit_answer(int a, int b) {
    std::cout << 1 << " " << a << " " << b << std::endl;
}

// Determines if A and B are on different sides of the partition S={0..k}, V\S.
// Returns:
// 0: A in S, B in V\S
// 1: A in V\S, B in S
// 2: A,B in same partition
int query_partition(int k) {
    // Note: edges are given as (U_i, V_i) with U_i < V_i.
    // A direction of 0 means U_i -> V_i.
    // A direction of 1 means V_i -> U_i.

    std::vector<int> s_l_to_s_r_dirs(M);
    std::vector<int> s_r_to_s_l_dirs(M);

    for (int i = 0; i < M; ++i) {
        auto [u, v] = edges[i];
        bool u_in_sl = (u <= k);
        bool v_in_sl = (v <= k);

        if (u_in_sl != v_in_sl) {
            // This is a cut edge. u is guaranteed to be in S_L, v in S_R.
            s_l_to_s_r_dirs[i] = 0; // u -> v (S_L -> S_R)
            s_r_to_s_l_dirs[i] = 1; // v -> u (S_R -> S_L)
        } else {
            // Internal edge, orient u -> v (as u < v)
            s_l_to_s_r_dirs[i] = 0;
            s_r_to_s_l_dirs[i] = 0;
        }
    }

    int res_sl_to_sr = ask_query(s_l_to_s_r_dirs);
    int res_sr_to_sl = ask_query(s_r_to_s_l_dirs);

    if (res_sl_to_sr == 1 && res_sr_to_sl == 0) return 0; // Path only possible S_L -> S_R
    if (res_sl_to_sr == 0 && res_sr_to_sl == 1) return 1; // Path only possible S_R -> S_L
    
    // Both 1 or both 0. In either case, they are on the same side.
    // If both 0, it means any path between them requires crossing the partition boundary twice,
    // which is blocked in both directions. So they must be on the same side.
    return 2;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::cin >> N >> M;
    edges.resize(M);
    for (int i = 0; i < M; ++i) {
        std::cin >> edges[i].first >> edges[i].second;
    }

    // Binary search for the first split point, which identifies min(A, B).
    int low = 0, high = N - 2;
    int min_ab_k = -1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        int res = query_partition(mid);
        if (res == 0 || res == 1) { // A and B are on different sides
            min_ab_k = mid;
            high = mid - 1;
        } else { // A and B are on the same side
            low = mid + 1;
        }
    }
    int u_cand = min_ab_k;

    // Binary search for the last split point, which identifies max(A, B).
    low = 0, high = N - 2;
    int max_ab_k = -1;
    while (low <= high) {
        int mid = low + (high-low) / 2;
        int res = query_partition(mid);
        if (res == 0 || res == 1) { // A and B are on different sides
            max_ab_k = mid;
            low = mid + 1;
        } else { // A and B are on the same side
            high = mid - 1;
        }
    }
    int v_cand = max_ab_k + 1;

    // Distinguish A from B
    // Test if A is u_cand
    std::vector<int> directions(M);
    for (int i = 0; i < M; ++i) {
        if (edges[i].first == u_cand) { // edge is (u_cand, V_i)
            directions[i] = 0; // u_cand -> V_i
        } else if (edges[i].second == u_cand) { // edge is (U_i, u_cand)
            directions[i] = 1; // u_cand -> U_i
        } else {
            // Other edges u -> v for u < v
            directions[i] = 0;
        }
    }

    if (ask_query(directions) == 1) {
        submit_answer(u_cand, v_cand);
    } else {
        submit_answer(v_cand, u_cand);
    }

    return 0;
}