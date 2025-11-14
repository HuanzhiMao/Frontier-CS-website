#include <iostream>
#include <vector>
#include <string>

void print_program(int n, const std::vector<std::string>& prog) {
    std::cout << n << std::endl;
    for (const auto& line : prog) {
        std::cout << line << std::endl;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    unsigned int k_unsigned;
    std::cin >> k_unsigned;

    if (k_unsigned == 1) {
        std::cout << 1 << std::endl;
        std::cout << "HALT PUSH 1 GOTO 1" << std::endl;
        return 0;
    }

    long long k = k_unsigned;
    long long m = (k - 1) / 2;

    int n = m + 2;
    std::vector<std::string> prog;

    // Instruction 1: Setup. Pushes value 1, goes to HALT gate.
    // This is effectively `PUSH 1 GOTO 2`.
    prog.push_back("POP 1 GOTO 2 PUSH 1 GOTO 2");

    // Instruction 2: HALT gate. Halts if stack is empty.
    // Otherwise, pushes value 1 and enters the chain at instruction 3.
    prog.push_back("HALT PUSH 1 GOTO 3");

    // Instruction chain of length m.
    // Instructions 3 to m+1 form a chain of POPs.
    for (int i = 0; i < m - 1; ++i) {
        int current_inst = 3 + i;
        int next_inst = current_inst + 1;
        // The value pushed on failure (2) is a "poison pill" that is never popped,
        // but the specific execution path for the given k avoids this branch.
        prog.push_back("POP 1 GOTO " + std::to_string(next_inst) + " PUSH 2 GOTO " + std::to_string(next_inst));
    }

    // Last instruction in the chain (at index m+2).
    // It pops the last '1' and returns to the HALT gate (inst 2).
    int last_chain_inst = m + 2;
    prog.push_back("POP 1 GOTO 2 PUSH 2 GOTO 2");
    
    print_program(n, prog);

    return 0;
}