#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

const long long MAX_MASS = 20000000;
const long long MAX_VOL = 25000000;
const long long INF = std::numeric_limits<long long>::max() / 2;

struct OriginalItem {
    std::string name;
    long long q, v, m, l;
    int id;
};

struct SplitItem {
    long long v, m, l;
    int original_id;
    int count;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    json input_json;
    std::cin >> input_json;

    std::vector<OriginalItem> original_items;
    int current_id = 0;
    for (auto& [name, data] : input_json.items()) {
        original_items.push_back({
            name,
            data[0].get<long long>(),
            data[1].get<long long>(),
            data[2].get<long long>(),
            data[3].get<long long>(),
            current_id++
        });
    }

    std::vector<SplitItem> split_items;
    for (const auto& item : original_items) {
        long long q = item.q;
        for (long long p = 1; q > 0; p <<= 1) {
            long long k = std::min(p, q);
            split_items.push_back({
                k * item.v,
                k * item.m,
                k * item.l,
                item.id,
                static_cast<int>(k)
            });
            q -= k;
        }
    }

    const int SCALE_V = 500;
    const int MAX_SCALED_V = 400000;

    std::vector<std::pair<long long, long long>> dp(MAX_SCALED_V + 1, {INF, INF});
    std::vector<int> prev_v_idx(MAX_SCALED_V + 1, 0);
    std::vector<int> item_idx(MAX_SCALED_V + 1, -1);

    dp[0] = {0, 0};

    for (size_t i = 0; i < split_items.size(); ++i) {
        const auto& item = split_items[i];
        long long scaled_v = item.v / SCALE_V;
        
        if (scaled_v == 0) continue;

        for (int sv = MAX_SCALED_V; sv >= scaled_v; --sv) {
            if (dp[sv - scaled_v].first != INF) {
                long long new_m = dp[sv - scaled_v].first + item.m;
                long long new_l = dp[sv - scaled_v].second + item.l;
                
                if (new_m < dp[sv].first || (new_m == dp[sv].first && new_l < dp[sv].second)) {
                    dp[sv] = {new_m, new_l};
                    prev_v_idx[sv] = sv - scaled_v;
                    item_idx[sv] = i;
                }
            }
        }
    }
    
    int best_sv = 0;
    for (int sv = MAX_SCALED_V; sv >= 0; --sv) {
        if (dp[sv].first <= MAX_MASS && dp[sv].second <= MAX_VOL) {
            best_sv = sv;
            break;
        }
    }

    std::vector<int> counts(original_items.size(), 0);
    int current_sv = best_sv;
    while(current_sv > 0) {
        int idx = item_idx[current_sv];
        if (idx == -1) break; 
        
        const auto& item_used = split_items[idx];
        counts[item_used.original_id] += item_used.count;
        
        current_sv = prev_v_idx[current_sv];
    }
    
    json output_json;
    for (const auto& item : original_items) {
        output_json[item.name] = counts[item.id];
    }

    std::cout << output_json.dump(2) << std::endl;

    return 0;
}