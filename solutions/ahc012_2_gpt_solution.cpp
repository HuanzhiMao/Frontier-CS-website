#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, K;
    if (!(cin >> N >> K)) return 0;
    vector<int> a(10);
    for (int i = 0; i < 10; ++i) cin >> a[i];
    for (int i = 0; i < N; ++i) {
        int x, y;
        cin >> x >> y;
    }
    
    int k = K;
    cout << k << "\n";
    for (int i = 1; i <= k; ++i) {
        long long px = i;
        long long py = 0;
        long long qx = 0;
        long long qy = 1LL * i * i;
        if (qy > 1000000000LL) qy = i; // Fallback (rare for given constraints)
        cout << px << " " << py << " " << qx << " " << qy << "\n";
    }
    return 0;
}