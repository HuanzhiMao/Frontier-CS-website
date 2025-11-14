#include <iostream>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    // The Tid is not used in the solution, so we can just read and ignore it.
    int tid;
    std::cin >> tid;

    // This C++ program prints an A=B program as its output.
    // The A=B program is designed to solve the substring problem.
    //
    // The overall strategy is as follows:
    // 1. Setup Phase: Transform the input string `sSt` into `s Y t_upper_rev`.
    //    - `S` is replaced by a marker `U`.
    //    - `U` moves to the right across `s`, resulting in `stU`.
    //    - Then, `U` moves left across `t`, converting each character of `t`
    //      to its uppercase version (A,B,C). This results in `s t_upper_rev U`.
    //    - Finally, `U` is replaced by a static separator `Y`.
    //
    // 2. Cancellation and Sorting Phase:
    //    - The uppercase letters `A,B,C` act as "anti-characters" for `a,b,c`.
    //    - Matching pairs like `aA` are cancelled (replaced with empty string).
    //    - Sorting rules are used to move all remaining anti-characters (`A,B,C`)
    //      to the very beginning of the string.
    //
    // 3. Termination Phase:
    //    - If all anti-characters were cancelled, it means `t` was a substring of `s`.
    //      The string will end with `Y`, possibly preceded by leftover characters from `s`.
    //      This is a successful match, so we return 1.
    //    - If any anti-characters remain, they will be at the start of the string.
    //      The string will have a form like `...[A/B/C]...Y...`. The sorting rules
    //      ensure an anti-character will be right before `Y` if `s` is fully cancelled.
    //      This is a failed match, so we return 0.
    //    - A general fallback rule returns 0 for any case not explicitly handled.

    // Phase 1: Setup. Transform `sSt` into `s Y t_upper_rev`.
    // Replace S with U to start the process.
    std::cout << "S=U" << std::endl;
    // These rules move U to the right, past s. `sUt` becomes `stU`.
    std::cout << "Ua=aU" << std::endl;
    std::cout << "Ub=bU" << std::endl;
    std::cout << "Uc=cU" << std::endl;
    // These rules convert t to uppercase in reverse order as U moves left through t.
    std::cout << "aU=UA" << std::endl;
    std::cout << "bU=UB" << std::endl;
    std::cout << "cU=UC" << std::endl;
    // Once U has processed t, it becomes the separator Y.
    std::cout << "U=Y" << std::endl;

    // Phase 2: Cancellation and Sorting.
    // Cancellation rules for matching characters and anti-characters.
    std::cout << "aA=" << std::endl;
    std::cout << "bB=" << std::endl;
    std::cout << "cC=" << std::endl;
    // Sorting rules to move anti-characters (A,B,C) to the left of s-characters (a,b,c).
    std::cout << "bA=Ab" << std::endl;
    std::cout << "cA=Ac" << std::endl;
    std::cout << "aB=Ba" << std::endl;
    std::cout << "cB=Bc" << std::endl;
    std::cout << "aC=Ca" << std::endl;
    std::cout << "bC=Cb" << std::endl;
    // Sorting rules for anti-characters among themselves.
    std::cout << "BA=AB" << std::endl;
    std::cout << "CA=AC" << std::endl;
    std::cout << "CB=BC" << std::endl;

    // Phase 3: Termination.
    // If no anti-characters are left, it's a match.
    std::cout << "Y=(return)1" << std::endl;
    std::cout << "aY=(return)1" << std::endl;
    std::cout << "bY=(return)1" << std::endl;
    std::cout << "cY=(return)1" << std::endl;
    // If anti-characters remain, they will be sorted to the front.
    // If they are next to Y, it means s part is gone, so it's a definite mismatch.
    std::cout << "AY=(return)0" << std::endl;
    std::cout << "BY=(return)0" << std::endl;
    std::cout << "CY=(return)0" << std::endl;
    // Fallback rule for any other case (which must be a mismatch).
    std::cout << "=(return)0" << std::endl;

    return 0;
}