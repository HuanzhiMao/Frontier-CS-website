#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <set>
#include <map>

void send_query(const std::vector<int>& positions) {
    std::cout << "? " << positions.size();
    for (int p : positions) {
        std::cout << " " << p;
    }
    std::cout << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int R, H;
    std::cin >> R >> H;

    std::vector<std::vector<int>> queries;
    std::vector<int> primes = {3, 5, 7, 11};

    for (int p : primes) {
        for (int j = 1; j < p; ++j) {
            std::vector<int> q;
            for (int pos = 1; pos <= 1000; ++pos) {
                if (pos % p == j) {
                    q.push_back(pos);
                }
            }
            queries.push_back(q);
        }
    }

    std::vector<std::pair<int, int>> extra_conds = {{3, 5}, {5, 7}, {7, 11}};
    for (const auto& cond : extra_conds) {
        std::vector<int> q;
        int p1 = cond.first;
        int p2 = cond.second;
        for (int pos = 1; pos <= 1000; ++pos) {
            if (pos % p1 < pos % p2) {
                q.push_back(pos);
            }
        }
        queries.push_back(q);
    }

    for (const auto& q : queries) {
        send_query(q);
    }

    std::cout << "@" << std::endl;

    int L;
    std::cin >> L;
    std::vector<int> results(L);
    for (int i = 0; i < L; ++i) {
        std::cin >> results[i];
    }

    std::map<int, std::pair<int, int>> mod_sets;
    int current_query_idx = 0;

    for (int p : primes) {
        std::vector<int> active_mods;
        for (int j = 1; j < p; ++j) {
            if (results[current_query_idx] == 1) {
                active_mods.push_back(j);
            }
            current_query_idx++;
        }

        if (active_mods.empty()) {
            mod_sets[p] = {0, 0};
        } else if (active_mods.size() == 1) {
            mod_sets[p] = {0, active_mods[0]};
        } else {
            mod_sets[p] = {active_mods[0], active_mods[1]};
        }
    }

    std::vector<int> extra_results;
    for (size_t i = 0; i < extra_conds.size(); ++i) {
        extra_results.push_back(results[current_query_idx++]);
    }
    
    for (int c1 = 1; c1 <= 1000; ++c1) {
        for (int c2 = c1; c2 <= 1000; ++c2) {
            bool ok = true;

            for (int p : primes) {
                int r1 = c1 % p;
                int r2 = c2 % p;
                if (r1 > r2) std::swap(r1, r2);
                
                int expected_r1 = mod_sets[p].first;
                int expected_r2 = mod_sets[p].second;
                if (expected_r1 > expected_r2) std::swap(expected_r1, expected_r2);

                if (r1 != expected_r1 || r2 != expected_r2) {
                    ok = false;
                    break;
                }
            }
            if (!ok) continue;

            for (size_t i = 0; i < extra_conds.size(); ++i) {
                int p1 = extra_conds[i].first;
                int p2 = extra_conds[i].second;
                bool c1_in_set = (c1 % p1 < c1 % p2);
                bool c2_in_set = (c2 % p1 < c2 % p2);
                int result = (c1_in_set || c2_in_set);
                if (result != extra_results[i]) {
                    ok = false;
                    break;
                }
            }

            if (ok) {
                std::cout << "! " << c1 << " " << c2 << std::endl;
                return 0;
            }
        }
    }

    return 0;
}