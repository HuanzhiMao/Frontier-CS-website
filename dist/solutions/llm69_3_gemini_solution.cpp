#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <unordered_map>
#include <algorithm>
#include <map>

// Using __int128_t to prevent overflow with large intermediate calculations, 
// as powers can exceed the capacity of a 64-bit integer.
using int128 = __int128_t;

// A helper function is needed to print __int128_t values.
std::ostream& operator<<(std::ostream& os, int128 val) {
    if (val == 0) return os << "0";
    std::string s;
    bool neg = val < 0;
    if (neg) val = -val;
    while (val > 0) {
        s += (val % 10) + '0';
        val /= 10;
    }
    if (neg) s += '-';
    std::reverse(s.begin(), s.end());
    return os << s;
}

// Calculates the power of a spell formed by concatenating w_u and w_v.
// The magic words are constructed as w_i = X^A O^{B+i}.
// The formula is derived from analyzing substring creation with a suffix automaton.
int128 calculate_power(long long u, long long v, int n) {
    long long A = 1;
    long long B = 1;

    long long au = A, bu = B + u;
    long long av = A, bv = B + v;

    int128 power = 0;

    // Power of the first word, w_u = X^au O^bu
    power += (int128)au * bu + au + bu;

    // Additional power from concatenating X^av
    power += (int128)av * (au + bu + 1);
    
    // Additional power from concatenating O^bv
    long long S_len_prefix = au + bu + av;
    long long a = au, c = av;
    long long b = bu, d = bv;
    
    if (d >= b) {
        // New substrings for the first b 'O's
        power += (int128)b * S_len_prefix;
        // New substrings for the remaining d-b 'O's
        long long diff = d - b;
        power += (int128)diff * (a + c);
        power += (int128)diff * (d + b + 1) / 2;
    } else { // d < b
        power += (int128)d * S_len_prefix;
    }

    return power;
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    long long A = 1;
    long long B = 1;

    // Output n magic words.
    for (int i = 1; i <= n; ++i) {
        std::cout << 'X';
        for (long long j = 0; j < B + i; ++j) {
            std::cout << 'O';
        }
        std::cout << '\n';
    }
    std::cout.flush();

    // Precompute powers for all n*n pairs and store in a map.
    // Using std::map as __int128_t is not hashable by default for unordered_map.
    std::map<int128, std::pair<int, int>> power_to_indices;

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            power_to_indices[calculate_power(i, j, n)] = {i, j};
        }
    }

    int q;
    std::cin >> q;

    // Answer queries using the precomputed map.
    for (int k = 0; k < q; ++k) {
        long long p_val;
        std::cin >> p_val;
        int128 p = p_val;
        
        auto it = power_to_indices.find(p);
        std::pair<int, int> indices = it->second;
        std::cout << indices.first << " " << indices.second << '\n';
        std::cout.flush();
    }

    return 0;
}