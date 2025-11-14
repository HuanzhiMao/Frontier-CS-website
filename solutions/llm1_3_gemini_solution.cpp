#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Capacity constraints
const long long MAX_MASS = 20000000;
const long long MAX_VOL = 25000000;
// Discretization factor for the DP. A higher K gives better accuracy but is slower.
const int K = 800;

// Represents a type of treasure, e.g., "crown"
struct ItemType {
    std::string name;
    int q;          // quantity available
    long long v;    // value
    long long m;    // mass
    long long l;    // volume
    int original_idx;
};

// Represents a bundle of items of the same type, created by binary splitting quantities.
struct MetaItem {
    long long v;
    long long m;
    long long l;
    int original_idx;
    int count; // how many original items this meta-item represents
};

// Represents a state in the DP, storing the max value and the item counts to achieve it.
struct State {
    long long value;
    std::vector<int> counts;
};

// Generates all reachable, non-dominated states for a given set of meta-items.
// A state is a combination of items, represented by its total discretized mass and volume.
// The map stores the maximum value achievable for each {mass, volume} pair.
std::map<std::pair<int, int>, State> generate_states(const std::vector<MetaItem>& items, int num_item_types) {
    std::map<std::pair<int, int>, State> states;
    states[{0, 0}] = {0, std::vector<int>(num_item_types, 0)};

    for (const auto& item : items) {
        // Discretize mass and volume using floor to ensure the final solution (before repair) is feasible.
        int w_disc = static_cast<int>(floor((double)item.m * K / MAX_MASS));
        int l_disc = static_cast<int>(floor((double)item.l * K / MAX_VOL));
        
        std::vector<std::pair<std::pair<int, int>, State>> new_additions;
        new_additions.reserve(states.size());

        for (const auto& s : states) {
            int new_w = s.first.first + w_disc;
            int new_l = s.first.second + l_disc;

            if (new_w <= K && new_l <= K) {
                State new_state = s.second;
                new_state.value += item.v;
                new_state.counts[item.original_idx] += item.count;
                new_additions.push_back({{new_w, new_l}, new_state});
            }
        }
        
        for(const auto& addition : new_additions) {
            // If this {mass, vol} pair is new, or we found a path with higher value, update it.
            if (states.find(addition.first) == states.end() || addition.second.value > states[addition.first].value) {
                states[addition.first] = addition.second;
            }
        }
    }
    return states;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    json input_json;
    std::cin >> input_json;

    std::vector<ItemType> item_types;
    std::vector<std::string> item_names;
    int current_idx = 0;
    for (auto& el : input_json.items()) {
        item_names.push_back(el.key());
        item_types.push_back({
            el.key(),
            el.value()[0].get<int>(),
            el.value()[1].get<long long>(),
            el.value()[2].get<long long>(),
            el.value()[3].get<long long>(),
            current_idx++
        });
    }
    const int num_item_types = item_types.size();

    // Binary splitting (or decomposition) of item quantities.
    // This transforms the bounded knapsack problem into a 0/1 knapsack problem.
    std::vector<MetaItem> meta_items;
    for (const auto& type : item_types) {
        int quantity = type.q;
        for (int i = 1; i <= quantity; i *= 2) {
            meta_items.push_back({(long long)i * type.v, (long long)i * type.m, (long long)i * type.l, type.original_idx, i});
            quantity -= i;
        }
        if (quantity > 0) {
            meta_items.push_back({(long long)quantity * type.v, (long long)quantity * type.m, (long long)quantity * type.l, type.original_idx, quantity});
        }
    }

    // Meet-in-the-middle: split meta-items into two groups.
    int split_point = meta_items.size() / 2;
    std::vector<MetaItem> group_a_items(meta_items.begin(), meta_items.begin() + split_point);
    std::vector<MetaItem> group_b_items(meta_items.begin() + split_point, meta_items.end());

    auto states_a = generate_states(group_a_items, num_item_types);
    auto states_b = generate_states(group_b_items, num_item_types);
    
    // Pre-process group B states for efficient lookup.
    // best_b[w][l] will store the best state from group B with mass <= w and volume <= l.
    std::vector<std::vector<State>> best_b(K + 1, std::vector<State>(K + 1, {-1, {}}));

    for(const auto& s : states_b) {
        best_b[s.first.first][s.first.second] = s.second;
    }

    for (int w = 0; w <= K; ++w) {
        for (int l = 0; l <= K; ++l) {
            if (w > 0 && best_b[w - 1][l].value > best_b[w][l].value) {
                best_b[w][l] = best_b[w - 1][l];
            }
            if (l > 0 && best_b[w][l - 1].value > best_b[w][l].value) {
                best_b[w][l] = best_b[w][l - 1];
            }
        }
    }

    long long max_total_value = 0;
    std::vector<int> best_counts(num_item_types, 0);

    // Combine results from group A and group B.
    for (const auto& s_a : states_a) {
        int w_a = s_a.first.first;
        int l_a = s_a.first.second;
        int w_rem = K - w_a;
        int l_rem = K - l_a;

        const auto& s_b = best_b[w_rem][l_rem];
        if (s_b.value != -1) {
            long long total_value = s_a.second.value + s_b.value;
            if (total_value > max_total_value) {
                max_total_value = total_value;
                for (int i = 0; i < num_item_types; ++i) {
                    best_counts[i] = s_a.second.counts[i] + s_b.counts[i];
                }
            }
        }
    }

    // The discretized solution might be slightly over capacity due to floor/ceil errors.
    // A repair step removes items greedily until the solution is feasible.
    long long current_m = 0;
    long long current_l = 0;
    for (int i = 0; i < num_item_types; ++i) {
        current_m += (long long)best_counts[i] * item_types[i].m;
        current_l += (long long)best_counts[i] * item_types[i].l;
    }

    if (current_m > MAX_MASS || current_l > MAX_VOL) {
        std::vector<std::pair<double, int>> removal_candidates;
        for (int i = 0; i < num_item_types; ++i) {
            if (best_counts[i] > 0) {
                double metric = (double)item_types[i].v / (item_types[i].m + item_types[i].l + 1e-9); // Value density
                removal_candidates.push_back({metric, i});
            }
        }
        std::sort(removal_candidates.begin(), removal_candidates.end());

        while (current_m > MAX_MASS || current_l > MAX_VOL) {
            bool removed = false;
            for (auto const& [metric, idx] : removal_candidates) {
                if (best_counts[idx] > 0) {
                    best_counts[idx]--;
                    current_m -= item_types[idx].m;
                    current_l -= item_types[idx].l;
                    removed = true;
                    break; 
                }
            }
            if (!removed) break;
        }
    }
    
    json output_json;
    for (int i = 0; i < num_item_types; ++i) {
        output_json[item_names[i]] = best_counts[i];
    }
    std::cout << output_json.dump(1, '\t') << std::endl;

    return 0;
}