#include <iostream>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int tid;
    std::cin >> tid;
    
    // This program simulates checking for the substring t in s.
    // The overall strategy is to iterate through all possible starting positions in s
    // and for each, attempt a prefix match with t.

    // P is the main "seeker" state. It is placed between s and t.
    std::cout << "S=P" << std::endl;

    // At each step of the main loop, the program can do one of two things, based on rule priority:
    // 1. Try to perform a prefix match of the current s with t. (High priority)
    // 2. If matching isn't possible or fails, advance to the next starting position in s
    //    by effectively deleting the first character of s. (Low priority)

    // --- Prefix Matching Phase ---
    // P transitions to M to start a prefix match. This rule has high priority.
    std::cout << "P=M" << std::endl;

    // Rules for a successful character-by-character match.
    // M consumes one character from the end of s and one from the start of t if they match.
    // This is a bit unusual; it matches the end of the s-part with the start of the t-part.
    // This is why we need to shift s first. The logic will become clear with the shift rules.
    std::cout << "aMa=M" << std::endl;
    std::cout << "bMb=M" << std::endl;
    std::cout << "cMc=M" << std::endl;

    // If t is completely consumed by matches, we are left with M. This is a successful match.
    std::cout << "M=(return)1" << std::endl;

    // Mismatch rules. If characters don't match, transition to a temporary fail state F.
    // For example, if s ends in 'a' and t starts with 'b', `aMb` becomes `aFb`.
    // The F state will be cleaned up to reset for the next starting position.
    std::cout << "aMb=aFb" << std::endl;
    std::cout << "aMc=aFc" << std::endl;
    std::cout << "bMa=bFa" << std::endl;
    std::cout << "bMc=bFc" << std::endl;
    std::cout << "cMa=cFa" << std::endl;
    std::cout << "cMb=cFb" << std::endl;
    
    // Case where s is a proper prefix of t. This is also a mismatch for our purpose.
    // `Ma` means s is consumed but t is not.
    std::cout << "Ma=Fa" << std::endl;
    std::cout << "Mb=Fb" << std::endl;
    std::cout << "Mc=Fc" << std::endl;

    // --- Main Loop and State Reset Phase ---
    // If a match fails (state F is introduced), we need to reset to check the next position.
    // The F state "eats" the rest of the t-part to its right.
    std::cout << "Fa=F" << std::endl;
    std::cout << "Fb=F" << std::endl;
    std::cout << "Fc=F" << std::endl;
    
    // After cleaning up t, F and the character from s that caused the mismatch are
    // replaced by P, resetting the state. The crucial part is that one character from s
    // is consumed in this process (e.g., in `aFb -> ... -> aF -> P`), effectively
    // shifting the window.
    std::cout << "aF=P" << std::endl;
    std::cout << "bF=P" << std::endl;
    std::cout << "cF=P" << std::endl;
    
    // If s is longer than t, after a successful match, `s_rem M` will be the string.
    // This should also be a success.
    std::cout << "aM=(return)1" << std::endl;
    std::cout << "bM=(return)1" << std::endl;
    std::cout << "cM=(return)1" << std::endl;

    // This is the "shift s" operation. If no match rules apply for P (e.g., s is empty),
    // P consumes a character from s. This rule has lower priority than P=M.
    // In A=B, if `P=M` can be applied, it will be. If not, but `aP...` can, it will be.
    // `aP` will match `aP...`, consuming `a` and leaving `P...`, thus trying the next position.
    std::cout << "aP=P" << std::endl;
    std::cout << "bP=P" << std::endl;
    std::cout << "cP=P" << std::endl;

    // If s becomes empty (P is at the beginning of the string with no characters to its left),
    // and no match has been found, it means t is not a substring of s.
    std::cout << "P=(return)0" << std::endl;

    return 0;
}