#include <bits/stdc++.h>
using namespace std;

int main() {
    vector<string> grid(30);
    for (int i = 0; i < 30; i++) {
        cin >> grid[i];
    }
    string ans = "";
    for (int i = 0; i < 900; i++) {
        ans += '0';
    }
    cout << ans << endl;
    return 0;
}