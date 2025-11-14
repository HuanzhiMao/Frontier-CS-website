#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    vector<vector<unsigned char>> s(n + 3, vector<unsigned char>(n + 3, 0));
    auto ask = [&](int l, int r) -> int {
        cout << "0 " << l << ' ' << r << '\n' << flush;
        int ans;
        if (!(cin >> ans)) exit(0);
        if (ans < 0) exit(0);
        return ans & 1;
    };

    for (int l = 1; l <= n; ++l) {
        for (int r = l + 1; r <= n; ++r) {
            s[l][r] = (unsigned char)ask(l, r);
        }
    }

    vector<vector<unsigned char>> x(n + 3, vector<unsigned char>(n + 3, 0));
    for (int i = 1; i <= n; ++i) {
        for (int j = i + 1; j <= n; ++j) {
            unsigned char a = s[i][j];
            unsigned char b = (j - 1 >= i) ? s[i][j - 1] : 0;
            unsigned char c = (i + 1 <= j) ? s[i + 1][j] : 0;
            unsigned char d = (i + 1 <= j - 1) ? s[i + 1][j - 1] : 0;
            x[i][j] = (unsigned char)((a ^ b ^ c ^ d) & 1);
        }
    }

    vector<int> p(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        int greaterCount = 0;
        for (int j = 1; j < i; ++j) {
            greaterCount += 1 - x[j][i];
        }
        for (int j = i + 1; j <= n; ++j) {
            greaterCount += x[i][j];
        }
        p[i] = greaterCount + 1;
    }

    cout << "1";
    for (int i = 1; i <= n; ++i) {
        cout << ' ' << p[i];
    }
    cout << '\n' << flush;

    return 0;
}