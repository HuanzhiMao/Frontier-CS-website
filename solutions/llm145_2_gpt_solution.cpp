#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    if(!(cin >> t)) return 0;
    char ch = (t == 0) ? '0' : '1';
    for (int i = 0; i < 12; ++i) {
        for (int j = 0; j < 12; ++j) cout << ch;
        cout << "\n";
    }
    return 0;
}