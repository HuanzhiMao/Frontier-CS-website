#include <bits/stdc++.h>
using namespace std;

long long count_divisors(long long n) {
    if (n == 1) return 1;
    long long res = 1;
    long long cnt = 0;
    while (n % 2 == 0) {
        n /= 2;
        cnt++;
    }
    res *= (cnt + 1);
    for (long long i = 3; i * i <= n; i += 2) {
        cnt = 0;
        while (n % i == 0) {
            n /= i;
            cnt++;
        }
        if (cnt) res *= (cnt + 1);
    }
    if (n > 1) res *= 2;
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        long long X;
        cin >> X;
        cout << count_divisors(X) << "\n";
    }
    return 0;
}