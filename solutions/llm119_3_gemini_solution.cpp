#include <iostream>
#include <vector>
#include <string>
#include <numeric>

using namespace std;

long long M = 1e9 + 7;

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

long long ask_query(const vector<long long>& a) {
    cout << "?";
    for (long long x : a) {
        cout << " " << x;
    }
    cout << endl;
    long long result;
    cin >> result;
    return result;
}

void print_answer(const vector<int>& ops) {
    cout << "!";
    for (int op : ops) {
        cout << " " << op;
    }
    cout << endl;
}

long long calculate_full_expr(const vector<long long>& a, const vector<int>& ops) {
    long long current_val = a[0];
    for (size_t i = 0; i < ops.size(); ++i) {
        if (ops[i] == 0) { // +
            current_val = (current_val + a[i + 1]) % M;
        } else { // *
            current_val = (current_val * a[i + 1]) % M;
        }
    }
    return current_val;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    vector<long long> res(n + 1);
    for (int j = 1; j <= n; ++j) {
        vector<long long> q(n + 1, 1);
        q[0] = 2;
        q[j] = 2;
        res[j] = ask_query(q);
    }

    vector<int> final_ops(n);
    bool found = false;

    // Case 1: op_1 is '+' (x_1 = 0)
    {
        vector<int> ops_candidate(n);
        ops_candidate[0] = 0;
        long long s = 0; // s will be sum of x_i up to i=j-1
        for (int j = 2; j <= n; ++j) {
            long long diff = (res[j] - res[1] + M) % M;
            // diff = x_j(j - s_{j-1}) - x_1
            // with x_1=0: diff = x_j(j - s)
            long long j_minus_s = j - s;
            long long inv = modInverse(j_minus_s);
            long long x_j = (diff * inv) % M;

            ops_candidate[j - 1] = x_j;
            s += x_j;
        }
        
        vector<long long> q1(n + 1, 1);
        q1[0] = 2;
        q1[1] = 2;
        if (calculate_full_expr(q1, ops_candidate) == res[1]) {
            final_ops = ops_candidate;
            found = true;
        }
    }

    // Case 2: op_1 is '*' (x_1 = 1)
    if (!found) {
        vector<int> ops_candidate(n);
        ops_candidate[0] = 1;
        long long s = 1;
        for (int j = 2; j <= n; ++j) {
            long long diff = (res[j] - res[1] + M) % M;
            // diff = x_j(j - s_{j-1}) - x_1
            // with x_1=1: diff = x_j(j - s) - 1
            long long to_solve = (diff + 1 + M) % M;
            long long j_minus_s = j - s;
            
            long long inv = modInverse(j_minus_s);
            long long x_j = (to_solve * inv) % M;

            ops_candidate[j-1] = x_j;
            s += x_j;
        }
        final_ops = ops_candidate;
    }

    print_answer(final_ops);

    return 0;
}