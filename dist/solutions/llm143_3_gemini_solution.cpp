#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <set>

// Standard normal cumulative distribution function
double normal_cdf(double value) {
   return 0.5 * erfc(-value / sqrt(2.0));
}

void solve_turn() {
    int h, r;
    long long a, b, P;
    int k;
    std::cin >> h >> r >> a >> b >> P >> k;

    // Read card data, but we don't use them directly.
    // The strategy relies entirely on the judge's equity estimation.
    std::string token;
    int s, v;
    std::cin >> token; // "ALICE"
    std::cin >> s >> v >> s >> v;
    std::cin >> token; // "BOARD"
    for (int i = 0; i < k; ++i) {
        std::cin >> s >> v;
    }

    int t_rate;
    double C_sigma;

    // Set per-round parameters for RATE queries and opponent modeling.
    // Later rounds have fewer unknowns, so variance is higher.
    // We use more samples preflop where decisions have the most impact.
    if (r == 1) { // Preflop
        t_rate = 90;
        C_sigma = 0.3;
    } else if (r == 2) { // Flop
        t_rate = 80;
        C_sigma = 0.5;
    } else if (r == 3) { // Turn
        t_rate = 70;
        C_sigma = 0.6;
    } else { // r == 4, River
        t_rate = 60;
        C_sigma = 0.7;
    }

    // Query the judge for our hand's equity (win + 0.5*tie probability).
    std::cout << "RATE " << t_rate << std::endl;
    double w, d;
    std::cin >> token >> w >> d; // "RATES" w d
    double eq = w + d / 2.0;
    eq = std::max(0.0, std::min(1.0, eq)); // Clamp equity to [0,1]

    long long my_investment = 100 - a;
    
    // Calculate the expected profit of checking down to showdown.
    double ev_check = eq * P - my_investment;

    double best_ev = ev_check;
    long long best_raise_amount = 0;

    // Evaluate a set of candidate raise sizes to find the most profitable one.
    std::set<long long> raise_candidates;
    if (P > 0) {
        raise_candidates.insert(P / 2);
        raise_candidates.insert(P * 3 / 4);
        raise_candidates.insert(P);
        raise_candidates.insert(P * 3 / 2);
    }
    raise_candidates.insert(a);

    for (long long x_ll : raise_candidates) {
        long long x = x_ll;
        x = std::max(1LL, x);
        x = std::min((long long)a, x);
        if (x == 0) continue;

        // Model Bob's folding probability based on her pot odds.
        // We assume Bob's equity against a random hand is normally distributed
        // around 1-eq, with variance depending on our hand's equity and the round.
        double mu_bob_eq = 1.0 - eq;
        double sigma = C_sigma * sqrt(eq * (1.0 - eq)) + 1e-9;
        
        double bob_pot_odds = (double)x / (P + 2.0 * x);
        
        double z_score = (bob_pot_odds - mu_bob_eq) / sigma;
        double fold_prob = normal_cdf(z_score);

        // Calculate expected profit of raising by x.
        double ev_raise_win_on_fold = P - my_investment;
        double ev_raise_gets_called = eq * (P + 2.0 * x) - (my_investment + x);
        double ev_raise = fold_prob * ev_raise_win_on_fold + (1.0 - fold_prob) * ev_raise_gets_called;
        
        if (ev_raise > best_ev) {
            best_ev = ev_raise;
            best_raise_amount = x;
        }
    }
    
    // Choose the action with the highest expected profit.
    if (best_raise_amount > 0) {
        std::cout << "ACTION RAISE " << best_raise_amount << std::endl;
    } else {
        std::cout << "ACTION CHECK" << std::endl;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int G_val;
    std::cin >> G_val;
    if (G_val == -1) return 0;

    std::string token;
    while (std::cin >> token) {
        if (token == "STATE") {
            solve_turn();
        } else if (token == "SCORE") {
            double final_score;
            std::cin >> final_score;
            break;
        } else if (token == "-1") {
            break;
        } else {
            // For other tokens like OPP, RESULT, consume the rest of the line
            // as they are purely informational for this strategy.
            std::string dummy;
            std::getline(std::cin, dummy);
        }
    }
    return 0;
}