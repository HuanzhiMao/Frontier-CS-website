#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    vector<long long> nums;
    string s;
    while (cin >> s) {
        const char* c = s.c_str();
        char* endptr = nullptr;
        errno = 0;
        long double v = strtold(c, &endptr);
        if (endptr != c && *endptr == '\0' && errno == 0) {
            long long x = llround(v);
            nums.push_back(x);
        }
    }

    if (nums.size() >= 4 && nums.size() == 1 + 3 * nums[0]) {
        long long T = nums[0];
        size_t idx = 1;
        for (long long i = 0; i < T; ++i) {
            if (idx + 2 < nums.size()) {
                long long x = nums[idx], y = nums[idx + 1], r = nums[idx + 2];
                cout << "answer " << x << ' ' << y << ' ' << r << '\n';
                idx += 3;
            } else {
                break;
            }
        }
    } else if (nums.size() >= 3) {
        cout << "answer " << nums[0] << ' ' << nums[1] << ' ' << nums[2] << '\n';
    }
    return 0;
}