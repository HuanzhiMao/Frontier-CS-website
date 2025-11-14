#include <bits/stdc++.h>
using namespace std;

using int64 = long long;
const int64 MOD = 1000000007;

int64 modnorm(int64 x){
    x %= MOD;
    if(x < 0) x += MOD;
    return x;
}
int64 modpow(int64 a, int64 e){
    int64 r = 1 % MOD;
    a %= MOD;
    while(e){
        if(e & 1) r = (r * a) % MOD;
        a = (a * a) % MOD;
        e >>= 1;
    }
    return r;
}
int64 modinv(int64 a){
    return modpow(a, MOD - 2);
}

int64 query(const vector<int64>& a){
    cout << "?";
    for(size_t i = 0; i < a.size(); ++i){
        cout << " " << a[i];
    }
    cout << endl;
    cout.flush();
    int64 r;
    if(!(cin >> r)){
        exit(0);
    }
    return r;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    if(!(cin >> n)){
        return 0;
    }
    vector<int> op(n, -1); // 0 for '+', 1 for 'x'
    
    // Step 1: Determine op1 and total number of pluses M_total with one query
    // Choose X1 = 2, v1 = n + 2 so delta = (X1-1)*(v1-1) = n+1 > n
    int64 X1 = 2;
    int64 v1 = n + 2;
    vector<int64> a(n + 1, 1);
    a[0] = X1;
    a[1] = v1;
    for(int i = 2; i <= n; ++i) a[i] = 1;
    int64 r1 = query(a);
    int64 delta = (X1 - 1) * (v1 - 1); // <= (n+1), definitely < MOD
    int64 s1 = modnorm(r1 - (X1 + v1) + 1); // s1 = M_total or M_total + delta
    int64 M_total;
    if(s1 <= n){
        op[0] = 0; // '+'
        M_total = s1;
    }else{
        op[0] = 1; // 'x'
        M_total = s1 - delta;
    }
    
    // Maintain count of pluses found so far
    int plus_count = (op[0] == 0 ? 1 : 0);
    
    // For i = 2..n, determine op_i with one query each
    for(int i = 2; i <= n; ++i){
        vector<int64> q(n + 1, 1);
        int64 v = 2; // choose v != 1
        // compute x_{i-2} when a_0..a_{i-2} are set to 1
        int p_prev = 0; // number of plus among first i-2
        for(int j = 0; j < i - 2; ++j) if(op[j] == 0) ++p_prev;
        int64 x_prev = 1 + p_prev; // x_{i-2}
        int64 X = x_prev + 1; // desired x_{i-1} != 1
        
        // set a_{j} for j < i-1 to 1 already
        // set a_{i-1} to achieve x_{i-1} = X
        if(i == 1){
            // not possible here because i >= 2 in this loop
        }else{
            if(op[i - 2] == 0){ // previous operator '+'
                // x_{i-1} = x_{i-2} + a_{i-1} => a_{i-1} = X - x_{i-2} = 1
                q[i - 1] = modnorm(X - x_prev);
                if(q[i - 1] == 0) q[i - 1] = 1; // but X - x_prev = 1 always
            }else{ // previous operator 'x'
                // x_{i-1} = x_{i-2} * a_{i-1} => a_{i-1} = X * inv(x_{i-2})
                int64 inv_prev = modinv(x_prev % MOD);
                q[i - 1] = (X % MOD) * inv_prev % MOD;
                if(q[i - 1] == 0) q[i - 1] = 1; // should not happen
            }
        }
        // set a_i = v
        q[i] = v;
        // j > i already set to 1
        
        int64 r = query(q);
        
        int p_left = plus_count; // plus among first i-1 ops
        // Predictions:
        // if op_i is 'x': r = X * v + (M_total - p_left)
        // if op_i is '+': r = X + v + (M_total - p_left - 1)
        int64 r_mult = modnorm((X % MOD) * (v % MOD) + (M_total - p_left));
        int64 r_plus = modnorm(X + v + (M_total - p_left - 1));
        
        if(r == r_mult){
            op[i - 1] = 1;
        }else{
            op[i - 1] = 0;
            ++plus_count;
        }
    }
    
    cout << "!";
    for(int i = 0; i < n; ++i){
        cout << " " << (op[i] == 1 ? 1 : 0);
    }
    cout << endl;
    cout.flush();
    return 0;
}