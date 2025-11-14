#include <bits/stdc++.h>
using namespace std;

static inline int read_int() {
    int x;
    if (!(cin >> x)) exit(0);
    return x;
}

int n;

int query(const vector<int>& q) {
    cout << 0;
    for (int i = 0; i < n; ++i) cout << ' ' << q[i];
    cout << endl;
    cout.flush();
    return read_int();
}

void guess(const vector<int>& p) {
    cout << 1;
    for (int i = 0; i < n; ++i) cout << ' ' << p[i];
    cout << endl;
    cout.flush();
    exit(0);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    n = read_int();
    if (n == 1) {
        guess(vector<int>{1});
    }

    mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

    vector<int> q(n, 3);

    // Step 1: Find the index of value 2 using randomized halving with values 2 and 3
    vector<int> candidates(n);
    iota(candidates.begin(), candidates.end(), 0); // 0-based indices

    vector<char> inS(n, 0);
    while ((int)candidates.size() > 1) {
        // Create random subset S of candidates (about half)
        fill(inS.begin(), inS.end(), 0);
        vector<int> S;
        S.reserve(candidates.size()/2 + 1);

        // Randomly include each candidate with p=1/2
        for (int idx : candidates) {
            if (uniform_int_distribution<int>(0, 1)(rng)) {
                inS[idx] = 1;
                S.push_back(idx);
            }
        }
        if (S.empty() || (int)S.size() == (int)candidates.size()) continue; // try again

        // Build query: set 2 for S, 3 for others
        for (int i = 0; i < n; ++i) q[i] = 3;
        for (int idx : S) q[idx] = 2;

        int ans = query(q); // 0,1, or 2

        if (ans == 2) {
            // i2 in S
            candidates.swap(S);
        } else if (ans == 0) {
            // i2 not in S -> candidates = candidates \ S
            vector<int> newC;
            newC.reserve(candidates.size() - S.size());
            for (int idx : candidates) if (!inS[idx]) newC.push_back(idx);
            candidates.swap(newC);
        } else {
            // ans == 1 -> no information; try again
            continue;
        }
    }
    int idx2 = candidates[0]; // position (0-based) where value 2 is located

    // Prepare result permutation (values at positions)
    vector<int> perm(n, 0);
    perm[idx2] = 2;

    // Maintain available indices excluding idx2
    vector<int> avail;
    avail.reserve(n - 1);
    for (int i = 0; i < n; ++i) if (i != idx2) avail.push_back(i);

    // Step 2: For each v in {3..n}, find its index using binary search with base value 2
    for (int v = 3; v <= n; ++v) {
        // If only one available index left, break early
        if ((int)avail.size() == 1) {
            perm[avail[0]] = v;
            avail.clear();
            break;
        }

        vector<int> cand = avail;
        while ((int)cand.size() > 1) {
            int half = (int)cand.size() / 2;
            if (half == 0) break; // safety

            // S = first half of cand
            vector<int> S(cand.begin(), cand.begin() + half);
            vector<char> inS_local(n, 0);
            for (int idx : S) inS_local[idx] = 1;

            // Build query: set v on S, 2 elsewhere; ensure idx2 has 2
            for (int i = 0; i < n; ++i) q[i] = 2;
            for (int idx : S) q[idx] = v;
            q[idx2] = 2; // explicit

            int ans = query(q); // equals 1 + [iv in S], because idx2 not in S

            if (ans >= 2) {
                // iv in S
                cand.swap(S);
            } else {
                // iv not in S -> rest half
                vector<int> rest(cand.begin() + half, cand.end());
                cand.swap(rest);
            }
        }
        int pos = cand[0];
        perm[pos] = v;

        // Remove pos from avail
        auto it = find(avail.begin(), avail.end(), pos);
        if (it != avail.end()) avail.erase(it);
    }

    // Remaining values (1 and maybe others if not assigned due to early break)
    // Assign 1 to the only unassigned index left
    if (!avail.empty()) {
        // Count how many values are unassigned
        vector<int> usedVal(n + 1, 0);
        for (int i = 0; i < n; ++i) if (perm[i] != 0) usedVal[perm[i]] = 1;
        int missingVal = -1;
        for (int v = 1; v <= n; ++v) if (!usedVal[v]) { missingVal = v; break; }
        if (missingVal == -1) missingVal = 1; // fallback
        perm[avail[0]] = missingVal;
        avail.clear();
    }

    // If still any position unfilled, fill with the remaining value(s)
    vector<int> remVals;
    vector<int> remPos;
    vector<int> used(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        if (perm[i] == 0) remPos.push_back(i);
        else used[perm[i]] = 1;
    }
    for (int v = 1; v <= n; ++v) if (!used[v]) remVals.push_back(v);
    if (!remPos.empty() && remPos.size() == remVals.size()) {
        for (size_t i = 0; i < remPos.size(); ++i) perm[remPos[i]] = remVals[i];
    }

    guess(perm);
    return 0;
}