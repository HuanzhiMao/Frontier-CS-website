#include <iostream>
#include <vector>
#include <string>
#include <numeric>

void do_query(int m, const std::vector<int>& a, const std::vector<int>& b) {
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
}

int read_response() {
    int x;
    std::cin >> x;
    return x;
}

void do_guess(const std::string& s) {
    std::cout << 0 << std::endl;
    std::cout << s << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int n;
    std::cin >> n;

    std::string s(n, '0');
    
    for (int k = 0; k < n; ++k) {
        int m = k + 3;
        
        std::vector<int> a(m);
        std::vector<int> b(m);
        
        // Build a chain of states 0 -> 1 -> ... -> k.
        // This makes the state before processing S[k] always equal to k,
        // regardless of the prefix S[0...k-1].
        for (int i = 0; i < k; ++i) {
            a[i] = i + 1;
            b[i] = i + 1;
        }
        
        // At state k, split based on S[k] into two new trap states.
        int trap0 = k + 1;
        int trap1 = k + 2;
        
        a[k] = trap0;
        b[k] = trap1;
        
        // Define trap states: they loop back to themselves.
        a[trap0] = trap0;
        b[trap0] = trap0;
        a[trap1] = trap1;
        b[trap1] = trap1;

        // Any other state that might be part of the vector but is unused
        // can be a self-loop. They are not reachable from state 0 anyway.
        for(int i = k + 3; i < m; ++i) {
            a[i] = i;
            b[i] = i;
        }

        do_query(m, a, b);
        int result = read_response();
        
        if (result == trap0) {
            s[k] = '0';
        } else {
            s[k] = '1';
        }
    }

    do_guess(s);

    return 0;
}