#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cassert>
#include <functional>

using namespace std;

int K, N;

// Stores the response from a query
struct QueryResult {
    int r;
    vector<pair<int, int>> pairs;

    bool contains(int u, int v) const {
        if (u > v) swap(u, v);
        for (const auto& p : pairs) {
            if (p.first == u && p.second == v) {
                return true;
            }
        }
        return false;
    }
};

QueryResult ask(int x, int y, int z) {
    cout << "? " << x << " " << y << " " << z << endl;
    QueryResult res;
    cin >> res.r;
    res.pairs.resize(res.r);
    for (int i = 0; i < res.r; ++i) {
        cin >> res.pairs[i].first >> res.pairs[i].second;
        if (res.pairs[i].first > res.pairs[i].second) {
            swap(res.pairs[i].first, res.pairs[i].second);
        }
    }
    return res;
}

// True if p is closer to refA than q is.
// refA, refB are endpoints of an arc. p, q are on that arc.
bool is_closer_to_A(int p, int q, int refA, int refB) {
    QueryResult res_A = ask(refA, p, q);
    if (res_A.contains(refA, p) && !res_A.contains(refA, q)) return true;
    if (!res_A.contains(refA, p) && res_A.contains(refA, q)) return false;

    // Ambiguous case for refA: can be {p,q} or both/neither pairs contain refA.
    QueryResult res_B = ask(refB, p, q);
    // p closer to B means further from A
    if (res_B.contains(refB, p) && !res_B.contains(refB, q)) return false; 
    // q closer to B means p is further from B, so p is closer to A
    if (!res_B.contains(refB, p) && res_B.contains(refB, q)) return true; 
    
    // Fallback for cases where both queries are ambiguous.
    // This can happen if p and q are symmetric wrt the midpoint of arc(A,B).
    return p < q; // Arbitrary but consistent tie-break
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> K >> N;

    if (N <= 2) {
        cout << "!";
        for (int i = 0; i < N; ++i) cout << " " << i;
        cout << endl;
        return 0;
    }

    int A = 0;
    int B = 1;

    vector<int> s_short, s_long;
    for (int i = 2; i < N; ++i) {
        QueryResult res = ask(A, B, i);
        if (res.contains(A, B)) {
            s_long.push_back(i);
        } else {
            s_short.push_back(i);
        }
    }

    if (s_short.size() > s_long.size()) {
        swap(s_short, s_long);
    }
    
    vector<int> sorted_short = s_short;
    vector<int> sorted_long = s_long;

    if (!sorted_short.empty()) {
        sort(sorted_short.begin(), sorted_short.end(), [&](int p, int q) {
            return is_closer_to_A(p, q, A, B);
        });
    }

    if (!sorted_long.empty()) {
        sort(sorted_long.begin(), sorted_long.end(), [&](int p, int q) {
            // Sort relative to B, so use B as the primary reference.
            return is_closer_to_A(p, q, B, A);
        });
    }

    cout << "!";
    cout << " " << A;
    for (int x : sorted_short) {
        cout << " " << x;
    }
    cout << " " << B;
    for (int x : sorted_long) {
        cout << " " << x;
    }
    cout << endl;

    return 0;
}