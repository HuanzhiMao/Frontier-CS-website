#include <bits/stdc++.h>
using namespace std;

struct Point {
    long long x, y;
};

static inline double dist(const Point &a, const Point &b) {
    long double dx = (long double)a.x - (long double)b.x;
    long double dy = (long double)a.y - (long double)b.y;
    return (double)std::sqrt((double)(dx*dx + dy*dy));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    if (!(cin >> N)) return 0;
    vector<Point> pts(N);
    for (int i = 0; i < N; ++i) {
        cin >> pts[i].x >> pts[i].y;
    }

    // Build an initial route using x-stripes and serpentine y-order within each stripe.
    int M = max(1, (int) sqrt((long double) max(1, N-1)));
    vector<int> order;
    order.reserve(max(0, N-1));
    for (int s = 0; s < M; ++s) {
        int l = 1 + (int)((long long)(N - 1) * s / M);
        int r = 1 + (int)((long long)(N - 1) * (s + 1) / M);
        if (l >= r) continue;
        vector<int> ids;
        ids.reserve(r - l);
        for (int i = l; i < r; ++i) ids.push_back(i);
        if (s % 2 == 0) {
            sort(ids.begin(), ids.end(), [&](int a, int b) {
                if (pts[a].y != pts[b].y) return pts[a].y < pts[b].y;
                return a < b;
            });
        } else {
            sort(ids.begin(), ids.end(), [&](int a, int b) {
                if (pts[a].y != pts[b].y) return pts[a].y > pts[b].y;
                return a < b;
            });
        }
        for (int id : ids) order.push_back(id);
    }

    // Optional rotation to reduce edges to/from 0 and a boundary internal edge.
    int msize = (int)order.size();
    if (msize >= 2) {
        double bestVal = 1e300;
        int bestK = 0;
        double constEdge = dist(pts[order.back()], pts[order.front()]);
        for (int k = 0; k < msize; ++k) {
            int prev = (k == 0 ? msize - 1 : k - 1);
            double val = dist(pts[0], pts[order[k]]) + dist(pts[order[prev]], pts[0]) + constEdge - dist(pts[order[prev]], pts[order[k]]);
            if (val < bestVal) {
                bestVal = val;
                bestK = k;
            }
        }
        if (bestK != 0) {
            rotate(order.begin(), order.begin() + bestK, order.end());
        }
    }

    // Sieve for primes up to N-1
    vector<char> isPrime(N, true);
    if (N > 0) isPrime[0] = false;
    if (N > 1) isPrime[1] = false;
    for (int i = 2; (long long)i * i < N; ++i) {
        if (isPrime[i]) {
            for (int j = i * i; j < N; j += i) isPrime[j] = false;
        }
    }

    // Carrot constraint adjustment:
    // For each chunk of 10 positions in the visiting sequence, put a prime at index (start+8) if possible.
    // This ensures the source of step t=10,20,... is prime, avoiding the 10% penalty where feasible.
    for (size_t start = 0; start < order.size(); start += 10) {
        size_t end = min(order.size(), start + 10);
        size_t target = start + 8; // corresponds to t = (target + 2)
        if (target < end) {
            if (!isPrime[order[target]]) {
                size_t posPrime = end;
                for (size_t j = start; j < end; ++j) {
                    if (isPrime[order[j]]) { posPrime = j; break; }
                }
                if (posPrime < end && posPrime != target) {
                    swap(order[posPrime], order[target]);
                }
            }
        }
    }

    // Output route
    cout << (N + 1) << '\n';
    cout << 0 << '\n';
    for (int id : order) cout << id << '\n';
    cout << 0 << '\n';

    return 0;
}