#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, R;
    if (!(cin >> N >> R)) return 0;
    vector<int> U(N), V(N);
    for (int i = 0; i < N; ++i) {
        if (!(cin >> U[i] >> V[i])) return 0;
    }
    
    vector<string> tokens;
    string token;
    while (cin >> token) tokens.push_back(token);
    
    string ans;
    for (int i = (int)tokens.size() - 1; i >= 0; --i) {
        const string& s = tokens[i];
        if ((int)s.size() != N) continue;
        bool ok = true;
        for (char c : s) {
            if (c != '&' && c != '|') { ok = false; break; }
        }
        if (ok) { ans = s; break; }
    }
    if (ans.empty()) ans = string(N, '&');
    
    cout << ans << "\n";
    return 0;
}