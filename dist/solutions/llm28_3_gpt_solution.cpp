#include <bits/stdc++.h>
using namespace std;

static inline bool isInteger(const string& s) {
    if (s.empty()) return false;
    for (char c : s) if (!isdigit((unsigned char)c)) return false;
    return true;
}

static inline bool isLowerWord(const string& s) {
    if (s.empty() || s.size() > 10) return false;
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
        if (!(cin >> N)) return 0;

        unordered_set<string> uniq;
        uniq.reserve(N * 2);
        vector<string> words;
        words.reserve(N);

        string tok;
        while ((int)uniq.size() < N && (cin >> tok)) {
            if (isInteger(tok)) {
                int k = stoi(tok);
                for (int i = 0; i < k; ++i) {
                    string w;
                    if (!(cin >> w)) break;
                    if (isLowerWord(w)) {
                        if (uniq.insert(w).second) {
                            words.push_back(w);
                            if ((int)uniq.size() == N) break;
                        }
                    }
                }
            } else {
                if (isLowerWord(tok)) {
                    if (uniq.insert(tok).second) {
                        words.push_back(tok);
                    }
                }
            }
        }

        cout << "answer";
        int count = 0;
        for (const string& w : words) {
            if (count >= N) break;
            cout << ' ' << w;
            ++count;
        }
        cout << '\n';
        cout.flush();
    }
    return 0;
}