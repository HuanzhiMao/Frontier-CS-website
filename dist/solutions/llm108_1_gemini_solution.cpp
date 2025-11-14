#include <iostream>
#include <vector>
#include <numeric>

// Function to perform a rotation query
int query(int r, int d) {
    // Print the query to standard output
    std::cout << "? " << r << " " << d << std::endl;
    // Read the result from standard input
    int result;
    std::cin >> result;
    return result;
}

int main() {
    // Fast I/O
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n, m;
    std::cin >> n >> m;

    int total_sections = n * m;

    // This vector will store the final relative positions p_i.
    // p[i] will be the number of clockwise rotations to align ring 0 with ring i.
    std::vector<int> p(n, 0);

    // The core of the strategy is to determine the initial relative position of each ring i (from 1 to n-1)
    // with respect to ring 0. Let the initial starting position of the arc on ring k be s_k.
    // We want to find d_i = (s_i - s_0) mod total_sections.

    // To find d_i for a specific ring i, we rotate only ring i a full circle (total_sections steps).
    // In each step, we receive the number of unblocked lasers. The configuration that maximizes this number
    // is very likely the one where ring i's arc aligns with another ring's arc, as this maximizes the
    // overlap of covered sections, thus minimizing the total number of unique covered sections.
    // We make the assumption that the strongest signal (highest peak in unblocked lasers) corresponds to
    // ring i aligning with our reference, ring 0. While other rings create "noise", aligning two rings
    // provides a significant overlap of 'm' sections, which is expected to dominate.

    for (int i = 1; i < n; ++i) {
        int best_a = -1;
        int best_t = 0;

        // We rotate ring i for a full circle (total_sections steps) to find the best alignment.
        for (int t = 1; t <= total_sections; ++t) {
            int a = query(i, 1);
            if (a > best_a) {
                best_a = a;
                best_t = t;
            }
        }
        
        // After this loop, ring i has been rotated by `total_sections` steps, which is a full circle.
        // This means its position relative to all other rings is unchanged from when this loop for `i` started.
        // The maximum number of unblocked lasers `best_a` was found after `best_t` clockwise rotations of ring i.
        // Let's assume this corresponds to ring i aligning with ring 0.
        // If s_i and s_0 are the starting positions (at the beginning of this loop), then alignment means:
        // s_i + best_t ≡ s_0 (mod total_sections)
        // This implies: s_i - s_0 ≡ -best_t (mod total_sections)
        // We store this initial relative difference.
        p[i] = (-best_t % total_sections + total_sections) % total_sections;
    }
    
    // The problem asks for the relative positions p_i in the final configuration, after all our rotations are complete.
    // In our strategy, for each ring i from 1 to n-1, we rotated it by `total_sections` steps. Ring 0 was never rotated.
    // A rotation by `total_sections` is a full circle, which is equivalent to zero net rotation.
    // Therefore, the final configuration of all rings is the same as their initial configuration.
    // The final relative positions are the same as the initial ones we calculated.
    // p_i = (s_i_final - s_0_final) mod total_sections = (s_i_initial - s_0_initial) mod total_sections.

    std::cout << "! ";
    for (int i = 1; i < n; ++i) {
        std::cout << p[i] << (i == n - 1 ? "" : " ");
    }
    std::cout << std::endl;

    return 0;
}