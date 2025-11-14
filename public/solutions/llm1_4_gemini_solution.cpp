#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <random>
#include <functional>
#include <numeric>
#include <limits>

#pragma GCC optimize("O3,unroll-loops")

const long long MAX_MASS = 20000000;
const long long MAX_VOLUME = 25000000;
const int NUM_TYPES = 12;

struct Item {
    std::string name;
    long long q, v, m, l;
    int id;
};

struct Solution {
    std::vector<long long> counts;
    long long total_mass;
    long long total_volume;
    long long total_value;

    Solution() : counts(NUM_TYPES, 0), total_mass(0), total_volume(0), total_value(0) {}
};

std::vector<Item> items;
std::mt19937 rng;
auto start_time = std::chrono::high_resolution_clock::now();

void parse_input() {
    std::string input_str;
    char c;
    while (std::cin.get(c)) {
        input_str += c;
    }

    size_t pos = 0;
    pos = input_str.find('{', pos);
    if (pos == std::string::npos) return;
    pos++;

    int item_id_counter = 0;
    while (items.size() < NUM_TYPES) {
        pos = input_str.find('"', pos);
        if (pos == std::string::npos) break;
        pos++;
        size_t key_end = input_str.find('"', pos);
        std::string key = input_str.substr(pos, key_end - pos);
        pos = key_end + 1;

        pos = input_str.find('[', pos);
        pos++;

        Item item;
        item.name = key;
        item.id = item_id_counter++;
        
        size_t end_num;
        pos = input_str.find_first_of("-0123456789", pos);
        item.q = std::stoll(input_str.substr(pos), &end_num);
        pos += end_num;
        
        pos = input_str.find_first_of("-0123456789", pos);
        item.v = std::stoll(input_str.substr(pos), &end_num);
        pos += end_num;
        
        pos = input_str.find_first_of("-0123456789", pos);
        item.m = std::stoll(input_str.substr(pos), &end_num);
        pos += end_num;
        
        pos = input_str.find_first_of("-0123456789", pos);
        item.l = std::stoll(input_str.substr(pos), &end_num);
        pos += end_num;

        items.push_back(item);

        pos = input_str.find_first_of(",}", pos);
        if (pos == std::string::npos || input_str[pos] == '}') {
            break;
        }
        pos++;
    }
}

void print_output(const Solution& sol) {
    std::map<std::string, long long> result;
    for (int i = 0; i < NUM_TYPES; ++i) {
        result[items[i].name] = sol.counts[i];
    }

    std::cout << "{\n";
    for (auto it = result.begin(); it != result.end(); ++it) {
        std::cout << " \"" << it->first << "\": " << it->second;
        if (std::next(it) != result.end()) {
            std::cout << ",\n";
        } else {
            std::cout << "\n";
        }
    }
    std::cout << "}\n";
}

Solution greedy(const std::function<double(const Item&)>& density_func) {
    std::vector<int> p(NUM_TYPES);
    std::iota(p.begin(), p.end(), 0);

    std::sort(p.begin(), p.end(), [&](int i, int j) {
        return density_func(items[i]) > density_func(items[j]);
    });
    
    Solution sol;
    for (int id : p) {
        const auto& item = items[id];
        if (item.m == 0 && item.l == 0) continue;

        long long num_to_take = item.q;
        if (item.m > 0) {
            num_to_take = std::min(num_to_take, (MAX_MASS - sol.total_mass) / item.m);
        }
        if (item.l > 0) {
            num_to_take = std::min(num_to_take, (MAX_VOLUME - sol.total_volume) / item.l);
        }

        if (num_to_take > 0) {
            sol.counts[id] += num_to_take;
            sol.total_mass += num_to_take * item.m;
            sol.total_volume += num_to_take * item.l;
            sol.total_value += num_to_take * item.v;
        }
    }
    
    return sol;
}

void run_sa() {
    Solution best_greedy_sol;
    best_greedy_sol.total_value = -1;

    std::vector<std::function<double(const Item&)>> densities;
    const double INF = std::numeric_limits<double>::infinity();
    densities.push_back([](const Item& item) { return (item.m > 0) ? (double)item.v / item.m : INF; });
    densities.push_back([](const Item& item) { return (item.l > 0) ? (double)item.v / item.l : INF; });
    densities.push_back([](const Item& item) {
        double norm_m = (double)item.m / MAX_MASS;
        double norm_l = (double)item.l / MAX_VOLUME;
        return (norm_m + norm_l > 1e-9) ? (double)item.v / (norm_m + norm_l) : INF;
    });
     densities.push_back([](const Item& item) { return (double)item.v; });


    for(const auto& density : densities) {
        Solution sol = greedy(density);
        if (sol.total_value > best_greedy_sol.total_value) {
            best_greedy_sol = sol;
        }
    }
    
    Solution current_sol = best_greedy_sol;
    Solution best_sol = best_greedy_sol;

    double temp = 50000.0;
    double cooling_rate = 0.9999998;

    std::uniform_int_distribution<int> type_dist(0, NUM_TYPES - 1);
    std::uniform_int_distribution<int> move_dist(0, 2);
    std::uniform_real_distribution<double> prob_dist(0.0, 1.0);
    
    for (int iterations = 0; ; ++iterations) {
        if (iterations % 1000 == 0) {
            auto now = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time);
            if (duration.count() > 950) {
                break;
            }
        }

        int move_type = move_dist(rng);
        int i = type_dist(rng);
        int j = type_dist(rng);

        long long delta_m = 0, delta_l = 0, delta_v = 0;
        bool move_valid = false;

        if (move_type == 0) { // ADD
            if (current_sol.counts[j] < items[j].q) {
                delta_m = items[j].m; delta_l = items[j].l; delta_v = items[j].v;
                move_valid = true;
            }
        } else if (move_type == 1) { // REMOVE
            if (current_sol.counts[i] > 0) {
                delta_m = -items[i].m; delta_l = -items[i].l; delta_v = -items[i].v;
                move_valid = true;
            }
        } else { // SWAP
            if (i != j && current_sol.counts[i] > 0 && current_sol.counts[j] < items[j].q) {
                delta_m = items[j].m - items[i].m;
                delta_l = items[j].l - items[i].l;
                delta_v = items[j].v - items[i].v;
                move_valid = true;
            }
        }

        if (!move_valid ||
            current_sol.total_mass + delta_m > MAX_MASS ||
            current_sol.total_volume + delta_l > MAX_VOLUME) {
            continue;
        }

        if (delta_v > 0 || (temp > 1e-9 && prob_dist(rng) < exp((double)delta_v / temp))) {
            current_sol.total_mass += delta_m;
            current_sol.total_volume += delta_l;
            current_sol.total_value += delta_v;
            
            if (move_type == 0) current_sol.counts[j]++;
            else if (move_type == 1) current_sol.counts[i]--;
            else { current_sol.counts[i]--; current_sol.counts[j]++; }
            
            if (current_sol.total_value > best_sol.total_value) {
                best_sol = current_sol;
            }
        }

        temp *= cooling_rate;
    }
    print_output(best_sol);
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    rng.seed(std::chrono::steady_clock::now().time_since_epoch().count());

    parse_input();
    run_sa();

    return 0;
}