#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    long long k;
    cin >> k;
    
    vector<string> instructions;
    
    // We'll build a binary representation of k
    // Each bit will correspond to a pattern in our program
    
    // First, let's handle the case where k = 1
    if (k == 1) {
        cout << "1\n";
        cout << "HALT PUSH 1 GOTO 1\n";
        return 0;
    }
    
    // For k > 1, we build a program that uses binary representation
    // We need to construct a program that executes exactly k instructions
    
    // Strategy: Use a binary counter approach
    // Each bit position has a specific pattern
    
    vector<string> program;
    
    // We'll use a simple approach: create a loop that decrements k
    // But since we need to work with stack operations, we'll simulate binary counting
    
    // Start with basic building blocks
    program.push_back("POP 1 GOTO 2 PUSH 1 GOTO 3");  // Line 1
    program.push_back("HALT PUSH 1 GOTO 1");          // Line 2
    
    long long remaining = k - 1;  // We already account for the final HALT
    int line_count = 2;
    
    // We need to create a pattern that will execute exactly 'remaining' more instructions
    // before reaching the HALT
    
    // Use binary decomposition
    vector<int> bits;
    long long temp = remaining;
    while (temp > 0) {
        bits.push_back(temp % 2);
        temp /= 2;
    }
    
    // Rebuild the program using binary approach
    program.clear();
    
    // Create a more systematic approach
    // We'll create patterns for each power of 2
    
    int next_line = 1;
    
    // For each bit in the binary representation of (k-1)/2
    // we create a specific pattern
    
    long long target = (k - 1) / 2;  // Since we need odd k, (k-1) is even
    
    for (int bit = 0; bit < 30 && target > 0; bit++) {
        if (target & (1LL << bit)) {
            // Add pattern for this bit
            for (int i = 0; i < (1LL << bit); i++) {
                program.push_back("POP 1 GOTO " + to_string(program.size() + 2) + " PUSH 1 GOTO " + to_string(program.size() + 2));
            }
        }
    }
    
    // Simpler approach: direct construction
    program.clear();
    
    if (k == 1) {
        program.push_back("HALT PUSH 1 GOTO 1");
    } else {
        // Create a loop structure
        long long loops = (k - 1) / 2;
        
        // Use a simple repeating pattern
        program.push_back("POP 1 GOTO 2 PUSH 1 GOTO 1");  // Line 1: loop body
        program.push_back("HALT PUSH 1 GOTO 1");          // Line 2: halt when stack empty
        
        // We need to pre-populate the stack to get the right number of iterations
        // Each iteration of line 1 takes 1 instruction, then we need 1 more for halt
        // So we need 'loops' iterations
        
        // Actually, let's use the approach from the example
        vector<string> result;
        
        // Build binary representation approach
        long long val = k;
        int line_num = 1;
        
        // Create the halt instruction first
        result.push_back("HALT PUSH 1 GOTO 2");
        
        // Create decrementing pattern
        val--;  // Account for the halt instruction
        
        while (val > 0) {
            if (val >= 2) {
                result.insert(result.begin(), "POP 1 GOTO " + to_string(result.size() + 2) + " PUSH 1 GOTO " + to_string(result.size() + 2));
                val -= 2;
            } else {
                result.insert(result.begin(), "POP 1 GOTO " + to_string(result.size() + 2) + " PUSH 1 GOTO " + to_string(result.size() + 1));
                val -= 1;
            }
        }
        
        program = result;
    }
    
    cout << program.size() << "\n";
    for (const string& inst : program) {
        cout << inst << "\n";
    }
    
    return 0;
}