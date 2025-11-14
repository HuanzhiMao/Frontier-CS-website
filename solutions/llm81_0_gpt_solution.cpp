#include <bits/stdc++.h>
using namespace std;

// Big integer modulo 2^N operations using vector<uint64_t>
struct BigMod2Pow {
    int Nbits;
    int W; // number of 64-bit words
    vector<uint64_t> v;
    uint64_t lastMask; // mask for the highest word to keep only Nbits

    BigMod2Pow() {}
    BigMod2Pow(int Nbits_): Nbits(Nbits_) {
        W = (Nbits + 63) / 64;
        v.assign(W, 0);
        int r = Nbits % 64;
        if (r == 0) lastMask = ~0ULL;
        else lastMask = (r == 64 ? ~0ULL : ((1ULL << r) - 1));
    }

    void zero() {
        std::fill(v.begin(), v.end(), 0ULL);
    }

    void setOne() {
        zero();
        if (W > 0) v[0] = 1ULL;
    }

    void maskTop() {
        if (W > 0) v[W - 1] &= lastMask;
    }

    // this += a * k (k is small <= 1002)
    void addMulSmall(const BigMod2Pow& a, uint64_t k) {
        if (k == 0) return;
        __uint128_t carry = 0;
        for (int i = 0; i < W; ++i) {
            __uint128_t prod = ( __uint128_t ) a.v[i] * k;
            __uint128_t sum = ( __uint128_t ) v[i] + prod + carry;
            v[i] = (uint64_t) sum;
            carry = sum >> 64;
        }
        maskTop();
    }

    // this *= k (k small)
    void mulSmall(uint64_t k) {
        if (k == 0) {
            zero();
            return;
        }
        __uint128_t carry = 0;
        for (int i = 0; i < W; ++i) {
            __uint128_t prod = ( __uint128_t ) v[i] * k + carry;
            v[i] = (uint64_t) prod;
            carry = prod >> 64;
        }
        maskTop();
    }

    // Get bit at position pos (0 = LSB)
    int getBit(int pos) const {
        int idx = pos >> 6;
        int off = pos & 63;
        if (idx >= W) return 0;
        return (int)((v[idx] >> off) & 1ULL);
    }
};

// Extended GCD for modular inverse
long long extgcd(long long a, long long b, long long &x, long long &y) {
    if (b == 0) { x = 1; y = 0; return a; }
    long long x1, y1;
    long long g = extgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

long long modinv(long long a, long long mod) {
    long long x, y;
    long long g = extgcd(a, mod, x, y);
    // assume coprime
    x %= mod;
    if (x < 0) x += mod;
    return x;
}

// Sieve primes up to n
vector<int> sieve_primes(int n) {
    vector<int> primes;
    vector<bool> is_prime(n + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i <= n; ++i) {
        if (is_prime[i]) {
            primes.push_back(i);
            if ((long long)i * i <= n) {
                for (int j = i * i; j <= n; j += i) is_prime[j] = false;
            }
        }
    }
    return primes;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    if (!(cin >> N)) {
        return 0;
    }

    // Choose odd primes <= 1002, sorted descending, accumulate until sum log2 >= N
    vector<int> primes = sieve_primes(1002);
    vector<int> odd_primes;
    for (int p : primes) if (p != 2) odd_primes.push_back(p);
    sort(odd_primes.begin(), odd_primes.end(), greater<int>());

    double needBits = N + 5; // a bit margin
    double accBits = 0.0;
    vector<int> mods;
    for (int p : odd_primes) {
        if (accBits >= needBits) break;
        mods.push_back(p);
        accBits += log2((double)p);
    }
    // In case not enough (shouldn't happen), add more smaller primes if any left
    for (int p : odd_primes) {
        if (accBits >= needBits) break;
        bool used = false;
        for (int q : mods) if (q == p) { used = true; break; }
        if (!used) { mods.push_back(p); accBits += log2((double)p); }
    }

    int K = (int)mods.size();
    vector<int> residues(K);

    // Perform queries
    for (int qi = 0; qi < K; ++qi) {
        int m = mods[qi];
        vector<int> a(m), b(m);
        for (int x = 0; x < m; ++x) {
            a[x] = (int)((2LL * x) % m);
            b[x] = a[x] + 1;
            if (b[x] >= m) b[x] -= m;
        }

        cout << 1 << "\n";
        cout << m << "\n";
        for (int i = 0; i < m; ++i) {
            if (i) cout << ' ';
            cout << a[i];
        }
        cout << "\n";
        for (int i = 0; i < m; ++i) {
            if (i) cout << ' ';
            cout << b[i];
        }
        cout << "\n";
        cout.flush();

        int x;
        if (!(cin >> x)) return 0; // interactor failure
        residues[qi] = x % m;
        if (residues[qi] < 0) residues[qi] += m;
    }

    // Garner's algorithm to reconstruct s modulo 2^N
    // Precompute inverses inv[i][j] = inverse(mods[i] mod mods[j]) in mods[j]
    vector<vector<long long>> inv(K, vector<long long>(K, 0));
    for (int j = 0; j < K; ++j) {
        for (int i = 0; i < j; ++i) {
            long long mi = mods[i];
            long long mj = mods[j];
            long long val = mi % mj;
            long long inv_ij = modinv(val, mj);
            inv[i][j] = inv_ij;
        }
    }

    vector<long long> coeff(K, 0);
    for (int k = 0; k < K; ++k) {
        long long t = residues[k] % mods[k];
        if (t < 0) t += mods[k];
        for (int i = 0; i < k; ++i) {
            long long diff = (t - coeff[i]) % mods[k];
            if (diff < 0) diff += mods[k];
            t = ( (__int128)diff * inv[i][k] ) % mods[k];
        }
        coeff[k] = t;
    }

    BigMod2Pow ans(N), mul(N);
    ans.zero();
    mul.setOne();

    for (int k = 0; k < K; ++k) {
        if (coeff[k]) ans.addMulSmall(mul, (uint64_t)coeff[k]);
        mul.mulSmall((uint64_t)mods[k]);
    }

    // Extract N bits to string S (MSB first)
    string S;
    S.resize(N);
    for (int i = 0; i < N; ++i) {
        int bit = ans.getBit(N - 1 - i);
        S[i] = char('0' + bit);
    }

    // Output guess
    cout << 0 << "\n";
    cout << S << "\n";
    cout.flush();

    return 0;
}