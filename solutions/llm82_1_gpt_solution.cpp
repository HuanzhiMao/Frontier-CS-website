#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    if(!(cin >> n)) return 0;
    
    // Read all remaining integers
    vector<int> all;
    int x;
    while (cin >> x) all.push_back(x);
    
    long long m = 1LL * n * (n - 1) / 2;
    vector<int> ans(n, 0);
    
    if ((long long)all.size() == m) {
        // We have all pairwise ORs for i<j in lex order
        vector<vector<int>> M(n, vector<int>(n, 0));
        long long k = 0;
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                M[i][j] = M[j][i] = all[k++];
            }
        }
        
        int s = -1;
        // Find index s such that row s contains exactly values 1..n-1 each once (since OR with 0 equals value itself)
        for (int i = 0; i < n; ++i) {
            bool ok = true;
            if (n-1 > 0) {
                vector<int> cnt(n, 0);
                for (int j = 0; j < n; ++j) if (j != i) {
                    int val = M[i][j];
                    if (val <= 0 || val >= n) { ok = false; break; }
                    cnt[val]++;
                }
                if (ok) {
                    for (int v = 1; v <= n - 1; ++v) {
                        if (cnt[v] != 1) { ok = false; break; }
                    }
                }
            }
            if (ok) { s = i; break; }
        }
        
        // Fallback in rare cases: identify s among indices with min row value = 1 using presence of value 2
        if (s == -1) {
            vector<int> mn(n, INT_MAX);
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) if (j != i) {
                    mn[i] = min(mn[i], M[i][j]);
                }
            }
            vector<int> cand;
            for (int i = 0; i < n; ++i) if (mn[i] == 1) cand.push_back(i);
            if ((int)cand.size() == 2) {
                auto has2 = [&](int i)->bool{
                    for (int j = 0; j < n; ++j) if (j != i) {
                        if (M[i][j] == 2) return true;
                    }
                    return false;
                };
                bool a = has2(cand[0]);
                bool b = has2(cand[1]);
                if (a ^ b) s = a ? cand[0] : cand[1];
            }
            if (s == -1) {
                // As an ultimate fallback (should not happen), pick the one with all values < n and most distinct
                int best = -1, bestDistinct = -1;
                for (int i = 0; i < n; ++i) {
                    bool okAllLess = true;
                    unordered_set<int> st;
                    for (int j = 0; j < n; ++j) if (j != i) {
                        int val = M[i][j];
                        if (val >= n) { okAllLess = false; break; }
                        st.insert(val);
                    }
                    if (okAllLess && (int)st.size() > bestDistinct) {
                        bestDistinct = (int)st.size();
                        best = i;
                    }
                }
                s = (best != -1 ? best : 0);
            }
        }
        
        // Reconstruct permutation
        ans[s] = 0;
        for (int i = 0; i < n; ++i) if (i != s) ans[i] = M[i][s];
    } else if ((int)all.size() == n) {
        // Possibly the input directly gives the permutation
        for (int i = 0; i < n; ++i) ans[i] = all[i];
    } else {
        // Fallback: cannot determine, output zeros
        // But try to output a valid permutation 0..n-1 in absence of enough data
        for (int i = 0; i < n; ++i) ans[i] = i;
    }
    
    for (int i = 0; i < n; ++i) {
        if (i) cout << ' ';
        cout << ans[i];
    }
    cout << '\n';
    return 0;
}