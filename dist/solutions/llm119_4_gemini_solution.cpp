#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <map>
#include <algorithm>

using namespace std;

long long M = 1000000007;

long long power(long long base, long long exp) {
    long long res = 1;
    base %= M;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % M;
        base = (base * base) % M;
        exp /= 2;
    }
    return res;
}

long long modInverse(long long n) {
    return power(n, M - 2);
}

void make_query(int n, const vector<long long>& a) {
    cout << "?";
    for (int i = 0; i <= n; ++i) {
        cout << " " << a[i];
    }
    cout << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    vector<int> ans(n);

    int B = 32;
    if (n < B) B = n;
    
    int num_chunks = (n + B - 1) / B;

    for (int k = 0; k < num_chunks; ++k) {
        int start_op_idx = k * B;
        int end_op_idx = min((k + 1) * B, n);
        int len = end_op_idx - start_op_idx;
        if (len == 0) continue;

        int left_len = len / 2;
        int right_len = len - left_len;

        long long V = 2;
        int plus_count = 0;
        for (int i = 0; i < start_op_idx; ++i) {
            if (ans[i] == 0) { // +
                plus_count++;
            }
        }
        V = (V + plus_count) % M;

        vector<long long> q1(n + 1, 1), q2(n + 1, 1);
        q1[0] = 2;
        q2[0] = 2;

        for (int i = 0; i < len; ++i) {
            q1[start_op_idx + i + 1] = i + 2;
            q2[start_op_idx + i + 1] = i + 2;
        }
        q2[start_op_idx + 1] = len + 2;
        
        make_query(n, q1);
        long long res1;
        cin >> res1;

        make_query(n, q2);
        long long res2;
        cin >> res2;

        long long diff = (res2 - res1 + M) % M;
        
        map<long long, int> p_diffs;
        for (int i = 0; i < (1 << left_len); ++i) {
            long long p1_val = V;
            long long p2_val = V;
            for (int j = 0; j < left_len; ++j) {
                if ((i >> j) & 1) { // *
                    p1_val = (p1_val * q1[start_op_idx + j + 1]) % M;
                    p2_val = (p2_val * q2[start_op_idx + j + 1]) % M;
                } else { // +
                    p1_val = (p1_val + q1[start_op_idx + j + 1]) % M;
                    p2_val = (p2_val + q2[start_op_idx + j + 1]) % M;
                }
            }
            p_diffs[(p2_val - p1_val + M) % M] = i;
        }

        int right_start_op_idx = start_op_idx + left_len;
        for (int i = 0; i < (1 << right_len); ++i) {
            long long A = 1;
            for (int j = 0; j < right_len; ++j) {
                if ((i >> j) & 1) { // *
                    A = (A * q1[right_start_op_idx + j + 1]) % M;
                }
            }

            long long A_inv = modInverse(A);
            long long target_p_diff = (diff * A_inv) % M;

            if (p_diffs.count(target_p_diff)) {
                int left_mask = p_diffs[target_p_diff];
                for (int j = 0; j < left_len; ++j) {
                    ans[start_op_idx + j] = (left_mask >> j) & 1;
                }
                for (int j = 0; j < right_len; ++j) {
                    ans[right_start_op_idx + j] = (i >> j) & 1;
                }
                goto next_chunk;
            }
        }
        next_chunk:;
    }

    cout << "!";
    for (int i = 0; i < n; ++i) {
        cout << " " << ans[i];
    }
    cout << endl;

    return 0;
}