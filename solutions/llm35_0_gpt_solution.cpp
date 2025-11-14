#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

    auto ask = [&](int x, const vector<int>& S) -> int {
        cout << "? " << x << " " << S.size();
        for (int i : S) {
            cout << " " << i;
        }
        cout << '\n';
        cout.flush();
        int ans;
        if (!(cin >> ans)) exit(0);
        if (ans == -1) exit(0);
        return ans;
    };

    auto make_partition = [&](int N, vector<int>& A, vector<int>& B) {
        A.clear(); B.clear();
        A.reserve((N+1)/2);
        B.reserve(N/2);
        for (int i = 1; i <= N; ++i) {
            if ((rng() & 1ULL) != 0ULL) A.push_back(i);
            else B.push_back(i);
        }
        if (A.empty()) {
            A.push_back(B.back());
            B.pop_back();
        } else if (B.empty()) {
            B.push_back(A.back());
            A.pop_back();
        }
    };

    int t;
    if (!(cin >> t)) return 0;
    for (int tc = 0; tc < t; ++tc) {
        int n; 
        if (!(cin >> n)) return 0;
        int ok;
        if (!(cin >> ok)) return 0;
        if (ok == -1) return 0;

        int N = 2 * n - 1;

        vector<int> twoOnes(n + 1, 0);
        long long usedQueries = 0;

        // Stage 1: 8 partitions, at most 4800 queries
        int P1 = 8;
        for (int r = 0; r < P1; ++r) {
            vector<int> A, B;
            make_partition(N, A, B);
            for (int x = 1; x <= n; ++x) {
                int a = ask(x, A);
                ++usedQueries;
                if (a == 1) {
                    int b = ask(x, B);
                    ++usedQueries;
                    if (b == 1) twoOnes[x]++;
                }
            }
        }

        // Candidates: those never split
        vector<int> C;
        C.reserve(n);
        for (int x = 1; x <= n; ++x) {
            if (twoOnes[x] == 0) C.push_back(x);
        }
        if (C.empty()) {
            // Fallback (shouldn't happen): choose smallest
            C.push_back(1);
        }

        // Stage 2: refine among candidates while we have queries
        while (C.size() > 1 && usedQueries + 2LL * (long long)C.size() <= 5000) {
            vector<int> A, B;
            make_partition(N, A, B);
            vector<int> survivors;
            survivors.reserve(C.size());
            for (int x : C) {
                int a = ask(x, A);
                ++usedQueries;
                if (a == 1) {
                    int b = ask(x, B);
                    ++usedQueries;
                    if (b == 1) {
                        // Split detected => not unique, eliminate
                    } else {
                        survivors.push_back(x);
                    }
                } else {
                    // (0,1) guaranteed, keep
                    survivors.push_back(x);
                }
            }
            if (survivors.empty()) {
                // Extremely unlikely; keep previous set to avoid empty candidates
                survivors = C;
            }
            C.swap(survivors);
        }

        int ans = C[0];
        cout << "! " << ans << '\n';
        cout.flush();
    }

    return 0;
}