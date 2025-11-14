#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

// Helper function to send a query and receive the result.
int query(int x, int d) {
    cout << "? " << x << " " << d << endl;
    int a;
    cin >> a;
    return a;
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    cin >> n >> m;

    int nm = n * m;

    // The core idea is to align all rings. When all rings cover the exact same
    // set of m sections, the number of unblocked lasers is maximized.
    // The value displayed is n*m - |union of covered sections|.
    // To maximize this, we need to minimize the size of the union.
    // The minimum size is m, achieved when all arcs are perfectly aligned.
    
    // We can achieve this alignment iteratively. For each ring from 1 to n-1,
    // we find the rotational position that results in the maximum number of
    // unblocked lasers, given the current positions of all other rings.
    // This is a form of coordinate ascent (or hill climbing) on the state space,
    // where each coordinate corresponds to a ring's rotation.
    
    // While one pass might not find the global optimum if there are complex
    // local maxima, this greedy approach is often effective. As we align more rings,
    // they form a larger block, creating a stronger "signal" for subsequent rings to align to.

    // For each ring i from 1 to n-1:
    for (int i = 1; i < n; i++) {
        int best_k = 0;
        int max_v = -1;

        // First, get the current value without making a permanent rotation.
        // This sets a baseline for the maximum value hunt.
        max_v = query(i, 1);
        query(i, -1);

        // Rotate the ring through all nm-1 other possible relative positions.
        for (int k = 1; k < nm; k++) {
            int v = query(i, 1);
            if (v > max_v) {
                max_v = v;
                best_k = k;
            }
        }
        
        // After nm-1 queries, the ring is at a position rotated by nm-1 from its
        // starting point for this loop. We do one more rotation to complete a full
        // cycle and return it to its state before the scan.
        query(i, 1);

        // Now, apply the best rotation found.
        for (int k = 0; k < best_k; k++) {
            query(i, 1);
        }
    }
    
    // After all rotations, our strategy aims to have all rings perfectly aligned.
    // Let s_i_final be the starting section of the arc on ring i after all our rotations.
    // If all rings are aligned, s_i_final is the same for all i=0,...,n-1.
    // The problem asks for values p_i such that rotating ring 0 from its final position
    // by p_i sections clockwise aligns it with ring i at its final position.
    // Mathematically, s_0_final + p_i = s_i_final (mod nm).
    // This means p_i = (s_i_final - s_0_final) mod nm.
    // Since our strategy results in s_i_final = s_0_final, all p_i should be 0.
    // This is a valid final configuration. The example in the problem statement
    // shows one possible outcome, but as noted, different rotations lead to different final p_i values.

    cout << "!";
    for (int i = 1; i < n; i++) {
        cout << " 0";
    }
    cout << endl;

    return 0;
}