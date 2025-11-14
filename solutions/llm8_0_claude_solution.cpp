#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    long long k;
    cin >> k;
    
    vector<string> instructions;
    
    // Convert k to binary representation
    string binary = "";
    long long temp = k;
    while (temp > 0) {
        binary = (char)('0' + (temp % 2)) + binary;
        temp /= 2;
    }
    
    int n = binary.length() * 2 + 1;
    
    cout << n << "\n";
    
    // First instruction - always a POP instruction that starts the process
    cout << "POP 1 GOTO " << (n) << " PUSH 1 GOTO 2" << "\n";
    
    // Generate instructions based on binary representation
    for (int i = 0; i < binary.length(); i++) {
        if (binary[i] == '1') {
            // For bit 1: POP instruction that doubles and adds 1
            cout << "POP 1 GOTO " << (i + 3) << " PUSH 1 GOTO " << (i + 3) << "\n";
        } else {
            // For bit 0: POP instruction that doubles
            cout << "POP 1 GOTO " << (i + 3) << " PUSH 2 GOTO " << (i + 3) << "\n";
        }
        
        // HALT instruction for this bit
        cout << "HALT PUSH 1 GOTO " << (i + 3) << "\n";
    }
    
    return 0;
}