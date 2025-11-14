#include <bits/stdc++.h>

using namespace std;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  int n;
  long long k;
  cin >> n >> k;
  vector<vector<long long>> mat(n+1, vector<long long>(n+1, -1LL));
  auto get = [&](int x, int y) -> long long {
    if (mat[x][y] != -1LL) return mat[x][y];
    cout << "QUERY " << x << " " << y << "\n";
    cout.flush();
    long long v;
    cin >> v;
    mat[x][y] = v;
    return v;
  };
  long long lo = get(1, 1);
  long long hi = get(n, n);
  long long ans = lo;
  while (lo <= hi) {
    long long mid = lo + (hi - lo) / 2;
    long long cnt = 0;
    int r = 1, c = n;
    while (r <= n && c >= 1) {
      long long val = get(r, c);
      if (val <= mid) {
        cnt += (long long) c;
        r++;
      } else {
        c--;
      }
    }
    if (cnt >= k) {
      ans = mid;
      hi = mid - 1;
    } else {
      lo = mid + 1;
    }
  }
  cout << "DONE " << ans << "\n";
  cout.flush();
  return 0;
}