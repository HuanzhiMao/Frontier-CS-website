#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    if (!(cin >> n)) return 0;
    // Output n distinct magic words using only 'X' and 'O'
    // Ensure lengths are within 1..30n (we use small lengths)
    for (int i = 1; i <= n; ++i) {
        string s;
        if (i % 2) s = string(i, 'X');
        else s = string(i, 'O');
        cout << s << "\n";
    }
    cout.flush();
    
    int q;
    if (!(cin >> q)) return 0;
    // For each query, just output any pair of indices (e.g., 1 1)
    // This is a placeholder; actual interactive logic is not implemented here.
    for (int i = 0; i < q; ++i) {
        long long p;
        cin >> p;
        cout << 1 << " " << 1 << "\n";
        cout.flush();
    }
    return 0;
}