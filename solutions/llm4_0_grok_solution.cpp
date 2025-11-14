#include <bits/stdc++.h>
using namespace std;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(NULL);
  int n;
  long long kk;
  cin >> n >> kk;
  vector<vector<long long>> mat(n + 1, vector<long long>(n + 1, -1LL));
  long long low = -1000000000000000000LL;
  long long high = 1000000000000000000LL;
  while (low < high) {
    long long mid = low + (high - low) / 2;
    long long cnt = 0;
    int r = 1, c = n;
    while (r <= n && c >= 1) {
      long long v;
      if (mat[r][c] != -1LL) {
        v = mat[r][c];
      } else {
        cout << "QUERY " << r << " " << c << "\n";
        cout.flush();
        cin >> v;
        mat[r][c] = v;
      }
      if (v <= mid) {
        cnt += c;
        r++;
      } else {
        c--;
      }
    }
    if (cnt >= kk) {
      high = mid;
    } else {
      low = mid + 1;
    }
  }
  cout << "DONE " << low << "\n";
  cout.flush();
  return 0;
}