#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    string s(n, '?');
    
    // First, determine which positions are '(' and which are ')'
    for (int i = 1; i <= n; i++) {
        cout << "0 1 " << i << endl;
        cout.flush();
        int result;
        cin >> result;
        
        if (result == 0) {
            s[i-1] = ')';
        } else {
            s[i-1] = '(';
        }
    }
    
    cout << "1 " << s << endl;
    cout.flush();
    
    return 0;
}