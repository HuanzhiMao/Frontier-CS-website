#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    long long n;
    if (!(cin >> n)) return 0;
    
    long long m = (long long)ceil(sqrt((long double)n));
    cout.setf(std::ios::fixed);
    cout << setprecision(6) << (double)m << "\n";
    
    long long cnt = 0;
    for (long long y = 0; y < m && cnt < n; ++y) {
        for (long long x = 0; x < m && cnt < n; ++x) {
            cout << setprecision(6) << (double)(x + 0.5) << " " << (double)(y + 0.5) << " " << 0.0 << "\n";
            ++cnt;
        }
    }
    return 0;
}