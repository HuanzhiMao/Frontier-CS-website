#include <iostream>
#include <vector>
#include <string>
#include <numeric>

using namespace std;

// The modulo for all calculations
const long long MOD = 1e9 + 7;

// Function to perform a query and read the response
long long query(const vector<long long>& a) {
    cout << "?";
    for (long long x : a) {
        cout << " " << x;
    }
    cout << endl;
    long long response;
    cin >> response;
    return response;
}

// Function to output the final answer
void answer(const vector<int>& ops) {
    cout << "!";
    for (int op : ops) {
        cout << " " << op;
    }
    cout << endl;
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    vector<int> ops(n);
    
    // This solution uses N+1 queries. It is correct but might exceed the
    // query limit if N is at the maximum value (600).
    // The core idea is to determine operators sequentially. For each operator op_k,
    // we craft a query that gives a different result depending on whether op_k is '+' or '*'.

    // Step 1: A single baseline query to find the total number of addition operators.
    // Querying with all a_i = 1 results in 1 + (number of '+' ops).
    vector<long long> ones_query(n + 1, 1);
    long long r_ones = query(ones_query);
    long long n_plus_total = (r_ones - 1 + MOD) % MOD;

    // Step 2: N queries to determine each operator op_k from k=1 to n.
    long long n_plus_prefix = 0; // Number of '+' ops found so far (in op_1 to op_{k-1})
    for (int k = 1; k <= n; ++k) {
        // We construct a query to test op_k.
        // The prefix a_0, ..., a_{k-1} is designed to make the intermediate result res_{k-1}
        // a known value C_k that behaves well. The suffix a_{k+1}, ..., a_n are set to 1.
        // A suffix of 1s makes its contribution to the final result simple: it just adds
        // the number of '+' operators in the suffix.
        vector<long long> q_k(n + 1);
        
        // Let's set a_0=3 and a_1...a_{k-1}=1.
        // The intermediate result res_{k-1} will be 3 + (number of '+' in op_1..op_{k-1}).
        // Let this be C_k. Since n_plus_prefix >= 0, C_k is always >= 3. This avoids
        // tricky cases where C_k might be 1 or 2, which could lead to ambiguity.
        q_k[0] = 3;
        for (int i = 1; i < k; ++i) {
            q_k[i] = 1;
        }
        // We vary a_k to see its effect. Let's set a_k=2.
        q_k[k] = 2;
        for (int i = k + 1; i <= n; ++i) {
            q_k[i] = 1;
        }

        // Now, we calculate the expected result for both possibilities of op_k.
        
        // Case 1: op_k is '+'
        // The number of '+' ops in the suffix (op_{k+1}..op_n) is n_plus_total - n_plus_prefix - 1.
        // The final result would be (C_k + a_k) + (number of '+' in suffix)
        // = (3 + n_plus_prefix + 2) + (n_plus_total - n_plus_prefix - 1)
        // = 4 + n_plus_total
        long long exp_r_plus = (4 + n_plus_total) % MOD;

        // Case 2: op_k is '*'
        // The number of '+' ops in the suffix is n_plus_total - n_plus_prefix.
        // The final result would be (C_k * a_k) + (number of '+' in suffix)
        // = (3 + n_plus_prefix) * 2 + (n_plus_total - n_plus_prefix)
        // = 6 + 2*n_plus_prefix + n_plus_total - n_plus_prefix
        // = 6 + n_plus_prefix + n_plus_total
        long long exp_r_mul = (6 + n_plus_prefix + n_plus_total) % MOD;
        
        // These two expected results are guaranteed to be different since
        // (6 + n_plus_prefix + n_plus_total) - (4 + n_plus_total) = 2 + n_plus_prefix > 0.

        // Perform the query and compare the result.
        long long r_k = query(q_k);

        if (r_k == exp_r_plus) {
            ops[k - 1] = 0; // op_k is '+'
            n_plus_prefix++;
        } else {
            ops[k - 1] = 1; // op_k is '*'
        }
    }

    answer(ops);

    return 0;
}