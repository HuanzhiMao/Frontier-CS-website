#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    long long k;
    cin >> k;
    
    vector<string> instructions;
    
    // Generate binary representation of (k-1)/2
    long long target = (k - 1) / 2;
    vector<int> bits;
    
    if (target == 0) {
        cout << "1\n";
        cout << "HALT PUSH 1 GOTO 1\n";
        return 0;
    }
    
    while (target > 0) {
        bits.push_back(target & 1);
        target >>= 1;
    }
    
    int n = bits.size() * 2 + 1;
    cout << n << "\n";
    
    // First instruction
    cout << "POP 1 GOTO 2 PUSH 1 GOTO 2\n";
    
    // Generate instructions based on binary representation
    for (int i = 0; i < bits.size(); i++) {
        int inst_num = i * 2 + 2;
        if (i == bits.size() - 1) {
            // Last bit
            if (bits[i] == 0) {
                cout << "HALT PUSH 1 GOTO " << inst_num + 1 << "\n";
                cout << "POP 1 GOTO 1 PUSH 2 GOTO 1\n";
            } else {
                cout << "POP 1 GOTO " << inst_num + 1 << " PUSH 1 GOTO " << inst_num + 1 << "\n";
                cout << "HALT PUSH 1 GOTO 1\n";
            }
        } else {
            if (bits[i] == 0) {
                cout << "HALT PUSH 1 GOTO " << inst_num + 2 << "\n";
                cout << "POP 1 GOTO " << inst_num + 2 << " PUSH 2 GOTO " << inst_num + 2 << "\n";
            } else {
                cout << "POP 1 GOTO " << inst_num + 2 << " PUSH 1 GOTO " << inst_num + 2 << "\n";
                cout << "POP 1 GOTO " << inst_num + 2 << " PUSH 2 GOTO " << inst_num + 2 << "\n";
            }
        }
    }
    
    return 0;
}