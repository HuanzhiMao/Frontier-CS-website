#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    unsigned long long n;
    if (!(cin >> n)) return 0;

    vector<unsigned long long> seq;
    seq.push_back(1);
    while (seq.back() <= n / 2 && seq.size() < 1000000) {
        seq.push_back(seq.back() * 2);
    }

    cout << seq.size() << "\n";
    for (size_t i = 0; i < seq.size(); ++i) {
        if (i) cout << ' ';
        cout << seq[i];
    }
    cout << "\n";
    return 0;
}