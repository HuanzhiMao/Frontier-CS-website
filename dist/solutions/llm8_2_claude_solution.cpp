#include <bits/stdc++.h>
using namespace std;

int main() {
    long long k;
    cin >> k;
    
    vector<string> instructions;
    
    // Convert k-1 to binary and build the program
    k--; // We need k-1 because the final HALT counts as 1 instruction
    
    int bit_pos = 0;
    while (k > 0) {
        if (k & 1) {
            // Add instructions for this bit position
            instructions.push_back("POP " + to_string(bit_pos + 1) + " GOTO " + to_string(instructions.size() + 3) + " PUSH " + to_string(bit_pos + 1) + " GOTO " + to_string(instructions.size() + 2));
            instructions.push_back("HALT PUSH 1 GOTO 1");
        } else {
            // Add a simple loop that doesn't affect the count
            instructions.push_back("POP " + to_string(bit_pos + 1) + " GOTO " + to_string(instructions.size() + 2) + " PUSH " + to_string(bit_pos + 1) + " GOTO " + to_string(instructions.size() + 2));
        }
        k >>= 1;
        bit_pos++;
    }
    
    // Add final HALT
    instructions.push_back("HALT PUSH 1 GOTO 1");
    
    // Simple approach: construct a linear program
    vector<string> program;
    
    // For k = 1, just halt
    if (k == 0) { // k was originally 1
        program.push_back("HALT PUSH 1 GOTO 1");
    } else {
        // Build a program that executes exactly k instructions
        long long remaining = k + 1; // +1 because we decremented earlier
        
        // Use binary representation approach
        program.clear();
        
        // Simple linear construction
        for (int i = 0; i < 30 && remaining > 1; i++) {
            if (remaining & (1LL << i)) {
                // Create a gadget that adds 2^i - 1 instructions
                long long count = (1LL << i) - 1;
                if (count > 0) {
                    program.push_back("POP 1 GOTO " + to_string(program.size() + 2) + " PUSH 1 GOTO " + to_string(program.size() + 2));
                    for (int j = 0; j < count; j++) {
                        if (j == count - 1) {
                            program.push_back("POP 1 GOTO " + to_string(program.size() + 2) + " PUSH 1 GOTO " + to_string(program.size() + 2));
                        } else {
                            program.push_back("POP 1 GOTO " + to_string(program.size() + 2) + " PUSH 1 GOTO " + to_string(program.size() + 2));
                        }
                    }
                }
                remaining -= (1LL << i);
            }
        }
        program.push_back("HALT PUSH 1 GOTO 1");
        
        // Fallback: simple approach
        if (program.size() > 512) {
            program.clear();
            long long target = k + 1;
            
            // Create a simple loop-based program
            program.push_back("POP 1 GOTO 2 PUSH 1 GOTO 2");
            
            while (target > 2) {
                program.push_back("POP 1 GOTO " + to_string(program.size() + 2) + " PUSH 1 GOTO " + to_string(program.size() + 2));
                target -= 2;
            }
            program.push_back("HALT PUSH 1 GOTO 1");
        }
    }
    
    // Even simpler approach
    program.clear();
    long long target = k + 1;
    
    while (target > 1) {
        program.push_back("POP 1 GOTO " + to_string(program.size() + 2) + " PUSH 1 GOTO " + to_string(program.size() + 2));
        target--;
    }
    program.push_back("HALT PUSH 1 GOTO 1");
    
    cout << program.size() << "\n";
    for (const string& inst : program) {
        cout << inst << "\n";
    }
    
    return 0;
}