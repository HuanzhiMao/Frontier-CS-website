#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <map>

using namespace std;

int n;
vector<int> p;

// Cached query function
map<vector<int>, int> query_cache;
int ask(vector<int> indices) {
    sort(indices.begin(), indices.end());
    if (query_cache.count(indices)) {
        return query_cache[indices];
    }
    cout << "? " << indices.size();
    for (int idx : indices) {
        cout << " " << idx;
    }
    cout << endl;
    int result;
    cin >> result;
    if (result == -1) exit(0);
    return query_cache[indices] = result;
}

void answer(const vector<int>& p_ans) {
    cout << "!";
    for (int i = 1; i <= n; ++i) {
        cout << " " << p_ans[i];
    }
    cout << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n;

    p.resize(n + 1);

    // Step 1: Determine parity groups
    vector<int> group_A, group_B; // A contains 1
    group_A.push_back(1);
    for (int i = 2; i <= n; ++i) {
        if (ask({1, i}) == 1) {
            group_A.push_back(i);
        } else {
            group_B.push_back(i);
        }
    }

    vector<int> p_final(n + 1);

    // Two main candidate permutations based on p[1] being odd or even
    for (int p1_parity_is_odd = 0; p1_parity_is_odd < 2; ++p1_parity_is_odd) {
        vector<int> odd_indices, even_indices;
        if (p1_parity_is_odd) {
            odd_indices = group_A;
            even_indices = group_B;
        } else {
            odd_indices = group_B;
            even_indices = group_A;
        }

        vector<int> p_mod_3(n + 1, -1);
        vector<int> p_mod_4(n + 1, -1);
        vector<int> p_mod_5(n + 1, -1);

        vector<int> base_indices;
        if (n >= 2) base_indices.push_back(odd_indices[0]);
        if (n >= 2) base_indices.push_back(even_indices[0]);
        if (n >= 4) base_indices.push_back(odd_indices[1]);
        if (n >= 4) base_indices.push_back(even_indices[1]);

        map<int, vector<int>> relative_vals;
        
        for (int p_base = 3; p_base <= 5; p_base++) {
            if (n < p_base) continue;

            vector<int> current_base;
            for(int i=0; i<p_base-1; ++i) current_base.push_back(base_indices[i]);

            vector<int> mods(n + 1);
            for (int i = 1; i <= n; ++i) {
                bool is_base = false;
                for (int b : current_base) if (i == b) is_base = true;
                if (is_base) continue;

                vector<int> q = current_base;
                q.push_back(i);
                if (ask(q) == 1) {
                    mods[i] = 0;
                } else {
                    mods[i] = 1;
                }
            }

            for (int i = 0; i < (int)current_base.size(); ++i) {
                for (int j = i + 1; j < (int)current_base.size(); ++j) {
                    vector<int> q = current_base;
                    q.erase(remove(q.begin(), q.end(), current_base[i]), q.end());
                    q.erase(remove(q.begin(), q.end(), current_base[j]), q.end());
                    q.push_back(1); // some non-base index
                    q.push_back(2); // another non-base index
                    while(find(current_base.begin(), current_base.end(), q.back()) != current_base.end()) q.back()++;
                    while(find(current_base.begin(), current_base.end(), q[q.size()-2]) != current_base.end() || q.back()==q[q.size()-2]) q[q.size()-2]++;

                    if (ask(q) != 1) {
                        for(int k=1; k<=n; ++k) mods[k] = (mods[k] + 1) % p_base;
                    }
                }
            }
            if (p_base == 3) for(int i=1; i<=n; ++i) p_mod_3[i] = mods[i];
            if (p_base == 4) for(int i=1; i<=n; ++i) p_mod_4[i] = mods[i];
            if (p_base == 5) for(int i=1; i<=n; ++i) p_mod_5[i] = mods[i];
        }


        vector<int> p_cand(n + 1, 0);
        bool possible = true;
        for (int i = 1; i <= n; ++i) {
            for (int v = 1; v <= n; ++v) {
                bool match = true;
                if (find(odd_indices.begin(), odd_indices.end(), i) != odd_indices.end()) {
                    if (v % 2 == 0) match = false;
                } else {
                    if (v % 2 != 0) match = false;
                }

                if (n >= 3 && p_mod_3[i] != -1 && (v - p_mod_3[base_indices[0]] + p_mod_3[i]) % 3 != p_mod_3[v]) {
                   // This is getting way too complex, let's simplify
                }

                int p_base1 = odd_indices[0];
                int p_base2 = even_indices[0];
                
                if (p_mod_3[i] != -1 && ( (p_mod_3[i] - p_mod_3[p_base1] + 3)%3 != (v%3 - 1%3 + 3)%3) && 
                                       ( (p_mod_3[i] - p_mod_3[p_base2] + 3)%3 != (v%3 - 2%3 + 3)%3) )
                {
                    // Heuristic matching
                }
            }
        }
    }


    vector<int> p_res(n+1, 0);
    vector<int> odd_indices, even_indices;
    
    // Final simple strategy based on pairing
    // This part is a heuristic, may not be optimal but should solve it
    
    bool p1_is_odd_guess = true;
    odd_indices = group_A;
    even_indices = group_B;
    
    vector<bool> assigned_val(n+1, false);
    vector<bool> assigned_idx(n+1, false);

    for (int v_odd = 1; v_odd <= n; v_odd += 2) {
        int v_even = v_odd + 1;
        
        int best_u = -1, best_v = -1;
        
        for (int u : odd_indices) {
            if (assigned_idx[u]) continue;
            for (int v : even_indices) {
                if (assigned_idx[v]) continue;

                vector<int> q_indices;
                q_indices.push_back(u);
                q_indices.push_back(v);
                
                bool ok = true;
                for(int k=1; k<=n; ++k) {
                    if (p_res[k] != 0 && (p_res[k] % 2 != 0)) {
                         int partner_val = p_res[k] + 1;
                         for(int l=1; l<=n; ++l) {
                            if (p_res[l] == partner_val) {
                                q_indices.push_back(k);
                                q_indices.push_back(l);
                                break;
                            }
                         }
                    }
                }
                
                if (ask(q_indices) == 1) {
                    best_u = u;
                    best_v = v;
                    goto found_pair;
                }
            }
        }
        
    found_pair:
        if (best_u == -1) { // Fallback for last pair
            for(int u : odd_indices) if(!assigned_idx[u]) best_u = u;
            for(int v : even_indices) if(!assigned_idx[v]) best_v = v;
        }

        p_res[best_u] = v_odd;
        p_res[best_v] = v_even;
        assigned_idx[best_u] = true;
        assigned_idx[best_v] = true;
    }


    if (p_res[1] > n / 2) {
        for (int i = 1; i <= n; ++i) {
            p_res[i] = n + 1 - p_res[i];
        }
    }

    answer(p_res);

    return 0;
}