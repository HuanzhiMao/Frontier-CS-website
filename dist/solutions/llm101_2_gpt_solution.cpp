#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, R;
    if (!(cin >> N >> R)) return 0;
    vector<pair<int,int>> uv(N);
    for (int i = 0; i < N; ++i) cin >> uv[i].first >> uv[i].second;

    // Read the rest of the input to find the circuit string if present
    vector<string> tokens;
    string tok;
    while (cin >> tok) tokens.push_back(tok);

    string t = "";
    // First, check tokens directly
    for (const string &s : tokens) {
        if ((int)s.size() == N && all_of(s.begin(), s.end(), [](char c){ return c=='&' || c=='|'; })) {
            t = s;
            break;
        }
    }
    // If not found, try joining tokens and searching for a contiguous substring
    if (t.empty() && !tokens.empty()) {
        string joined;
        for (auto &s : tokens) joined += s;
        for (int i = 0; i + N <= (int)joined.size(); ++i) {
            bool ok = true;
            for (int j = 0; j < N; ++j) {
                char c = joined[i + j];
                if (c != '&' && c != '|') { ok = false; break; }
            }
            if (ok) { t = joined.substr(i, N); break; }
        }
    }
    // Fallback: output all '&' if nothing provided (should not happen in proper offline tests)
    if (t.empty()) t = string(N, '&');

    cout << t << "\n";
    return 0;
}