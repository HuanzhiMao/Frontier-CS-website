#include <bits/stdc++.h>
using namespace std;

int main() {
    int t;
    cin >> t;
    vector<string> lines = {
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
    char dig = (t == 0) ? '0' : '1';
    for (string s : lines) {
        string out = s;
        for (char &c : out) {
            if (c >= '0' && c <= '9') c = dig;
        }
        cout << out << endl;
    }
    return 0;
}