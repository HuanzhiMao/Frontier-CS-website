#include <bits/stdc++.h>
using namespace std;

static inline int getCharFast() {
#ifdef _WIN32
    return getchar();
#else
    return getchar_unlocked();
#endif
}

template<typename T>
bool readInt(T &out) {
    int c = getCharFast();
    if (c == EOF) return false;
    while (c != '-' && (c < '0' || c > '9')) {
        c = getCharFast();
        if (c == EOF) return false;
    }
    bool neg = false;
    if (c == '-') {
        neg = true;
        c = getCharFast();
    }
    T val = 0;
    while (c >= '0' && c <= '9') {
        val = val * 10 + (c - '0');
        c = getCharFast();
    }
    out = neg ? -val : val;
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n, k;
    if (!readInt(n)) return 0;
    if (!readInt(k)) return 0;

    long long N = n * n;
    vector<long long> arr;
    arr.reserve((size_t)N);
    long long x;
    for (long long i = 0; i < N; ++i) {
        if (!readInt(x)) {
            // If not enough numbers, exit gracefully
            return 0;
        }
        arr.push_back(x);
    }

    if (k < 1) k = 1;
    if (k > N) k = N;

    nth_element(arr.begin(), arr.begin() + (k - 1), arr.end());
    long long ans = arr[k - 1];
    cout << ans << "\n";
    return 0;
}