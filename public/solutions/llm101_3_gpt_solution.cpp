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

    string T;
    char c;
    while (cin.get(c)) {
        if (c == '&' || c == '|') T.push_back(c);
    }
    if ((int)T.size() >= N) T.resize(N);
    else if ((int)T.size() < N) T.append(N - T.size(), '&');

    cout << T << "\n";
    return 0;
}