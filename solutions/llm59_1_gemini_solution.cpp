#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>

char query(int i, int j) {
    std::cout << "? " << i << " " << j << std::endl;
    char response;
    std::cin >> response;
    return response;
}

int get_min(int i, int j) {
    if (query(i, j) == '<') {
        return i;
    }
    return j;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    std::vector<int> p(n + 1, 0);
    std::vector<int> U;
    for (int i = 1; i <= n; ++i) {
        U.push_back(i);
    }

    int v = 1;

    while (U.size() >= 3) {
        if (U.size() >= 5) {
            int u1 = U[0], u2 = U[1], u3 = U[2], u4 = U[3], u5 = U[4];
            
            // Find inv(v), inv(v+1), inv(v+2) with 5 queries
            int m12 = get_min(u1, u2);
            int m34 = get_min(u3, u4);
            
            int inv_v = get_min(m12, u3);
            
            std::vector<int> rem_v;
            if (u1 != inv_v) rem_v.push_back(u1);
            if (u2 != inv_v) rem_v.push_back(u2);
            if (u3 != inv_v) rem_v.push_back(u3);

            int inv_v1 = get_min(rem_v[0], rem_v[1]);
            inv_v1 = get_min(inv_v1, u4);
            
            std::vector<int> rem_v1;
            if(rem_v[0] != inv_v1) rem_v1.push_back(rem_v[0]);
            if(rem_v[1] != inv_v1) rem_v1.push_back(rem_v[1]);
            if(u4 != inv_v1) rem_v1.push_back(u4);
            
            int inv_v2 = get_min(rem_v1[0], rem_v1[1]);
            inv_v2 = get_min(inv_v2, u5);

            p[inv_v] = v;
            p[inv_v1] = v + 1;
            p[inv_v2] = v + 2;

            v += 3;
            U.erase(std::remove_if(U.begin(), U.end(), [&](int val) {
                return val == inv_v || val == inv_v1 || val == inv_v2;
            }), U.end());

        } else { // Handle remaining 3 or 4
            std::vector<int> sorted_indices = U;
            std::sort(sorted_indices.begin(), sorted_indices.end(), [](int a, int b) {
                return query(a, b) == '<';
            });
            for (size_t i = 0; i < sorted_indices.size(); ++i) {
                p[sorted_indices[i]] = v + i;
            }
            U.clear();
        }
    }

    if (U.size() == 2) {
        if (query(U[0], U[1]) == '<') {
            p[U[0]] = v;
            p[U[1]] = v + 1;
        } else {
            p[U[1]] = v;
            p[U[0]] = v + 1;
        }
    } else if (U.size() == 1) {
        p[U[0]] = v;
    }

    std::cout << "! ";
    for (int i = 1; i <= n; ++i) {
        std::cout << p[i] << (i == n ? "" : " ");
    }
    std::cout << std::endl;

    return 0;
}