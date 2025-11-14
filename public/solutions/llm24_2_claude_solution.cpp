#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    while (cin >> n) {
        vector<vector<int>> C(n, vector<int>(n));
        for (int i = 0; i < n; i++) {
            string s;
            cin >> s;
            for (int j = 0; j < n; j++) {
                C[i][j] = s[j] - '0';
            }
        }
        
        vector<int> best_perm;
        long long best_score = -1;
        
        // Try all possible starting vertices
        for (int start = 0; start < n; start++) {
            vector<bool> used(n, false);
            vector<int> perm;
            perm.push_back(start);
            used[start] = true;
            
            // Greedy construction
            bool valid = true;
            while (perm.size() < n && valid) {
                int curr = perm.back();
                int next = -1;
                
                // Find the lexicographically smallest unused vertex
                for (int i = 0; i < n; i++) {
                    if (!used[i]) {
                        next = i;
                        break;
                    }
                }
                
                if (next == -1) break;
                
                perm.push_back(next);
                used[next] = true;
            }
            
            if (perm.size() == n) {
                // Check if this permutation is almost monochromatic
                vector<int> c(n);
                for (int i = 0; i < n - 1; i++) {
                    c[i] = C[perm[i]][perm[i + 1]];
                }
                c[n - 1] = C[perm[n - 1]][perm[0]];
                
                int changes = 0;
                for (int i = 0; i < n - 1; i++) {
                    if (c[i] != c[i + 1]) changes++;
                }
                if (c[n - 1] != c[0]) changes++;
                
                if (changes <= 2) { // At most one change in cyclic sequence
                    long long score = 0;
                    for (int i = 0; i < n; i++) {
                        score += (long long)(n - i) * (perm[i] + 1);
                    }
                    
                    if (best_score == -1 || score > best_score) {
                        best_score = score;
                        best_perm = perm;
                    }
                }
            }
        }
        
        // Try a more sophisticated approach
        function<void(vector<int>&)> solve = [&](vector<int>& current_best) {
            for (int start = 0; start < n; start++) {
                vector<bool> used(n, false);
                vector<int> perm;
                perm.push_back(start);
                used[start] = true;
                
                function<void(int)> backtrack = [&](int pos) {
                    if (pos == n) {
                        // Check if almost monochromatic
                        vector<int> c(n);
                        for (int i = 0; i < n - 1; i++) {
                            c[i] = C[perm[i]][perm[i + 1]];
                        }
                        c[n - 1] = C[perm[n - 1]][perm[0]];
                        
                        int changes = 0;
                        for (int i = 0; i < n - 1; i++) {
                            if (c[i] != c[i + 1]) changes++;
                        }
                        if (c[n - 1] != c[0]) changes++;
                        
                        if (changes <= 2) {
                            long long score = 0;
                            for (int i = 0; i < n; i++) {
                                score += (long long)(n - i) * (perm[i] + 1);
                            }
                            
                            if (best_score == -1 || score > best_score) {
                                best_score = score;
                                current_best = perm;
                            }
                        }
                        return;
                    }
                    
                    for (int next = 0; next < n; next++) {
                        if (!used[next]) {
                            used[next] = true;
                            perm.push_back(next);
                            backtrack(pos + 1);
                            perm.pop_back();
                            used[next] = false;
                        }
                    }
                };
                
                if (n <= 8) {
                    backtrack(1);
                }
            }
        };
        
        if (n <= 8) {
            solve(best_perm);
        } else {
            // For larger n, use heuristic approach
            for (int start = 0; start < n; start++) {
                vector<int> perm = {start};
                vector<bool> used(n, false);
                used[start] = true;
                
                while (perm.size() < n) {
                    int next = -1;
                    for (int i = 0; i < n; i++) {
                        if (!used[i]) {
                            next = i;
                            break;
                        }
                    }
                    perm.push_back(next);
                    used[next] = true;
                }
                
                vector<int> c(n);
                for (int i = 0; i < n - 1; i++) {
                    c[i] = C[perm[i]][perm[i + 1]];
                }
                c[n - 1] = C[perm[n - 1]][perm[0]];
                
                int changes = 0;
                for (int i = 0; i < n - 1; i++) {
                    if (c[i] != c[i + 1]) changes++;
                }
                if (c[n - 1] != c[0]) changes++;
                
                if (changes <= 2) {
                    long long score = 0;
                    for (int i = 0; i < n; i++) {
                        score += (long long)(n - i) * (perm[i] + 1);
                    }
                    
                    if (best_score == -1 || score > best_score) {
                        best_score = score;
                        best_perm = perm;
                    }
                }
            }
        }
        
        if (best_perm.empty()) {
            cout << "-1\n";
        } else {
            for (int i = 0; i < n; i++) {
                if (i > 0) cout << " ";
                cout << best_perm[i] + 1;
            }
            cout << "\n";
        }
    }
    
    return 0;
}