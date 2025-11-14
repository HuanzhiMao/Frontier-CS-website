#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <iterator>

void ask_query(const std::vector<int>& s) {
    std::cout << "? " << s.size();
    for (int x : s) {
        std::cout << " " << x;
    }
    std::cout << std::endl;
}

void make_guess(int g) {
    std::cout << "! " << g << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    std::vector<int> p(n);
    std::iota(p.begin(), p.end(), 1);
    std::vector<int> q;

    while (p.size() + q.size() > 2) {
        std::vector<int> s;
        
        int p_split_idx = p.size() / 2;
        int q_split_idx = q.size() / 2;

        s.reserve(p_split_idx + q_split_idx);
        s.insert(s.end(), p.begin(), p.begin() + p_split_idx);
        s.insert(s.end(), q.begin(), q.begin() + q_split_idx);
        
        if (s.empty()) {
            if (!p.empty()) s.push_back(p[0]);
            else if (!q.empty()) s.push_back(q[0]);
            else break; 
        }

        ask_query(s);

        std::string reply;
        std::cin >> reply;

        std::sort(s.begin(), s.end());

        std::vector<int> p_cup_q;
        p_cup_q.reserve(p.size() + q.size());
        std::merge(p.begin(), p.end(),
                   q.begin(), q.end(),
                   std::back_inserter(p_cup_q));
        
        std::vector<int> p_new, q_new;

        if (reply == "YES") { // T=S, L=U\S
            p_new.reserve(p_cup_q.size());
            std::set_intersection(p_cup_q.begin(), p_cup_q.end(),
                                  s.begin(), s.end(),
                                  std::back_inserter(p_new));
            q_new.reserve(p.size());
            std::set_difference(p.begin(), p.end(),
                                s.begin(), s.end(),
                                std::back_inserter(q_new));
        } else { // T=U\S, L=S
            p_new.reserve(p_cup_q.size());
            std::set_difference(p_cup_q.begin(), p_cup_q.end(),
                                s.begin(), s.end(),
                                std::back_inserter(p_new));
            q_new.reserve(p.size());
            std::set_intersection(p.begin(), p.end(),
                                  s.begin(), s.end(),
                                  std::back_inserter(q_new));
        }

        p = std::move(p_new);
        q = std::move(q_new);
    }

    std::vector<int> candidates;
    candidates.reserve(p.size() + q.size());
    std::merge(p.begin(), p.end(),
               q.begin(), q.end(),
               std::back_inserter(candidates));
    
    for (int cand : candidates) {
        make_guess(cand);
        std::string res;
        std::cin >> res;
        if (res == ":)") {
            return 0;
        }
    }

    return 0;
}