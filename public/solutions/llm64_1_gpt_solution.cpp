#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ull = unsigned long long;

static inline ull absvll(ll x){ return x < 0 ? (ull)(-x) : (ull)x; }

struct Bits2 {
    uint64_t w0 = 0, w1 = 0;
    inline bool get(int i) const {
        if (i < 64) return (w0 >> i) & 1ULL;
        return (w1 >> (i - 64)) & 1ULL;
    }
    inline void set1(int i) {
        if (i < 64) w0 |= (1ULL << i);
        else w1 |= (1ULL << (i - 64));
    }
    inline void toggle(int i) {
        if (i < 64) w0 ^= (1ULL << i);
        else w1 ^= (1ULL << (i - 64));
    }
};

struct State {
    ll sum;
    ull diff;
    Bits2 bits;
};

static inline string bitsToString(const Bits2& b, int n) {
    string s(n, '0');
    for (int i = 0; i < n; ++i) if (b.get(i)) s[i] = '1';
    return s;
}

static inline void greedy_order(const vector<int>& order, const vector<ll>& a, ll T, Bits2& outBits, ll& outSum) {
    ll sum = 0;
    Bits2 bits;
    for (int idx : order) {
        ll s2 = sum + a[idx];
        if (absvll(T - s2) < absvll(T - sum)) {
            sum = s2;
            bits.set1(idx);
        }
    }
    outBits = bits;
    outSum = sum;
}

static inline void local_improve_1flip(const vector<ll>& a, ll T, Bits2& bits, ll& sum) {
    int n = (int)a.size();
    ll residual = T - sum;
    ull bestErr = absvll(residual);
    bool improved = true;
    while (improved) {
        improved = false;
        ull curErr = absvll(residual);
        int bestIdx = -1;
        bool flipToOne = false;
        for (int i = 0; i < n; ++i) {
            if (!bits.get(i)) {
                ll newRes = residual - a[i];
                ull e = absvll(newRes);
                if (e < curErr) {
                    curErr = e;
                    bestIdx = i;
                    flipToOne = true;
                }
            } else {
                ll newRes = residual + a[i];
                ull e = absvll(newRes);
                if (e < curErr) {
                    curErr = e;
                    bestIdx = i;
                    flipToOne = false;
                }
            }
        }
        if (bestIdx != -1) {
            bits.toggle(bestIdx);
            if (flipToOne) { sum += a[bestIdx]; residual -= a[bestIdx]; }
            else { sum -= a[bestIdx]; residual += a[bestIdx]; }
            improved = true;
        }
    }
}

static inline void try_pair_flip(const vector<ll>& a, ll T, Bits2& bits, ll& sum) {
    // Attempt one best pair flip: remove one used and add one unused to reduce |residual|
    int n = (int)a.size();
    ll residual = T - sum;
    ull bestErr = absvll(residual);
    int bestOff = -1, bestOn = -1;
    vector<pair<ll,int>> on, off;
    on.reserve(n); off.reserve(n);
    for (int i = 0; i < n; ++i) {
        if (bits.get(i)) off.emplace_back(a[i], i);
        else on.emplace_back(a[i], i);
    }
    if (on.empty() || off.empty()) return;
    sort(on.begin(), on.end());
    for (auto [vOff, iOff] : off) {
        ll target = vOff + residual;
        auto it = lower_bound(on.begin(), on.end(), make_pair(target, -1));
        if (it != on.end()) {
            ll vOn = it->first;
            ull e = absvll(residual + vOff - vOn);
            if (e < bestErr) { bestErr = e; bestOff = iOff; bestOn = it->second; }
        }
        if (it != on.begin()) {
            --it;
            ll vOn = it->first;
            ull e = absvll(residual + vOff - vOn);
            if (e < bestErr) { bestErr = e; bestOff = iOff; bestOn = it->second; }
        }
    }
    if (bestOff != -1 && bestOn != -1) {
        bits.toggle(bestOff); // turn off
        bits.toggle(bestOn);  // turn on
        sum += -a[bestOff] + a[bestOn];
    }
}

static inline State run_beam(const vector<int>& order, const vector<ll>& a, ll T, int K) {
    vector<State> cur;
    cur.reserve(K);
    State init;
    init.sum = 0; init.diff = absvll(T); init.bits = Bits2();
    cur.push_back(init);

    vector<State> nxt;
    nxt.reserve(2 * K);

    auto cmpDiff = [](const State& x, const State& y){ return x.diff < y.diff; };

    for (int idx : order) {
        nxt.clear();
        // copy "not take" states (diff unchanged)
        nxt.insert(nxt.end(), cur.begin(), cur.end());

        // generate "take" states
        ll val = a[idx];
        for (const State& s : cur) {
            State t = s;
            t.sum = s.sum + val;
            t.bits.set1(idx);
            t.diff = absvll(T - t.sum);
            nxt.push_back(t);
        }

        if ((int)nxt.size() > K) {
            nth_element(nxt.begin(), nxt.begin() + K, nxt.end(), cmpDiff);
            nxt.resize(K);
        }
        cur.swap(nxt);
    }

    // pick best
    State best = cur[0];
    for (const State& s : cur) if (s.diff < best.diff) best = s;
    return best;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    ll T;
    if (!(cin >> n >> T)) {
        return 0;
    }
    vector<ll> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];

    // Orders
    vector<int> order_id(n), order_desc(n);
    iota(order_id.begin(), order_id.end(), 0);
    iota(order_desc.begin(), order_desc.end(), 0);
    sort(order_desc.begin(), order_desc.end(), [&](int i, int j){
        if (a[i] != a[j]) return a[i] > a[j];
        return i < j;
    });

    Bits2 bestBits;
    ll bestSum = 0;
    ull bestErr = absvll(T);

    // Greedy original
    {
        Bits2 b; ll s;
        greedy_order(order_id, a, T, b, s);
        ull e = absvll(T - s);
        if (e < bestErr) { bestErr = e; bestBits = b; bestSum = s; }
    }
    // Greedy descending
    {
        Bits2 b; ll s;
        greedy_order(order_desc, a, T, b, s);
        ull e = absvll(T - s);
        if (e < bestErr) { bestErr = e; bestBits = b; bestSum = s; }
    }

    // Beam search with a dynamic K based on n
    int K;
    if (n <= 40) K = 140000;
    else if (n <= 60) K = 80000;
    else if (n <= 80) K = 50000;
    else K = 35000;

    // Beam original
    {
        State s = run_beam(order_id, a, T, K);
        if (s.diff < bestErr) { bestErr = s.diff; bestBits = s.bits; bestSum = s.sum; }
    }
    // Beam descending
    {
        State s = run_beam(order_desc, a, T, K);
        if (s.diff < bestErr) { bestErr = s.diff; bestBits = s.bits; bestSum = s.sum; }
    }

    // Local improvement
    local_improve_1flip(a, T, bestBits, bestSum);
    try_pair_flip(a, T, bestBits, bestSum);
    local_improve_1flip(a, T, bestBits, bestSum);

    cout << bitsToString(bestBits, n) << '\n';
    return 0;
}