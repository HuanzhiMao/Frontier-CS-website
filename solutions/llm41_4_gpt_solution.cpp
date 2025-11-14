#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    unsigned long long n;
    if (!(cin >> n)) return 0;
    
    vector<unsigned long long> seq;
    if (n == 1) {
        cout << 1 << "\n1\n";
        return 0;
    }
    
    // Compute L = floor(log2(n)) + 1
    int L = 0;
    unsigned long long t = n;
    while (t) { L++; t >>= 1; }
    
    unsigned long long base = 1ULL << (L - 2); // 2^(L-2), valid since L >= 2 here
    unsigned long long s = n / base; // This will be 2 or 3
    if (s > 3) s = 3; // Safety, though it should never exceed 3
    
    unsigned long long cur = 1;
    seq.push_back(cur);
    cur *= s; seq.push_back(cur);
    for (int i = 0; i < L - 2; ++i) {
        cur <<= 1; // multiply by 2
        seq.push_back(cur);
    }
    
    cout << seq.size() << "\n";
    for (size_t i = 0; i < seq.size(); ++i) {
        if (i) cout << ' ';
        cout << seq[i];
    }
    cout << "\n";
    return 0;
}