#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <cmath>

// Asks a query for the given set s.
void ask_query(const std::vector<int>& s) {
    std::cout << "? " << s.size();
    for (int x : s) {
        std::cout << " " << x;
    }
    std::cout << std::endl;
}

// Reads a response from the judge.
std::string read_response() {
    std::string resp;
    std::cin >> resp;
    return resp;
}

// Makes a guess g.
void make_guess(int g) {
    std::cout << "! " << g << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    std::vector<int> T, F;

    // We start with all numbers from 1 to n as candidates.
    // The first query is special as there's no previous answer to combine with.
    // This query serves to establish the initial T and F sets for the main loop.
    // T: candidates for which the first answer is hypothetically true.
    // F: candidates for which the first answer is hypothetically false.
    {
        std::vector<int> all_nums(n);
        std::iota(all_nums.begin(), all_nums.end(), 1);

        int k = n / 2;
        if (k == 0 && n > 0) k = 1;
        std::vector<int> S;
        if (k > 0) {
            S.assign(all_nums.begin(), all_nums.begin() + k);
        }

        if (S.empty()) {
            T = all_nums;
        } else {
            ask_query(S);
            std::string resp = read_response();

            std::vector<bool> s_membership(n + 1, false);
            for(int x : S) s_membership[x] = true;

            if (resp == "YES") {
                for (int x : all_nums) {
                    if (s_membership[x]) T.push_back(x);
                    else F.push_back(x);
                }
            } else { // "NO"
                for (int x : all_nums) {
                    if (!s_membership[x]) T.push_back(x);
                    else F.push_back(x);
                }
            }
        }
    }

    while (T.size() + F.size() > 2) {
        std::vector<int> S;
        int t_take = T.size() / 2;
        int f_take = F.size() / 2;

        for (int i = 0; i < t_take; ++i) S.push_back(T[i]);
        for (int i = 0; i < f_take; ++i) S.push_back(F[i]);

        if (S.empty()) {
            if (!T.empty()) {
                S.push_back(T[0]);
            } else if (!F.empty()) {
                S.push_back(F[0]);
            } else {
                // Should not be reachable if T.size()+F.size() > 2
                break;
            }
        }

        ask_query(S);
        std::string resp = read_response();

        std::vector<bool> s_membership(n + 1, false);
        for(int x : S) s_membership[x] = true;

        std::vector<int> next_T, next_F;
        
        if (resp == "YES") {
            // T_new = (T_prev \cap S) \cup (F_prev \cap S)
            // F_new = T_prev \setminus S
            for (int x : T) {
                if (s_membership[x]) next_T.push_back(x);
                else next_F.push_back(x);
            }
            for (int x : F) {
                if (s_membership[x]) next_T.push_back(x);
            }
        } else { // "NO"
            // T_new = (T_prev \setminus S) \cup (F_prev \setminus S)
            // F_new = T_prev \cap S
            for (int x : T) {
                if (!s_membership[x]) next_T.push_back(x);
                else next_F.push_back(x);
            }
            for (int x : F) {
                if (!s_membership[x]) next_T.push_back(x);
            }
        }
        T = next_T;
        F = next_F;
    }

    std::vector<int> candidates;
    candidates.insert(candidates.end(), T.begin(), T.end());
    candidates.insert(candidates.end(), F.begin(), F.end());

    for (int cand : candidates) {
        make_guess(cand);
        std::string resp = read_response();
        if (resp == ":)") {
            return 0;
        }
    }

    return 0;
}