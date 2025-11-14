#include <bits/stdc++.h>
using namespace std;

int main() {
    int n = 20, m = 20;
    cout << n << " " << m << "\n";
    for (int i = 1; i <= n; ++i) {
        string s(m, '0');
        if (i % 2 == 1) {
            s = string(m, '1');
        } else {
            if (i % 4 == 2) s[m - 1] = '1';
            else s[0] = '1';
        }
        cout << s << "\n";
    }
    return 0;
}