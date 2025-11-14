#include <iostream>
#include <vector>
#include <string>
#include <numeric>

void solve() {
    int n;
    std::cin >> n;

    // We determine the string in blocks of B bits.
    const int B = 5;
    const int num_blocks = n / B;
    
    // We fix m to be 102 to get the maximum score.
    const int m = 102;
    std::string s_known = "";

    for (int k = 0; k < num_blocks; ++k) {
        // Length of the prefix we already know
        int l = k * B;

        std::cout << 1 << std::endl;
        std::cout << m << std::endl;

        std::vector<int> a(m), b(m);
        // Initialize all transitions to self-loops.
        for (int i = 0; i < m; ++i) {
            a[i] = i;
            b[i] = i;
        }

        // We partition the states into two groups:
        // 1. Tree states {1..63}: used to encode the B unknown bits.
        // 2. Prologue states {0, 64..101}: used to traverse the known prefix.

        // Setup the tree part.
        // States 1..31 are internal nodes of a complete binary tree.
        // A transition from state `i` on '0' goes to `2*i`, on '1' to `2*i+1`.
        for (int i = 1; i <= 31; ++i) {
            a[i] = 2 * i;
            b[i] = 2 * i + 1;
        }
        // States 32..63 are leaves. They are trap states (self-loops).
        // This is already handled by the initialization.

        // Setup the prologue part.
        // We need to construct a path of length `l` that the machine will follow
        // given the known prefix `s_known`. This path must end at state 1, the tree root.
        if (l == 0) {
            // For the first block, we don't have a prefix.
            // We transition from the initial state 0 directly to the tree root 1.
            a[0] = 1;
            b[0] = 1;
        } else {
            // For subsequent blocks, we define a sequence of states for the path.
            // The path starts at state 0, uses states {64..101} in a cycle,
            // and ends at state 1.
            auto get_prologue_state = [&](int t) {
                if (t == 0) return 0;       // Start at state 0
                if (t == l) return 1;       // End at state 1 (tree root)
                // Intermediate states cycle through {64..101}
                return 64 + (t - 1) % 38;
            };

            // Set the transitions along the path according to the known prefix.
            for (int t = 0; t < l; ++t) {
                int u = get_prologue_state(t);
                int v = get_prologue_state(t + 1);
                if (s_known[t] == '0') {
                    a[u] = v;
                } else {
                    b[u] = v;
                }
            }
        }
        
        // Output the query.
        for (int i = 0; i < m; ++i) {
            std::cout << a[i] << (i == m - 1 ? "" : " ");
        }
        std::cout << std::endl;
        for (int i = 0; i < m; ++i) {
            std::cout << b[i] << (i == m - 1 ? "" : " ");
        }
        std::cout << std::endl;

        // Read the result of the query.
        int res;
        std::cin >> res;

        // The result `res` is a leaf state from 32 to 63.
        // The value `res - 32` corresponds to the integer value of the B bits.
        int val = res - 32;
        std::string block = "";
        for (int i = 0; i < B; ++i) {
            if ((val >> (B - 1 - i)) & 1) {
                block += '1';
            } else {
                block += '0';
            }
        }
        s_known += block;
    }

    // After all blocks are determined, output the final guess.
    std::cout << 0 << std::endl;
    std::cout << s_known << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    solve();
    return 0;
}