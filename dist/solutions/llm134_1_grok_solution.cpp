#include <bits/stdc++.h>

using namespace std;

int main() {
  long long n;
  cin >> n;
  long long la = 1, ha = n;
  while (la <= ha) {
    long long mida = la + (ha - la) / 2;
    long long lb = 1, hb = n;
    bool gotone = false;
    while (lb <= hb) {
      long long midb = lb + (hb - lb) / 2;
      cout << mida << " " << midb << endl;
      cout.flush();
      int r;
      cin >> r;
      if (r == 0) {
        return 0;
      }
      if (r == 1) {
        gotone = true;
        break;
      } else if (r == 2) {
        lb = midb + 1;
      } else if (r == 3) {
        hb = midb - 1;
      }
    }
    if (gotone) {
      la = mida + 1;
    } else {
      ha = mida - 1;
    }
  }
  return 0;
}