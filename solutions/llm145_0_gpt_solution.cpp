#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    if (!(cin >> t)) return 0;
    char d = t ? '1' : '0';

    vector<string> out;

    // Top: "MT" (each 5 wide, 2 spaces between = 12 columns)
    string M1 = string(1,d) + "   " + string(1,d);
    string M2 = string(2,d) + " " + string(2,d);
    string M3 = string(1,d) + " " + string(1,d) + " " + string(1,d);
    string M4 = string(1,d) + "   " + string(1,d);
    string M5 = string(1,d) + "   " + string(1,d);
    string M6 = string(1,d) + "   " + string(1,d);

    string Ttop = string(5,d);
    string Tmid = "  " + string(1,d) + "  ";

    out.push_back(M1 + "  " + Ttop);
    out.push_back(M2 + "  " + Tmid);
    out.push_back(M3 + "  " + Tmid);
    out.push_back(M4 + "  " + Tmid);
    out.push_back(M5 + "  " + Tmid);
    out.push_back(M6 + "  " + Tmid);

    // Bottom: "PKU" (each 4 wide, total 12)
    string P1 = string(3,d) + " ";
    string P2 = string(1,d) + "  " + string(1,d);
    string P3 = string(3,d) + " ";
    string P4 = string(1,d) + "   ";
    string P5 = string(1,d) + "   ";
    string P6 = string(1,d) + "   ";

    string K1 = string(1,d) + "  " + string(1,d);
    string K2 = string(1,d) + " " + string(1,d) + " ";
    string K3 = string(2,d) + "  ";
    string K4 = string(1,d) + " " + string(1,d) + " ";
    string K5 = string(1,d) + "  " + string(1,d);
    string K6 = string(1,d) + "   ";

    string U1 = string(1,d) + "  " + string(1,d);
    string U2 = string(1,d) + "  " + string(1,d);
    string U3 = string(1,d) + "  " + string(1,d);
    string U4 = string(1,d) + "  " + string(1,d);
    string U5 = string(1,d) + "  " + string(1,d);
    string U6 = " " + string(2,d) + " ";

    out.push_back(P1 + K1 + U1);
    out.push_back(P2 + K2 + U2);
    out.push_back(P3 + K3 + U3);
    out.push_back(P4 + K4 + U4);
    out.push_back(P5 + K5 + U5);
    out.push_back(P6 + K6 + U6);

    for (auto &row : out) {
        cout << row << "\n";
    }
    return 0;
}