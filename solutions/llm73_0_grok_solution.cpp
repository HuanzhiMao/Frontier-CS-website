#include <bits/stdc++.h>
using namespace std;

class Fenwick {
public:
    vector<int> tree;
    int n;
    Fenwick(int _n) : n(_n), tree(_n + 2, 0) {}
    void update(int idx, int val) {
        while (idx <= n) {
            tree[idx] ^= val;
            idx += idx & -idx;
        }
    }
    int query(int idx) {
        int res = 0;
        while (idx > 0) {
            res ^= tree[idx];
            idx -= idx & -idx;
        }
        return res;
    }
    int query(int l, int r) {
        if (l > r) return 0;
        return query(r) ^ query(l - 1);
    }
};

int main() {
    int n;
    cin >> n;
    vector<int> sorted_pos;
    for (int k = 1; k <= n; ++k) {
        if (k == 1) {
            sorted_pos.push_back(1);
            continue;
        }
        int m = k - 1;
        vector<int> temp_p(m + 1, 0);
        for (int i = 0; i < m; ++i) {
            temp_p[sorted_pos[i]] = i + 1;
        }
        Fenwick ft(m);
        vector<int> parity_count(m + 1, 0);
        for (int i = m; i >= 1; --i) {
            int val = temp_p[i];
            parity_count[i] = ft.query(1, val - 1);
            ft.update(val, 1);
        }
        vector<int> parity_inv(m + 2, 0);
        parity_inv[m + 1] = 0;
        for (int l = m; l >= 1; --l) {
            parity_inv[l] = parity_inv[l + 1] ^ parity_count[l];
        }
        int low = 0, high = m;
        while (low < high) {
            int mid = (low + high) / 2;
            int j = sorted_pos[mid];
            cout << 0 << " " << j << " " << k << endl;
            int ans1;
            cin >> ans1;
            int l2 = j + 1;
            cout << 0 << " " << l2 << " " << k << endl;
            int ans2;
            cin >> ans2;
            int bit = (ans1 ^ parity_inv[j]) ^ (ans2 ^ parity_inv[l2]);
            if (bit == 0) {
                low = mid + 1;
            } else {
                high = mid;
            }
        }
        int r = low;
        sorted_pos.insert(sorted_pos.begin() + r, k);
    }
    vector<int> p(n + 1);
    for (int i = 0; i < n; ++i) {
        p[sorted_pos[i]] = i + 1;
    }
    cout << 1;
    for (int i = 1; i <= n; ++i) {
        cout << " " << p[i];
    }
    cout << endl;
    return 0;
}