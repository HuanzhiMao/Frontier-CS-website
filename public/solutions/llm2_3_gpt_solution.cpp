#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<vector<int>> pendingQueries;

    string line;
    while (true) {
        string s;
        if (!getline(cin, s)) break;
        // Remove possible carriage return
        if (!s.empty() && s.back() == '\r') s.pop_back();
        // Trim leading and trailing spaces
        size_t start = s.find_first_not_of(" \t\n\v\f\r");
        if (start == string::npos) continue;
        size_t end = s.find_last_not_of(" \t\n\v\f\r");
        s = s.substr(start, end - start + 1);

        // Parse integers from the line
        istringstream iss(s);
        vector<long long> vals;
        long long x;
        while (iss >> x) vals.push_back(x);
        if (vals.empty()) continue;

        long long op = vals[0];
        if ((op == 0 || op == 1) && vals.size() >= 2) {
            vector<int> seq;
            seq.reserve(vals.size() - 1);
            for (size_t i = 1; i < vals.size(); ++i) seq.push_back((int)vals[i]);

            if (op == 0) {
                pendingQueries.push_back(move(seq));
            } else {
                // op == 1, this is the permutation guess; answer all pending queries
                const vector<int>& perm = seq;
                for (const auto& q : pendingQueries) {
                    int len = (int)min(q.size(), perm.size());
                    int cnt = 0;
                    for (int i = 0; i < len; ++i) {
                        if (q[i] == perm[i]) ++cnt;
                    }
                    cout << cnt << '\n';
                }
                pendingQueries.clear();
            }
        } else {
            // Line doesn't conform to expected "0/1 followed by sequence" format; ignore.
            continue;
        }
    }

    return 0;
}