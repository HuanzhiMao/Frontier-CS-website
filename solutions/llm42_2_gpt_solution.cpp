#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    long long n;
    if(!(cin >> n)) return 0;
    long long Lint = (long long)ceil(sqrt((long double)n));
    double L = (double)Lint;
    cout.setf(std::ios::fixed); 
    cout << setprecision(6);
    cout << L << "\n";
    for(long long i = 0; i < n; ++i) {
        long long x = i % Lint;
        long long y = i / Lint;
        cout << (x + 0.5) << " " << (y + 0.5) << " " << 0.0 << "\n";
    }
    return 0;
}