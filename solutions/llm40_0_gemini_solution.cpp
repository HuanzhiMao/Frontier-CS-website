#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <map>

// Using a map for memoization to avoid re-querying for the same set of indices.
// Key: sorted vector of indices. Value: query result.
std::map<std::vector<int>, int> memo;

// Function to ask a query and get the result.
// It also handles memoization to reduce redundant queries.
int query(const std::vector<int>& indices) {
    if (indices.empty()) {
        return 0;
    }

    // Use a sorted version of indices as the key for memoization
    // to ensure that the order of indices doesn't matter for caching.
    std::vector<int> sorted_indices = indices;
    std::sort(sorted_indices.begin(), sorted_indices.end());

    if (memo.count(sorted_indices)) {
        return memo[sorted_indices];
    }

    std::cout << "0 " << indices.size();
    for (int idx : indices) {
        std::cout << " " << idx;
    }
    std::cout << std::endl;

    int result;
    std::cin >> result;
    if (result == -1) {
        // Error case provided by some interactive judges, exit gracefully.
        exit(0);
    }

    memo[sorted_indices] = result;
    return result;
}

// Function to print the final answer.
void print_answer(const std::string& s) {
    std::cout << "1 " << s << std::endl;
}

int main() {
    // Fast I/O
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    std::string result_s(n, ' ');
    std::vector<int> st; // This stack will hold indices of what we infer to be open brackets.
    
    // We keep track of the f-value of the string formed by characters at indices in the stack.
    // Let f_st be this value. It gets updated as we iterate.
    int f_st = 0;

    for (int i = 1; i <= n; ++i) {
        // Form a new set of indices by adding the current index 'i' to the stack.
        std::vector<int> st_plus_i = st;
        st_plus_i.push_back(i);

        // Query for the string formed by (stack indices + current index i).
        int f_st_i = query(st_plus_i);

        // The core logic:
        // If adding s[i] creates new regular bracket sequences that end at s[i],
        // then s[i] must be a closing bracket that matches an opening bracket from the stack.
        // `f(P + c) > f(P)` implies `c` is a closing bracket.
        if (f_st_i > f_st) {
            result_s[i - 1] = ')';
            // Since s[i] is a closing bracket, it matches the last open bracket.
            // So we pop from the stack.
            if (!st.empty()) {
                st.pop_back();
            }
            // After popping, we need to find the f-value for the new stack configuration.
            f_st = query(st);
        } else {
            // If no new regular bracket sequence is formed, s[i] acts as an opening bracket.
            // `f(P + c) == f(P)` implies `c` is an opening bracket.
            result_s[i - 1] = '(';
            // Push the index of this new open bracket onto the stack.
            st.push_back(i);
            // The f-value for the new stack is what we just queried for (st + i).
            f_st = f_st_i;
        }
    }

    print_answer(result_s);

    return 0;
}