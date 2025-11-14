#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    long long n;
    if (!(cin >> n)) return 0;
    
    long long k = (long long)ceil(sqrt((long double)n));
    double L = (double)k;
    
    cout.setf(std::ios::fixed); 
    cout << setprecision(6);
    cout << L << "\n";
    
    for (long long i = 0; i < n; ++i) {
        long long x = i % k;
        long long y = i / k;
        double xi = x + 0.5;
        double yi = y + 0.5;
        double ai = 0.0;
        cout << xi << " " << yi << " " << ai << "\n";
    }
    return 0;
}