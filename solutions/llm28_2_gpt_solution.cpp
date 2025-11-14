#include <bits/stdc++.h>
using namespace std;

static inline bool isInteger(const string& s) {
    if (s.empty()) return false;
    for (char c : s) if (!isdigit((unsigned char)c)) return false;
    return true;
}
static inline bool isWord(const string& s) {
    if (s.empty()) return false;
    if (s.size() > 10) return false;
    for (char c : s) if (c < 'a' || c > 'z') return false;
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    if (!(cin >> T)) return 0;
    for (int tc = 0; tc < T; ++tc) {
        int N;
        if (!(cin >> N)) N = 0;

        unordered_set<string> seen;
        seen.reserve(N * 2 + 10);
        vector<string> words;
        words.reserve(N);

        string tok;
        while ((int)seen.size() < N && (cin >> tok)) {
            if (isInteger(tok)) {
                int k = stoi(tok);
                for (int i = 0; i < k; ++i) {
                    string w;
                    if (!(cin >> w)) break;
                    if (isWord(w) && !seen.count(w)) {
                        seen.insert(w);
                        words.push_back(w);
                        if ((int)seen.size() == N) break;
                    }
                }
            } else {
                if (isWord(tok) && !seen.count(tok)) {
                    seen.insert(tok);
                    words.push_back(tok);
                }
            }
        }

        cout << "answer";
        for (const string& w : words) cout << ' ' << w;
        if (tc + 1 < T) cout << '\n';
    }
    return 0;
}