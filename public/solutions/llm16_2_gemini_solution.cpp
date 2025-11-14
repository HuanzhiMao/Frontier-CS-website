#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

long long N;

// Function to perform a query
long long query(long long u, long long v) {
    if (u == v) return 0;
    std::cout << "? " << u << " " << v << std::endl;
    long long dist;
    std::cin >> dist;
    return dist;
}

// Function to find the antipodal vertex
long long get_antipodal(long long u) {
    return (u - 1 + N / 2) % N + 1;
}

void solve() {
    std::cin >> N;

    // 1. Find m that minimizes f(i) = dist(i, i_antipodal)
    // We use a search similar to ternary search on the range [1, N/2 + 1].
    long long m_low = 1, m_high = 1 + N / 2;
    while (m_high - m_low >= 5) { // Reduce range to a small constant size
        long long m1 = m_low + (m_high - m_low) / 3;
        long long m2 = m_high - (m_high - m_low) / 3;
        if (query(m1, get_antipodal(m1)) < query(m2, get_antipodal(m2))) {
            m_high = m2;
        } else {
            m_low = m1;
        }
    }
    
    // Linear scan on the small remaining range to find the true minimum
    long long m = -1;
    long long min_dist = -1;
    for (long long i = m_low; i <= m_high; ++i) {
        long long current_dist = query(i, get_antipodal(i));
        if (m == -1 || current_dist < min_dist) {
            min_dist = current_dist;
            m = i;
        }
    }

    long long m_antipodal = get_antipodal(m);

    // 2. Find a zero of h(k) = dist(m, k) - dist(m_antipodal, k)
    // We expect two zeros, u and v.
    
    // Find the first zero, u
    long long u = -1;
    long long low = 1, high = N;
    long long h_low = query(m, low) - query(m_antipodal, low);
    
    if (h_low == 0) {
        u = low;
    } else {
        // Binary search for sign change
        while (high - low > 1) {
            long long mid = low + (high - low) / 2;
            long long h_mid = query(m, mid) - query(m_antipodal, mid);
            if (h_mid == 0) {
                // To ensure we find one of the boundaries of the zero region
                high = mid; 
            } else if ((h_low > 0 && h_mid < 0) || (h_low < 0 && h_mid > 0)) {
                high = mid;
            } else {
                low = mid;
            }
        }
        u = high;
    }

    // Find the second zero, v, by searching in the range [u+1, u+N-1]
    long long v = -1;
    low = u + 1, high = u + N - 1;
    long long h_low_v_node = (low - 1) % N + 1;
    long long h_low_v = query(m, h_low_v_node) - query(m_antipodal, h_low_v_node);

    if(h_low_v == 0) {
        v = h_low_v_node;
    } else {
        while (high - low > 1) {
            long long mid_idx = low + (high - low) / 2;
            long long mid_node = (mid_idx - 1) % N + 1;
            long long h_mid = query(m, mid_node) - query(m_antipodal, mid_node);
            if (h_mid == 0) {
                high = mid_idx;
            } else if ((h_low_v > 0 && h_mid < 0) || (h_low_v < 0 && h_mid > 0)) {
                high = mid_idx;
            } else {
                low = mid_idx;
            }
        }
        v = (high - 1) % N + 1;
    }

    if (u > v) std::swap(u, v);
    std::cout << "! " << u << " " << v << std::endl;
    int res;
    std::cin >> res;
    if (res == -1) exit(0);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int T;
    std::cin >> T;
    while (T--) {
        solve();
    }
    return 0;
}