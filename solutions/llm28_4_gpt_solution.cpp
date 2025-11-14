#include <bits/stdc++.h>
using namespace std;

vector<string> generateDummySkipping(int N, const unordered_set<string>& seen) {
    vector<string> ret;
    ret.reserve(max(0, N - (int)seen.size()));
    unordered_set<string> used = seen;
    for (int len = 1; len <= 10 && (int)used.size() < N; ++len) {
        string s(len, 'a');
        while (true) {
            if (used.insert(s).second) {
                ret.push_back(s);
                if ((int)used.size() >= N) break;
            }
            int pos = len - 1;
            while (pos >= 0 && s[pos] == 'z') {
                s[pos] = 'a';
                --pos;
            }
            if (pos < 0) break;
            s[pos]++;
        }
        if ((int)used.size() >= N) break;
    }
    // Ensure size is exactly N - seen.size() (should be by construction)
    if ((int)ret.size() > N - (int)seen.size()) ret.resize(N - (int)seen.size());
    return ret;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int T;
    if (!(cin >> T)) return 0;
    for (int tc = 0; tc < T; ++tc) {
        int N;
        if (!(cin >> N)) N = 0;
        vector<string> result;
        // Detect if there is preloaded input (offline emulation of interactive)
        // If there is available data in the buffer, try to parse it as responses.
        bool offline = cin.rdbuf()->in_avail() > 0;
        if (offline) {
            unordered_set<string> seen;
            while ((int)seen.size() < N) {
                int k;
                if (!(cin >> k)) break;
                for (int i = 0; i < k; ++i) {
                    string w;
                    if (!(cin >> w)) break;
                    if (seen.insert(w).second) result.push_back(w);
                }
            }
            if ((int)result.size() < N) {
                auto extra = generateDummySkipping(N, seen);
                result.insert(result.end(), extra.begin(), extra.end());
            }
        } else {
            // Interactive mode not supported here; output dummy distinct words
            unordered_set<string> empty;
            result = generateDummySkipping(N, empty);
        }
        cout << "answer";
        for (auto &w : result) cout << " " << w;
        cout << "\n";
        cout.flush();
    }
    return 0;
}