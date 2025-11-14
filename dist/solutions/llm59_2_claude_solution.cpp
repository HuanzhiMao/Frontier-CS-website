#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<vector<int>> comp(n + 1, vector<int>(n + 1, 0));
    
    auto query = [&](int i, int j) -> int {
        if (comp[i][j] != 0) return comp[i][j];
        if (i == j) return 0;
        
        cout << "? " << i << " " << j << endl;
        cout.flush();
        
        char c;
        cin >> c;
        
        if (c == '<') {
            comp[i][j] = -1;
            comp[j][i] = 1;
            return -1;
        } else {
            comp[i][j] = 1;
            comp[j][i] = -1;
            return 1;
        }
    };
    
    vector<int> result(n + 1);
    
    for (int pos = 1; pos <= n; pos++) {
        vector<int> candidates;
        for (int val = 1; val <= n; val++) {
            bool used = false;
            for (int p = 1; p < pos; p++) {
                if (result[p] == val) {
                    used = true;
                    break;
                }
            }
            if (!used) {
                candidates.push_back(val);
            }
        }
        
        if (candidates.size() == 1) {
            result[pos] = candidates[0];
            continue;
        }
        
        // Sort candidates based on comparisons
        sort(candidates.begin(), candidates.end(), [&](int a, int b) {
            return query(pos, a) > query(pos, b);
        });
        
        result[pos] = candidates[0];
    }
    
    cout << "!";
    for (int i = 1; i <= n; i++) {
        cout << " " << result[i];
    }
    cout << endl;
    cout.flush();
    
    return 0;
}