#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <algorithm>

void solve() {
    int n;
    std::cin >> n;

    std::set<std::string> found_words;
    std::set<std::string> prefixes_to_explore;

    for (char c = 'a'; c <= 'z'; ++c) {
        prefixes_to_explore.insert(std::string(1, c));
    }

    while (found_words.size() < n && !prefixes_to_explore.empty()) {
        std::string prefix = *prefixes_to_explore.begin();
        prefixes_to_explore.erase(prefixes_to_explore.begin());

        if (prefix.length() > 10) {
            continue;
        }

        int known_count = 0;
        for (const auto& w : found_words) {
            if (w.rfind(prefix, 0) == 0) {
                known_count++;
            }
        }

        int k_to_ask = known_count + 1;
        
        std::cout << "query " << prefix << " " << k_to_ask << std::endl;
        
        int k_response;
        std::cin >> k_response;
        std::vector<std::string> response_words(k_response);
        for (int i = 0; i < k_response; ++i) {
            std::cin >> response_words[i];
        }
        
        std::vector<std::string> new_words;
        for (const auto& w : response_words) {
            if (found_words.find(w) == found_words.end()) {
                new_words.push_back(w);
                found_words.insert(w);
            }
        }

        if (response_words.size() == k_to_ask) {
            // Potentially more words with this prefix. Re-add.
            prefixes_to_explore.insert(prefix);
        }

        for (const auto& w : new_words) {
            if (w.length() < 10) {
                prefixes_to_explore.insert(w);
            }
        }
    }

    std::cout << "answer";
    for (const auto& word : found_words) {
        std::cout << " " << word;
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