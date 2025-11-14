#include <iostream>
#include <vector>
#include <string>
using namespace std;

int query(vector<int> indices) {
    cout << "0 " << indices.size();
    for (int idx : indices) {
        cout << " " << idx;
    }
    cout << endl;
    cout.flush();
    
    int result;
    cin >> result;
    return result;
}

int main() {
    int n;
    cin >> n;
    
    string s(n, '?');
    
    // First, determine which positions are '(' and which are ')'
    for (int i = 1; i <= n; i++) {
        // Query position i twice to see if it forms a valid pair "()"
        int result = query({i, i});
        if (result == 0) {
            // Single character gives 0, check if it's '(' or ')'
            // Try pairing it with itself multiple times
            vector<int> indices;
            for (int j = 0; j < 4; j++) {
                indices.push_back(i);
            }
            int result2 = query(indices);
            
            if (result2 == 0) {
                // Still 0, likely all same character that doesn't form pairs
                // Need to check with a different approach
                bool found = false;
                for (int j = 1; j <= n && !found; j++) {
                    if (i != j) {
                        int pairResult = query({i, j});
                        if (pairResult == 1) {
                            s[i-1] = '(';
                            s[j-1] = ')';
                            found = true;
                        } else {
                            int pairResult2 = query({j, i});
                            if (pairResult2 == 1) {
                                s[j-1] = '(';
                                s[i-1] = ')';
                                found = true;
                            }
                        }
                    }
                }
            }
        }
    }
    
    // Alternative approach: check each position individually
    for (int i = 1; i <= n; i++) {
        if (s[i-1] == '?') {
            // Try to determine if position i is '(' or ')'
            bool determined = false;
            
            for (int j = 1; j <= n && !determined; j++) {
                if (i != j) {
                    int result1 = query({i, j});
                    if (result1 == 1) {
                        s[i-1] = '(';
                        s[j-1] = ')';
                        determined = true;
                    } else {
                        int result2 = query({j, i});
                        if (result2 == 1) {
                            s[j-1] = '(';
                            s[i-1] = ')';
                            determined = true;
                        }
                    }
                }
            }
        }
    }
    
    // Fill remaining positions by process of elimination
    int openCount = 0, closeCount = 0;
    for (char c : s) {
        if (c == '(') openCount++;
        else if (c == ')') closeCount++;
    }
    
    for (int i = 0; i < n; i++) {
        if (s[i] == '?') {
            if (openCount < closeCount) {
                s[i] = '(';
                openCount++;
            } else {
                s[i] = ')';
                closeCount++;
            }
        }
    }
    
    cout << "1 " << s << endl;
    cout.flush();
    
    return 0;
}