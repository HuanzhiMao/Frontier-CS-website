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
        unordered_set<string> seen;
        vector<string> words;
        while ((int)seen.size() < N) {
            int k;
            if (!(cin >> k)) return 0;
            for (int i = 0; i < k; ++i) {
                string s;
                cin >> s;
                if (seen.insert(s).second) {
                    words.push_back(s);
                }
            }
        }
        sort(words.begin(), words.end());
        cout << "answer";
        for (auto &w : words) {
            cout << " " << w;
        }
        cout << "\n";
    }
    return 0;
}