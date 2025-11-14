#include <bits/stdc++.h>

using namespace std;

int main() {
  long long n;
  cin >> n;
  long long la = 1, ha = n;
  while (la < ha) {
    long long mid = la + (ha - la) / 2;
    cout << mid << " " << 1 << endl;
    cout.flush();
    int r;
    cin >> r;
    if (r == 0) return 0;
    if (r == 1) {
      la = mid + 1;
    } else if (r == 3) {
      ha = mid - 1;
    } else if (r == 2) {
      long long lb = 1, hb = n;
      bool found = false;
      bool got_one = false;
      while (lb <= hb && !found && !got_one) {
        long long midb = lb + (hb - lb) / 2;
        cout << mid << " " << midb << endl;
        cout.flush();
        int s;
        cin >> s;
        if (s == 0) {
          found = true;
          return 0;
        } else if (s == 1) {
          got_one = true;
          la = mid + 1;
        } else if (s == 2) {
          lb = midb + 1;
        } else if (s == 3) {
          hb = midb - 1;
        }
      }
      if (!found && !got_one) {
        ha = mid - 1;
      }
    }
  }
  cout << la << " " << 1 << endl;
  cout.flush();
  int r;
  cin >> r;
  if (r == 0) return 0;
  if (r == 2) {
    long long lb = 2, hb = n;
    while (lb < hb) {
      long long midb = lb + (hb - lb) / 2;
      cout << la << " " << midb << endl;
      cout.flush();
      int s;
      cin >> s;
      if (s == 0) return 0;
      if (s == 2) {
        lb = midb + 1;
      } else if (s == 3) {
        hb = midb - 1;
      }
    }
    cout << la << " " << lb << endl;
    cout.flush();
    int s;
    cin >> s;
    if (s == 0) return 0;
  }
  return 0;
}