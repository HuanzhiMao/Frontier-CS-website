#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

using int128 = __int128_t;

long long get_level(int128 exp, const vector<int128>& B_prefix) {
    if (exp < 0) return 0;
    auto it = upper_bound(B_prefix.begin() + 1, B_prefix.end(), exp);
    return (it - B_prefix.begin() - 1);
}

const long long NEGINF = -2e18; 

void solve() {
    int n;
    long long m_ll, c;
    cin >> n >> m_ll >> c;
    int m = m_ll;

    vector<int128> a(n);
    for (int i = 0; i < n; ++i) {
        long long val;
        cin >> val;
        a[i] = val;
    }

    vector<int128> b(m);
    for (int i = 0; i < m; ++i) {
        long long val;
        cin >> val;
        b[i] = val;
    }

    vector<int128> A(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        A[i + 1] = A[i] + a[i];
    }

    vector<int128> B(m + 1, 0);
    for (int i = 0; i < m; ++i) {
        B[i + 1] = B[i] + b[i];
    }
    
    vector<long long> dp(n + 1, NEGINF);
    dp[0] = 0;

    for (int i = 1; i <= n; ++i) {
        for (int j = 0; j < i; ++j) {
            if (dp[j] > NEGINF) {
                long long level = get_level(A[i] - A[j], B);
                dp[i] = max(dp[i], dp[j] + level - c);
            }
        }
    }

    cout << max(0LL, dp[n]) << "\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}