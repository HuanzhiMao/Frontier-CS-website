#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

// Function to send a query and receive the answer
int ask(int n, const std::vector<int>& q) {
    std::cout << "0";
    for (int i = 0; i < n; ++i) {
        std::cout << " " << q[i];
    }
    std::cout << std::endl;
    int x;
    std::cin >> x;
    if (x == -1) exit(0); // Exit on error
    return x;
}

// Function to submit the final guess
void guess(int n, const std::vector<int>& p) {
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

    if (n == 1) {
        guess(1, {1});
        return 0;
    }

    std::vector<int> p(n, 0);
    std::vector<int> unknown_indices(n);
    std::iota(unknown_indices.begin(), unknown_indices.end(), 0);

    for (int v = n; v >= 2; --v) {
        int base_val = v - 1;
        
        std::vector<int> base_q(n);
        for (int i = 0; i < n; ++i) {
            if (p[i] != 0) {
                base_q[i] = p[i];
            } else {
                base_q[i] = base_val;
            }
        }
        
        int base_matches = ask(n, base_q);
        
        int found_pos = -1;
        for (int idx : unknown_indices) {
            std::vector<int> current_q = base_q;
            current_q[idx] = v;
            int current_matches = ask(n, current_q);
            
            if (current_matches > base_matches) {
                found_pos = idx;
                break;
            }
        }
        
        p[found_pos] = v;
        
        unknown_indices.erase(std::remove(unknown_indices.begin(), unknown_indices.end(), found_pos), unknown_indices.end());
    }

    if (!unknown_indices.empty()) {
        p[unknown_indices[0]] = 1;
    }
    
    guess(n, p);

    return 0;
}