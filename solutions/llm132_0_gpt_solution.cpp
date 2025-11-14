#include <bits/stdc++.h>
using namespace std;

// Simple SplitMix64 RNG for deterministic pseudo-random numbers
struct SplitMix64 {
    uint64_t x;
    SplitMix64(uint64_t seed = 0x9e3779b97f4a7c15ULL) : x(seed) {}
    uint64_t next() {
        uint64_t z = (x += 0x9e3779b97f4a7c15ULL);
        z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
        z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
        return z ^ (z >> 31);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int R, H;
    if (!(cin >> R >> H)) return 0;

    const int N = 1000;
    int Q = min(R, 63); // number of robots to use; keep <=63 to pack into 64-bit
    uint64_t mask = (Q == 64 ? ~0ULL : ((1ULL << Q) - 1));

    // Generate a random binary code (Q bits) for each of the 1000 positions
    SplitMix64 rng(0xC0FFEE123456789ULL);
    vector<uint64_t> code(N + 1);
    for (int i = 1; i <= N; ++i) {
        uint64_t c = 0;
        while (c == 0) { // avoid zero vector
            c = rng.next() & mask;
        }
        code[i] = c;
    }

    // Send Q robots, each corresponding to one bit position
    for (int b = 0; b < Q; ++b) {
        vector<int> P;
        P.reserve(600);
        for (int i = 1; i <= N; ++i) {
            if ((code[i] >> b) & 1ULL) P.push_back(i);
        }
        cout << "? " << P.size();
        for (int v : P) cout << " " << v;
        cout << "\n";
        cout.flush();
    }

    // Get the answers
    cout << "@\n";
    cout.flush();

    int L;
    if (!(cin >> L)) return 0;
    vector<int> ans(L);
    for (int i = 0; i < L; ++i) cin >> ans[i];

    // Build the result bitmask
    uint64_t rmask = 0;
    for (int i = 0; i < L && i < Q; ++i) {
        if (ans[i]) rmask |= (1ULL << i);
    }

    // Try to find a pair (a,b) such that code[a] | code[b] == rmask
    // First, filter candidates whose code is subset of rmask
    vector<int> cand;
    cand.reserve(16);
    for (int i = 1; i <= N; ++i) {
        if ((code[i] & ~rmask) == 0ULL) cand.push_back(i);
    }

    int a = -1, b = -1;
    bool found = false;

    // Prefer small candidate search
    for (size_t i = 0; i < cand.size() && !found; ++i) {
        for (size_t j = i; j < cand.size(); ++j) {
            if ((code[cand[i]] | code[cand[j]]) == rmask) {
                a = cand[i];
                b = cand[j];
                found = true;
                break;
            }
        }
    }

    // Fallback: full search if not found (very unlikely)
    if (!found) {
        for (int i = 1; i <= N && !found; ++i) {
            for (int j = i; j <= N; ++j) {
                if ((code[i] | code[j]) == rmask) {
                    a = i; b = j; found = true; break;
                }
            }
        }
    }

    if (!found) {
        // As a last resort, output something valid (should not happen)
        a = 1; b = 1;
    }

    // Output the result
    cout << "! " << a << " " << b << "\n";
    cout.flush();

    return 0;
}