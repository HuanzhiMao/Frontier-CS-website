#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>

using namespace std;

// Function to ask a query of type 1
int ask(int l, int r) {
    cout << "1 " << l << " " << r << endl;
    int result;
    cin >> result;
    return result;
}

// Function to perform a swap of type 2
void do_swap(int i, int j) {
    if (i == j) return;
    cout << "2 " << i << " " << j << endl;
    int confirmation;
    cin >> confirmation;
}

// Function to report the answer
void report(const vector<int>& p) {
    cout << "3";
    for (size_t i = 1; i < p.size(); ++i) {
        cout << " " << p[i];
    }
    cout << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, l1, l2;
    cin >> n >> l1 >> l2;

    if (n == 1) {
        report({0, 1});
        return 0;
    }

    vector<long long> A(n + 1, 0);
    vector<long long> B(n + 2, 0);

    A[1] = 1;
    for (int i = 2; i <= n; ++i) {
        A[i] = ask(1, i);
    }

    B[n] = 1;
    for (int i = n - 1; i >= 1; --i) {
        B[i] = ask(i, n);
    }
    
    int k = -1;
    long long min_d_val = -1;

    long long d1 = B[1] - B[2];
    k = 1;
    min_d_val = d1;

    for (int i = 2; i < n; ++i) {
        long long di = (A[i] - A[i - 1]) + (B[i] - B[i + 1]) - 1;
        if (di < min_d_val) {
            min_d_val = di;
            k = i;
        }
    }

    long long dn = A[n] - A[n - 1];
    if (dn < min_d_val) {
        min_d_val = dn;
        k = n;
    }

    do_swap(1, k);
    
    vector<int> p_ans(n + 1);
    vector<bool> used(n + 1, false);

    p_ans[1] = 1;
    used[1] = true;

    vector<long long> count1i(n + 1);
    count1i[1] = 1;
    if (n > 1) {
        count1i[2] = ask(1, 2);
    }
    for (int i = 3; i <= n; ++i) {
        count1i[i] = ask(1, i);
    }

    for (int i = 2; i <= n; ++i) {
        long long E_i = count1i[i] - count1i[i - 1];
        
        vector<int> votes(n + 1, 0);
        int min_p = p_ans[i - 1];
        int max_p = p_ans[i - 1];

        for (int j = i - 1; j >= 1; --j) {
            min_p = min(min_p, p_ans[j]);
            max_p = max(max_p, p_ans[j]);
            
            int len = i - j;
            
            int v1 = max_p - len;
            if (v1 >= 1 && v1 < min_p) {
                votes[v1]++;
            }
            int v2 = min_p + len;
            if (v2 <= n && v2 > max_p) {
                votes[v2]++;
            }
        }
        
        for (int v = 1; v <= n; ++v) {
            if (!used[v]) {
                if (votes[v] + 1 == E_i) {
                    p_ans[i] = v;
                    used[v] = true;
                    break;
                }
            }
        }
    }
    
    if (n > 1) {
        bool is_neighbor_real = (count1i[2] == 3);
        bool is_neighbor_ans = (abs(p_ans[1] - p_ans[2]) == 1);
        if (is_neighbor_real != is_neighbor_ans) {
            for (int i = 1; i <= n; ++i) {
                p_ans[i] = n + 1 - p_ans[i];
            }
        }
    }
    
    if (k != 1) {
        swap(p_ans[1], p_ans[k]);
    }

    report(p_ans);

    return 0;
}