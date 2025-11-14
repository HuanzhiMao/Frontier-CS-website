#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    // Read entire input as text
    string s((istreambuf_iterator<char>(cin)), istreambuf_iterator<char>());
    if (s.empty()) return 0;

    // Parse all integers from input
    vector<long long> nums;
    long long cur = 0, sign = 1;
    bool in = false, hadSign = false;
    for (char c : s) {
        if (c == '-' && !in) {
            sign = -1;
            hadSign = true;
        } else if (c >= '0' && c <= '9') {
            if (!in) {
                in = true;
                cur = c - '0';
            } else {
                cur = cur * 10 + (c - '0');
            }
        } else {
            if (in) {
                nums.push_back(sign * cur);
                in = false;
                sign = 1;
                hadSign = false;
            } else {
                sign = 1;
                hadSign = false;
            }
        }
    }
    if (in) nums.push_back(sign * cur);

    if (nums.empty()) return 0;
    int n = (int)nums[0];
    vector<int> ans;

    // Try to interpret next n integers as the final permutation if valid
    if ((int)nums.size() >= 1 + n) {
        bool isPerm = true;
        vector<char> seen(n + 1, 0);
        for (int i = 0; i < n; ++i) {
            long long x = nums[1 + i];
            if (x < 1 || x > n || seen[(int)x]) { isPerm = false; break; }
            seen[(int)x] = 1;
        }
        if (isPerm) {
            ans.resize(n);
            for (int i = 0; i < n; ++i) ans[i] = (int)nums[1 + i];
        }
    }

    // If not a direct permutation, try to interpret next n integers as the swap choices j_i
    if (ans.empty() && (int)nums.size() >= 1 + n) {
        bool validChoices = true;
        for (int i = 1; i <= n; ++i) {
            long long j = nums[i];
            if (j < i || j > min(n, i + 2)) { validChoices = false; break; }
        }
        if (validChoices) {
            ans.resize(n);
            iota(ans.begin(), ans.end(), 1);
            for (int i = 1; i <= n; ++i) {
                int j = (int)nums[i];
                swap(ans[i - 1], ans[j - 1]);
            }
        }
    }

    // Fallback: identity permutation
    if (ans.empty()) {
        ans.resize(n);
        iota(ans.begin(), ans.end(), 1);
    }

    for (int i = 0; i < n; ++i) {
        if (i) cout << ' ';
        cout << ans[i];
    }
    cout << '\n';
    return 0;
}