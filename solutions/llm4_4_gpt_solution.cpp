#include <bits/stdc++.h>
using namespace std;

struct FastScanner {
    static const int BUFSIZE = 1 << 20;
    int idx, size;
    char buf[BUFSIZE];
    FastScanner(): idx(0), size(0) {}
    inline char getch() {
        if (idx >= size) {
            size = (int)fread(buf, 1, BUFSIZE, stdin);
            idx = 0;
            if (size == 0) return EOF;
        }
        return buf[idx++];
    }
    template<typename T>
    bool readInt(T &out) {
        char c = getch();
        if (c == EOF) return false;
        while (c != '-' && (c < '0' || c > '9')) {
            c = getch();
            if (c == EOF) return false;
        }
        T sign = 1;
        if (c == '-') {
            sign = -1;
            c = getch();
        }
        T val = 0;
        for (; c >= '0' && c <= '9'; c = getch()) {
            val = val * 10 + (c - '0');
        }
        out = val * sign;
        return true;
    }
} fs;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n, k;
    if (!fs.readInt(n)) return 0;
    if (!fs.readInt(k)) return 0;

    long long total = n * n;
    vector<long long> arr;
    arr.reserve((size_t)total);

    for (long long i = 0; i < total; ++i) {
        long long x;
        fs.readInt(x);
        arr.push_back(x);
    }

    size_t idx = (size_t)(k - 1);
    nth_element(arr.begin(), arr.begin() + idx, arr.end());
    long long ans = arr[idx];

    cout << ans << '\n';
    return 0;
}