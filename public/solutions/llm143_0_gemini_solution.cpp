#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdlib>

// This function is called for each decision point.
// It reads the current game state, queries for win/draw rates,
// decides on an action, and prints it to standard output.
void process_state() {
    long long h, r, a, b, P, k;
    // Read the main state parameters
    std::cin >> h >> r >> a >> b >> P >> k;

    // Read and discard Alice's card details, as they are not used directly in the strategy logic.
    // The RATE query implicitly uses them on the judge's side.
    std::string token;
    std::cin >> token; // "ALICE"
    int s1, v1, s2, v2;
    std::cin >> s1 >> v1 >> s2 >> v2;
    
    // Read and discard board card details.
    std::cin >> token; // "BOARD"
    for (int i = 0; i < k; ++i) {
        int card_s, card_v;
        std::cin >> card_s >> card_v;
    }
    
    // Determine the number of Monte Carlo rollouts for the RATE query based on the current round.
    // This is a simple schedule to stay within the total budget of 3,000,000 rollouts over 10,000 hands.
    // Later rounds get more samples as decisions are more critical.
    int t = 0;
    if (r == 1) t = 50;      // Pre-flop
    else if (r == 2) t = 60; // Flop
    else if (r == 3) t = 70; // Turn
    else if (r == 4) t = 80; // River

    std::cout << "RATE " << t << std::endl;

    // Read the win (w) and draw (d) probabilities from the judge's response.
    double w, d;
    std::cin >> token; // "RATES"
    if (token != "RATES") {
        // If we don't get "RATES", it's likely an error or end-of-game signal.
        exit(0);
    }
    std::cin >> w >> d;

    // The core of the strategy: determine if our hand is "strong" enough to bet for value.
    // A hand is considered strong if its win probability `w` is greater than its loss probability `1-w-d`.
    // This simplifies to w > 0.5 - d/2.0.
    double threshold = 0.5 - d / 2.0;

    if (w > threshold + 1e-9) { // Strong hand: bet for value.
        // We calculate the maximum bet `x` that the opponent (Bob) will call.
        // Bob calls if `(1-w-d)(P+2x) + d(x+P/2) - x > 0`.
        // We find the root of this linear equation in x.
        double denom = 2.0 * w + d - 1.0;
        
        long long bet_amount;

        if (std::abs(denom) < 1e-9) {
             // If `w` is very close to the threshold, `x_maxcall` approaches infinity.
             // This suggests betting our entire stack.
             bet_amount = a;
        } else {
            double numer = (1.0 - w - d) * P + d * P / 2.0;
            double x_maxcall = numer / denom;
            
            if (x_maxcall < 1.0) {
                // If even the smallest bet will be folded, bet 1 to take down the current pot.
                bet_amount = 1;
            } else {
                // Bet the largest integer amount that Bob is expected to call.
                bet_amount = static_cast<long long>(std::floor(x_maxcall));
            }
        }
        
        // Ensure the bet is valid: between 1 and our current stack size.
        bet_amount = std::max(1LL, bet_amount);
        bet_amount = std::min((long long)a, bet_amount);

        if (bet_amount > 0) {
            std::cout << "ACTION RAISE " << bet_amount << std::endl;
        } else {
            // If our stack is 0, we cannot raise, so we must check.
            std::cout << "ACTION CHECK" << std::endl;
        }
    } else { // Weak hand: check.
        // With a weak hand, betting is unprofitable as Bob will always call.
        // Checking allows us to see more cards for free, potentially improving our hand.
        std::cout << "ACTION CHECK" << std::endl;
    }
}


int main() {
    // Fast I/O
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int G;
    std::cin >> G;
    if (G < 0) return 0;

    // Main game loop. The judge drives the game by sending tokens.
    // We only need to act when we receive a "STATE" token.
    std::string token;
    while(std::cin >> token) {
        if (token == "STATE") {
            process_state();
        } else if (token == "SCORE") {
            // End of the match.
            break;
        } else if (token == "-1") {
            // Error signal from judge.
            break;
        }
        // Other tokens like OPP, RESULT are part of the protocol flow but
        // don't require action from us. This simple loop structure correctly
        // consumes them and waits for the next STATE.
    }

    return 0;
}