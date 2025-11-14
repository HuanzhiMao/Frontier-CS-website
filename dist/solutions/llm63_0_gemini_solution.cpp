#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <random>
#include <chrono>

// Globals for graph
int N, M;
std::vector<std::pair<int, int>> edges;

// Function to ask a query
int ask_query(const std::vector<int>& dirs) {
    std::cout << 0;
    for (int dir : dirs) {
        std::cout << " " << dir;
    }
    std::cout << std::endl;
    int response;
    std::cin >> response;
    if (response == -1) exit(0); // Judge may terminate early on error
    return response;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::cin >> N >> M;
    edges.resize(M);
    for (int i = 0; i < M; ++i) {
        std::cin >> edges[i].first >> edges[i].second;
    }

    // Part 1: Find C = A XOR B
    int C = 0;
    int logN = 0;
    if (N > 1) {
        logN = std::floor(std::log2(N - 1)) + 1;
    }

    for (int b = 0; b < logN; ++b) {
        std::vector<int> d1(M), d2(M);
        for (int i = 0; i < M; ++i) {
            auto [u, v] = edges[i];
            bool u_bit_is_1 = (u >> b) & 1;
            bool v_bit_is_1 = (v >> b) & 1;

            if (u_bit_is_1 == v_bit_is_1) {
                d1[i] = d2[i] = 0; // Arbitrary fixed direction for non-crossing edges
            } else {
                if (!u_bit_is_1) { // u is in S0, v is in S1
                    d1[i] = 0; // S0 -> S1 requires U_i -> V_i
                    d2[i] = 1; // S1 -> S0 requires V_i -> U_i
                } else { // u is in S1, v is in S0
                    d1[i] = 1; // S0 -> S1 requires V_i -> U_i
                    d2[i] = 0; // S1 -> S0 requires U_i -> V_i
                }
            }
        }
        int r1 = ask_query(d1);
        int r2 = ask_query(d2);
        if (r1 != r2) {
            C |= (1 << b);
        }
    }

    // Part 2: Find A
    std::vector<int> p(N);
    std::iota(p.begin(), p.end(), 0);

    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

    while (p.size() > 1) {
        std::vector<bool> s(N);
        for (int i = 0; i < N; ++i) {
            s[i] = std::uniform_int_distribution<int>(0, 1)(rng);
        }

        std::vector<int> q1(M), q2(M);
        for (int i = 0; i < M; ++i) {
            auto [u, v] = edges[i];
            if (s[u] == s[v]) {
                q1[i] = q2[i] = 0; // Arbitrary
            } else {
                if (!s[u] && s[v]) { // u in V\S, v in S
                    q1[i] = 0; // into S (V\S -> S) requires U_i -> V_i
                    q2[i] = 1; // out of S (S -> V\S) requires V_i -> U_i
                } else { // u in S, v in V\S
                    q1[i] = 1; // into S (V\S -> S) requires V_i -> U_i
                    q2[i] = 0; // out of S (S -> V\S) requires U_i -> V_i
                }
            }
        }
        
        int r1 = ask_query(q1); // Query Q(S): orient into S
        int r2 = ask_query(q2); // Query Q(V\S): orient out of S

        std::vector<int> p_new;
        for (int a : p) {
            bool a_in_s = s[a];
            bool b_in_s = s[a ^ C];

            bool is_case1 = !a_in_s && b_in_s; // A in V\S, B in S
            bool is_case2 = a_in_s && !b_in_s; // A in S, B in V\S

            if (r1 == 0) { // True A, B must satisfy case 1
                if (is_case1) p_new.push_back(a);
            } else if (r2 == 0) { // True A, B must satisfy case 2
                if (is_case2) p_new.push_back(a);
            } else { // True A, B in same partition
                if (!is_case1 && !is_case2) p_new.push_back(a);
            }
        }
        
        // This case is unlikely with random partitions but possible.
        // If no candidates are eliminated, just try another random partition.
        if (p_new.size() == p.size() && p.size() > 1) {
            continue;
        }
        p = p_new;
    }

    int A = p[0];
    int B = A ^ C;
    
    std::cout << 1 << " " << A << " " << B << std::endl;

    return 0;
}