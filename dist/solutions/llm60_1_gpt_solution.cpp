#include <bits/stdc++.h>
using namespace std;

bool isIntegerString(const string& s) {
    if (s.empty()) return false;
    size_t i = 0;
    if (s[0] == '+' || s[0] == '-') i = 1;
    if (i == s.size()) return false;
    for (; i < s.size(); ++i) {
        if (!isdigit(static_cast<unsigned char>(s[i]))) return false;
    }
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    vector<long long> nums;
    string tok;
    while (cin >> tok) {
        if (isIntegerString(tok)) {
            nums.push_back(stoll(tok));
        }
    }
    if (nums.empty()) return 0;

    size_t offset = 0;
    if (nums.size() % 3 != 0) {
        if (nums.size() >= 1) {
            long long T = nums[0];
            if (T >= 0 && static_cast<size_t>(1 + 3 * T) == nums.size()) {
                offset = 1;
            } else {
                // Fallback: cannot interpret input format
                return 0;
            }
        } else {
            return 0;
        }
    }

    for (size_t i = offset; i + 2 < nums.size(); i += 3) {
        long long x = nums[i], y = nums[i+1], r = nums[i+2];
        cout << "answer " << x << ' ' << y << ' ' << r << '\n';
    }
    return 0;
}