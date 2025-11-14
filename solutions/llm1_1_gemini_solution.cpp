#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include "json.hpp"

// Using nlohmann::json for convenience.
// This header should be available in the execution environment.
// If not, it's a single-header library that can be easily included.
using json = nlohmann::json;

// Global constants for bag capacity
const long long MAX_MASS = 20000000; // 20 kg in mg
const long long MAX_VOL = 25000000;  // 25 liters in µliters

// Struct to hold original item properties
struct Item {
    std::string name;
    int q;
    long long v, m, l;
    int id;
};

// Struct for meta-items created by binary splitting
struct MetaItem {
    long long v, m, l;
    int original_id;
    int count;
};

int main() {
    // Fast I/O
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    // Parse JSON input
    json input;
    std::cin >> input;

    // Store items in a vector for indexed access
    std::vector<Item> items;
    int current_id = 0;
    for (auto& [key, value] : input.items()) {
        items.push_back({
            key,
            static_cast<int>(value[0]),
            static_cast<long long>(value[1]),
            static_cast<long long>(value[2]),
            static_cast<long long>(value[3]),
            current_id++
        });
    }

    // Binary splitting to convert bounded knapsack to 0-1 knapsack
    std::vector<MetaItem> meta_items;
    for (const auto& item : items) {
        int q = item.q;
        int count = 1;
        while (q > 0) {
            int num_to_take = std::min(q, count);
            meta_items.push_back({
                item.v * num_to_take,
                item.m * num_to_take,
                item.l * num_to_take,
                item.id,
                num_to_take
            });
            q -= num_to_take;
            count *= 2;
        }
    }

    // Scaling factors for mass and volume to reduce DP state space.
    // Chosen to balance runtime and memory usage against solution accuracy.
    // Using ceiling division for scaling weights ensures that any solution
    // found in the scaled-down DP space is valid for the original capacities.
    const int SCALE_M = 40000;
    const int SCALE_L = 50000;
    const int MAX_M_S = MAX_MASS / SCALE_M;
    const int MAX_L_S = MAX_VOL / SCALE_L;

    // DP table storing history for reconstruction.
    // all_dp[i][m][l] = max value using first 'i' meta-items with scaled mass 'm' and volume 'l'
    std::vector<std::vector<std::vector<long long>>> all_dp(meta_items.size() + 1, 
        std::vector<std::vector<long long>>(MAX_M_S + 1, std::vector<long long>(MAX_L_S + 1, 0)));

    // Fill the DP table
    for (size_t i = 0; i < meta_items.size(); ++i) {
        all_dp[i+1] = all_dp[i];
        const auto& meta_item = meta_items[i];
        long long m_s = (meta_item.m + SCALE_M - 1) / SCALE_M;
        long long l_s = (meta_item.l + SCALE_L - 1) / SCALE_L;
        if (m_s > MAX_M_S || l_s > MAX_L_S) continue;

        for (int m = MAX_M_S; m >= m_s; --m) {
            for (int l = MAX_L_S; l >= l_s; --l) {
                all_dp[i+1][m][l] = std::max(all_dp[i+1][m][l], all_dp[i][m - m_s][l - l_s] + meta_item.v);
            }
        }
    }

    // Find the best solution from the final DP table
    long long max_val = 0;
    int best_m = 0, best_l = 0;
    for (int m = 0; m <= MAX_M_S; ++m) {
        for (int l = 0; l <= MAX_L_S; ++l) {
            if (all_dp.back()[m][l] > max_val) {
                max_val = all_dp.back()[m][l];
                best_m = m;
                best_l = l;
            }
        }
    }

    // Reconstruct the solution by backtracking through the DP tables
    std::vector<int> counts(items.size(), 0);
    int current_m = best_m;
    int current_l = best_l;
    for (int i = meta_items.size(); i > 0; --i) {
        // If the value at the current state is the same as the state before considering this item,
        // it means we didn't take this item to reach this optimal state.
        if (all_dp[i][current_m][current_l] != all_dp[i-1][current_m][current_l]) {
            // Otherwise, this meta-item must have been part of the optimal solution.
            const auto& meta_item = meta_items[i-1];
            counts[meta_item.original_id] += meta_item.count;
            long long m_s = (meta_item.m + SCALE_M - 1) / SCALE_M;
            long long l_s = (meta_item.l + SCALE_L - 1) / SCALE_L;
            current_m -= m_s;
            current_l -= l_s;
        }
    }

    // Format and print the output JSON
    json output;
    for (const auto& item : items) {
        output[item.name] = counts[item.id];
    }
    std::cout << output.dump(1, '\t') << std::endl;

    return 0;
}