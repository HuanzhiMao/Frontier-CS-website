#include <bits/stdc++.h>
using namespace std;

static inline void flush_out() {
    cout.flush();
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    if (!(cin >> n)) return 0;

    auto ask = [&](const vector<int>& seq) -> long long {
        if (seq.empty()) return 0;
        cout << "0 " << seq.size();
        for (int x : seq) cout << " " << x;
        cout << "\n";
        flush_out();
        long long res;
        if (!(cin >> res)) exit(0);
        if (res < 0) exit(0);
        return res;
    };

    int j = 1;
    vector<int> others;
    others.reserve(n-1);
    for (int i = 1; i <= n; ++i) if (i != j) others.push_back(i);

    // Determine character of j using up to 3 groups (each <= 333 items).
    char cj = '?';
    int idx = 0;
    const int G = 333; // max group size so that 3*G <= 1000
    while (idx < (int)others.size()) {
        int len = min(G, (int)others.size() - idx);
        vector<int> seqA, seqB;
        seqA.reserve(3 * len);
        seqB.reserve(3 * len);
        for (int t = 0; t < len; ++t) {
            int i = others[idx + t];
            // A: i, j, j
            seqA.push_back(i); seqA.push_back(j); seqA.push_back(j);
            // B: j, j, i
            seqB.push_back(j); seqB.push_back(j); seqB.push_back(i);
        }
        long long rA = ask(seqA);
        long long rB = ask(seqB);
        if (rA > 0 || rB > 0) {
            cj = (rA > 0) ? ')' : '(';
            break;
        }
        idx += len;
    }

    // In theory, cj should be determined due to the problem's guarantee.
    if (cj == '?') {
        // Fallback: try a different j (shouldn't happen).
        for (int cand = 2; cand <= n && cj == '?'; ++cand) {
            j = cand;
            others.clear();
            for (int i = 1; i <= n; ++i) if (i != j) others.push_back(i);
            idx = 0;
            while (idx < (int)others.size()) {
                int len = min(G, (int)others.size() - idx);
                vector<int> seqA, seqB;
                seqA.reserve(3 * len);
                seqB.reserve(3 * len);
                for (int t = 0; t < len; ++t) {
                    int i = others[idx + t];
                    seqA.push_back(i); seqA.push_back(j); seqA.push_back(j);
                    seqB.push_back(j); seqB.push_back(j); seqB.push_back(i);
                }
                long long rA = ask(seqA);
                long long rB = ask(seqB);
                if (rA > 0 || rB > 0) {
                    cj = (rA > 0) ? ')' : '(';
                    break;
                }
                idx += len;
            }
        }
        if (cj == '?') cj = '('; // arbitrary, should not be reached
    }

    string ans(n, '?');
    ans[j - 1] = cj;
    bool jIsClose = (cj == ')');

    // Rebuild list of others excluding j (in case j changed in fallback)
    others.clear();
    for (int i = 1; i <= n; ++i) if (i != j) others.push_back(i);

    // Decode others in blocks of up to 8 using weighted segments.
    const int B = 8; // block size so that 3*(2^B - 1) <= 1000
    int pos = 0;
    while (pos < (int)others.size()) {
        int m = min(B, (int)others.size() - pos);
        vector<int> seq;
        // total tokens = 3*(2^m - 1) <= 1000
        // For each i in block, repeat unit segment weight times with weight=2^t
        for (int t = 0; t < m; ++t) {
            int i = others[pos + t];
            int w = 1 << t;
            if (jIsClose) {
                // unit: i, j, j
                seq.reserve(seq.size() + 3 * w);
                for (int rep = 0; rep < w; ++rep) {
                    seq.push_back(i); seq.push_back(j); seq.push_back(j);
                }
            } else {
                // unit: j, j, i
                seq.reserve(seq.size() + 3 * w);
                for (int rep = 0; rep < w; ++rep) {
                    seq.push_back(j); seq.push_back(j); seq.push_back(i);
                }
            }
        }
        long long r = ask(seq);
        // Decode bits
        for (int t = 0; t < m; ++t) {
            int i = others[pos + t];
            bool opposite = ((r >> t) & 1LL) != 0; // 1 if opposite to j
            if (jIsClose) {
                ans[i - 1] = opposite ? '(' : ')';
            } else {
                ans[i - 1] = opposite ? ')' : '(';
            }
        }
        pos += m;
    }

    cout << "1 " << ans << "\n";
    flush_out();
    return 0;
}