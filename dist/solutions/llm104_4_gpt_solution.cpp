#include <bits/stdc++.h>
using namespace std;

using u64 = unsigned long long;

struct BitOps {
    int n;
    int W;
    BitOps(int n_=0): n(n_) {
        W = (n + 63) / 64;
    }
    void setAll(vector<u64>& v) const {
        v.assign(W, ~0ULL);
        if (W) {
            int lastBits = ((n - 1) % 64) + 1;
            if (lastBits < 64) {
                u64 mask = (1ULL << lastBits) - 1ULL;
                v[W - 1] = mask;
            }
        }
    }
    inline bool getBit(const vector<u64>& v, int idx1) const {
        int i = idx1 - 1;
        int w = i >> 6, b = i & 63;
        return (v[w] >> b) & 1ULL;
    }
    inline void orVec(vector<u64>& a, const vector<u64>& b) const {
        for (int i = 0; i < W; ++i) a[i] |= b[i];
    }
    inline int popcountVec(const vector<u64>& a) const {
        int s = 0;
        for (int i = 0; i < W; ++i) s += __builtin_popcountll(a[i]);
        return s;
    }
    // OR partition of src into destIn (indices <= m) and/or destOut (indices > m)
    void ORPartition(const vector<u64>& src, int m, vector<u64>* destIn, vector<u64>* destOut) const {
        int idxWord = (m - 1) >> 6;
        int bitsIn = ((m - 1) & 63) + 1;
        u64 mask = (bitsIn == 64) ? ~0ULL : ((1ULL << bitsIn) - 1ULL);
        for (int w = 0; w < W; ++w) {
            u64 val = src[w];
            if (!val) continue;
            if (w < idxWord) {
                if (destIn) (*destIn)[w] |= val;
            } else if (w == idxWord) {
                u64 inVal = val & mask;
                if (destIn) (*destIn)[w] |= inVal;
                if (destOut && bitsIn < 64) {
                    u64 outVal = val & (~mask);
                    (*destOut)[w] |= outVal;
                }
            } else {
                if (destOut) (*destOut)[w] |= val;
            }
        }
    }
    // OR only <= m part
    void ORInOnly(const vector<u64>& src, int m, vector<u64>& destIn) const {
        int idxWord = (m - 1) >> 6;
        int bitsIn = ((m - 1) & 63) + 1;
        u64 mask = (bitsIn == 64) ? ~0ULL : ((1ULL << bitsIn) - 1ULL);
        for (int w = 0; w < W; ++w) {
            u64 val = src[w];
            if (!val) continue;
            if (w < idxWord) {
                destIn[w] |= val;
            } else if (w == idxWord) {
                u64 inVal = val & mask;
                destIn[w] |= inVal;
            }
        }
    }
    // OR only > m part
    void OROutOnly(const vector<u64>& src, int m, vector<u64>& destOut) const {
        int idxWord = (m - 1) >> 6;
        int bitsIn = ((m - 1) & 63) + 1;
        u64 mask = (bitsIn == 64) ? ~0ULL : ((1ULL << bitsIn) - 1ULL);
        for (int w = 0; w < W; ++w) {
            u64 val = src[w];
            if (!val) continue;
            if (w == idxWord) {
                if (bitsIn < 64) {
                    u64 outVal = val & (~mask);
                    destOut[w] |= outVal;
                }
            } else if (w > idxWord) {
                destOut[w] |= val;
            }
        }
    }
};

// State indices
// 0: E (no history)
// 1: L0 (last h=0)
// 2: L1 (last h=1)
// 3: "00"
// 4: "01"
// 5: "10"
// 6: "11"
static inline bool allow_next(int state, int h) {
    if (state == 0 || state == 1 || state == 2 || state == 4 || state == 5) return true;
    if (state == 3) return h == 1; // "00" -> only 1 allowed
    if (state == 6) return h == 0; // "11" -> only 0 allowed
    return false;
}
static inline int next_state(int state, int h) {
    if (state == 0) return h ? 2 : 1;
    if (state == 1) return h ? 4 : 3; // L0 -> h=1 => "01", h=0 => "00"
    if (state == 2) return h ? 6 : 5; // L1 -> h=1 => "11", h=0 => "10"
    if (state == 3) { // "00"
        // h must be 1
        return 4; // "01"
    }
    if (state == 4) { // "01"
        return h ? 6 : 5; // h=1 -> "11", h=0 -> "10"
    }
    if (state == 5) { // "10"
        return h ? 4 : 3; // h=1 -> "01", h=0 -> "00"
    }
    if (state == 6) { // "11"
        // h must be 0
        return 5; // "10"
    }
    return 0;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    if (!(cin >> t)) return 0;
    while (t--) {
        int n;
        cin >> n;

        BitOps B(n);
        int W = B.W;

        vector<vector<u64>> states(7, vector<u64>(W, 0));
        B.setAll(states[0]); // initially all candidates with empty history

        auto compute_A_bits = [&](vector<u64>& A0, vector<u64>& A1) {
            // A0: states that allow next h=0 -> {0,1,2,4,5,6}
            // A1: states that allow next h=1 -> {0,1,2,4,5,3}
            fill(A0.begin(), A0.end(), 0ULL);
            fill(A1.begin(), A1.end(), 0ULL);
            for (int w = 0; w < W; ++w) {
                A0[w] = states[0][w] | states[1][w] | states[2][w] | states[4][w] | states[5][w] | states[6][w];
                A1[w] = states[0][w] | states[1][w] | states[2][w] | states[4][w] | states[5][w] | states[3][w];
            }
        };

        auto build_prefix = [&](const vector<u64>& bits, vector<int>& pre) {
            pre.assign(n + 1, 0);
            int acc = 0;
            for (int i = 1; i <= n; ++i) {
                int w = (i - 1) >> 6, b = (i - 1) & 63;
                int bit = (bits[w] >> b) & 1ULL;
                acc += bit;
                pre[i] = acc;
            }
        };

        auto union_bits = [&](const vector<u64>& A0, const vector<u64>& A1, vector<u64>& U) {
            U.resize(W);
            for (int w = 0; w < W; ++w) U[w] = A0[w] | A1[w];
        };

        auto get_first_k = [&](const vector<u64>& U, int k) {
            vector<int> res;
            for (int w = 0; w < W && (int)res.size() < k; ++w) {
                u64 val = U[w];
                while (val && (int)res.size() < k) {
                    int b = __builtin_ctzll(val);
                    int idx = w * 64 + b + 1;
                    if (idx <= n) res.push_back(idx);
                    val &= val - 1;
                }
            }
            return res;
        };

        // Query limit
        double base = 1.116;
        int qLimit = 2 * (int)ceil(log((double)n) / log(base));
        if (qLimit < 0) qLimit = 0; // safety
        int queriesUsed = 0;

        while (true) {
            vector<u64> A0(W, 0), A1(W, 0), Ubits;
            compute_A_bits(A0, A1);
            union_bits(A0, A1, Ubits);
            int Usize = B.popcountVec(Ubits);

            if (Usize <= 2 || queriesUsed >= qLimit) break;

            vector<int> pre0, pre1;
            build_prefix(A0, pre0);
            build_prefix(A1, pre1);
            int tot0 = pre0[n], tot1 = pre1[n];

            int bestM = 1;
            int bestF = INT_MAX;
            int bestBal = INT_MAX;

            for (int m = 1; m <= n; ++m) {
                int c1 = pre1[m] + (tot0 - pre0[m]);
                int c0 = pre0[m] + (tot1 - pre1[m]);
                int f = max(c1, c0);
                int bal = abs(c1 - c0);
                if (f < bestF || (f == bestF && bal < bestBal)) {
                    bestF = f;
                    bestBal = bal;
                    bestM = m;
                }
            }

            cout << "? " << 1 << " " << bestM << endl;
            cout.flush();
            int x;
            if (!(cin >> x)) return 0;
            int y = bestM - x; // 0 or 1

            vector<vector<u64>> newStates(7, vector<u64>(W, 0));

            int noty = 1 - y;
            for (int s = 0; s < 7; ++s) {
                bool allowY = allow_next(s, y);
                bool allowNY = allow_next(s, noty);
                if (!allowY && !allowNY) continue;
                int destIn = allowY ? next_state(s, y) : -1;
                int destOut = allowNY ? next_state(s, noty) : -1;
                if (allowY && allowNY) {
                    B.ORPartition(states[s], bestM, &newStates[destIn], &newStates[destOut]);
                } else if (allowY) {
                    B.ORInOnly(states[s], bestM, newStates[destIn]);
                } else {
                    B.OROutOnly(states[s], bestM, newStates[destOut]);
                }
            }

            states.swap(newStates);
            ++queriesUsed;
        }

        // Prepare final candidates from union
        vector<u64> A0(W, 0), A1(W, 0), Ubits;
        compute_A_bits(A0, A1);
        union_bits(A0, A1, Ubits);
        vector<int> cand = get_first_k(Ubits, 2);

        if (cand.empty()) {
            // Fallback: mark two arbitrary students 1 and 2 (within bounds)
            int a = 1, b = min(2, n);
            cout << "! " << a << endl;
            cout.flush();
            int y1; if (!(cin >> y1)) return 0;
            if (b != a) {
                cout << "! " << b << endl;
                cout.flush();
                int y2; if (!(cin >> y2)) return 0;
            }
        } else if ((int)cand.size() == 1) {
            cout << "! " << cand[0] << endl;
            cout.flush();
            int y1; if (!(cin >> y1)) return 0;
        } else {
            cout << "! " << cand[0] << endl;
            cout.flush();
            int y1; if (!(cin >> y1)) return 0;
            cout << "! " << cand[1] << endl;
            cout.flush();
            int y2; if (!(cin >> y2)) return 0;
        }

        cout << "#" << endl;
        cout.flush();
    }

    return 0;
}