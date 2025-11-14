#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cmath>

long long G_val;
const long long RATE_BUDGET = 2800000;

void play_hand() {
    int h, r, a, b, P, k;
    std::string token;

    std::cin >> h >> r >> a >> b >> P >> k;

    int c1, v1, c2, v2;
    std::cin >> token >> c1 >> v1 >> c2 >> v2;

    std::cin >> token; // "BOARD"
    for (int i = 0; i < k; ++i) {
        int bc, bv;
        std::cin >> bc >> bv;
    }
    
    long long rate_budget_per_hand = (G_val > 0) ? (RATE_BUDGET / G_val) : 700;
    long long samples_this_turn = rate_budget_per_hand / 4;
    samples_this_turn = std::max(1LL, samples_this_turn);

    std::cout << "RATE " << samples_this_turn << std::endl;

    double w, d;
    std::cin >> token;
    if (token != "RATES") {
        return; 
    }
    std::cin >> w >> d;
    
    double E = w + d / 2.0;

    int raise_amount = 0;
    if (r == 1) { // Preflop
        if (E > 0.65) raise_amount = 10;
        else if (E > 0.55) raise_amount = 5;
    } else if (r == 2) { // Flop
        if (E > 0.85) raise_amount = P / 2;
        else if (E > 0.70) raise_amount = P * 2 / 3;
        else if (E <= 0.50) {
            if (P > 10) {
                raise_amount = P / 2;
            }
        }
    } else if (r == 3) { // Turn
        if (E > 0.90) raise_amount = P * 2 / 3;
        else if (E > 0.75) raise_amount = P * 3 / 4;
    } else if (r == 4) { // River
        if (E > 0.90) raise_amount = a;
        else if (E > 0.60) raise_amount = P * 2 / 3;
    }

    if (raise_amount > 0) {
        raise_amount = std::min(a, std::max(1, raise_amount));
        std::cout << "ACTION RAISE " << raise_amount << std::endl;
    } else {
        std::cout << "ACTION CHECK" << std::endl;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::cin >> G_val;
    if (G_val == -1) return 0;

    std::string token;
    while (std::cin >> token) {
        if (token == "STATE") {
            play_hand();
        } else if (token == "SCORE") {
            break;
        } else if (token == "-1") {
            break;
        }
    }
    return 0;
}