#include <iostream>
#include <vector>
#include <string>

int main() {
    // Fast I/O for competitive programming.
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    int m;
    std::cin >> n >> m;

    // Use long double for weights to avoid underflow over many rounds.
    // Initialize weights for all n participants to 1.0.
    std::vector<long double> weights(n, 1.0L);

    // Process m wagers.
    for (int i = 0; i < m; ++i) {
        // Read the predictions of the n other participants for the current wager.
        std::string predictions;
        std::cin >> predictions;

        // Calculate the weighted sum for predicting '0' and '1'.
        long double sum_for_0 = 0.0L;
        long double sum_for_1 = 0.0L;

        for (int j = 0; j < n; ++j) {
            if (predictions[j] == '0') {
                sum_for_0 += weights[j];
            } else {
                sum_for_1 += weights[j];
            }
        }

        // Izzy's guess is the outcome with the higher weighted sum.
        // Tie-breaking rule: default to '0' if sums are equal.
        char my_guess = (sum_for_1 > sum_for_0) ? '1' : '0';

        // Output Izzy's guess. std::endl also flushes the output buffer,
        // which is essential for interactive problems.
        std::cout << my_guess << std::endl;

        // Read the actual outcome of the wager.
        char actual_outcome;
        std::cin >> actual_outcome;
        
        // Update weights. Penalize participants who guessed incorrectly.
        // A learning rate (beta) of 0.5 is a common and effective choice.
        const long double beta = 0.5L;
        for (int j = 0; j < n; ++j) {
            if (predictions[j] != actual_outcome) {
                weights[j] *= beta;
            }
        }
    }

    return 0;
}