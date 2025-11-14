#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <cmath>

// Helper to ask a query for a set represented by a vector
std::string ask_query(const std::vector<int>& s) {
    if (s.empty()) {
        // According to the problem, k >= 1.
        // This path should ideally not be taken by the main logic.
        // Answering "NO" is a safe default for an empty set query.
        return "NO";
    }
    std::cout << "? " << s.size();
    for (int x : s) {
        std::cout << " " << x;
    }
    std::cout << std::endl;
    std::string response;
    std::cin >> response;
    return response;
}

// Helper to make a guess
bool make_guess(int g) {
    std::cout << "! " << g << std::endl;
    std::string response;
    std::cin >> response;
    return response == ":)";
}

// Checks if a number satisfies the condition from a query.
// The query set 's' must be sorted for binary_search.
bool check_membership(int val, const std::vector<int>& s, const std::string& ans) {
    bool is_in_s = std::binary_search(s.begin(), s.end(), val);
    if (ans == "YES") {
        return is_in_s;
    } else {
        return !is_in_s;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    if (n == 1) {
        make_guess(1);
        return 0;
    }

    std::vector<int> candidates(n);
    std::iota(candidates.begin(), candidates.end(), 1);

    std::vector<int> q_prev;
    std::string ans_prev;

    // Bootstrap phase: First two queries to establish a reliable candidate set.
    std::vector<int> q1;
    for (int i = 1; i <= n / 2; ++i) {
        q1.push_back(i);
    }
    std::string ans1 = ask_query(q1);

    std::vector<int> temp_y;
    for (int x : candidates) {
        if (!check_membership(x, q1, ans1)) {
            temp_y.push_back(x);
        }
    }
    
    std::vector<int> q2;
    for (size_t i = 0; i < temp_y.size() / 2; ++i) {
        q2.push_back(temp_y[i]);
    }
    if (q2.empty() && !temp_y.empty()){
        q2.push_back(temp_y[0]);
    }

    std::string ans2 = q2.empty() ? "NO" : ask_query(q2);

    std::vector<int> new_candidates;
    for (int x : candidates) {
        if (check_membership(x, q1, ans1) || check_membership(x, q2, ans2)) {
            new_candidates.push_back(x);
        }
    }
    candidates = new_candidates;
    q_prev = q2;
    ans_prev = ans2;

    // Main loop to reduce candidates
    while (candidates.size() > 2) {
        std::vector<int> X, Y;
        for (int x : candidates) {
            if (check_membership(x, q_prev, ans_prev)) {
                X.push_back(x);
            } else {
                Y.push_back(x);
            }
        }

        std::vector<int> q_curr;
        if (Y.size() > 1) {
            for(size_t i = 0; i < Y.size() / 2; ++i) {
                q_curr.push_back(Y[i]);
            }
        } else {
            for(size_t i = 0; i < X.size() / 2; ++i) {
                q_curr.push_back(X[i]);
            }
        }

        if (q_curr.empty()) {
            if (!Y.empty()) q_curr.push_back(Y[0]);
            else if (!X.empty()) q_curr.push_back(X[0]);
            else break;
        }

        std::string ans_curr = ask_query(q_curr);
        std::sort(q_curr.begin(), q_curr.end());
        
        new_candidates.clear();
        for (int x : candidates) {
            bool in_prev_cond = check_membership(x, q_prev, ans_prev);
            bool in_curr_cond = check_membership(x, q_curr, ans_curr);
            if (in_prev_cond || in_curr_cond) {
                new_candidates.push_back(x);
            }
        }
        candidates = new_candidates;
        
        q_prev = q_curr;
        ans_prev = ans_curr;
    }

    // Final guesses
    for (int x : candidates) {
        if (make_guess(x)) {
            return 0;
        }
    }

    return 0;
}