#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Structure to hold a single instruction
struct Instruction {
    string type;
    int a, x, b, y;
};

// Helper to print the program in the required format
void print_program(const vector<Instruction>& prog) {
    cout << prog.size() << endl;
    for (const auto& instr : prog) {
        if (instr.type == "POP") {
            cout << "POP " << instr.a << " GOTO " << instr.x << " PUSH " << instr.b << " GOTO " << instr.y << endl;
        } else {
            cout << "HALT PUSH " << instr.b << " GOTO " << instr.y << endl;
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    unsigned int k;
    cin >> k;

    if (k == 1) {
        cout << 1 << endl;
        cout << "HALT PUSH 1 GOTO 1" << endl;
        return 0;
    }

    unsigned int m = (k - 1) / 2;
    vector<int> bits;
    if (m > 0) {
        unsigned int temp_m = m;
        while (temp_m > 0) {
            bits.push_back(temp_m % 2);
            temp_m /= 2;
        }
        reverse(bits.begin(), bits.end());
    }
    
    vector<Instruction> prog;

    // --- Module Addresses ---
    int doubler_entry = 2;
    int doubler_move_back = 3;
    int adder_entry = 4;
    int popper_entry = 5;
    int halt_pc = 6;

    // Instruction 1: Entry point. Push 1 for the MSB of m, then start bit processing.
    int first_bit_handler_pc = halt_pc;
    if (bits.size() > 1) {
        first_bit_handler_pc = halt_pc + 1;
    } else { // m=1, bits.size()=1
        first_bit_handler_pc = popper_entry;
    }
    prog.push_back({"POP", 1024, 1, 1, first_bit_handler_pc});

    // --- Modules ---
    // Instruction 2: Doubler entry. Moves 1s to 2s.
    prog.push_back({"POP", 1, doubler_entry, 2, doubler_move_back});
    
    // Instruction 3: Doubler move back. Moves 2s to two 1s.
    prog.push_back({"POP", 2, adder_entry, 1, 0}); // y will be set by caller
    
    // Instruction 4: Adder. Pushes 1, then returns.
    prog.push_back({"POP", 1024, 1, 1, 0}); // y will be set by caller
    
    // Instruction 5: Popper. Pops all 1s.
    prog.push_back({"POP", 1, popper_entry, 1024, halt_pc});

    // Instruction 6: Halt.
    prog.push_back({"HALT", 0, 0, 1, 1});

    // --- Main Logic Chain ---
    // For each bit (after the first), create a handler instruction.
    for (size_t i = 1; i < bits.size(); ++i) {
        int bit = bits[i];
        
        int next_handler_pc;
        if (i == bits.size() - 1) { // Last bit
            next_handler_pc = popper_entry;
        } else {
            next_handler_pc = halt_pc + i + 1;
        }

        // The handler for bit i is just a GOTO.
        // It starts the doubling process. The return from doubling
        // will go to the adder (if bit is 1) or the next handler.
        prog[2].y = bit == 1 ? adder_entry : next_handler_pc; // Set doubler's return
        prog[3].y = next_handler_pc; // Set adder's return
        
        prog.push_back({"POP", 1024, 1, 100, doubler_entry});
    }

    print_program(prog);

    return 0;
}