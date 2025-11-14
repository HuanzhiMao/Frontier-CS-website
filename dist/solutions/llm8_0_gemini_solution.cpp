#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

struct Instruction {
    // For POP: type="POP", a, x, b, y are used.
    // For HALT: type="HALT", b, y are used.
    std::string type;
    int a, x, b, y;
};

// Function to get the 1-based address for the instruction handling bit i
int get_addr(int i, int N) {
    if (i == N) return 1;
    if (i == N - 1) return 3;
    // For i <= N-2
    return (N - i + 2);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    unsigned int k_uint;
    std::cin >> k_uint;
    long long k = k_uint;

    if (k == 1) {
        std::cout << 1 << std::endl;
        std::cout << "HALT PUSH 1 GOTO 1" << std::endl;
        return 0;
    }

    int N = 0;
    if (k > 0) {
        N = static_cast<int>(floor(log2(k)));
    }

    int n = N + 3;
    std::cout << n << std::endl;

    std::vector<Instruction> program(n + 1);

    int h1_addr = 2;
    int h2_addr = N + 3;

    // H1 instruction
    program[h1_addr].type = "HALT";
    program[h1_addr].b = 1;
    program[h1_addr].y = (N > 0) ? get_addr(N - 1, N) : 1;

    // H2 instruction
    program[h2_addr].type = "HALT";
    program[h2_addr].b = 99;
    program[h2_addr].y = get_addr(0, N);

    // Instruction for the most significant bit, b_N
    int pN_addr = get_addr(N, N);
    program[pN_addr].type = "POP";
    program[pN_addr].a = 1;
    program[pN_addr].x = h1_addr;
    program[pN_addr].b = 1;
    program[pN_addr].y = h1_addr;

    // Instructions for bits b_{N-1} down to b_1
    for (int i = N - 1; i >= 1; --i) {
        int p_i_addr = get_addr(i, N);
        int p_i_minus_1_addr = get_addr(i - 1, N);
        bool bit = (k >> i) & 1;

        program[p_i_addr].type = "POP";
        program[p_i_addr].a = 1;
        program[p_i_addr].b = 2;
        program[p_i_addr].y = p_i_minus_1_addr;

        if (bit == 0) {
            program[p_i_addr].x = p_i_minus_1_addr;
        } else {
            program[p_i_addr].x = h1_addr;
        }
    }

    // Instruction for the least significant bit, b_0
    int p0_addr = get_addr(0, N);
    // k is odd, so b_0 is always 1.
    program[p0_addr].type = "POP";
    program[p0_addr].a = 1;
    program[p0_addr].x = h1_addr;
    program[p0_addr].b = 2;
    program[p0_addr].y = p0_addr;

    for (int i = 1; i <= n; ++i) {
        if (program[i].type == "POP") {
            std::cout << "POP " << program[i].a << " GOTO " << program[i].x
                      << " PUSH " << program[i].b << " GOTO " << program[i].y << '\n';
        } else {
            std::cout << "HALT PUSH " << program[i].b << " GOTO " << program[i].y << '\n';
        }
    }

    return 0;
}