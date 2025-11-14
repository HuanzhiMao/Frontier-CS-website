#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    if (!(cin >> n)) {
        cout << "! 0\n" << flush;
        return 0;
    }
    
    mt19937 rng((uint64_t)chrono::high_resolution_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> distAll(0, n - 1);
    
    vector<int> a0(n, -1), a1(n, -1), K(n, -1);
    vector<char> asked(n, 0);
    auto finish = [&](int idx) {
        cout << "! " << idx << "\n" << flush;
        exit(0);
    };
    auto safe_finish = [&]() {
        int guess = -1;
        for (int i = 0; i < n; ++i) if (K[i] == 0) { guess = i; break; }
        if (guess == -1) {
            int bestK = INT_MAX, bestIdx = 0;
            for (int i = 0; i < n; ++i) if (K[i] >= 0 && K[i] < bestK) { bestK = K[i]; bestIdx = i; }
            guess = bestIdx;
        }
        if (guess < 0) guess = 0;
        finish(guess);
    };
    auto ask = [&](int idx) -> pair<int,int> {
        if (asked[idx]) return {a0[idx], a1[idx]};
        cout << "? " << idx << "\n" << flush;
        int x, y;
        if (!(cin >> x >> y)) {
            safe_finish();
        }
        asked[idx] = 1;
        a0[idx] = x;
        a1[idx] = y;
        K[idx] = x + y;
        if (K[idx] == 0) finish(idx);
        return {x, y};
    };
    
    // Maintain a candidate best index with minimal K seen so far
    int L = 0, R = n - 1;
    int bestIdx = n / 2;
    ask(bestIdx);
    int bestK = K[bestIdx];
    if (a0[bestIdx] == 0 && a1[bestIdx] > 0) L = max(L, bestIdx + 1);
    if (a1[bestIdx] == 0 && a0[bestIdx] > 0) R = min(R, bestIdx - 1);
    
    auto update_best = [&](int idx) {
        if (K[idx] < bestK) {
            bestK = K[idx];
            bestIdx = idx;
        }
        if (a0[idx] == 0 && a1[idx] > 0) L = max(L, idx + 1);
        if (a1[idx] == 0 && a0[idx] > 0) R = min(R, idx - 1);
        if (L < 0) L = 0;
        if (R >= n) R = n - 1;
        if (L > R) { // inconsistent, but be safe
            safe_finish();
        }
    };
    
    // Try descending by focusing on sides with more expensive items
    auto sample_side = [&](int center) -> bool {
        int leftCnt = a0[center];
        int rightCnt = a1[center];
        int leftLen = center;
        int rightLen = n - 1 - center;
        
        if (leftCnt <= 0 && rightCnt <= 0) return false; // should not happen since K>0 is handled
        
        // Choose side with more "more-expensive" items
        bool useLeft = (leftCnt > rightCnt);
        if (leftCnt == 0 && rightCnt > 0) useLeft = false;
        if (rightCnt == 0 && leftCnt > 0) useLeft = true;
        
        long long sc = useLeft ? leftCnt : rightCnt;
        long long sl = useLeft ? leftLen : rightLen;
        if (sl <= 0 || sc <= 0) return false;
        
        long long tries = min(1000LL, 10LL + 5LL * sl / max(1LL, sc));
        if (tries < 20) tries = 20;
        
        uniform_int_distribution<int> distL(0, center - 1);
        uniform_int_distribution<int> distR(center + 1, n - 1);
        
        for (long long t = 0; t < tries; ++t) {
            int j = useLeft ? distL(rng) : distR(rng);
            auto pr = ask(j);
            if (K[j] < bestK) {
                update_best(j);
                return true;
            } else {
                update_best(j);
            }
        }
        return false;
    };
    
    // Global random sampling to try to find better K
    auto global_sample = [&](int trials) -> bool {
        if (L > R) return false;
        uniform_int_distribution<int> distSeg(L, R);
        bool improved = false;
        for (int t = 0; t < trials; ++t) {
            int j = distSeg(rng);
            auto pr = ask(j);
            if (K[j] < bestK) {
                update_best(j);
                improved = true;
                break;
            } else {
                update_best(j);
            }
        }
        return improved;
    };
    
    // Descent loop with bounded attempts, then fallback to scanning
    int outerRounds = 0;
    while (bestK > 0 && outerRounds < 200) {
        bool improved = false;
        // Try to improve focusing on side information
        if (sample_side(bestIdx)) {
            improved = true;
        } else {
            // Try global sampling inside [L, R]
            if (global_sample(300)) improved = true;
        }
        if (!improved) break;
        ++outerRounds;
    }
    
    // Fallback: systematic scan within [L, R]
    for (int i = L; i <= R; ++i) {
        if (!asked[i]) {
            ask(i);
            if (K[i] == 0) finish(i);
            update_best(i);
        }
    }
    
    // As a last resort (shouldn't reach here), output best known
    safe_finish();
    return 0;
}