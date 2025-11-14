#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t = 0;
    if (!(cin >> t)) t = 0;

    vector<string> small = {
        "0   0   000 ",
        "00 00  0   0",
        "0 0 0  0   0",
        "0 0 0  0000 ",
        "0 0 0  0    ",
        "0   0  0    ",
        "            ",
        "0  0   00000",
        "0 0      0  ",
        "00   0 0 0  ",
        "0 0  0 0 0  ",
        "0  0 000 0  "
    };

    vector<string> large = {
        "1   1   111 ",
        "11 11  1   1",
        "1 1 1  1   1",
        "1 1 1  1111 ",
        "1 1 1  1    ",
        "1   1  1    ",
        "            ",
        "1  1   11111",
        "1 1      1  ",
        "11   1 1 1  ",
        "1 1  1 1 1  ",
        "1  1 111 1  "
    };

    const auto &grid = (t == 0 ? small : large);
    for (const auto &row : grid) {
        cout << row << '\n';
    }
    return 0;
}