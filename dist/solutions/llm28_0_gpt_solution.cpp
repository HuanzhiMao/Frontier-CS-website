#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    if (!(cin >> T)) return 0;
    for (int tc = 0; tc < T; ++tc) {
        int N;
        if (!(cin >> N)) return 0;

        set<string> words;
        cin >> ws;
        int c = cin.peek();

        if (c != EOF && isalpha(static_cast<unsigned char>(c))) {
            // Direct list of words
            for (int i = 0; i < N; ++i) {
                string s;
                if (!(cin >> s)) break;
                words.insert(s);
            }
        } else {
            // Sequence of responses: k followed by k words
            while ((int)words.size() < N) {
                int k;
                if (!(cin >> k)) break;
                for (int i = 0; i < k; ++i) {
                    string s;
                    cin >> s;
                    words.insert(s);
                }
            }
        }

        cout << "answer";
        for (const auto& s : words) cout << ' ' << s;
        cout << '\n';
    }
    return 0;
}