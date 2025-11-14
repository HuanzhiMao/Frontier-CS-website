#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    if (!(cin >> n)) return 0;
    long long B = n + 1; // ensure B > max A_i
    
    // Output n distinct magic words: w_i = X^i O^B
    for (int i = 1; i <= n; ++i) {
        string s(i, 'X');
        s.append((size_t)B, 'O');
        cout << s << '\n';
    }
    cout.flush();
    
    int q;
    if (!(cin >> q)) return 0;
    long long K = B * (B + 1);
    
    while (q--) {
        long long p;
        cin >> p;
        long long qprod = p / K;           // (A+1)*C
        long long r = p % K;               // B + (B+1)*max(A,C)
        long long M = (r - B) / (B + 1);   // max(A, C)
        
        long long A = -1, C = -1;
        if (qprod % (M + 1) == 0) {
            // Case A = M >= C
            A = M;
            C = qprod / (M + 1);
        } else {
            // Case C = M >= A
            C = M;
            A = qprod / C - 1;
        }
        
        // Map A, C back to indices (since A_i = i)
        cout << A << ' ' << C << '\n';
        cout.flush();
    }
    return 0;
}