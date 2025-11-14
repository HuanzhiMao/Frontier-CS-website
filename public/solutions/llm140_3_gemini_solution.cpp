#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <map>
#include <numeric>

using namespace std;

long long b;
int k;
int w;

struct Point {
    long long x, y;
};

// Function to make a query and get distances
vector<long long> query(const vector<Point>& probes) {
    if (probes.empty()) return {};
    cout << "? " << probes.size();
    for (const auto& p : probes) {
        cout << " " << p.x << " " << p.y;
    }
    cout << endl;

    int num_distances = k * probes.size();
    vector<long long> distances(num_distances);
    for (int i = 0; i < num_distances; ++i) {
        cin >> distances[i];
    }
    return distances;
}

// Decompose distances into arithmetic progressions
vector<long long> get_ap_starts(vector<long long>& distances, int ap_len) {
    map<long long, int> counts;
    for (long long d : distances) {
        counts[d]++;
    }

    vector<long long> starts;
    vector<long long> sorted_unique_dists;
    for(auto const& [d, count] : counts) {
        sorted_unique_dists.push_back(d);
    }

    for (long long v : sorted_unique_dists) {
        while (counts.count(v) && counts[v] > 0) {
            bool is_ap = true;
            for (int i = 0; i < ap_len; ++i) {
                if (!counts.count(v + i) || counts[v + i] == 0) {
                    is_ap = false;
                    break;
                }
            }
            if (is_ap) {
                starts.push_back(v);
                for (int i = 0; i < ap_len; ++i) {
                    counts[v + i]--;
                }
            } else {
                break;
            }
        }
    }
    return starts;
}

vector<long long> S_part, D_part;
vector<bool> used_D_part;
vector<int> current_matching_part;
vector<vector<pair<long long, long long>>> partial_solutions;

void find_all_matchings_part(int s_idx) {
    if (partial_solutions.size() >= 200) return;

    if (s_idx == S_part.size()) {
        vector<pair<long long, long long>> current_solution;
        for (size_t i = 0; i < S_part.size(); ++i) {
            current_solution.push_back({S_part[i], D_part[current_matching_part[i]]});
        }
        partial_solutions.push_back(current_solution);
        return;
    }

    long long S = S_part[s_idx];
    for (size_t i = 0; i < D_part.size(); ++i) {
        if (!used_D_part[i]) {
            long long D = D_part[i];
            
            long long S_plus_D = S + D;
            long long S_minus_D = S - D;

            if (abs(S_plus_D) <= 2 * b && abs(S_minus_D) <= 2 * b) {
                used_D_part[i] = true;
                current_matching_part[s_idx] = i;
                find_all_matchings_part(s_idx + 1);
                used_D_part[i] = false;
            }
        }
    }
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> b >> k >> w;

    long long C = 100000000;
    int dp = 2 * k + 1;

    vector<Point> probes1;
    for (int j = 1; j <= dp; ++j) {
        probes1.push_back({C, C + j});
    }
    auto dists1 = query(probes1);
    auto starts1 = get_ap_starts(dists1, dp);
    vector<long long> S_vals;
    for (long long start : starts1) {
        S_vals.push_back(2 * C + 1 - start);
    }

    vector<Point> probes2;
    for (int j = 1; j <= dp; ++j) {
        probes2.push_back({C, -C + j});
    }
    auto dists2 = query(probes2);
    auto starts2 = get_ap_starts(dists2, dp);
    vector<long long> D_vals;
    for (long long start : starts2) {
        D_vals.push_back(2 * C - dp - start);
    }

    vector<long long> S_even, S_odd, D_even, D_odd;
    for(long long s : S_vals) {
        if (abs(s) % 2 == 0) S_even.push_back(s); else S_odd.push_back(s);
    }
    for(long long d : D_vals) {
        if (abs(d) % 2 == 0) D_even.push_back(d); else D_odd.push_back(d);
    }
    
    // Match even
    S_part = S_even;
    D_part = D_even;
    used_D_part.assign(D_part.size(), false);
    current_matching_part.resize(S_part.size());
    partial_solutions.clear();
    if(!S_part.empty()) find_all_matchings_part(0);
    vector<vector<pair<long long, long long>>> even_solutions = partial_solutions;

    // Match odd
    S_part = S_odd;
    D_part = D_odd;
    used_D_part.assign(D_part.size(), false);
    current_matching_part.resize(S_part.size());
    partial_solutions.clear();
    if(!S_part.empty()) find_all_matchings_part(0);
    vector<vector<pair<long long, long long>>> odd_solutions = partial_solutions;
    
    vector<vector<Point>> solution_sets;
    if (S_even.empty()) {
        for(const auto& odd_sol : odd_solutions) {
            if (solution_sets.size() >= 200) break;
            vector<Point> full_sol;
            for(const auto& p : odd_sol) {
                full_sol.push_back({(p.first + p.second)/2, (p.first - p.second)/2});
            }
            solution_sets.push_back(full_sol);
        }
    } else if (S_odd.empty()) {
        for(const auto& even_sol : even_solutions) {
             if (solution_sets.size() >= 200) break;
            vector<Point> full_sol;
            for(const auto& p : even_sol) {
                full_sol.push_back({(p.first + p.second)/2, (p.first - p.second)/2});
            }
            solution_sets.push_back(full_sol);
        }
    } else {
        for(const auto& even_sol : even_solutions) {
            if (solution_sets.size() >= 200) break;
            for(const auto& odd_sol : odd_solutions) {
                if (solution_sets.size() >= 200) break;
                vector<Point> full_sol;
                for(const auto& p : even_sol) full_sol.push_back({(p.first + p.second)/2, (p.first - p.second)/2});
                for(const auto& p : odd_sol) full_sol.push_back({(p.first + p.second)/2, (p.first - p.second)/2});
                solution_sets.push_back(full_sol);
            }
        }
    }
    
    while (solution_sets.size() > 1) {
        Point p_to_test = {-1, -1};
        
        vector<Point> s1 = solution_sets[0];
        vector<Point> s2 = solution_sets[1];
        sort(s1.begin(), s1.end(), [](Point a, Point b){ return a.x < b.x || (a.x == b.x && a.y < b.y); });
        sort(s2.begin(), s2.end(), [](Point a, Point b){ return a.x < b.x || (a.x == b.x && a.y < b.y); });

        bool found_diff = false;
        for(int i=0; i<k; ++i){
            if(s1[i].x != s2[i].x || s1[i].y != s2[i].y){
                p_to_test = s1[i];
                found_diff = true;
                break;
            }
        }
        if(!found_diff){
           p_to_test = s2[0];
        }


        auto dists = query({{p_to_test}});
        bool is_deposit = false;
        for (long long d : dists) {
            if (d == 0) {
                is_deposit = true;
                break;
            }
        }

        vector<vector<Point>> next_solutions;
        for (const auto& sol : solution_sets) {
            bool has_p = false;
            for (const auto& p_sol : sol) {
                if (p_sol.x == p_to_test.x && p_sol.y == p_to_test.y) {
                    has_p = true;
                    break;
                }
            }
            if (has_p == is_deposit) {
                next_solutions.push_back(sol);
            }
        }
        solution_sets = next_solutions;
        if(solution_sets.empty()) return 1;
    }

    cout << "!";
    for (const auto& p : solution_sets[0]) {
        cout << " " << p.x << " " << p.y;
    }
    cout << endl;

    return 0;
}