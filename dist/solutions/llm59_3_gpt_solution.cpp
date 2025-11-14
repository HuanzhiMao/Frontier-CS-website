#include <bits/stdc++.h>
using namespace std;

static bool isIntegerToken(const string &s) {
    if (s.empty()) return false;
    size_t i = 0;
    if (s[0] == '+' || s[0] == '-') i = 1;
    if (i >= s.size()) return false;
    for (; i < s.size(); ++i) {
        if (!isdigit((unsigned char)s[i])) return false;
    }
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    vector<long long> nums;
    string tok;
    while (cin >> tok) {
        if (isIntegerToken(tok)) {
            try {
                long long val = stoll(tok);
                nums.push_back(val);
            } catch (...) {
                // ignore parsing errors
            }
        }
    }
    vector<vector<long long>> outputs;
    size_t p = 0;
    while (p < nums.size()) {
        long long nll = nums[p++];
        if (nll <= 0) {
            // invalid n, skip
            continue;
        }
        int n = (int)nll;
        size_t left = (p <= nums.size() ? nums.size() - p : 0);
        if (left >= (size_t)n) {
            // Try interpret next n numbers
            vector<long long> next(n);
            for (int i = 0; i < n; ++i) next[i] = nums[p + i];
            // Check if they are j_i in [i, min(n, i+2)]
            bool asJ = true;
            for (int i = 1; i <= n; ++i) {
                long long x = next[i - 1];
                long long hi = min<long long>(n, i + 2);
                if (x < i || x > hi) { asJ = false; break; }
            }
            if (asJ) {
                vector<long long> arr(n);
                for (int i = 0; i < n; ++i) arr[i] = i + 1;
                for (int i = 0; i < n; ++i) {
                    int j = (int)next[i] - 1;
                    if (j >= i && j < n) swap(arr[i], arr[j]);
                }
                outputs.push_back(arr);
                p += n;
                continue;
            }
            // Else check if it's a permutation of 1..n
            vector<char> seen(n + 1, 0);
            bool isPerm = true;
            for (int i = 0; i < n; ++i) {
                long long x = next[i];
                if (x < 1 || x > n || seen[(int)x]) { isPerm = false; break; }
                seen[(int)x] = 1;
            }
            if (isPerm) {
                outputs.push_back(next);
                p += n;
                continue;
            }
            // Fallback: identity
            vector<long long> id(n);
            iota(id.begin(), id.end(), 1);
            outputs.push_back(id);
        } else {
            // Not enough numbers; fallback identity
            vector<long long> id(n);
            iota(id.begin(), id.end(), 1);
            outputs.push_back(id);
            break;
        }
    }
    // If no inputs at all, do nothing.
    for (size_t k = 0; k < outputs.size(); ++k) {
        for (size_t i = 0; i < outputs[k].size(); ++i) {
            if (i) cout << ' ';
            cout << outputs[k][i];
        }
        if (k + 1 < outputs.size()) cout << '\n';
    }
    return 0;
}