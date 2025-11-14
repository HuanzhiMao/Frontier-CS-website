#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>
#include <set>

using namespace std;

pair<int, int> do_query(const vector<int>& indices) {
    cout << "0 " << indices.size();
    for (int idx : indices) {
        cout << " " << idx;
    }
    cout << endl;
    int m1, m2;
    cin >> m1 >> m2;
    if (m1 > m2) swap(m1, m2);
    return {m1, m2};
}

void give_answer(int i1, int i2) {
    if (i1 > i2) swap(i1, i2);
    cout << "1 " << i1 << " " << i2 << endl;
}

int get_median_of_3(int i, int j, int k, int n) {
    vector<int> others;
    for (int l = 1; l <= n; ++l) {
        if (l != i && l != j && l != k) {
            others.push_back(l);
        }
    }
    
    pair<int, int> m1 = do_query({i, j, k, others[0]});
    pair<int, int> m2 = do_query({i, j, k, others[1]});

    if (m1.first == m2.first || m1.first == m2.second) {
        return m1.first;
    }
    return m1.second;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    int cand1 = 1, cand2 = 2;
    for (int i = 3; i <= n; ++i) {
        int median_val = get_median_of_3(cand1, cand2, i, n);
        
        pair<int, int> med_cand1_i = do_query({cand1, i, (i == 1 ? 2 : 1), (i==2?3:(i==1?3:2)) });
        
        bool cand1_is_median = (median_val == med_cand1_i.first || median_val == med_cand1_i.second);
        
        if (cand1_is_median) {
            // cand1 is median of {p[cand1], p[cand2], p[i]}, so cand2 or i is more extreme
            // To compare cand2 and i, we would need another query.
            // A simpler logic is to check which one of cand1, cand2, i is NOT the median_val.
            // The two that are not are the new candidates for extremes.
            // But we don't know which index has which value.
            // Instead, just compare the new element 'i' with each candidate
            int med_1_i_vs = get_median_of_3(cand1, i, (cand1==1||i==1)?(cand1==2||i==2?3:2):(1) , n);
            int med_2_i_vs = get_median_of_3(cand2, i, (cand2==1||i==1)?(cand2==2||i==2?3:2):(1) , n);
            
            if (med_1_i_vs != median_val) {
                cand2 = i;
            } else if (med_2_i_vs != median_val){
                cand1 = i;
            }
        }
    }

    int min_idx = cand1;
    int max_idx = cand2;

    vector<int> p_val(n + 1, 0);

    vector<int> other_indices;
    for(int i = 1; i <= n; ++i) {
        if (i != min_idx && i != max_idx) {
            other_indices.push_back(i);
        }
    }
    
    int ref_idx = other_indices[0];
    map<int, int> counts;
    vector<pair<int, int>> med_pairs;
    
    for(size_t i = 1; i < other_indices.size(); ++i) {
        int cur_idx = other_indices[i];
        pair<int, int> med = do_query({min_idx, max_idx, ref_idx, cur_idx});
        med_pairs.push_back(med);
        counts[med.first]++;
        counts[med.second]++;
    }

    int p_ref_val = -1;
    if (other_indices.size() > 1) {
        for(auto const& [val, num] : counts) {
            if(num == (int)other_indices.size() - 1) {
                p_ref_val = val;
                break;
            }
        }
    }
    
    if (p_ref_val == -1 && other_indices.size() > 2) {
        pair<int, int> m1 = do_query({min_idx, max_idx, ref_idx, other_indices[1]});
        pair<int, int> m2 = do_query({min_idx, max_idx, ref_idx, other_indices[2]});
        if (m1.first == m2.first || m1.first == m2.second) p_ref_val = m1.first;
        else p_ref_val = m1.second;
    } else if (p_ref_val == -1 && other_indices.size() > 1) {
        p_ref_val = do_query({min_idx, max_idx, ref_idx, other_indices[1]}).first;
    }


    p_val[ref_idx] = p_ref_val;
    int pair_idx = 0;
    for(size_t i = 1; i < other_indices.size(); ++i) {
        int cur_idx = other_indices[i];
        pair<int, int> med = med_pairs[pair_idx++];
        if (med.first == p_ref_val) {
            p_val[cur_idx] = med.second;
        } else {
            p_val[cur_idx] = med.first;
        }
    }

    vector<bool> seen(n + 1, false);
    for(int i = 1; i <= n; ++i) {
        if (p_val[i] > 0) seen[p_val[i]] = true;
    }
    int v_min = -1, v_max = -1;
    for(int i = 1; i <= n; ++i) {
        if (!seen[i]) {
            if (v_min == -1) v_min = i;
            else v_max = i;
        }
    }
    
    if(other_indices.size() >= 3) {
        pair<int,int> med_test = get_median_of_3(min_idx, other_indices[0], other_indices[1], n);
        if (med_test == p_val[other_indices[0]] || med_test == p_val[other_indices[1]]) {
            p_val[min_idx] = v_max;
            p_val[max_idx] = v_min;
        } else {
            p_val[min_idx] = v_min;
            p_val[max_idx] = v_max;
        }
    } else { // Fallback for n=6 or smaller testcases
        pair<int,int> med_test = do_query({min_idx, max_idx, other_indices[0], other_indices[1]});
        if (med_test.first > v_min && med_test.first < v_max) {
             p_val[min_idx] = v_min;
             p_val[max_idx] = v_max;
        } else {
             p_val[min_idx] = v_max;
             p_val[max_idx] = v_min;
        }
    }


    int ans_idx1 = -1, ans_idx2 = -1;
    for(int i=1; i<=n; ++i) {
        if (p_val[i] == n/2) ans_idx1 = i;
        if (p_val[i] == n/2 + 1) ans_idx2 = i;
    }

    give_answer(ans_idx1, ans_idx2);

    return 0;
}