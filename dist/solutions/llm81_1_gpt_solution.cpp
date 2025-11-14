#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    if (!(cin >> N)) return 0;
    string S;
    S.reserve(N);
    
    for (int i = 0; i < N; ++i) {
        int m = i + 3;
        vector<int> a(m), b(m);
        // Initialize as self-loops
        for (int j = 0; j < m; ++j) {
            a[j] = j;
            b[j] = j;
        }
        // Build path for known prefix
        for (int j = 0; j < i; ++j) {
            if (S[j] == '0') {
                a[j] = j + 1; // correct bit moves forward
                b[j] = j;     // wrong bit stays
            } else {
                a[j] = j;     // wrong bit stays
                b[j] = j + 1; // correct bit moves forward
            }
        }
        // Current bit branching
        a[i] = i + 1; // if current bit is 0 -> state i+1
        b[i] = i + 2; // if current bit is 1 -> state i+2
        // Absorbing leaves
        a[i + 1] = i + 1; b[i + 1] = i + 1;
        a[i + 2] = i + 2; b[i + 2] = i + 2;
        
        cout << 1 << "\n";
        cout << m << "\n";
        for (int j = 0; j < m; ++j) {
            if (j) cout << ' ';
            cout << a[j];
        }
        cout << "\n";
        for (int j = 0; j < m; ++j) {
            if (j) cout << ' ';
            cout << b[j];
        }
        cout << "\n";
        cout.flush();
        
        int x;
        if (!(cin >> x)) return 0;
        if (x == i + 1) S.push_back('0');
        else S.push_back('1');
    }
    
    cout << 0 << "\n";
    cout << S << "\n";
    cout.flush();
    return 0;
}