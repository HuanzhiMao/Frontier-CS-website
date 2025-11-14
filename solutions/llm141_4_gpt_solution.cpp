#include <bits/stdc++.h>
using namespace std;

int n, k;

bool ask(int c) {
    cout << "? " << c << endl;
    cout.flush();
    string res;
    if (!(cin >> res)) exit(0);
    return res[0] == 'Y';
}

void reset() {
    cout << "R" << endl;
    cout.flush();
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    if (!(cin >> n >> k)) return 0;
    vector<char> dup(n + 1, 0);
    
    int LOG = 0;
    while ((1 << LOG) < n) ++LOG;
    
    for (int i = 0; i < LOG; ++i) {
        int group = 1 << (i + 1);
        int half = 1 << i;
        int Lsize = max(1, min(half, k / 2));
        int Rsize = max(1, min(half, k - Lsize + 1));
        
        for (int s = 1; s <= n; s += group) {
            int leftStart = s;
            int leftEndAll = s + half - 1;
            int rightStartAll = s + half;
            int rightEndAll = s + group - 1;
            
            for (int lstart = leftStart; lstart <= leftEndAll; lstart += Lsize) {
                int lend = min(leftEndAll, lstart + Lsize - 1);
                for (int rstart = rightStartAll; rstart <= rightEndAll; rstart += Rsize) {
                    int rend = min(rightEndAll, rstart + Rsize - 1);
                    reset();
                    for (int x = lstart; x <= lend; ++x) {
                        ask(x); // response ignored
                    }
                    for (int y = rstart; y <= rend; ++y) {
                        if (ask(y)) dup[y] = 1;
                    }
                }
            }
        }
    }
    
    int d = 0;
    for (int i = 1; i <= n; ++i) if (!dup[i]) ++d;
    cout << "! " << d << endl;
    cout.flush();
    return 0;
}