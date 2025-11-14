#include <bits/stdc++.h>
using namespace std;

int main() {
    int N, L;
    cin >> N >> L;
    vector<long long> T(N);
    for (auto &x : T) cin >> x;
    vector<long long> V(N);
    for (int j = 0; j < N; j++) {
        V[j] = T[j] - (j == 0 ? 1LL : 0LL);
    }
    vector<pair<long long, int>> bundles;
    for (int i = 0; i < N; i++) {
        long long s = (T[i] + 1) / 2;
        long long r = T[i] / 2;
        bundles.push_back({s, i * N});
        bundles.push_back({r, i * N + 1});
    }
    sort(bundles.rbegin(), bundles.rend());
    vector<long long> current(N, 0);
    vector<int> assign_to(N * 2);
    for (auto &b : bundles) {
        long long size = b.first;
        int id = b.second;
        int best = -1;
        long long best_diff = LLONG_MIN;
        for (int j = 0; j < N; j++) {
            long long diff = V[j] - current[j];
            if (diff > best_diff) {
                best_diff = diff;
                best = j;
            }
        }
        current[best] += size;
        assign_to[id] = best;
    }
    vector<int> a(N), b(N);
    for (int i = 0; i < N; i++) {
        a[i] = assign_to[i * N];
        b[i] = assign_to[i * N + 1];
    }
    for (int i = 0; i < N; i++) {
        cout << a[i] << " " << b[i] << endl;
    }
    return 0;
}