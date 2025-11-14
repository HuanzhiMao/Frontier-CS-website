#include <bits/stdc++.h>
using namespace std;

int main() {
    vector<pair<int, int>> required;
    for (int d = 1; d <= 9; d++) required.push_back({d, d});
    for (int d = 1; d <= 9; d++) required.push_back({d, 0});
    for (int a = 1; a <= 9; a++) for (int b = a + 1; b <= 9; b++) required.push_back({a, b});
    vector<string> grid(8, string(14, '0'));
    int cnt = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 14; j += 2) {
            if (cnt < required.size()) {
                auto p = required[cnt];
                grid[i][j] = '0' + p.first;
                grid[i][j + 1] = '0' + p.second;
                cnt++;
            }
        }
    }
    for (auto& s : grid) cout << s << endl;
    return 0;
}