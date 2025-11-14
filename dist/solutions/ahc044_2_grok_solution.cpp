#include <bits/stdc++.h>
using namespace std;

int main() {
    int N, L;
    cin >> N >> L;
    vector<long long> T(N);
    for (auto &x : T) cin >> x;
    vector<int> A(N, -1), B(N, -1);
    vector<int> visits(N, 0);
    int current = 0;
    visits[0] = 1;
    for (int week = 2; week <= L; week++) {
        int x = current;
        int t = visits[x];
        bool is_odd = t % 2;
        int &target = is_odd ? A[x] : B[x];
        if (target == -1) {
            int best_j = 0;
            long long best_score = T[0] - visits[0];
            int best_vis = visits[0];
            for (int j = 0; j < N; j++) {
                long long score = T[j] - visits[j];
                int vis = visits[j];
                if (score > best_score ||
                    (score == best_score && vis < best_vis) ||
                    (score == best_score && vis == best_vis && j < best_j)) {
                    best_score = score;
                    best_vis = vis;
                    best_j = j;
                }
            }
            target = best_j;
        }
        current = target;
        visits[current]++;
    }
    for (int i = 0; i < N; i++) {
        if (A[i] == -1) A[i] = 0;
        if (B[i] == -1) B[i] = 0;
    }
    for (int i = 0; i < N; i++) {
        cout << A[i] << " " << B[i] << "\n";
    }
    return 0;
}