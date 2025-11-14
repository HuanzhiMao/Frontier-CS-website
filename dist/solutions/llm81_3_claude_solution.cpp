#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N;
    cin >> N;
    
    string result(N, '0');
    
    for (int i = 0; i < N; i++) {
        cout << "1\n";
        cout << "3\n";
        
        // Create arrays a and b
        vector<int> a(3), b(3);
        
        if (i == 0) {
            a = {1, 1, 1};
            b = {2, 2, 2};
        } else {
            a = {1, 1, 1};
            b = {2, 2, 2};
        }
        
        for (int j = 0; j < 3; j++) {
            cout << a[j];
            if (j < 2) cout << " ";
        }
        cout << "\n";
        
        for (int j = 0; j < 3; j++) {
            cout << b[j];
            if (j < 2) cout << " ";
        }
        cout << "\n";
        
        cout.flush();
        
        int response;
        cin >> response;
        
        // Now we need to determine what character at position i is
        // We'll use a different strategy - binary search approach
        cout << "1\n";
        cout << "2\n";
        cout << "1 0\n";
        cout << "1 0\n";
        cout.flush();
        
        cin >> response;
        
        if (response == 1) {
            result[i] = '0';
        } else {
            result[i] = '1';
        }
    }
    
    // Actually, let me use a simpler approach
    result = "";
    
    for (int i = 0; i < N; i++) {
        cout << "1\n";
        cout << "2\n";
        
        // For position i, we want to track what happens
        // a[0] = 1, a[1] = 0 (if we see '0' at pos i, go to state 1, then 0)
        // b[0] = 0, b[1] = 0 (if we see '1' at pos i, stay at 0)
        cout << "1 0\n";
        cout << "0 0\n";
        cout.flush();
        
        int response;
        cin >> response;
        
        if (response == 0) {
            result += '1';
        } else {
            result += '0';
        }
    }
    
    cout << "0\n";
    cout << result << "\n";
    cout.flush();
    
    return 0;
}