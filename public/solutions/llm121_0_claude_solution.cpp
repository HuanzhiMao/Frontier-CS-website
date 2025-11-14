#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m;
    cin >> n >> m;
    
    vector<string> s(m);
    for (int i = 0; i < m; i++) {
        cin >> s[i];
    }
    
    int total = 1 << (2 * n); // 4^n
    int valid = 0;
    
    // Try all possible DNA sequences
    for (int mask = 0; mask < total; mask++) {
        string dna = "";
        int temp = mask;
        for (int j = 0; j < n; j++) {
            int ch = temp & 3;
            if (ch == 0) dna += 'A';
            else if (ch == 1) dna += 'C';
            else if (ch == 2) dna += 'G';
            else dna += 'T';
            temp >>= 2;
        }
        
        // Check if this DNA sequence is valid
        bool isValid = false;
        for (int i = 0; i < m && !isValid; i++) {
            bool canMatch = true;
            for (int j = 0; j < n; j++) {
                if (s[i][j] != '?' && s[i][j] != dna[j]) {
                    canMatch = false;
                    break;
                }
            }
            if (canMatch) {
                isValid = true;
            }
        }
        
        if (isValid) {
            valid++;
        }
    }
    
    double probability = (double)valid / total;
    cout << fixed << setprecision(10) << probability << endl;
    
    return 0;
}