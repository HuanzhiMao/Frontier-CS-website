#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

bool ask_query(const vector<int>& positions) {
    if (positions.empty()) return true;
    cout << "? " << positions.size();
    for (int pos : positions) {
        cout << " " << pos;
    }
    cout << endl;
    int result;
    cin >> result;
    return result == 1;
}

void answer(const vector<int>& p) {
    cout << "!";
    for (size_t i = 1; i < p.size(); ++i) {
        cout << " " << p[i];
    }
    cout << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    vector<int> p(n + 1, 0);
    int pos1 = -1, posn = -1;

    for (int i = 1; i <= n; ++i) {
        vector<int> query_indices;
        for (int j = 1; j <= n; ++j) {
            if (i == j) continue;
            query_indices.push_back(j);
        }
        if (ask_query(query_indices)) {
            if (pos1 == -1) pos1 = i;
            else posn = i;
        }
    }

    p[pos1] = 1;
    p[posn] = n;

    vector<int> mod2(n + 1, -1);
    mod2[pos1] = 1;
    mod2[posn] = n % 2;

    for (int i = 1; i <= n; ++i) {
        if (i == pos1 || i == posn) continue;
        if (ask_query({pos1, i})) {
            mod2[i] = 1;
        } else {
            mod2[i] = 0;
        }
    }
    
    vector<int> mod3(n + 1, -1);
    mod3[pos1] = 1;
    mod3[posn] = n % 3 == 0 ? 0 : n % 3;
    
    int other_idx = -1;
    for (int i = 1; i <= n; ++i) {
        if (i != pos1 && i != posn) {
            other_idx = i;
            break;
        }
    }
    
    int target_mod3 = (-(n + 1)) % 3;
    if (target_mod3 < 0) target_mod3 += 3;

    if (ask_query({pos1, posn, other_idx})) {
        mod3[other_idx] = target_mod3;
    } else {
        int r1 = -1, r2 = -1;
        for (int k = 0; k < 3; ++k) {
            if (k != target_mod3) {
                if (r1 == -1) r1 = k;
                else r2 = k;
            }
        }
        
        vector<int> group_cand;
        for(int i = 1; i <= n; ++i) {
            if (i == pos1 || i == posn || i == other_idx) continue;
            group_cand.push_back(i);
        }
        
        int count = 0;
        for(int i : group_cand) {
            if(ask_query({pos1, other_idx, i})) count++;
        }
        
        vector<int> s(3,0);
        for(int i=1; i<=n; ++i) s[i%3]++;

        int p1_mod3 = 1;
        int pn_mod3 = n%3;
        
        s[p1_mod3]--; s[pn_mod3]--; 

        s[r1]--;
        int r_cand_1 = (-1 - r1) % 3; if(r_cand_1 < 0) r_cand_1 += 3;
        
        if (count == s[r_cand_1]) {
            mod3[other_idx] = r1;
        } else {
            mod3[other_idx] = r2;
        }
    }
    
    for(int i = 1; i <= n; ++i) {
        if(i == pos1 || i == posn || i == other_idx) continue;
        if (ask_query({pos1, other_idx, i})) {
            mod3[i] = (-1 - mod3[other_idx]) % 3;
            if (mod3[i] < 0) mod3[i] += 3;
        } else {
            int r_cand = (-1 - mod3[other_idx]) % 3; if (r_cand < 0) r_cand += 3;
            int r_other1 = -1, r_other2 = -1;
            for (int k = 0; k < 3; ++k) if (k != r_cand) { if (r_other1 == -1) r_other1 = k; else r_other2 = k; }
            
            bool pos1_exists = false;
            for (int v = 1; v <= n; ++v) {
                if (v % 2 == mod2[i] && v % 3 == r_other1) {
                    pos1_exists = true;
                    break;
                }
            }
            if (pos1_exists) mod3[i] = r_other1;
            else mod3[i] = r_other2;
        }
    }

    vector<vector<int>> val_groups(6);
    for(int v=1; v<=n; ++v){
        val_groups[v%6].push_back(v);
    }

    for(int i=1; i<=n; ++i){
        if(p[i] != 0) continue;
        int mod6 = -1;
        for(int k=0; k<6; ++k) {
            if(k%2 == mod2[i] && k%3 == mod3[i]) {
                mod6 = k;
                break;
            }
        }
        p[i] = val_groups[mod6].back();
        val_groups[mod6].pop_back();
    }

    if (p[1] > n / 2) {
        for (int i = 1; i <= n; ++i) {
            p[i] = n + 1 - p[i];
        }
    }
    
    answer(p);

    return 0;
}