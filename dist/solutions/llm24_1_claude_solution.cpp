#include <bits/stdc++.h>
using namespace std;

int n;
vector<vector<int>> C;
vector<int> best_perm;
long long best_score;

void dfs(vector<int>& perm, vector<bool>& used, int pos) {
    if (pos == n) {
        // Check if this permutation is almost monochromatic
        vector<int> c(n);
        for (int i = 0; i < n - 1; i++) {
            c[i] = C[perm[i]][perm[i + 1]];
        }
        c[n - 1] = C[perm[n - 1]][perm[0]];
        
        int changes = 0;
        for (int i = 0; i < n - 1; i++) {
            if (c[i] != c[(i + 1) % n]) {
                changes++;
            }
        }
        
        if (changes <= 1) {
            // Calculate score
            long long score = 0;
            for (int i = 0; i < n; i++) {
                score += (long long)(n - i) * (perm[i] + 1);
            }
            
            if (best_perm.empty() || score > best_score) {
                best_score = score;
                best_perm = perm;
            }
        }
        return;
    }
    
    for (int i = 0; i < n; i++) {
        if (!used[i]) {
            used[i] = true;
            perm[pos] = i;
            dfs(perm, used, pos + 1);
            used[i] = false;
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    while (cin >> n) {
        C.assign(n, vector<int>(n));
        for (int i = 0; i < n; i++) {
            string s;
            cin >> s;
            for (int j = 0; j < n; j++) {
                C[i][j] = s[j] - '0';
            }
        }
        
        best_perm.clear();
        best_score = -1;
        
        vector<int> perm(n);
        vector<bool> used(n, false);
        dfs(perm, used, 0);
        
        if (best_perm.empty()) {
            cout << -1 << "\n";
        } else {
            for (int i = 0; i < n; i++) {
                cout << best_perm[i] + 1;
                if (i < n - 1) cout << " ";
            }
            cout << "\n";
        }
    }
    
    return 0;
}