#include <bits/stdc++.h>
using namespace std;

struct Point {
    double x, y;
};

inline double dist(const Point& a, const Point& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    if (!(cin >> N)) return 0;
    vector<Point> pts(N);
    for (int i = 0; i < N; ++i) {
        long long xi, yi;
        cin >> xi >> yi;
        pts[i].x = (double)xi;
        pts[i].y = (double)yi;
    }

    // Sieve for prime city IDs
    vector<char> isPrime(max(N, 2), false);
    if (N > 2) {
        vector<char> sieve(N, true);
        if (N > 0) sieve[0] = false;
        if (N > 1) sieve[1] = false;
        for (int i = 2; (long long)i * i < N; ++i) {
            if (sieve[i]) {
                for (long long j = 1LL * i * i; j < N; j += i) sieve[(int)j] = false;
            }
        }
        for (int i = 0; i < N; ++i) isPrime[i] = sieve[i];
    }

    // Initial route: 0,1,2,...,N-1,0
    vector<int> P(N + 1);
    P[0] = 0;
    for (int i = 1; i < N; ++i) P[i] = i;
    P[N] = 0;

    // Set of positions (indices in P) where current city is prime and not yet "protected"
    // Positions range: 1..N-1
    set<int> primePosFree;
    for (int i = 1; i < N; ++i) {
        if (isPrime[P[i]]) primePosFree.insert(i);
    }

    auto swapDelta = [&](int pos1, int pos2) -> double {
        if (pos1 == pos2) return 0.0;
        if (pos1 > pos2) swap(pos1, pos2);

        int Nedges = N; // edges t=1..N (between P[t-1] and P[t])

        auto edgeDist = [&](int t, int id1_override_pos, int id2_override_pos, int new_id1, int new_id2) -> double {
            // edge index t connects node at index t-1 and t
            int aIdx = t - 1;
            int bIdx = t;
            int a = P[aIdx];
            int b = P[bIdx];
            if (aIdx == id1_override_pos) a = new_id1;
            else if (aIdx == id2_override_pos) a = new_id2;
            if (bIdx == id1_override_pos) b = new_id1;
            else if (bIdx == id2_override_pos) b = new_id2;
            return dist(pts[a], pts[b]);
        };

        int id1 = P[pos1];
        int id2 = P[pos2];
        double oldSum = 0.0, newSum = 0.0;

        if (pos1 + 1 == pos2) {
            // adjacent case: changed edges are pos1, pos2, pos2+1
            if (pos1 >= 1) oldSum += dist(pts[P[pos1 - 1]], pts[P[pos1]]);
            oldSum += dist(pts[P[pos2 - 1]], pts[P[pos2]]); // equals (P[pos1], P[pos2])
            if (pos2 + 1 <= N) oldSum += dist(pts[P[pos2]], pts[P[pos2 + 1]]);

            if (pos1 >= 1) newSum += dist(pts[P[pos1 - 1]], pts[id2]);
            newSum += dist(pts[id2], pts[id1]);
            if (pos2 + 1 <= N) newSum += dist(pts[id1], pts[P[pos2 + 1]]);
        } else {
            // non-adjacent: changed edges are pos1, pos1+1, pos2, pos2+1
            if (pos1 >= 1) oldSum += dist(pts[P[pos1 - 1]], pts[P[pos1]]);
            if (pos1 + 1 <= N) oldSum += dist(pts[P[pos1]], pts[P[pos1 + 1]]);
            if (pos2 >= 1) oldSum += dist(pts[P[pos2 - 1]], pts[P[pos2]]);
            if (pos2 + 1 <= N) oldSum += dist(pts[P[pos2]], pts[P[pos2 + 1]]);

            if (pos1 >= 1) newSum += dist(pts[P[pos1 - 1]], pts[id2]);
            if (pos1 + 1 <= N) newSum += dist(pts[id2], pts[P[pos1 + 1]]);
            if (pos2 >= 1) newSum += dist(pts[P[pos2 - 1]], pts[id1]);
            if (pos2 + 1 <= N) newSum += dist(pts[id1], pts[P[pos2 + 1]]);
        }
        return newSum - oldSum;
    };

    // Positions that need prime at the start of step t where t % 10 == 0 -> pos = t-1
    vector<int> sPositions;
    for (int t = 10; t <= N; t += 10) {
        int pos = t - 1;
        sPositions.push_back(pos);
    }

    const int SIDE_CAND = 6; // candidates on each side

    for (int pos : sPositions) {
        if (pos < 1 || pos > N - 1) continue; // safety

        if (isPrime[P[pos]]) {
            // Protect this position: ensure we don't move its prime away
            auto it = primePosFree.find(pos);
            if (it != primePosFree.end()) primePosFree.erase(it);
            continue;
        }

        if (primePosFree.empty()) continue; // fallback: can't fix, but very unlikely

        // Gather candidate prime positions near 'pos'
        vector<int> candidates;
        candidates.reserve(SIDE_CAND * 2);

        auto itR = primePosFree.lower_bound(pos);
        for (int c = 0; c < SIDE_CAND && itR != primePosFree.end(); ++c, ++itR) {
            if (*itR != pos) candidates.push_back(*itR);
        }
        auto itL = primePosFree.lower_bound(pos);
        for (int c = 0; c < SIDE_CAND && itL != primePosFree.begin(); ++c) {
            --itL;
            if (*itL != pos) candidates.push_back(*itL);
        }

        if (candidates.empty()) {
            // fallback to nearest
            int bestJ = -1;
            auto it = primePosFree.lower_bound(pos);
            int jRight = (it != primePosFree.end() ? *it : -1);
            int jLeft = -1;
            if (it != primePosFree.begin()) {
                auto it2 = it;
                --it2;
                jLeft = *it2;
            }
            if (jRight == -1 && jLeft == -1) continue;
            if (jRight == -1) bestJ = jLeft;
            else if (jLeft == -1) bestJ = jRight;
            else bestJ = (abs(jRight - pos) < abs(jLeft - pos) ? jRight : jLeft);

            // perform swap
            primePosFree.erase(bestJ);
            swap(P[pos], P[bestJ]);
            // pos becomes protected prime
            continue;
        }

        // Choose candidate with minimal local distance delta
        double bestDelta = numeric_limits<double>::infinity();
        int bestJ = candidates[0];
        for (int j : candidates) {
            double d = swapDelta(pos, j);
            if (d < bestDelta) {
                bestDelta = d;
                bestJ = j;
            }
        }

        primePosFree.erase(bestJ);
        swap(P[pos], P[bestJ]);
        // pos is now protected prime
    }

    // Output the route
    cout << (N + 1) << '\n';
    for (int i = 0; i <= N; ++i) {
        cout << P[i] << '\n';
    }

    return 0;
}