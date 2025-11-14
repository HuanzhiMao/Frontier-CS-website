#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <map>

using namespace std;

// Helper to ask a query
int ask(int l, int r) {
    cout << "? " << l << " " << r << endl;
    int x;
    cin >> x;
    return x;
}

// State for each test case
int n;
vector<int> candidates;
map<int, vector<pair<int, int>>> states;

// Update candidates and their states based on query response
void update(int l, int r, int x) {
    int len = r - l + 1;
    
    vector<int> next_candidates;
    map<int, vector<pair<int, int>>> next_states;

    for (int cand : candidates) {
        bool is_in = (cand >= l && cand <= r);
        vector<pair<int, int>> possible_new_states;

        for (auto p : states[cand]) {
            int ch = p.first;
            int cd = p.second;

            // Try honest transition
            if (ch < 2) {
                bool honest_cond = (x == len && !is_in) || (x == len - 1 && is_in);
                if (honest_cond) {
                    possible_new_states.push_back({ch + 1, 0});
                }
            }

            // Try dishonest transition
            if (cd < 2) {
                bool dishonest_cond = (x == len && is_in) || (x == len - 1 && !is_in);
                if (dishonest_cond) {
                    possible_new_states.push_back({0, cd + 1});
                }
            }
        }
        
        if (!possible_new_states.empty()) {
            sort(possible_new_states.begin(), possible_new_states.end());
            possible_new_states.erase(unique(possible_new_states.begin(), possible_new_states.end()), possible_new_states.end());
            
            next_candidates.push_back(cand);
            next_states[cand] = possible_new_states;
        }
    }
    candidates = next_candidates;
    states = next_states;
}

void solve() {
    cin >> n;

    // Initialization
    candidates.clear();
    states.clear();
    for (int i = 1; i <= n; ++i) {
        candidates.push_back(i);
        states[i].push_back({0, 0});
    }

    // Initial queries to create state diversity
    if (n >= 4) {
        int m = n / 2;
        int x1 = ask(1, m);
        update(1, m, x1);
        if (candidates.size() > 2) {
            int x2 = ask(m + 1, n);
            update(m + 1, n, x2);
        }
    } else { 
        int m = n / 2;
        if(m > 0) {
            int x1 = ask(1, m);
            update(1, m, x1);
        }
    }


    while (candidates.size() > 2) {
        vector<int> A, B, C; // H-only, D-only, both
        for (int cand : candidates) {
            bool can_H = false, can_D = false;
            for (auto p : states[cand]) {
                if (p.first < 2) can_H = true;
                if (p.second < 2) can_D = true;
            }
            if (can_H && !can_D) A.push_back(cand);
            else if (!can_H && can_D) B.push_back(cand);
            else C.push_back(cand);
        }

        int l = 1, r = 1;

        if (A.empty() && B.empty()) {
            vector<int> sorted_cands = candidates;
            sort(sorted_cands.begin(), sorted_cands.end());
            int mid_idx = sorted_cands.size() / 2;
            if (mid_idx > 0) {
                 r = sorted_cands[mid_idx - 1];
            } else {
                 r = sorted_cands[0];
            }
            l = 1;
        } else {
            vector<pair<int, int>> S; // {val, type}, type 0 for A, 1 for B
            for (int val : A) S.push_back({val, 0});
            for (int val : B) S.push_back({val, 1});
            sort(S.begin(), S.end());

            vector<int> prefA(S.size() + 1, 0);
            vector<int> prefB(S.size() + 1, 0);
            for (size_t i = 0; i < S.size(); ++i) {
                prefA[i+1] = prefA[i] + (S[i].second == 0);
                prefB[i+1] = prefB[i] + (S[i].second == 1);
            }
            
            long long target_val_num = (long long)B.size() - A.size();
            int best_k = 0;
            long long min_abs_diff = -1;

            for (size_t k = 0; k <= S.size(); ++k) {
                long long current_diff = 2LL * (prefB[k] - prefA[k]) - target_val_num;
                if (min_abs_diff == -1 || abs(current_diff) < min_abs_diff) {
                    min_abs_diff = abs(current_diff);
                    best_k = k;
                }
            }
            
            l = 1;
            if (best_k == 0) {
                if (S.empty()) {
                    r = n / 2;
                } else {
                    r = S[0].first - 1;
                }
            } else {
                r = S[best_k-1].first;
            }
            
            if (r < l) {
                r = 1; 
            }
        }
        
        int x = ask(l, r);
        update(l, r, x);
    }

    // Guessing phase
    for (int cand : candidates) {
        cout << "! " << cand << endl;
        int y;
        cin >> y;
        if (y == 1) break;
    }

    cout << "#" << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.flush();
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}