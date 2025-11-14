#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <functional>

// Use a custom struct to represent an instruction for easy generation.
struct Instruction {
    std::string type;
    int a = 1, x = 1, b = 1, y = 1;
};

// Global vector to store generated instructions before re-indexing.
// prog[0] is a dummy to make 1-based indexing of addresses natural.
std::vector<Instruction> prog;
// Memoization table to avoid re-generating identical sub-programs.
// Key: {bit index i, continuation address}, Value: entry address of P(i, cont).
std::map<std::pair<int, int>, int> memo;
// Allocator for new instruction addresses.
int pc_alloc = 1;

int add_instr(Instruction instr) {
    prog.push_back(instr);
    return pc_alloc++;
}

// Generates a procedure that executes (k >> i) instructions and then jumps to cont_pc.
// This corresponds to P(i, cont) from the explanation.
int generate(int i, int cont_pc, unsigned int k) {
    // Base case: For bits beyond the 31st, k >> i is 0.
    // The procedure should execute 0 steps and go to the continuation.
    if (i > 30) {
        return cont_pc;
    }
    // Return memoized result if this subproblem was already solved.
    if (memo.count({i, cont_pc})) {
        return memo[{i, cont_pc}];
    }

    int bit = (k >> i) & 1;

    int cont_after_first_call;

    if (bit == 0) {
        // If the i-th bit is 0, the total steps are 2 * (k >> (i+1)).
        // The continuation after the second call to P(i+1,...) is the original continuation.
        int cont_after_second_call = cont_pc;
        cont_after_first_call = generate(i + 1, cont_after_second_call, k);
    } else {
        // If the i-th bit is 1, we need 2 * (k >> (i+1)) + 1 steps.
        // We add one instruction before jumping to the original continuation.
        int one_step_then_cont = add_instr({"POP", 1, cont_pc, 1, cont_pc});
        int cont_after_second_call = one_step_then_cont;
        cont_after_first_call = generate(i + 1, cont_after_second_call, k);
    }
    
    // The entry point for P(i, cont) is a call to P(i+1, cont_after_first_call).
    int entry_pc = generate(i + 1, cont_after_first_call, k);
    
    // Memoize and return the entry point for this subproblem.
    return memo[{i, cont_pc}] = entry_pc;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    unsigned int k;
    std::cin >> k;
    
    prog.push_back({}); // Dummy for 1-based indexing.

    // The final instruction is a HALT. This is the base continuation.
    int halt_pc = add_instr({"HALT"});
    // Start the generation for the full number k (from bit 0) with HALT as the final continuation.
    int start_pc = generate(0, halt_pc, k);

    // The generation process creates a sparse set of instructions.
    // We need to remap their addresses to be dense from 1 to n.
    std::map<int, int> pc_map;
    std::vector<Instruction> final_prog;
    int current_pc = 1;

    // Use a lambda for a recursive traversal of the generated program graph to perform remapping.
    std::function<int(int)> remap_pc = 
        [&](int old_pc) -> int {
        if (pc_map.find(old_pc) == pc_map.end()) {
            pc_map[old_pc] = current_pc++;
            Instruction original_instr = prog[old_pc];
            final_prog.push_back(original_instr);
            int idx = final_prog.size() - 1;
            // Recursively remap GOTO targets.
            final_prog[idx].x = remap_pc(original_instr.x);
            final_prog[idx].y = remap_pc(original_instr.y);
        }
        return pc_map[old_pc];
    };
    
    remap_pc(start_pc);

    // Output the final, remapped program.
    std::cout << final_prog.size() << std::endl;
    for (const auto& instr : final_prog) {
        if (instr.type == "HALT") {
            std::cout << "HALT PUSH " << instr.b << " GOTO " << instr.y << std::endl;
        } else {
            std::cout << "POP " << instr.a << " GOTO " << instr.x << " PUSH " << instr.b << " GOTO " << instr.y << std::endl;
        }
    }

    return 0;
}