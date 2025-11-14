#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    if (!(cin >> t)) return 0;
    vector<long long> W(t);
    for (int i = 0; i < t; ++i) {
        if (!(cin >> W[i])) return 0;
    }
    for (int i = 0; i < t; ++i) {
        cout << W[i] << "\n";
    }
    return 0;
}