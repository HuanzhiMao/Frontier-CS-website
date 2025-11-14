#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<vector<int>> comp(n + 1, vector<int>(n + 1, 0));
    
    auto ask = [&](int i, int j) -> char {
        cout << "? " << i << " " << j << endl;
        cout.flush();
        char c;
        cin >> c;
        if (c == '<') {
            comp[i][j] = -1;
            comp[j][i] = 1;
        } else {
            comp[i][j] = 1;
            comp[j][i] = -1;
        }
        return c;
    };
    
    auto get_comp = [&](int i, int j) -> int {
        if (i == j) return 0;
        if (comp[i][j] != 0) return comp[i][j];
        ask(i, j);
        return comp[i][j];
    };
    
    vector<int> result(n + 1);
    vector<bool> used(n + 1, false);
    
    for (int pos = 1; pos <= n; pos++) {
        vector<int> candidates;
        for (int i = 1; i <= n; i++) {
            if (!used[i]) {
                candidates.push_back(i);
            }
        }
        
        while (candidates.size() > 1) {
            vector<int> next_candidates;
            for (int i = 0; i < candidates.size(); i += 2) {
                if (i + 1 < candidates.size()) {
                    int a = candidates[i];
                    int b = candidates[i + 1];
                    if (get_comp(a, b) < 0) {
                        next_candidates.push_back(a);
                    } else {
                        next_candidates.push_back(b);
                    }
                } else {
                    next_candidates.push_back(candidates[i]);
                }
            }
            candidates = next_candidates;
        }
        
        result[pos] = candidates[0];
        used[candidates[0]] = true;
    }
    
    cout << "!";
    for (int i = 1; i <= n; i++) {
        cout << " " << result[i];
    }
    cout << endl;
    cout.flush();
    
    return 0;
}