#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

struct Instruction {
    std::string type;
    int a, x, b, y;
};

void print_program(int n, const std::vector<Instruction>& prog) {
    std::cout << n << std::endl;
    for (int i = 0; i < n; ++i) {
        if (prog[i].type == "POP") {
            std::cout << "POP " << prog[i].a << " GOTO " << prog[i].x << " PUSH " << prog[i].b << " GOTO " << prog[i].y << std::endl;
        } else {
            std::cout << "HALT PUSH " << prog[i].b << " GOTO " << prog[i].y << std::endl;
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    unsigned int k;
    std::cin >> k;

    if (k == 1) {
        std::cout << 1 << std::endl;
        std::cout << "HALT PUSH 1 GOTO 1" << std::endl;
        return 0;
    }

    unsigned int v = (k - 1) / 2;
    int m = (v == 0) ? -1 : static_cast<int>(floor(log2(v)));
    int n = m + 3;

    std::vector<Instruction> prog(n);

    // Instruction 1 (PC=1)
    prog[0] = {"POP", 1, 2, 1, 2};
    // Instruction 2 (PC=2)
    prog[1] = {"HALT", 0, 0, 1, 3};

    // Chain for processing v
    for (int i = 0; i <= m; ++i) {
        bool bit = (v >> i) & 1;
        int current_pc_idx = i + 2;
        int next_pc = i + 4;
        
        int pop_goto;
        int push_goto = m + 3;

        if (bit == 0) { // c_i = 0
            pop_goto = next_pc;
        } else { // c_i = 1
            pop_goto = 2;
        }
        
        if (i == m) {
            if (bit == 0) {
                pop_goto = 2;
            }
        }
        
        prog[current_pc_idx] = {"POP", 1, pop_goto, 2, push_goto};
    }
    
    print_program(n, prog);

    return 0;
}