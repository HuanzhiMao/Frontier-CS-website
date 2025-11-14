#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    vector<vector<char>> comp(n + 1, vector<char>(n + 1, 0));

    for (int i = 1; i <= n; ++i) {
        for (int j = i + 1; j <= n; ++j) {
            cout << "0 " << i << " " << j << "\n";
            cout.flush();
            int ans;
            if (!(cin >> ans)) return 0;
            comp[i][j] = (char)(ans & 1);
        }
    }

    vector<int> p(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        int S = 0;
        for (int j = 1; j < i; ++j) {
            S += 1 - comp[j][i];
        }
        for (int j = i + 1; j <= n; ++j) {
            S += comp[i][j];
        }
        p[i] = S + 1;
    }

    cout << "1";
    for (int i = 1; i <= n; ++i) cout << " " << p[i];
    cout << "\n";
    cout.flush();

    return 0;
}