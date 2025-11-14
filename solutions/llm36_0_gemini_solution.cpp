#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <set>

long long query(const std::vector<long long>& q) {
    if (q.empty()) {
        return 0;
    }
    std::cout << "0 " << q.size();
    for (long long x : q) {
        std::cout << " " << x;
    }
    std::cout << std::endl;
    long long collisions;
    std::cin >> collisions;
    return collisions;
}

void answer(long long n) {
    std::cout << "1 " << n << std::endl;
}

// Checks if n >= m, using O(sqrt(m)) elements
bool check_sqrt(long long m) {
    if (m <= 1) return true;
    long long s = static_cast<long long>(sqrt(m - 1.0)) + 1;

    std::vector<long long> q;
    std::set<long long> distinct_elements;
    long long offset = 2000000000000000000LL; 

    for (long long i = 0; i < s; ++i) {
        distinct_elements.insert(offset + i);
    }
    for (long long i = 0; i < s; ++i) {
        distinct_elements.insert(offset + i * s);
    }

    for (long long val : distinct_elements) {
        q.push_back(val);
    }

    return query(q) == 0;
}

// Checks if n >= m, using m elements
bool check_linear(long long m) {
    if (m <= 1) return true;
    std::vector<long long> q;
    for (long long i = 1; i <= m; ++i) {
        q.push_back(i);
    }
    return query(q) == 0;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    long long threshold = 35000;
    
    std::vector<long long> initial_q;
    for (long long i = 1; i <= threshold; ++i) {
        initial_q.push_back(i);
    }

    if (query(initial_q) > 0) {
        // n < threshold
        long long low = 1, high = threshold, ans = threshold;
        while (low <= high) {
            long long mid = low + (high - low) / 2;
            if (mid == 0) {
                low = 1;
                continue;
            }
            if (check_linear(mid)) {
                ans = mid;
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        answer(ans);
    } else {
        // n >= threshold
        long long low = threshold, high = 1000000000, ans = threshold;
        while (low <= high) {
            long long mid = low + (high - low) / 2;
            if (mid <= ans) { 
                low = mid + 1;
                continue;
            }
            if (check_sqrt(mid)) {
                ans = mid;
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        answer(ans);
    }

    return 0;
}