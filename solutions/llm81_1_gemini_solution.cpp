#include <iostream>
#include <vector>
#include <string>
#include <numeric>

// Function to ask a query
int ask_query(int m, const std::vector<int>& a, const std::vector<int>& b) {
    std::cout << 1 << std::endl;
    std::cout << m << std::endl;
    for (int i = 0; i < m; ++i) {
        std::cout << a[i] << (i == m - 1 ? "" : " ");
    }
    std::cout << std::endl;
    for (int i = 0; i < m; ++i) {
        std::cout << b[i] << (i == m - 1 ? "" : " ");
    }
    std::cout << std::endl;
    std::cout.flush();
    int result;
    std::cin >> result;
    return result;
}

// Function to submit the guess
void guess_string(const std::string& s) {
    std::cout << 0 << std::endl;
    std::cout << s << std::endl;
    std::cout.flush();
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    std::string s(n, '0');
    
    // We will use m=102 states to get the full score.
    // States 0..99 are regular states. States 100 and 101 are trap states.
    const int m = 102;
    const int num_regular_states = m - 2;
    const int trap0 = m - 2;
    const int trap1 = m - 1;

    // A base automaton that defines default transitions.
    // The transitions should map regular states to regular states.
    std::vector<int> base_a(m);
    std::vector<int> base_b(m);
    for (int i = 0; i < num_regular_states; ++i) {
        base_a[i] = (i * 3 + 1) % num_regular_states;
        base_b[i] = (i * 5 + 2) % num_regular_states;
    }

    // Determine S bit by bit
    for (int k = 0; k < n; ++k) {
        // 1. Predict the state of the base automaton just before reading S[k].
        // This is possible because we already know S[0...k-1].
        int current_state = 0;
        for (int i = 0; i < k; ++i) {
            if (s[i] == '0') {
                current_state = base_a[current_state];
            } else {
                current_state = base_b[current_state];
            }
        }
        int state_before_k = current_state;

        // 2. Craft a special query for S[k].
        // It's mostly the base automaton, but with modified transitions for state_before_k.
        std::vector<int> query_a = base_a;
        std::vector<int> query_b = base_b;
        
        // From state_before_k, transition to trap states based on S[k].
        query_a[state_before_k] = trap0;
        query_b[state_before_k] = trap1;

        // The trap states are absorbing.
        query_a[trap0] = trap0;
        query_b[trap0] = trap0;
        query_a[trap1] = trap1;
        query_b[trap1] = trap1;

        // 3. Ask the query.
        int result = ask_query(m, query_a, query_b);

        // 4. Deduce S[k] from the result.
        if (result == trap0) {
            s[k] = '0';
        } else {
            // It must be trap1
            s[k] = '1';
        }
    }

    guess_string(s);

    return 0;
}