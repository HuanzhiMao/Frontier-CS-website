#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <random>
#include <set>
#include <chrono>
#include <bitset>

const int N_POSITIONS = 1000;
const int N_ROBOTS = 26;
const int SIG_WEIGHT = 7;

std::vector<std::bitset<N_ROBOTS>> generate_signatures() {
    unsigned seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937 rng(seed);

    std::vector<int> p(N_ROBOTS);
    std::iota(p.begin(), p.end(), 0);

    std::set<std::bitset<N_ROBOTS>> sig_set;
    while (sig_set.size() < N_POSITIONS) {
        std::shuffle(p.begin(), p.end(), rng);
        std::bitset<N_ROBOTS> current_sig;
        for (int i = 0; i < SIG_WEIGHT; ++i) {
            current_sig[p[i]] = 1;
        }
        sig_set.insert(current_sig);
    }
    
    std::vector<std::bitset<N_ROBOTS>> signatures(sig_set.begin(), sig_set.end());
    return signatures;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int R_max, H_max;
    std::cin >> R_max >> H_max;

    auto signatures = generate_signatures();

    std::vector<std::vector<int>> queries(N_ROBOTS);
    for (int p = 0; p < N_POSITIONS; ++p) {
        for (int i = 0; i < N_ROBOTS; ++i) {
            if (signatures[p][i]) {
                queries[i].push_back(p + 1);
            }
        }
    }

    for (int i = 0; i < N_ROBOTS; ++i) {
        std::cout << "? " << queries[i].size();
        for (int pos : queries[i]) {
            std::cout << " " << pos;
        }
        std::cout << std::endl;
    }

    std::cout << "@" << std::endl;
    int L;
    std::cin >> L;
    std::bitset<N_ROBOTS> result_b;
    for (int i = 0; i < L; ++i) {
        int bit;
        std::cin >> bit;
        if (bit == 1) {
            result_b[i] = 1;
        }
    }

    std::vector<int> candidates;
    for (int p = 0; p < N_POSITIONS; ++p) {
        if ((signatures[p] & result_b) == signatures[p]) {
            candidates.push_back(p + 1);
        }
    }

    int c1 = -1, c2 = -1;
    bool found = false;
    for (size_t i = 0; i < candidates.size(); ++i) {
        for (size_t j = i; j < candidates.size(); ++j) {
            int p1 = candidates[i];
            int p2 = candidates[j];
            if ((signatures[p1 - 1] | signatures[p2 - 1]) == result_b) {
                c1 = p1;
                c2 = p2;
                found = true;
                break;
            }
        }
        if (found) break;
    }

    std::cout << "! " << c1 << " " << c2 << std::endl;

    return 0;
}