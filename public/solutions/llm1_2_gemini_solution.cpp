#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <cmath>
#include <numeric>

// This solution uses the nlohmann/json library for JSON parsing and serialization.
// It is a single-header library, typically included directly in the source file
// or made available in the compilation environment.
#include "nlohmann/json.hpp"

using json = nlohmann::json;

// Problem constraints
const long long MAX_MASS = 20000000LL;
const long long MAX_VOLUME = 25000000LL;

// Structure to hold item properties
struct Item {
    std::string name;
    int q;
    long long v;
    long long m, l;
    double density;
};

// Structure to hold a solution state (counts and total stats)
struct State {
    std::vector<int> counts;
    long long current_mass;
    long long current_volume;
    long long current_value;
};

// Global random number generator for efficiency
std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

int main() {
    // Fast I/O
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    json j_in;
    std::cin >> j_in;

    std::vector<Item> items;
    for (auto& el : j_in.items()) {
        Item item;
        item.name = el.key();
        item.q = el.value()[0];
        item.v = el.value()[1];
        item.m = el.value()[2];
        item.l = el.value()[3];
        // Calculate a normalized density for the greedy initialization.
        // This metric helps prioritize items that give the most value per unit of "cost" (normalized mass and volume).
        item.density = (double)item.v / (item.m / (double)MAX_MASS + item.l / (double)MAX_VOLUME);
        items.push_back(item);
    }
    const int NUM_ITEM_TYPES = items.size();

    // --- Greedy Initial Solution ---
    // Start with a good solution by greedily picking items with the best value density.
    State best_state;
    best_state.counts.resize(NUM_ITEM_TYPES, 0);
    best_state.current_mass = 0;
    best_state.current_volume = 0;
    best_state.current_value = 0;
    
    std::vector<int> sorted_indices(NUM_ITEM_TYPES);
    std::iota(sorted_indices.begin(), sorted_indices.end(), 0);
    std::sort(sorted_indices.begin(), sorted_indices.end(), [&](int a, int b) {
        return items[a].density > items[b].density;
    });

    for (int idx : sorted_indices) {
        long long can_take = items[idx].q;
        if (items[idx].m > 0) {
            can_take = std::min(can_take, (MAX_MASS - best_state.current_mass) / items[idx].m);
        }
        if (items[idx].l > 0) {
            can_take = std::min(can_take, (MAX_VOLUME - best_state.current_volume) / items[idx].l);
        }
        if (can_take > 0) {
            best_state.counts[idx] += can_take;
            best_state.current_mass += can_take * items[idx].m;
            best_state.current_volume += can_take * items[idx].l;
            best_state.current_value += can_take * items[idx].v;
        }
    }
    
    State current_state = best_state;

    // --- Simulated Annealing for Optimization ---
    // Explore the solution space to improve upon the greedy solution.
    auto start_time = std::chrono::steady_clock::now();
    double temperature = 100000.0;
    double cooling_rate = 0.999995;
    
    std::uniform_real_distribution<double> dist_01(0.0, 1.0);
    std::uniform_int_distribution<int> move_dist(0, 2);

    // Run for slightly less than the time limit to ensure timely output.
    while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time).count() < 950) {
        
        temperature *= cooling_rate;
        if(temperature < 1e-9) temperature = 1e-9; // Prevent underflow

        State neighbor_state = current_state;
        int move_type = move_dist(rng);
        
        // Randomly choose a move type: add, remove, or swap.
        if (move_type == 0) { // Add an item
            std::vector<int> addable;
            for(int i=0; i<NUM_ITEM_TYPES; ++i) if(neighbor_state.counts[i] < items[i].q) addable.push_back(i);
            if(addable.empty()) continue;
            int i_idx = addable[rng() % addable.size()];
            
            neighbor_state.counts[i_idx]++;
            neighbor_state.current_mass += items[i_idx].m;
            neighbor_state.current_volume += items[i_idx].l;
            neighbor_state.current_value += items[i_idx].v;

        } else if (move_type == 1) { // Remove an item
            std::vector<int> removable;
            for(int i=0; i<NUM_ITEM_TYPES; ++i) if(neighbor_state.counts[i] > 0) removable.push_back(i);
            if(removable.empty()) continue;
            int j_idx = removable[rng() % removable.size()];

            neighbor_state.counts[j_idx]--;
            neighbor_state.current_mass -= items[j_idx].m;
            neighbor_state.current_volume -= items[j_idx].l;
            neighbor_state.current_value -= items[j_idx].v;
        
        } else { // Swap an item in the bag for one outside
            std::vector<int> addable;
            for(int i=0; i<NUM_ITEM_TYPES; ++i) if(neighbor_state.counts[i] < items[i].q) addable.push_back(i);
            std::vector<int> removable;
            for(int i=0; i<NUM_ITEM_TYPES; ++i) if(neighbor_state.counts[i] > 0) removable.push_back(i);
            if(addable.empty() || removable.empty()) continue;

            int i_idx = addable[rng() % addable.size()];
            int j_idx = removable[rng() % removable.size()];
            
            neighbor_state.counts[i_idx]++;
            neighbor_state.counts[j_idx]--;
            neighbor_state.current_mass += items[i_idx].m - items[j_idx].m;
            neighbor_state.current_volume += items[i_idx].l - items[j_idx].l;
            neighbor_state.current_value += items[i_idx].v - items[j_idx].v;
        }

        // Check if the generated neighbor state is valid (within capacity).
        if (neighbor_state.current_mass <= MAX_MASS && neighbor_state.current_volume <= MAX_VOLUME) {
            long long value_delta = neighbor_state.current_value - current_state.current_value;
            // Metropolis-Hastings acceptance criterion:
            // Accept better solutions, or accept worse solutions with a probability dependent on temperature.
            if (value_delta > 0 || dist_01(rng) < std::exp((double)value_delta / temperature)) {
                current_state = neighbor_state;
                if (current_state.current_value > best_state.current_value) {
                    best_state = current_state;
                }
            }
        }
    }
    
    // Format and print the final output JSON
    json j_out;
    for (int i = 0; i < NUM_ITEM_TYPES; ++i) {
        j_out[items[i].name] = best_state.counts[i];
    }
    std::cout << j_out.dump(2) << std::endl;

    return 0;
}