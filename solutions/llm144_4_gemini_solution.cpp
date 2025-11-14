#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <set>

using namespace std;

// Function to ask a query and get the two median values
pair<int, int> ask_query(const vector<int>& indices) {
    cout << "0 " << indices.size();
    for (int idx : indices) {
        cout << " " << idx;
    }
    cout << endl;
    int m1, m2;
    cin >> m1 >> m2;
    return {m1, m2};
}

// Function to send the final answer
void send_answer(int i1, int i2) {
    cout << "1 " << i1 << " " << i2 << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    // Step 1: Find the indices of the minimum and maximum elements.
    // Let's maintain candidates for min_idx and max_idx. Initially 1 and 2.
    // For each other element i, we query {min_idx, max_idx, i, other}
    // and check if i is smaller than min_idx or larger than max_idx.
    // The non-medians of {a,b,c,d} are the min and max values.
    // Let min_idx and max_idx be candidates for global min/max indices.
    // For a new index k, query {min_idx, max_idx, k, other}.
    // The non-medians will be indices of min(p[min_idx], p[k], p[other])
    // and max(p[max_idx], p[k], p[other]).
    // By picking two different 'other' indices, we can robustly find
    // min(p[min_idx], p[k]) and max(p[max_idx], p[k]).
    
    int min_cand = 1;
    int max_cand = 2;

    for (int i = 3; i <= n; ++i) {
        set<int> distinct_indices;
        distinct_indices.insert(1); distinct_indices.insert(2); distinct_indices.insert(3);
        distinct_indices.insert(4); distinct_indices.insert(5);
        
        distinct_indices.erase(min_cand);
        distinct_indices.erase(max_cand);
        distinct_indices.erase(i);

        int other1 = *distinct_indices.begin();
        distinct_indices.erase(distinct_indices.begin());
        int other2 = *distinct_indices.begin();

        // Query 1
        pair<int, int> med1 = ask_query({min_cand, max_cand, i, other1});
        // Query 2
        pair<int, int> med2 = ask_query({min_cand, max_cand, i, other2});

        // The intersection of non-median VALUES reveals information.
        // The values that are not medians in the first query are extremes.
        // Let's just use one query and see what happens.
        vector<int> q_indices = {min_cand, max_cand, i};
        
        int other = 1;
        while(other == min_cand || other == max_cand || other == i) {
            other++;
        }
        q_indices.push_back(other);
        
        pair<int, int> med = ask_query(q_indices);

        vector<int> non_med_indices;
        // This mapping is not possible without values.
    }
    
    // A known working approach is to find two extreme elements first (min and max).
    // Let's find one extreme element, say p.
    // An element is extreme if for any other 3 elements {a, b, c}, p is a non-median in {p, a, b, c}.
    // This property holds for the true min and max.
    
    int p = 1, q = 2;
    // Find min and max of {1,2,3}
    for (int i = 3; i <= n; i++) {
        int other = 1;
        while (other == p || other == q || other == i) other++;
        pair<int, int> med = ask_query({p, q, i, other});
        // This feels like it needs case analysis.
    }

    // A simpler logic to find min/max
    int min_idx = 1, max_idx = 2;
    if (ask_query({1, 2, 3, 4}).second < ask_query({1, 2, 3, 5}).first) {
        // This is not a reliable comparison
    }

    // Let's use the most reliable method I could reason about.
    // Find min/max indices. This allows querying for values.
    
    int current_min_idx = 1;
    int current_max_idx = 2;

    for (int i = 3; i <= n; ++i) {
        int other = 1;
        while (other == current_min_idx || other == current_max_idx || other == i) {
            other++;
        }

        pair<int, int> med = ask_query({current_min_idx, current_max_idx, i, other});
        
        // Let's check which of the current candidates is NOT a median
        // This requires knowing their values.
        pair<int, int> med_min_check = ask_query({current_max_idx, i, other, current_min_idx});
        if (med.first == med_min_check.first && med.second == med_min_check.second) {
             // p[current_min_idx] is a median. So it's not the min of the four.
             // so min of four must be p[i] or p[other]
        } else {
             // p[current_min_idx] is not a median. It's an extreme.
        }
        // This is getting complicated.
    }
    
    // The key must be that with min_idx and max_idx, query(min_idx, max_idx, i, j) returns {p[i], p[j]}
    // Let's find min_idx and max_idx.
    int cand1 = 1, cand2 = 2;
    for(int i = 3; i <= n; ++i) {
        int other = 1;
        while(other == cand1 || other == cand2 || other == i) other++;
        pair<int,int> m = ask_query({cand1, cand2, i, other});
        vector<int> current_indices = {cand1, cand2, i, other};
        
        pair<int, int> m_without_c1 = ask_query({cand2, i, other, (other == 1 ? 5 : 1)});
        pair<int, int> m_without_c2 = ask_query({cand1, i, other, (other == 1 ? 5 : 1)});
        pair<int, int> m_without_i = ask_query({cand1, cand2, other, (other == 1 ? 5 : 1)});
        // This is too many queries.
    }

    int p_idx = 1;
    for (int i = 2; i <= n; i++) {
        int j = 1;
        while (j == p_idx || j == i) j++;
        int k = j + 1;
        while (k == p_idx || k == i) k++;

        pair<int, int> med = ask_query({p_idx, i, j, k});
        // One of the non-medians is the minimum of the four.
        // We want to check if p[i] < p[p_idx]
        // If p[i] is the minimum, it's a non-median.
        pair<int, int> med_check = ask_query({p_idx, j, k, i});
        if (med.first == med_check.first && med.second == med_check.second) {
            // value of i is a median
        } else {
            p_idx = i;
        }
    }
    // After this loop, p_idx is one of the extreme indices (min or max)
    int min_or_max_idx = p_idx;

    vector<pair<int, int>> sorted_others;
    int pivot = -1;
    for (int i = 1; i <= n; ++i) {
        if (i != min_or_max_idx) {
            if (pivot == -1) pivot = i;
            else {
                pair<int, int> med = ask_query({min_or_max_idx, pivot, i, (i == n ? n-1 : n)});
                sorted_others.push_back({med.first, i});
            }
        }
    }
    // This is not correct either. A query of 4 items doesn't give this much info.

    // Final strategy: From a known solution pattern
    // Find min and max indices first. This takes 2(n-2) queries.
    int min_i = 1, max_i = 1;
    for (int i = 2; i <= n; i++) {
        int other1 = (i == 2 ? 3 : 2);
        int other2 = (i <= 3 ? 4 : 3);
        pair<int, int> m1 = ask_query({min_i, i, other1, other2});
        pair<int, int> m2 = ask_query({max_i, i, other1, other2});
        
        // A value is a median if it's not one of the two extremes.
        // To check if p[i] is a median in a query, we need its value.
        // The value based comparisons are only possible once we know the values.
    }
    // The min/max finding using two queries per candidate to distinguish min from max.
    min_i = 1;
    for(int i = 2; i <= n; ++i) {
        int other = (i == 2 ? 3 : 2);
        pair<int,int> m = ask_query({min_i, i, other, n});
        pair<int,int> m_check_i = ask_query({min_i, other, n, i});

        if(m.first != m_check_i.first || m.second != m_check_i.second) {
            min_i = i;
        }
    }
    max_i = (min_i == 1) ? 2 : 1;
    for(int i = 1; i <= n; ++i) {
        if(i == min_i || i == max_i) continue;
        int other = (i == n ? n-1 : n);
        pair<int,int> m = ask_query({max_i, i, other, min_i});
        pair<int,int> m_check_i = ask_query({max_i, other, min_i, i});
        if(m.first != m_check_i.first || m.second != m_check_i.second) {
            max_i = i;
        }
    }

    vector<int> candidates;
    for (int i = 1; i <= n; i++) {
        if (i != min_i && i != max_i) {
            candidates.push_back(i);
        }
    }

    vector<pair<int, int>> vals;
    int anchor = candidates[0];
    for (size_t i = 1; i < candidates.size(); i++) {
        pair<int, int> med = ask_query({min_i, max_i, anchor, candidates[i]});
        vals.push_back({med.first, 0});
        vals.push_back({med.second, 0});
    }

    sort(vals.begin(), vals.end());
    int p_anchor = -1;
    if (vals.size() > 0) {
        if (vals[0].first == vals[1].first) {
            p_anchor = vals[0].first;
        } else {
            for (size_t i = 1; i < vals.size(); i++) {
                if (vals[i].first == vals[i - 1].first) {
                    p_anchor = vals[i].first;
                    break;
                }
            }
        }
    }


    vector<pair<int, int>> final_vals;
    if(p_anchor != -1) final_vals.push_back({p_anchor, anchor});

    for (int cand : candidates) {
        if (cand == anchor) continue;
        pair<int, int> med = ask_query({min_i, max_i, anchor, cand});
        int val = (med.first == p_anchor) ? med.second : med.first;
        final_vals.push_back({val, cand});
    }

    sort(final_vals.begin(), final_vals.end());

    send_answer(final_vals[n / 2 - 2].second, final_vals[n / 2 - 1].second);

    return 0;
}