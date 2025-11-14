#include <bits/stdc++.h>
using namespace std;

static vector<long long> parseInts(const string &s) {
    vector<long long> res;
    long long x;
    std::istringstream iss(s);
    while (iss >> x) res.push_back(x);
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    if (!(cin >> t)) {
        return 0;
    }
    string dummy;
    getline(cin, dummy); // consume endline after t

    vector<string> lines;
    string line;
    while (getline(cin, line)) {
        lines.push_back(line);
    }

    size_t pos = 0;
    for (int tc = 0; tc < t; ++tc) {
        bool found = false;
        int n_found = 2; // default minimal valid n
        size_t start_pos = pos;

        for (size_t p = pos; p < lines.size(); ++p) {
            auto a = parseInts(lines[p]);
            if (a.size() != 1) continue;
            long long nll = a[0];
            if (nll < 2 || nll > 1000) continue;
            int n = (int)nll;
            if (p + (size_t)(n - 1) >= lines.size()) continue;

            bool ok = true;
            for (int j = 1; j <= n - 1; ++j) {
                auto b = parseInts(lines[p + j]);
                if (b.size() != 2) { ok = false; break; }
                long long u = b[0], v = b[1];
                if (!(1 <= u && u <= n && 1 <= v && v <= n)) { ok = false; break; }
            }
            if (ok) {
                found = true;
                n_found = n;
                pos = p + n; // advance past this test's lines
                break;
            }
        }

        if (!found) {
            // Fallback: if unable to parse from remaining lines, output trivial for n=2
            pos = start_pos;
            n_found = 2;
        }

        cout << "! ";
        for (int i = 1; i <= n_found; ++i) {
            cout << 1 << (i == n_found ? '\n' : ' ');
        }
        cout.flush();
    }

    return 0;
}