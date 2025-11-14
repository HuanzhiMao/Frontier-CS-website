#include <bits/stdc++.h>
using namespace std;

void sift_down(int sx, int sy, vector<vector<int>>& p, vector<vector<int>>& swaps) {
    int cx = sx, cy = sy;
    while (cx < 29) {
        int lx = cx + 1, ly = cy;
        int rx = cx + 1, ry = cy + 1;
        int cval = p[cx][cy];
        int lval = p[lx][ly];
        int rval = p[rx][ry];
        int smallx = lx, smally = ly, smallval = lval;
        if (rval < smallval) {
            smallx = rx;
            smally = ry;
            smallval = rval;
        }
        if (cval <= smallval) break;
        swaps.push_back({cx, cy, smallx, smally});
        swap(p[cx][cy], p[smallx][smally]);
        cx = smallx;
        cy = smally;
    }
}

int main() {
    vector<vector<int>> p(30);
    for (int x = 0; x < 30; x++) {
        p[x].resize(x + 1);
        for (int y = 0; y <= x; y++) {
            cin >> p[x][y];
        }
    }
    vector<vector<int>> swaps;
    for (int lev = 28; lev >= 0; lev--) {
        for (int y = 0; y <= lev; y++) {
            sift_down(lev, y, p, swaps);
        }
    }
    cout << swaps.size() << endl;
    for (auto& s : swaps) {
        cout << s[0] << " " << s[1] << " " << s[2] << " " << s[3] << endl;
    }
    return 0;
}