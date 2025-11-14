#include <bits/stdc++.h>
using namespace std;

int main() {
    int t;
    cin >> t;
    string lines[11] = {
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
        "0  0 000 0  "
    };
    char digit = (t == 0) ? '0' : '2';
    for (int i = 0; i < 11; i++) {
        string s = lines[i];
        for (char& c : s) {
            if (c >= '0' && c <= '9') c = digit;
        }
        cout << s << endl;
    }
    cout << "            " << endl;
    return 0;
}