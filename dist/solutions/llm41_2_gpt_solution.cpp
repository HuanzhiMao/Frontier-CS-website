#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    unsigned long long n;
    if (!(cin >> n)) return 0;

    vector<unsigned long long> ans;
    ans.push_back(1);
    while (true) {
        __int128 next = (__int128)ans.back() * 2;
        if (next > n) break;
        ans.push_back((unsigned long long)next);
    }

    cout << ans.size() << "\n";
    for (size_t i = 0; i < ans.size(); ++i) {
        if (i) cout << ' ';
        cout << ans[i];
    }
    cout << "\n";
    return 0;
}