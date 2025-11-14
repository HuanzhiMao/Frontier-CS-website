#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>

// Function to ask a query
int ask_query(int n, const std::vector<int>& q) {
    std::cout << "0";
    for (int i = 0; i < n; ++i) {
        std::cout << " " << q[i];
    }
    std::cout << std::endl;
    int result;
    std::cin >> result;
    return result;
}

// Function to submit the final answer
void submit_answer(int n, const std::vector<int>& p) {
    std::cout << "1";
    for (int i = 0; i < n; ++i) {
        std::cout << " " << p[i];
    }
    std::cout << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    std::vector<int> p(n);
    std::vector<int> p_inv(n + 1, 0);

    for (int i = 0; i * 2 < n; ++i) {
        int a = 2 * i + 1;
        int b = 2 * i + 2;

        // This function f(k) gives the answer to a query where the first k elements
        // are 'a' and the rest are 'b'. The answer is equivalent to:
        // ([position of a <= k]) + ([position of b > k]).
        // This function has a specific shape: it's 1, then changes to 0 or 2, then back to 1.
        // We find the two positions where it changes. These are the positions of a and b.
        auto f = [&](int k) {
            if (k == 0) return 1;
            std::vector<int> q(n);
            for (int j = 0; j < n; ++j) {
                q[j] = (j < k) ? a : b;
            }
            return ask_query(n, q);
        };

        // Binary search for p1 = min(pos_a, pos_b).
        // This is the first k where f(k) is not 1.
        int low = 1, high = n, p1 = n + 1;
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (f(mid) != 1) {
                p1 = mid;
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }

        // Binary search for p2 = max(pos_a, pos_b).
        // This is the first k >= p1 where f(k) changes back to 1.
        // We find the last k where f(k) is equal to f(p1), then add 1.
        int middle_val = f(p1);
        low = p1;
        high = n;
        int last_k_with_middle_val = p1;
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (f(mid) == middle_val) {
                last_k_with_middle_val = mid;
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        int p2 = last_k_with_middle_val + 1;
        
        // Distinguish pos_a and pos_b
        // Query with Q[p1-1] = a and all other elements as b.
        // Answer is ([P[p1-1] == a]) + 1.
        // If ans=2, P[p1-1] is a. Otherwise, it must be b.
        std::vector<int> q_dist(n, b);
        q_dist[p1 - 1] = a;
        int res = ask_query(n, q_dist);

        if (res == 2) {
            p_inv[a] = p1;
            p_inv[b] = p2;
        } else {
            p_inv[a] = p2;
            p_inv[b] = p1;
        }
    }

    if (n % 2 == 1) {
        std::vector<bool> pos_taken(n + 1, false);
        int last_val = -1;
        for (int i = 1; i <= n; ++i) {
            if (p_inv[i] != 0) {
                pos_taken[p_inv[i]] = true;
            } else {
                last_val = i;
            }
        }
        for (int i = 1; i <= n; ++i) {
            if (!pos_taken[i]) {
                p_inv[last_val] = i;
                break;
            }
        }
    }
    
    for (int i = 1; i <= n; ++i) {
        p[p_inv[i] - 1] = i;
    }

    submit_answer(n, p);

    return 0;
}