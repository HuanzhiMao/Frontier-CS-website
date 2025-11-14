#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <map>
#include <utility>

const int NUM_POSITIONS = 1000;
const int CODE_LEN = 13;
const int CODE_WEIGHT = 5;

std::vector<int> a_codes(NUM_POSITIONS);
std::vector<int> b_codes(NUM_POSITIONS);
std::map<long long, std::pair<int, int>> lookup_table;

void generate_all_codes(std::vector<int>& all_codes) {
    all_codes.reserve(1287);
    int mask = (1 << CODE_WEIGHT) - 1;
    int limit = 1 << CODE_LEN;
    while (mask < limit) {
        all_codes.push_back(mask);
        if (all_codes.size() >= 1287) break;
        int lsb = mask & -mask;
        int r = mask + lsb;
        mask = (((mask ^ r) >> 2) / lsb) | r;
    }
}

void precompute() {
    std::vector<int> all_codes;
    generate_all_codes(all_codes);

    for (int i = 0; i < NUM_POSITIONS; ++i) {
        a_codes[i] = all_codes[i];
        b_codes[i] = all_codes[all_codes.size() - 1 - i];
    }

    for (int i = 0; i < NUM_POSITIONS; ++i) {
        for (int j = i; j < NUM_POSITIONS; ++j) {
            int ra = a_codes[i] | a_codes[j];
            int rb = b_codes[i] | b_codes[j];
            long long key = (long long)ra << CODE_LEN | rb;
            lookup_table[key] = {i + 1, j + 1};
        }
    }
}

void solve() {
    int R, H;
    std::cin >> R >> H;

    precompute();

    int total_robots = CODE_LEN + CODE_LEN;
    std::vector<std::vector<int>> queries(total_robots);
    for (int p_idx = 0; p_idx < NUM_POSITIONS; ++p_idx) {
        int p = p_idx + 1;
        int ac = a_codes[p_idx];
        int bc = b_codes[p_idx];
        for (int bit = 0; bit < CODE_LEN; ++bit) {
            if ((ac >> bit) & 1) {
                queries[bit].push_back(p);
            }
            if ((bc >> bit) & 1) {
                queries[CODE_LEN + bit].push_back(p);
            }
        }
    }

    for (int i = 0; i < total_robots; ++i) {
        std::cout << "? " << queries[i].size();
        for (int pos : queries[i]) {
            std::cout << " " << pos;
        }
        std::cout << std::endl;
    }

    std::cout << "@" << std::endl;
    
    int L;
    std::cin >> L;
    std::vector<int> results(L);
    for (int i = 0; i < L; ++i) {
        std::cin >> results[i];
    }

    int ra_res = 0, rb_res = 0;
    for (int i = 0; i < CODE_LEN; ++i) {
        if (results[i]) ra_res |= (1 << i);
    }
    for (int i = 0; i < CODE_LEN; ++i) {
        if (results[CODE_LEN + i]) rb_res |= (1 << i);
    }

    long long res_key = (long long)ra_res << CODE_LEN | rb_res;
    std::pair<int, int> ans = lookup_table.at(res_key);
    std::cout << "! " << ans.first << " " << ans.second << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    solve();
    return 0;
}