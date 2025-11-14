#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    long long n;
    if (!(cin >> n)) return 0;

    long long L = sqrtl((long double)n);
    while (L * L < n) ++L;

    cout.setf(std::ios::fixed); 
    cout << setprecision(6);
    cout << (double)L << "\n";

    long long count = 0;
    for (long long y = 0; y < L && count < n; ++y) {
        for (long long x = 0; x < L && count < n; ++x) {
            double xi = (double)x + 0.5;
            double yi = (double)y + 0.5;
            double ai = 0.0;
            cout << xi << " " << yi << " " << ai << "\n";
            ++count;
        }
    }
    return 0;
}