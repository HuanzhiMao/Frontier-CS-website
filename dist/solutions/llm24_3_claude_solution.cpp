#include <bits/stdc++.h>
using namespace std;

int n;
vector<vector<int>> C;
vector<int> result;
vector<bool> used;

bool isAlmostMonochromatic(const vector<int>& perm) {
    vector<int> c(n);
    for (int i = 0; i < n - 1; i++) {
        c[i] = C[perm[i] - 1][perm[i + 1] - 1];
    }
    c[n - 1] = C[perm[n - 1] - 1][perm[0] - 1];
    
    int changes = 0;
    for (int i = 0; i < n - 1; i++) {
        if (c[i] != c[(i + 1) % n]) {
            changes++;
        }
    }
    return changes <= 1;
}

bool solve(vector<int>& perm, int pos) {
    if (pos == n) {
        return isAlmostMonochromatic(perm);
    }
    
    for (int i = 1; i <= n; i++) {
        if (!used[i]) {
            used[i] = true;
            perm[pos] = i;
            if (solve(perm, pos + 1)) {
                return true;
            }
            used[i] = false;
        }
    }
    return false;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    while (cin >> n) {
        C.assign(n, vector<int>(n));
        used.assign(n + 1, false);
        result.assign(n, 0);
        
        for (int i = 0; i < n; i++) {
            string s;
            cin >> s;
            for (int j = 0; j < n; j++) {
                C[i][j] = s[j] - '0';
            }
        }
        
        vector<int> perm(n);
        if (solve(perm, 0)) {
            for (int i = 0; i < n; i++) {
                cout << perm[i];
                if (i < n - 1) cout << " ";
            }
            cout << "\n";
        } else {
            cout << "-1\n";
        }
    }
    
    return 0;
}