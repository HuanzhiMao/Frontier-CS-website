#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include <chrono>

void ask(int i, int& a0, int& a1) {
    std::cout << "? " << i << std::endl;
    std::cin >> a0 >> a1;
}

void answer(int i) {
    std::cout << "! " << i << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    int l = 0, r = n - 1;

    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    
    int num_initial_queries = 80;
    if (n < 100) {
        num_initial_queries = n;
    }

    std::vector<int> type2_indices;
    std::vector<bool> queried(n, false);
    
    for (int k = 0; k < num_initial_queries; ++k) {
        int i;
        if (n < 100) {
            i = k;
        } else {
             if (l > r) break;
             i = std::uniform_int_distribution<int>(l, r)(rng);
        }
        
        if (queried[i]) {
            k--; 
            continue;
        }
        queried[i] = true;
        
        int a0, a1;
        ask(i, a0, a1);

        if (a0 == 0 && a1 == 0) {
            answer(i);
            return 0;
        }

        if (a0 + a1 == 1) {
            type2_indices.push_back(i);
            if (a0 == 1) {
                r = std::min(r, i - 1);
            } else { // a1 == 1
                l = std::max(l, i + 1);
            }
        }
    }
    
    std::sort(type2_indices.begin(), type2_indices.end());

    int known_2_left = 0;
    for(int idx : type2_indices) {
        if (idx < l) {
            known_2_left++;
        }
    }

    while (l <= r) {
        if (l == r) {
            answer(l);
            return 0;
        }

        int m = l + (r - l) / 2;
        int a0, a1;
        ask(m, a0, a1);
        
        if (a0 == 0 && a1 == 0) {
            answer(m);
            return 0;
        }
        
        int total_more_expensive_in_prefix = a0;
        if (total_more_expensive_in_prefix > known_2_left) {
            r = m - 1;
        } else {
            l = m + 1;
        }
    }

    answer(l);

    return 0;
}