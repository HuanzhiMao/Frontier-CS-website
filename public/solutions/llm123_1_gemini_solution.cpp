#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <iterator>

// Helper function to ask a query about a set S.
// Returns true for "YES", false for "NO".
bool ask(const std::vector<int>& s) {
    if (s.empty()) {
        // A "NO" is implied for an empty set query.
        // The problem guarantees we query non-empty sets, but this branch is for logical safety.
        return false;
    }
    std::cout << "? " << s.size();
    for (int x : s) {
        std::cout << " " << x;
    }
    std::cout << std::endl;
    std::string response;
    std::cin >> response;
    return response == "YES";
}

// Helper function to make a guess.
// Returns true if the guess is correct (program should terminate).
bool guess(int g) {
    std::cout << "! " << g << std::endl;
    std::string response;
    std::cin >> response;
    return response == ":)";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    // C: set of candidates assuming the last answer was correct.
    // L: set of candidates assuming the last answer was a lie.
    std::vector<int> c(n);
    std::iota(c.begin(), c.end(), 1);
    std::vector<int> l;

    // We stop querying when the total number of candidates is small enough
    // to be resolved with our two guesses. With 2 guesses, we can distinguish
    // up to 3 candidates, but to be absolutely safe, we stop at size 2.
    while (c.size() + l.size() > 2) {
        // Partition C and L into two halves each.
        auto c_mid_it = c.begin() + c.size() / 2;
        auto l_mid_it = l.begin() + l.size() / 2;
        
        std::vector<int> c_half1(c.begin(), c_mid_it);
        std::vector<int> c_half2(c_mid_it, c.end());
        std::vector<int> l_half1(l.begin(), l_mid_it);
        std::vector<int> l_half2(l_mid_it, l.end());
        
        // Construct the query set S = C_half1 U L_half1.
        // Since C and L are disjoint and sorted, and their halves are sorted,
        // we can use std::merge to create a sorted union.
        std::vector<int> s;
        s.reserve(c_half1.size() + l_half1.size());
        std::merge(c_half1.begin(), c_half1.end(),
                   l_half1.begin(), l_half1.end(),
                   std::back_inserter(s));

        bool response = ask(s);

        if (response) { // Answer was "YES"
            // C_new = (C U L) intersect S = (C intersect S) U (L intersect S)
            //       = C_half1 U L_half1 = S
            // L_new = C \ S = C_half2
            c = std::move(s);
            l = std::move(c_half2);
        } else { // Answer was "NO"
            // C_new = (C U L) \ S = (C \ S) U (L \ S)
            //       = C_half2 U L_half2
            // L_new = C intersect S = C_half1
            std::vector<int> next_c;
            next_c.reserve(c_half2.size() + l_half2.size());
            std::merge(c_half2.begin(), c_half2.end(),
                       l_half2.begin(), l_half2.end(),
                       std::back_inserter(next_c));
            c = std::move(next_c);
            l = std::move(c_half1);
        }
    }

    // After the query phase, combine C and L to get the final list of candidates.
    // C and L are disjoint and sorted, so we can merge them.
    std::vector<int> candidates;
    candidates.reserve(c.size() + l.size());
    std::merge(c.begin(), c.end(), l.begin(), l.end(), std::back_inserter(candidates));
    
    // Guess each remaining candidate.
    for (int cand : candidates) {
        if (guess(cand)) {
            // Correct guess, terminate.
            return 0;
        }
    }

    return 0;
}