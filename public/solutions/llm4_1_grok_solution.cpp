#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    long long n, k;
    cin >> n >> k;
    vector<vector<long long>> mat(n + 1, vector<long long>(n + 1, -1LL));
    auto get = [&](int x, int y) -> long long {
        if (mat[x][y] != -1LL) return mat[x][y];
        cout << "QUERY " << x << " " << y << endl;
        cout.flush();
        long long v;
        cin >> v;
        mat[x][y] = v;
        return v;
    };
    long long low = get(1, 1);
    long long high = get(n, n);
    while (low < high) {
        long long mid = low + (high - low) / 2;
        long long cnt = 0;
        int j = n;
        for (int i = 1; i <= n; ++i) {
            while (j >= 1 && get(i, j) > mid) --j;
            cnt += j;
        }
        if (cnt >= k) high = mid;
        else low = mid + 1;
    }
    cout << "DONE " << low << endl;
    cout.flush();
    return 0;
}