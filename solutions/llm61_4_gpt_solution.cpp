#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while(T--){
        long long n, m, c;
        cin >> n >> m >> c;
        vector<long long> a(n+1), b(m+1);
        for(int i=1;i<=n;i++) cin >> a[i];
        for(int i=1;i<=m;i++) cin >> b[i];
        // Trivial slow solution placeholder: always play a single character all days
        // Compute levels for total sum and subtract c once.
        vector<unsigned long long> B(m+1);
        B[0]=0;
        for(int i=1;i<=m;i++) B[i]=B[i-1]+(unsigned long long)b[i];
        unsigned long long S=0;
        for(int i=1;i<=n;i++) S += (unsigned long long)a[i];
        int k = upper_bound(B.begin(), B.end(), S) - B.begin() - 1;
        long long ans = (long long)k - c;
        cout << ans << "\n";
    }
    return 0;
}