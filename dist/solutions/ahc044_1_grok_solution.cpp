#include <bits/stdc++.h>
using namespace std;

int main() {
  int N;
  long long L;
  cin >> N >> L;
  vector<long long> T(N);
  for(int i=0; i<N; i++) cin >> T[i];
  vector<int> a(N), b(N);
  vector<pair<long long, int>> ord(N);
  for(int i=0; i<N; i++) ord[i] = {T[i], i};
  sort(ord.rbegin(), ord.rend());
  vector<long long> demand = T;
  demand[0]--;
  vector<long long> current_in(N, 0);
  vector<long long> remain = demand;
  set<pair<long long, int>> pq;
  for(int i=0; i<N; i++) {
    pq.insert({ -remain[i], i });
  }
  for(auto &p : ord) {
    int x = p.second;
    long long d = T[x];
    if(d == 0) {
      a[x] = 0;
      b[x] = 0;
      continue;
    }
    long long num_a = (d + 1LL) / 2;
    long long num_b = d / 2;
    auto it1 = *pq.begin();
    pq.erase(pq.begin());
    auto it2 = *pq.begin();
    pq.erase(pq.begin());
    long long r1 = -it1.first;
    int j1 = it1.second;
    long long r2 = -it2.first;
    int j2 = it2.second;
    pq.insert(it1);
    pq.insert(it2);
    if(r2 > r1) {
      swap(r1, r2);
      swap(j1, j2);
    }
    long long flow_j1 = num_a;
    long long flow_j2 = num_b;
    a[x] = j1;
    b[x] = j2;
    pq.erase({ -remain[j1], j1 });
    current_in[j1] += flow_j1;
    remain[j1] = demand[j1] - current_in[j1];
    pq.insert({ -remain[j1], j1 });
    if(num_b > 0) {
      pq.erase({ -remain[j2], j2 });
      current_in[j2] += flow_j2;
      remain[j2] = demand[j2] - current_in[j2];
      pq.insert({ -remain[j2], j2 });
    }
  }
  for(int i=0; i<N; i++) {
    cout << a[i] << " " << b[i] << endl;
  }
  return 0;
}