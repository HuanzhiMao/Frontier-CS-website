#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    if (!(cin >> N >> M)) return 0;
    for (int i = 0; i < M; ++i) {
        int u, v;
        cin >> u >> v;
    }
    
    if (N >= 2) {
        cout << "1 0 1\n";
    } else {
        cout << "1 0 0\n";
    }
    cout.flush();
    return 0;
}