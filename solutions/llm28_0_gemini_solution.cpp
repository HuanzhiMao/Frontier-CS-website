#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include <cmath>

void solve() {
    int n;
    std::cin >> n;

    std::set<std::string> found_words;

    for (char c = 'a'; c <= 'z'; ++c) {
        if (found_words.size() == n) {
            break;
        }

        int remaining_cap = n - found_words.size();
        if (remaining_cap == 0) {
            break;
        }
        
        std::string prefix(1, c);
        
        int k = 1;
        while (true) {
            int query_k = std::min(k, remaining_cap);
            
            std::cout << "query " << prefix << " " << query_k << std::endl;
            int m;
            std::cin >> m;
            std::vector<std::string> words(m);
            for (int i = 0; i < m; ++i) {
                std::cin >> words[i];
            }

            if (m < query_k) {
                // Found all words for this prefix
                for (const auto& word : words) {
                    found_words.insert(word);
                }
                break;
            }
            
            if (query_k == remaining_cap) {
                 for (const auto& word : words) {
                    found_words.insert(word);
                }
                break;
            }
            
            k *= 2;
        }
    }

    std::cout << "answer";
    bool first = true;
    for (const auto& word : found_words) {
        if (first) {
            std::cout << " ";
            first = false;
        } else {
            std::cout << " ";
        }
        std::cout << word;
    }
    std::cout << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int t;
    std::cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}