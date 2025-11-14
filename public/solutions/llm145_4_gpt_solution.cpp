#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t = 0;
    if (!(cin >> t)) t = 0;
    char ch = (t == 1 ? '1' : '0');
    string line(12, ch);
    for (int i = 0; i < 12; ++i) {
        cout << line << "\n";
    }
    return 0;
}