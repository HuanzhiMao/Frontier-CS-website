#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    long long n;
    if (!(cin >> n)) n = 0;
    
    cout << "!";
    for (long long i = 0; i < n; ++i) {
        long long a = (i + 1); // unique slopes within [-1e4,1e4] since n<=100
        cout << " " << a;
    }
    for (long long i = 0; i < n; ++i) {
        cout << " " << 0; // b_i = 0
    }
    cout << "\n";
    cout.flush();
    return 0;
}