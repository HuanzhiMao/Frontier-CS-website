#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>

void ask_query(const std::vector<int>& s) {
    std::cout << "? " << s.size();
    for (int x : s) {
        std::cout << " " << x;
    }
    std::cout << std::endl;
}

void make_guess(int g) {
    std::cout << "! " << g << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    std::vector<int> g, b;
    g.resize(n);
    std::iota(g.begin(), g.end(), 1);

    std::vector<bool> in_query_set(n + 1, false);

    while (g.size() + b.size() > 2) {
        int g_half = g.size() / 2;
        std::vector<int> g1(g.begin(), g.begin() + g_half);
        
        int b_half = b.size() / 2;
        std::vector<int> b1(b.begin(), b.begin() + b_half);
        
        std::vector<int> query_set = g1;
        query_set.insert(query_set.end(), b1.begin(), b1.end());
        
        if (query_set.empty()) {
            if (!g.empty()) {
                query_set.assign(g.begin() + g_half, g.end());
            }
            if (!b.empty() && query_set.empty()) {
                 query_set.assign(b.begin() + b_half, b.end());
            }
             if (query_set.empty()){
                // This case occurs if |g|+|b|<=2, but loop condition prevents it.
                // Or if one set has 1 element, another has 0. e.g. |g|=1, |b|=0. Total=1.
                // Loop ends. As a safeguard, if we must query, query something.
                if (!g.empty()) query_set = g; else query_set = b;
            }
        }

        ask_query(query_set);

        std::string reply;
        std::cin >> reply;

        for (int x : query_set) {
            in_query_set[x] = true;
        }

        std::vector<int> next_g, next_b;
        if (reply == "YES") {
            for (int x : g) {
                if (in_query_set[x]) {
                    next_g.push_back(x);
                } else {
                    next_b.push_back(x);
                }
            }
            for (int x : b) {
                if (in_query_set[x]) {
                    next_g.push_back(x);
                }
            }
        } else { // NO
            for (int x : g) {
                if (in_query_set[x]) {
                    next_b.push_back(x);
                } else {
                    next_g.push_back(x);
                }
            }
            for (int x : b) {
                if (!in_query_set[x]) {
                    next_g.push_back(x);
                }
            }
        }
        
        for (int x : query_set) {
            in_query_set[x] = false;
        }

        g = std::move(next_g);
        b = std::move(next_b);
    }

    std::vector<int> candidates = g;
    candidates.insert(candidates.end(), b.begin(), b.end());

    for (int cand : candidates) {
        make_guess(cand);
        std::string reply;
        std::cin >> reply;
        if (reply == ":)") {
            return 0;
        }
    }

    return 0;
}