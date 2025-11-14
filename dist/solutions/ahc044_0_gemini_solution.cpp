#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <random>
#include <chrono>
#include <bitset>
#include <unordered_map>
#include <set>
#include <array>

using namespace std;

constexpr int N = 100;
constexpr int L = 500000;

array<int, N> T;

struct Problem {
    array<int, N> a, b;
};

struct Result {
    array<long long, N> t;
    long long error;
};

// Hash for bitset
namespace std {
    template<>
    struct hash<std::bitset<N>> {
        std::size_t operator()(const std::bitset<N>& b) const {
            std::hash<std::uint64_t> hasher;
            std::uint64_t part1 = 0, part2 = 0;
            for (int i = 0; i < 64; ++i) if (b[i]) part1 |= (1ULL << i);
            for (int i = 64; i < N; ++i) if (b[i]) part2 |= (1ULL << (i-64));
            return hasher(part1) ^ (hasher(part2) << 1);
        }
    };
}
// Hash for pair
struct pair_hash {
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2> &p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ (h2 << 1);
    }
};

Result calculate_error(const Problem& prob) {
    array<long long, N> t{};
    t[0] = 1;

    unordered_map<pair<int, bitset<N>>, pair<int, array<long long, N>>, pair_hash> history;
    
    int current_cleaner = 0;
    for (int week = 1; week < L; ++week) {
        bitset<N> parities;
        for (int i = 0; i < N; ++i) {
            if (t[i] % 2 != 0) {
                parities[i] = 1;
            }
        }

        auto state = make_pair(current_cleaner, parities);
        if (history.count(state)) {
            auto& prev = history[state];
            int prev_week = prev.first;
            auto& prev_t = prev.second;

            int cycle_len = week - prev_week;
            if (cycle_len > 0) {
                int remaining_weeks = L - week;
                long long num_cycles = remaining_weeks / cycle_len;
                
                array<long long, N> delta_t;
                for(int i = 0; i < N; ++i) {
                    delta_t[i] = t[i] - prev_t[i];
                }

                for(int i = 0; i < N; ++i) {
                    t[i] += num_cycles * delta_t[i];
                }
                week += num_cycles * cycle_len;

                if (week >= L) break;
            }
            history.clear(); 
        }
        history[state] = {week, t};

        int prev_cleaner = current_cleaner;
        long long times_cleaned = t[prev_cleaner];

        if (times_cleaned % 2 != 0) {
            current_cleaner = prob.a[prev_cleaner];
        } else {
            current_cleaner = prob.b[prev_cleaner];
        }
        t[current_cleaner]++;
    }

    long long total_error = 0;
    for (int i = 0; i < N; ++i) {
        total_error += abs(t[i] - T[i]);
    }
    return {t, total_error};
}

struct Job {
    int size;
    int from_person;
    bool is_odd; 

    bool operator<(const Job& other) const {
        if (size != other.size) {
            return size < other.size;
        }
        if (from_person != other.from_person) {
            return from_person < other.from_person;
        }
        return is_odd < other.is_odd;
    }
};

Problem generate_initial_solution(int depleted_person) {
    vector<int> T_prime(T.begin(), T.end());
    if (T_prime[depleted_person] > 0) {
        T_prime[depleted_person]--;
    }

    vector<Job> jobs;
    for (int i = 0; i < N; ++i) {
        int o_i = (T_prime[i] >= 0) ? (T_prime[i] + 1) / 2 : T_prime[i]/2;
        int e_i = (T_prime[i] >= 0) ? T_prime[i] / 2 : (T_prime[i]-1)/2;
        jobs.push_back({o_i, i, true});
        jobs.push_back({e_i, i, false});
    }
    sort(jobs.rbegin(), jobs.rend());
    
    vector<long long> rem_demand(N);
    for (int i = 0; i < N; ++i) {
        rem_demand[i] = T[i];
    }
    rem_demand[0]--;

    set<pair<long long, int>> pq;
    for (int i = 0; i < N; ++i) {
        pq.insert({rem_demand[i], i});
    }

    Problem prob;
    for (const auto& job : jobs) {
        auto it = pq.rbegin();
        int target_person = it->second;
        
        pq.erase(*it);

        if (job.is_odd) {
            prob.a[job.from_person] = target_person;
        } else {
            prob.b[job.from_person] = target_person;
        }
        rem_demand[target_person] -= job.size;
        pq.insert({rem_demand[target_person], target_person});
    }
    return prob;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n_dummy, l_dummy;
    cin >> n_dummy >> l_dummy;
    for (int i = 0; i < N; ++i) {
        cin >> T[i];
    }

    auto start_time = chrono::high_resolution_clock::now();

    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

    Problem best_prob;
    long long min_error = -1;

    vector<int> candidates;
    for(int i=0; i<N; ++i) if(T[i] > 0) candidates.push_back(i);
    if(candidates.empty()) candidates.push_back(0);


    for (int k : candidates) {
        Problem current_prob = generate_initial_solution(k);
        Result res = calculate_error(current_prob);
        if (min_error == -1 || res.error < min_error) {
            min_error = res.error;
            best_prob = current_prob;
        }
    }

    Problem current_prob = best_prob;
    long long current_error = min_error;
    
    double start_temp = 50;
    double end_temp = 0.1;
    double time_limit = 1.95;

    int iter = 0;
    while(true) {
        auto now = chrono::high_resolution_clock::now();
        double elapsed = chrono::duration_cast<chrono::duration<double>>(now - start_time).count();
        if (elapsed > time_limit) break;

        iter++;

        Problem next_prob = current_prob;
        int i = rng() % N;
        int k = rng() % N;

        if (rng() % 2 == 0) { // change a_i
            int old_k = next_prob.a[i];
            if (old_k == k) continue;
            next_prob.a[i] = k;
            Result res = calculate_error(next_prob);
            
            double temp = start_temp * pow(end_temp / start_temp, elapsed / time_limit);
            double acceptance_prob = exp((double)(current_error - res.error) / temp);

            if (res.error < current_error || (double)rng() / rng.max() < acceptance_prob) {
                current_prob = next_prob;
                current_error = res.error;
                if (current_error < min_error) {
                    min_error = current_error;
                    best_prob = current_prob;
                }
            }
        } else { // change b_i
            int old_k = next_prob.b[i];
            if (old_k == k) continue;
            next_prob.b[i] = k;
            Result res = calculate_error(next_prob);

            double temp = start_temp * pow(end_temp / start_temp, elapsed / time_limit);
            double acceptance_prob = exp((double)(current_error - res.error) / temp);

            if (res.error < current_error || (double)rng() / rng.max() < acceptance_prob) {
                current_prob = next_prob;
                current_error = res.error;
                if (current_error < min_error) {
                    min_error = current_error;
                    best_prob = current_prob;
                }
            }
        }
    }

    for (int i = 0; i < N; ++i) {
        cout << best_prob.a[i] << " " << best_prob.b[i] << "\n";
    }

    return 0;
}