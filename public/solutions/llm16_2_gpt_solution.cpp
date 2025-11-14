#include <bits/stdc++.h>
using namespace std;

using int64 = long long;

static mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

int64 n;

int64 norm(int64 x) {
    x %= n;
    if (x <= 0) x += n;
    return x;
}

int64 addmod(int64 x, int64 t) {
    // x in [1..n]
    int64 y = ( (x - 1 + t) % n ) + 1;
    if (y <= 0) y += n;
    return y;
}

long long ask(long long x, long long y) {
    cout << "? " << x << " " << y << endl;
    cout.flush();
    long long d;
    if (!(cin >> d)) exit(0);
    return d;
}

void answer(long long u, long long v) {
    cout << "! " << u << " " << v << endl;
    cout.flush();
    int r;
    if (!(cin >> r)) exit(0);
    if (r == -1) exit(0);
}

bool F(long long x, long long s) {
    long long y = addmod(x, s);
    long long d = ask(x, y);
    long long base = min(s, n - s);
    return d < base;
}

pair<long long,long long> locate_chord_with_block(long long x0, long long s) {
    // We assume F(x0, s) is true and s <= n/2
    // Find right boundary (a): the largest t in [0, s] with F(x0 + t, s) = true
    long long lo = 0, hi = s; // F(x0+lo)=true; we expect F(x0+hi)=false
    if (F(addmod(x0, hi), s)) {
        // Should not happen for valid s (s >= 2), but safeguard: shrink hi until false or lo+1
        long long step = 1;
        while (hi > 0 && F(addmod(x0, hi), s)) {
            hi--;
            if (hi == 0) break;
        }
    }
    while (lo + 1 < hi) {
        long long mid = (lo + hi) >> 1;
        if (F(addmod(x0, mid), s)) lo = mid;
        else hi = mid;
    }
    long long a = addmod(x0, lo);

    // Find left boundary (b - s): the smallest t in [0, s] so that F(x0 - t, s) = true; i.e., maximize t with true, then sL = x0 - t
    lo = 0, hi = s;
    if (F(addmod(x0, -hi), s)) {
        long long step = 1;
        while (hi > 0 && F(addmod(x0, -hi), s)) {
            hi--;
            if (hi == 0) break;
        }
    }
    while (lo + 1 < hi) {
        long long mid = (lo + hi) >> 1;
        if (F(addmod(x0, -mid), s)) lo = mid;
        else hi = mid;
    }
    long long sL = addmod(x0, -lo);
    long long b = addmod(sL, s);
    return {a, b};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        cin >> n;
        long long m = n / 2;
        // Try to find x0 such that F(x0, s) is true
        vector<long long> sChoices;
        sChoices.push_back(m);
        if (m > 1) sChoices.push_back(m - 1);
        if (m > 2) sChoices.push_back(m - 2);
        if (m > 3) sChoices.push_back(m - 3);

        bool found = false;
        long long x0 = 1, sUsed = m;

        // Deterministic sampling: evenly spaced
        int SAMPLE_EVEN = 60;
        vector<long long> candidates;
        if (n <= 120) {
            for (long long i = 1; i <= n; ++i) candidates.push_back(i);
        } else {
            long long step = max(1LL, n / SAMPLE_EVEN);
            long long cur = 1;
            for (int i = 0; i < SAMPLE_EVEN; ++i) {
                candidates.push_back(cur);
                cur = addmod(cur, step);
            }
        }
        // Random sampling
        int SAMPLE_RAND = 120;
        for (int i = 0; i < SAMPLE_RAND; ++i) {
            long long r = (rng() % n) + 1;
            candidates.push_back(r);
        }

        for (long long s : sChoices) {
            if (found) break;
            long long bestX = -1;
            long long bestVal = (long long)1e18;
            long long base = min(s, n - s);
            for (size_t i = 0; i < candidates.size(); ++i) {
                long long x = candidates[i];
                long long y = addmod(x, s);
                long long d = ask(x, y);
                if (d < base) {
                    x0 = x; sUsed = s; found = true; break;
                }
                if (d < bestVal) {
                    bestVal = d;
                    bestX = x;
                }
                // Limit queries if too many
                if (i > 200) break;
            }
            if (!found && bestX != -1) {
                // Try neighbors around bestX to see if block is close
                for (int t = 1; t <= 50 && !found; ++t) {
                    long long x = addmod(bestX, t);
                    long long y = addmod(x, s);
                    long long d = ask(x, y);
                    if (d < base) { x0 = x; sUsed = s; found = true; break; }
                    x = addmod(bestX, -t);
                    y = addmod(x, s);
                    d = ask(x, y);
                    if (d < base) { x0 = x; sUsed = s; found = true; break; }
                }
            }
        }

        if (!found) {
            // Fallback: brute try a limited linear scan for s = m
            long long step = max(1LL, n / 200);
            long long cur = 1;
            long long s = m;
            long long base = min(s, n - s);
            for (int i = 0; i < 400 && i < (int) (n); ++i) {
                long long d = ask(cur, addmod(cur, s));
                if (d < base) { x0 = cur; sUsed = s; found = true; break; }
                cur = addmod(cur, step);
            }
        }

        if (!found) {
            // As a last resort, output some fixed guess (this should not happen under ideal interactivity)
            // but proceed to avoid deadlock.
            answer(1, 3);
            continue;
        }

        auto pr = locate_chord_with_block(x0, sUsed);
        long long a = pr.first, b = pr.second;

        // Ensure output is non-adjacent and chord could be in any order
        if (a == b) {
            // Something wrong; fallback
            answer(1, 3);
            continue;
        }
        // It's possible order swapped; output as is
        answer(a, b);
    }
    return 0;
}