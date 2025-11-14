#include <bits/stdc++.h>

using namespace std;

int main() {
    int t;
    cin >> t;
    vector<string> grid;
    if (t == 0) {
        grid = {
            "0   0   000 ",
            "00 00  0   0",
            "0 0 0  0   0",
            "0 0 0  0000 ",
            "0 0 0  0    ",
            "0   0  0    ",
            "0  0   00000",
            "0 0      0  ",
            "00   0 0 0  ",
            "0 0  0 0 0  ",
            "0  0 000 0  ",
            "            "
        };
    } else {
        grid = {
            "1   1   111 ",
            "11 11  1   1",
            "1 1 1  1   1",
            "1 1 1  1111 ",
            "1 1 1  1    ",
            "1   1  1    ",
            "1  1   11111",
            "1 1      1  ",
            "11   1 1 1  ",
            "1 1  1 1 1  ",
            "1  1 111 1  ",
            "            "
        };
    }
    for (auto& s : grid) {
        cout << s << endl;
    }
    return 0;
}