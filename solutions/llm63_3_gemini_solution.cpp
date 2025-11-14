#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <queue>

int N, M;
std::vector<std::pair<int, int>> edges;

int ask_query(const std::vector<int>& dirs) {
    std::cout << 0;
    for (int dir : dirs) {
        std::cout << " " << dir;
    }
    std::cout << std::endl;
    int response;
    std::cin >> response;
    if (response == -1) exit(0);
    return response;
}

// Q(S) query: orient edges from S to V\S.
// returns 1 if NOT (A in V\S and B in S), 0 otherwise
int do_partition_query(const std::vector<bool>& in_S) {
    std::vector<int> dirs(M);
    for (int i = 0; i < M; ++i) {
        bool u_in = in_S[edges[i].first];
        bool v_in = in_S[edges[i].second];
        if (u_in && !v_in) {
            dirs[i] = 0; // U_i -> V_i
        } else if (!u_in && v_in) {
            dirs[i] = 1; // V_i -> U_i
        } else {
            dirs[i] = 0; // Arbitrary, U_i -> V_i
        }
    }
    return ask_query(dirs);
}

// Q(r) query: orient edges away from r
// returns 1 if A can reach B, which is equivalent to dist(A,r) <= dist(B,r)
int do_dist_query(int r, const std::vector<std::vector<int>>& adj, std::vector<int>& dist) {
    std::fill(dist.begin(), dist.end(), -1);
    
    std::vector<int> q_vec;
    q_vec.reserve(N);
    q_vec.push_back(r);
    dist[r] = 0;
    int head = 0;

    while(head < q_vec.size()){
        int u = q_vec[head++];
        for (int v : adj[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q_vec.push_back(v);
            }
        }
    }

    std::vector<int> dirs(M);
    for (int i = 0; i < M; ++i) {
        int u = edges[i].first;
        int v = edges[i].second;
        if (dist[u] < dist[v]) {
            dirs[i] = 0; // u -> v
        } else if (dist[v] < dist[u]) {
            dirs[i] = 1; // v -> u
        } else {
            dirs[i] = 0; // arbitrary for same-level edges
        }
    }
    return ask_query(dirs);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::cin >> N >> M;
    edges.resize(M);
    std::vector<std::vector<int>> adj(N);
    for (int i = 0; i < M; ++i) {
        std::cin >> edges[i].first >> edges[i].second;
        adj[edges[i].first].push_back(edges[i].second);
        adj[edges[i].second].push_back(edges[i].first);
    }

    int L = 0;
    if (N > 1) {
        L = std::floor(std::log2(N - 1)) + 1;
    }
    
    // Phase 1: Find C = A ^ B
    int C = 0;
    for (int k = 0; k < L; ++k) {
        std::vector<bool> in_S(N, false);
        for (int i = 0; i < N; ++i) {
            if ((i >> k) & 1) {
                in_S[i] = true;
            }
        }
        int res_S = do_partition_query(in_S);

        std::vector<bool> in_VS(N, false);
        for (int i = 0; i < N; ++i) {
            if (!((i >> k) & 1)) {
                in_VS[i] = true;
            }
        }
        int res_VS = do_partition_query(in_VS);

        if (res_S == 1 && res_VS == 0) { // A in S, B in V\S -> a_k=1, b_k=0
            C |= (1 << k);
        } else if (res_S == 0 && res_VS == 1) { // A in V\S, B in S -> a_k=0, b_k=1
            C |= (1 << k);
        }
    }

    // Phase 2: Determine bits of A where c_k=1
    int A_known_bits = 0;
    int A_mask = 0;

    for (int k = 0; k < L; ++k) {
        if ((C >> k) & 1) {
            std::vector<bool> in_S(N, false);
            for (int i = 0; i < N; ++i) {
                if ((i >> k) & 1) {
                    in_S[i] = true;
                }
            }
            int res = do_partition_query(in_S);
            // res = 0 -> A in V\S, B in S -> a_k=0
            // res = 1 -> A in S or B in V\S -> a_k=1
            if (res == 1) {
                A_known_bits |= (1 << k);
            }
            A_mask |= (1 << k);
        }
    }

    std::vector<int> candA;
    candA.reserve(N);
    for (int i = 0; i < N; ++i) {
        if ((i & A_mask) == A_known_bits) {
            candA.push_back(i);
        }
    }

    // Phase 3: Binary search for A among remaining candidates using distance queries
    std::vector<int> dist(N);
    while (candA.size() > 1) {
        int r = candA[0];
        int res = do_dist_query(r, adj, dist);

        std::vector<int> next_candA;
        next_candA.reserve(candA.size());
        for (int a_cand : candA) {
            int b_cand = a_cand ^ C;
            if (res == 1) { // dist(A,r) <= dist(B,r)
                if (dist[a_cand] <= dist[b_cand]) {
                    next_candA.push_back(a_cand);
                }
            } else { // dist(A,r) > dist(B,r)
                if (dist[a_cand] > dist[b_cand]) {
                    next_candA.push_back(a_cand);
                }
            }
        }
        candA = next_candA;
    }

    int A = candA[0];
    int B = A ^ C;

    std::cout << 1 << " " << A << " " << B << std::endl;

    return 0;
}