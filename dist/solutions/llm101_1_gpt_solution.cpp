#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, R;
    if (!(cin >> N >> R)) return 0;
    for (int i = 0; i < N; ++i) {
        int u, v;
        cin >> u >> v;
    }
    string t, last;
    if (cin >> t) last = t;
    while (cin >> t) last = t;
    if (!last.empty()) cout << last << "\n";
    return 0;
}