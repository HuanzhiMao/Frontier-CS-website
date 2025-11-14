#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>

/**
 * @brief Performs a query to rotate a ring and get the number of unblocked lasers.
 * 
 * @param x The ring to rotate (0 to n-1).
 * @param d The direction to rotate (-1 for anti-clockwise, 1 for clockwise).
 * @return The number of unblocked lasers after the rotation.
 */
int query(int x, int d) {
    std::cout << "? " << x << " " << d << std::endl;
    int a;
    std::cin >> a;
    return a;
}

int main() {
    // Fast I/O
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n, m;
    std::cin >> n >> m;

    int N = n * m;

    std::vector<std::pair<int, int>> all_a;
    // By rotating ring 0 a total of N times, we test every possible relative alignment
    // of ring 0 with the other rings.
    for (int i = 0; i < N; ++i) {
        int val = query(0, 1);
        // After i+1 clockwise rotations, the relative offset of ring 0 is i+1.
        all_a.push_back({val, i + 1});
    }
    
    // After N rotations, ring 0 is back at its original position relative to other rings.
    // The problem asks for the relative positions p_i after all rotations are completed.
    // Since our final state for ring 0 is same as initial, we find the p_i for the initial configuration.
    
    // The core idea is that when ring 0 aligns with another ring, the total number of
    // blocked sections is minimized (or at least locally minimized), which maximizes the number
    // of unblocked lasers 'a'.
    // Therefore, the offsets that result in the highest 'a' values are the desired relative positions p_i.
    
    // We sort the collected 'a' values in descending order. If 'a' values are tied,
    // we sort by the offset to maintain a consistent order.
    std::sort(all_a.begin(), all_a.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
        if (a.first != b.first) {
            return a.first > b.first;
        }
        return a.second < b.second;
    });
    
    std::vector<int> p_res;
    std::vector<bool> p_found_val(N, false);
    
    // We need to find n-1 relative positions. It is a reasonable assumption that these
    // correspond to the top n-1 distinct alignment positions. If multiple rings share a
    // relative position, it would likely create a single, stronger peak. However,
    // without further information, identifying multiplicity is hard. The simplest
    // interpretation is to find n-1 distinct best positions.
    for(const auto& p_pair : all_a) {
        if (p_res.size() == n - 1) {
            break;
        }
        // The offset is the number of rotations. `p_pair.second` is from 1 to N.
        // `(p_pair.second) % N` maps it to `0` to `N-1`.
        int offset = p_pair.second % N;
        
        if (!p_found_val[offset]) {
             p_res.push_back(offset);
             p_found_val[offset] = true;
        }
    }
    
    // This case is unlikely if the problem guarantees n-1 discoverable unique relative positions.
    while(p_res.size() < n-1) {
        p_res.push_back(0); 
    }

    // Output the final answer.
    std::cout << "!";
    for (int val : p_res) {
        std::cout << " " << val;
    }
    std::cout << std::endl;

    return 0;
}