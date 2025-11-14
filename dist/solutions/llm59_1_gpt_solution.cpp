#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    // Read entire input as string
    string input((istreambuf_iterator<char>(cin)), istreambuf_iterator<char>());
    vector<long long> nums;
    long long val = 0;
    bool inNum = false;

    for (char c : input) {
        if (c >= '0' && c <= '9') {
            val = val * 10 + (c - '0');
            inNum = true;
        } else {
            if (inNum) {
                nums.push_back(val);
                val = 0;
                inNum = false;
            }
        }
    }
    if (inNum) nums.push_back(val);

    if (nums.empty()) return 0;

    size_t idx = 0;
    bool firstOutput = true;
    while (idx < nums.size()) {
        int n = (int)nums[idx++];
        vector<int> ans;
        if (idx + n <= nums.size()) {
            ans.reserve(n);
            for (int i = 0; i < n; ++i) ans.push_back((int)nums[idx++]);
        } else {
            ans.reserve(n);
            for (int i = 1; i <= n; ++i) ans.push_back(i);
        }
        for (int i = 0; i < n; ++i) {
            if (i) cout << ' ';
            cout << ans[i];
        }
        if (idx < nums.size()) cout << '\n';
    }
    return 0;
}