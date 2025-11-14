#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <cassert>

using namespace std;

// Function to ask a query and get the result
char ask(int i, int j) {
    cout << "? " << i << " " << j << endl;
    char response;
    cin >> response;
    if (response == '0') exit(0); // Exit if interactor signals an error
    return response;
}

// Function to print the final answer
void answer(int n, const vector<int>& p) {
    cout << "! ";
    for (int i = 1; i <= n; ++i) {
        cout << p[i] << (i == n ? "" : " ");
    }
    cout << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    vector<int> p(n + 1, 0);
    vector<int> R; // Carry-over values

    int i = 1;
    for (; i <= n - 2; i += 3) {
        // Determine the 5 candidate values for the block i, i+1, i+2
        vector<int> U = R;
        if (i == 1) {
            for (int k = 1; k <= 5 && k <= n; ++k) U.push_back(k);
        } else {
            for (int k = 2; k <= 4; ++k) if (i + k <= n) U.push_back(i + k);
        }
        sort(U.begin(), U.end());
        U.erase(unique(U.begin(), U.end()), U.end());
        
        // Sort indices i, i+1, i+2 based on their values
        vector<int> current_indices = {i, i + 1, i + 2};
        if (ask(current_indices[0], current_indices[1]) == '>') swap(current_indices[0], current_indices[1]);
        if (ask(current_indices[1], current_indices[2]) == '>') {
             swap(current_indices[1], current_indices[2]);
             if (ask(current_indices[0], current_indices[1]) == '>') {
                 swap(current_indices[0], current_indices[1]);
             }
        }
        int p_a_idx = current_indices[0], p_c_idx = current_indices[2];

        // Identify the 3 values from the 5 candidates
        vector<int> identified_p_vals;
        int next_query_idx = i + 3;

        if (next_query_idx > n || U.size() < 5) {
            identified_p_vals = U;
        } else {
            char res_a = ask(p_a_idx, next_query_idx);
            char res_c = ask(p_c_idx, next_query_idx);

            for (int j1 = 0; j1 < U.size(); ++j1) {
            for (int j2 = j1 + 1; j2 < U.size(); ++j2) {
            for (int j3 = j2 + 1; j3 < U.size(); ++j3) {
                vector<int> current_p_subset = {U[j1], U[j2], U[j3]};
                vector<int> g_vals;
                vector<bool> is_p_val(n + 6, false);
                for (int v : current_p_subset) is_p_val[v] = true;
                for (int v : U) if (!is_p_val[v]) g_vals.push_back(v);

                vector<int> p4_cands = g_vals;
                if (i + 5 <= n) p4_cands.push_back(i + 5);

                bool possible = false;
                if (p4_cands.empty()) {
                    possible = true;
                } else {
                    for (int p4_cand : p4_cands) {
                        if ((current_p_subset[0] < p4_cand) == (res_a == '<') && 
                            (current_p_subset[2] < p4_cand) == (res_c == '<')) {
                            possible = true;
                            break;
                        }
                    }
                }
                if (possible) {
                    identified_p_vals = current_p_subset;
                }
            }}}
        }
        
        // Assign identified values to p[i], p[i+1], p[i+2]
        vector<int> R_prev = R;
        if (i == 1) R_prev.clear();

        vector<int> C_i = R_prev;
        if (i + 2 <= n) C_i.push_back(i + 2);
        for(int val : identified_p_vals) {
            if(find(C_i.begin(), C_i.end(), val) != C_i.end()) {
                p[i] = val;
                break;
            }
        }

        vector<int> C_i1;
        for(int c: C_i) if(c!=p[i]) C_i1.push_back(c);
        if (i + 3 <= n) C_i1.push_back(i+3);
        for(int val : identified_p_vals) {
            if(val == p[i]) continue;
            if(find(C_i1.begin(), C_i1.end(), val) != C_i1.end()) {
                p[i+1] = val;
                break;
            }
        }
        
        for(int val : identified_p_vals) {
            if (val != p[i] && val != p[i+1]) {
                p[i+2] = val;
                break;
            }
        }

        // Update carry-over set R
        R.clear();
        vector<bool> p_block_vals(n + 6, false);
        p_block_vals[p[i]] = p_block_vals[p[i+1]] = p_block_vals[p[i+2]] = true;
        for (int v : U) {
            if (!p_block_vals[v]) {
                R.push_back(v);
            }
        }
        sort(R.begin(), R.end());
    }

    // Handle remaining elements if n is not a multiple of 3
    if (i <= n) { 
        vector<int> unused_vals;
        vector<bool> used(n + 1, false);
        for (int k = 1; k < i; ++k) used[p[k]] = true;
        for (int k = 1; k <= n; ++k) if (!used[k]) unused_vals.push_back(k);

        if (i + 1 == n) { // two elements left
            char res = ask(i, i + 1);
            if (res == '<') {
                p[i] = unused_vals[0];
                p[i + 1] = unused_vals[1];
            } else {
                p[i] = unused_vals[1];
                p[i + 1] = unused_vals[0];
            }
        } else if (i == n) { // one element left
            p[i] = unused_vals[0];
        }
    }
    
    answer(n, p);

    return 0;
}
