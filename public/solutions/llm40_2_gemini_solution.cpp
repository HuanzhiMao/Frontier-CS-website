#include <iostream>
#include <vector>
#include <string>
#include <numeric>

// Function to perform a query
int ask(const std::vector<int>& indices) {
    if (indices.empty()) {
        return 0;
    }
    std::cout << "0 " << indices.size();
    for (int idx : indices) {
        std::cout << " " << idx;
    }
    std::cout << std::endl;
    int result;
    std::cin >> result;
    if (result == -1) exit(0); // Exit if judge signals an error
    return result;
}

// Function to submit the final answer
void answer(const std::string& s) {
    std::cout << "1 " << s << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    std::string s(n, ' ');
    
    int open_idx = -1;
    int close_idx = -1;

    // Step 1: Determine s[0] and find a corresponding opposite bracket
    for (int i = 1; i < n; ++i) {
        if (ask({1, i + 1}) == 1) {
            s[0] = '(';
            s[i] = ')';
            open_idx = 1;
            close_idx = i + 1;
            break;
        }
    }

    // If no pair was found, s[0] must be ')'
    if (open_idx == -1) {
        s[0] = ')';
        close_idx = 1;
        
        // Step 2: Find an open bracket anchor
        for (int i = 1; i < n; ++i) {
            if (ask({i + 1, 1}) == 1) {
                s[i] = '(';
                open_idx = i + 1;
                break;
            }
        }
    }
    
    // Step 3: Determine the rest of the characters using the open_idx as an anchor
    for (int i = 0; i < n; ++i) {
        // If character at i is not yet known
        if (s[i] == ' ') {
            if (ask({open_idx, i + 1}) == 1) {
                s[i] = ')';
            } else {
                s[i] = '(';
            }
        }
    }

    answer(s);

    return 0;
}