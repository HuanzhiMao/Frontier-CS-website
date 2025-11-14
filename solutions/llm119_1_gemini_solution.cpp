#include <iostream>
#include <vector>
#include <string>
#include <numeric>

using namespace std;

// The modulo for all calculations.
long long M = 1e9 + 7;

/**
 * @brief Sends a query to the interactor.
 * @param a The sequence of n+1 integers for the query.
 * @return The result from the interactor.
 */
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

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    // If there are no operators, we are done.
    if (n == 0) {
        cout << "!" << endl;
        return 0;
    }

    // We will make n queries to determine the n operators.
    // Query i (0-indexed) will have a_0, ..., a_i set to 2, and a_{i+1}, ..., a_n set to 1.
    // Let R[i] be the result of query i.
    vector<long long> R(n);
    for (int i = 0; i < n; ++i) {
        vector<long long> a(n + 1);
        for (int j = 0; j <= i; ++j) {
            a[j] = 2;
        }
        for (int j = i + 1; j <= n; ++j) {
            a[j] = 1;
        }
        R[i] = query(a);
    }

    vector<int> ops(n + 1);
    
    // C_prev will store the intermediate result E_{i-1} = (((...(a_0 op_1 a_1)...) op_{i-1} a_{i-1}))
    // where all a_j (for j < i) are 2. We start with C_0 = a_0 = 2.
    long long C_prev = 2; 
    long long R_prev = R[0];

    // Determine operators op_1 to op_{n-1}.
    for (int i = 1; i < n; ++i) {
        long long R_curr = R[i];
        
        // Let's analyze the difference between R_curr (from query i) and R_prev (from query i-1).
        // R_{i-1} = C_{i-1} + (number of '+' in op_i, ..., op_n)
        // R_i     = C_i     + (number of '+' in op_{i+1}, ..., op_n)
        // R_i - R_{i-1} = (C_i - C_{i-1}) - (1 if op_i is '+' else 0)
        // C_i = C_{i-1} op_i 2.
        // If op_i is '+': diff = (C_{i-1}+2 - C_{i-1}) - 1 = 1.
        // If op_i is 'x': diff = (C_{i-1}*2 - C_{i-1}) - 0 = C_{i-1}.
        // Since C_k >= 2 for all k, C_{i-1} can't be 1. This allows distinguishing the cases.
        long long diff = (R_curr - R_prev + M) % M;
        
        long long C_curr;
        if (diff == 1) {
            ops[i] = 0; // '+'
            C_curr = (C_prev + 2) % M;
        } else {
            ops[i] = 1; // '*'
            C_curr = (C_prev * 2) % M;
        }
        C_prev = C_curr;
        R_prev = R_curr;
    }

    // Determine the last operator, op_n.
    // The last query result we have is R[n-1], from query n-1 (a_0...a_{n-1} = 2, a_n = 1).
    // The expression is E_{n-1} op_n a_n, which is C_{n-1} op_n 1.
    // C_{n-1} is the final value of C_prev.
    // If op_n is '+': R[n-1] = C_{n-1} + 1.
    // If op_n is 'x': R[n-1] = C_{n-1} * 1 = C_{n-1}.
    if (R[n - 1] == C_prev) {
        ops[n] = 1; // '*'
    } else {
        ops[n] = 0; // '+'
    }

    // Output the final answer.
    cout << "!";
    for (int i = 1; i <= n; ++i) {
        cout << " " << ops[i];
    }
    cout << endl;

    return 0;
}