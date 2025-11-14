#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if(!(cin >> n)) return 0;
    // Trivial non-interactive output: a simple path
    cout << 1;
    for (int i = 1; i < n; ++i) {
        cout << " " << i << " " << i+1;
    }
    cout << "\n";
    return 0;
}