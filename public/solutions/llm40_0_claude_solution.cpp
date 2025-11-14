#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    string result(n, '?');
    
    // First, determine each position individually
    for (int i = 1; i <= n; i++) {
        // Query position i twice to see if it's '(' or ')'
        cout << "0 2 " << i << " " << i << endl;
        cout.flush();
        int resp;
        cin >> resp;
        
        if (resp == 0) {
            // Could be either '(' or ')'
            // Try with a known pattern to distinguish
            cout << "0 1 " << i << endl;
            cout.flush();
            cin >> resp;
            
            // Since f(single char) = 0 always, we need another approach
            // Let's try position i followed by all positions
            cout << "0 " << n+1;
            for (int j = 1; j <= n; j++) {
                cout << " " << j;
            }
            cout << " " << i << endl;
            cout.flush();
            int resp1;
            cin >> resp1;
            
            // Try all positions followed by position i
            cout << "0 " << n+1;
            cout << " " << i;
            for (int j = 1; j <= n; j++) {
                cout << " " << j;
            }
            cout << endl;
            cout.flush();
            int resp2;
            cin >> resp2;
            
            if (resp1 > resp2) {
                result[i-1] = ')';
            } else {
                result[i-1] = '(';
            }
        }
    }
    
    // If we still have unknown positions, use binary search approach
    vector<int> unknown;
    for (int i = 0; i < n; i++) {
        if (result[i] == '?') {
            unknown.push_back(i);
        }
    }
    
    // For remaining positions, use systematic approach
    for (int pos = 1; pos <= n; pos++) {
        if (result[pos-1] != '?') continue;
        
        // Test if position pos is '('
        // Create a test sequence: pos followed by positions we think are ')'
        vector<int> test_seq;
        test_seq.push_back(pos);
        
        for (int i = 1; i <= n; i++) {
            if (i != pos && (result[i-1] == ')' || result[i-1] == '?')) {
                test_seq.push_back(i);
                if (test_seq.size() >= 100) break;
            }
        }
        
        if (test_seq.size() > 1) {
            cout << "0 " << test_seq.size();
            for (int idx : test_seq) {
                cout << " " << idx;
            }
            cout << endl;
            cout.flush();
            int resp1;
            cin >> resp1;
            
            // Now test without the position
            cout << "0 " << test_seq.size() - 1;
            for (int i = 1; i < test_seq.size(); i++) {
                cout << " " << test_seq[i];
            }
            cout << endl;
            cout.flush();
            int resp2;
            cin >> resp2;
            
            if (resp1 > resp2) {
                result[pos-1] = '(';
            } else {
                result[pos-1] = ')';
            }
        } else {
            // Default assignment if we can't determine
            result[pos-1] = '(';
        }
    }
    
    // Validate and fix the result
    int open = 0;
    for (int i = 0; i < n; i++) {
        if (result[i] == '(') open++;
        else open--;
        
        if (open < 0) {
            // Find a ')' before this position and change it to '('
            for (int j = i-1; j >= 0; j--) {
                if (result[j] == ')') {
                    result[j] = '(';
                    break;
                }
            }
            open = 0;
            i = -1; // Restart check
        }
    }
    
    // Ensure we have at least one '(' and one ')'
    bool has_open = false, has_close = false;
    for (char c : result) {
        if (c == '(') has_open = true;
        if (c == ')') has_close = true;
    }
    
    if (!has_open) result[0] = '(';
    if (!has_close) result[n-1] = ')';
    
    cout << "1 " << result << endl;
    cout.flush();
    
    return 0;
}