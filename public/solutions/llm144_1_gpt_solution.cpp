#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    int M1 = n / 2;
    int M2 = M1 + 1;

    // Cache to avoid repeating the same query pair
    unordered_map<long long, pair<int,int>> cache;

    auto keyPair = [&](int i, int j) -> long long {
        if (i > j) swap(i, j);
        return 1LL * i * (n + 1) + j;
    };

    auto query_excl = [&](int i, int j) -> pair<int,int> {
        if (i > j) swap(i, j);
        long long key = keyPair(i, j);
        auto it = cache.find(key);
        if (it != cache.end()) return it->second;

        cout << 0 << ' ' << (n - 2) << ' ';
        for (int idx = 1; idx <= n; ++idx) {
            if (idx != i && idx != j) cout << idx << ' ';
        }
        cout << endl;
        cout.flush();

        int m1, m2;
        if (!(cin >> m1 >> m2)) exit(0);
        if (m1 > m2) swap(m1, m2);
        cache[key] = {m1, m2};
        return {m1, m2};
    };

    int a = -1, b = -1;
    vector<int> mediansFound;

    // Step 1: Find two anchors a and b from opposite sides (S and L),
    // i.e., S\{a,b} returns medians (M1, M2)
    for (int i = 1; i <= n; ++i) {
        bool foundOpp = false;
        for (int j = 1; j <= n; ++j) {
            if (i == j) continue;
            auto ans = query_excl(i, j);
            if (ans.first == M1 && ans.second == M2) {
                a = i; b = j;
                foundOpp = true;
                break;
            }
        }
        if (foundOpp) break;
        // If for index i none of the j yielded base medians, i is one of the medians
        mediansFound.push_back(i);
        if ((int)mediansFound.size() == 2) {
            cout << 1 << ' ' << mediansFound[0] << ' ' << mediansFound[1] << endl;
            cout.flush();
            return 0;
        }
    }

    // Step 2: Identify indices of M1 and M2 using anchors a and b
    int idxM1 = -1, idxM2 = -1;

    // First pass using anchor a
    for (int t = 1; t <= n; ++t) {
        if (t == a || t == b) continue;
        auto ans = query_excl(t, a);
        // With anchor in L: (M1-1, M2) => t = M1
        // With anchor in S: (M1, M2+1) => t = M2
        if (ans.first == M1 - 1 && ans.second == M2) {
            idxM1 = t;
        }
        if (ans.first == M1 && ans.second == M2 + 1) {
            idxM2 = t;
        }
        if (idxM1 != -1 && idxM2 != -1) break;
    }

    // Second pass using anchor b for any missing
    if (idxM1 == -1 || idxM2 == -1) {
        for (int t = 1; t <= n; ++t) {
            if (t == a || t == b) continue;
            if (t == idxM1 || t == idxM2) continue;
            auto ans = query_excl(t, b);
            if (idxM1 == -1 && ans.first == M1 - 1 && ans.second == M2) {
                idxM1 = t;
            }
            if (idxM2 == -1 && ans.first == M1 && ans.second == M2 + 1) {
                idxM2 = t;
            }
            if (idxM1 != -1 && idxM2 != -1) break;
        }
    }

    cout << 1 << ' ' << idxM1 << ' ' << idxM2 << endl;
    cout.flush();
    return 0;
}