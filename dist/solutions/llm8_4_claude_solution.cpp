#include <bits/stdc++.h>
using namespace std;

int main() {
    long long k;
    cin >> k;
    
    vector<string> instructions;
    
    // Convert k to binary and process from most significant bit
    vector<int> bits;
    long long temp = k;
    while (temp > 0) {
        bits.push_back(temp & 1);
        temp >>= 1;
    }
    reverse(bits.begin(), bits.end());
    
    int n = bits.size() * 2;
    
    cout << n << "\n";
    
    // For each bit position i (0-indexed from left)
    for (int i = 0; i < bits.size(); i++) {
        int inst1 = i * 2 + 1; // 1-indexed
        int inst2 = i * 2 + 2; // 1-indexed
        
        if (i == bits.size() - 1) {
            // Last bit - use HALT instructions
            if (bits[i] == 1) {
                cout << "POP 1 GOTO " << inst2 << " PUSH 1 GOTO " << inst2 << "\n";
                cout << "HALT PUSH 1 GOTO 1\n";
            } else {
                cout << "HALT PUSH 1 GOTO " << inst2 << "\n";
                cout << "HALT PUSH 1 GOTO 1\n";
            }
        } else {
            // Not last bit
            int next_start = (i + 1) * 2 + 1;
            
            if (bits[i] == 1) {
                cout << "POP 1 GOTO " << inst2 << " PUSH 1 GOTO " << inst2 << "\n";
                cout << "POP 1 GOTO " << next_start << " PUSH 1 GOTO " << next_start << "\n";
            } else {
                cout << "POP 1 GOTO " << next_start << " PUSH 1 GOTO " << inst2 << "\n";
                cout << "POP 1 GOTO " << next_start << " PUSH 1 GOTO " << next_start << "\n";
            }
        }
    }
    
    return 0;
}