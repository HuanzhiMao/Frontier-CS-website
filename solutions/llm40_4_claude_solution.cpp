#include <bits/stdc++.h>
using namespace std;

int query(vector<int>& indices) {
    cout << "0 " << indices.size();
    for (int i : indices) {
        cout << " " << i;
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
    // Query each position individually to see if it contributes to valid brackets
    
    for (int i = 1; i <= n; i++) {
        // Query position i twice
        vector<int> q = {i, i};
        int result = query(q);
        
        if (result == 0) {
            // No valid bracket sequence from two identical characters
            // This doesn't help us distinguish
        }
        
        // Try combining with other positions to determine the character
        bool found = false;
        
        for (int j = 1; j <= n && !found; j++) {
            if (i == j) continue;
            
            vector<int> q1 = {i, j};
            int result1 = query(q1);
            
            vector<int> q2 = {j, i};
            int result2 = query(q2);
            
            if (result1 == 1 && result2 == 0) {
                // i is '(' and j is ')'
                s[i-1] = '(';
                s[j-1] = ')';
                found = true;
            } else if (result1 == 0 && result2 == 1) {
                // i is ')' and j is '('
                s[i-1] = ')';
                s[j-1] = '(';
                found = true;
            }
        }
        
        if (found) {
            break;
        }
    }
    
    // Now determine remaining positions
    for (int i = 1; i <= n; i++) {
        if (s[i-1] != '?') continue;
        
        // Find a known position
        int known_pos = -1;
        char known_char = '?';
        for (int j = 1; j <= n; j++) {
            if (s[j-1] != '?') {
                known_pos = j;
                known_char = s[j-1];
                break;
            }
        }
        
        if (known_pos != -1) {
            vector<int> q = {i, known_pos};
            int result = query(q);
            
            if (known_char == '(' && result == 1) {
                s[i-1] = ')';
            } else if (known_char == ')' && result == 1) {
                s[i-1] = '(';
            } else if (known_char == '(' && result == 0) {
                s[i-1] = '(';
            } else if (known_char == ')' && result == 0) {
                s[i-1] = ')';
            }
        }
    }
    
    cout << "1 " << s << endl;
    cout.flush();
    
    return 0;
}