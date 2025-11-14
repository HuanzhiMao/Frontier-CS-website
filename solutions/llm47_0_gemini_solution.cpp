#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <optional>
#include <functional>
#include <numeric>

#include "json.hpp"

using json = nlohmann::json;

struct ItemType {
    std::string id;
    int w, h, v, limit;
};

struct Placement {
    std::string type_id;
    int x, y, rot;
    int w, h;
    int v;
};

struct Solver {
    int W_orig, H_orig;
    bool allow_rotate;
    std::vector<ItemType> item_types;

    struct ItemInstance {
        int w, h, v, rot, type_idx;
        double density;
        int max_dim;
        int area;
    };
    
    std::vector<Placement> solve() {
        std::vector<ItemInstance> instances;
        for (size_t i = 0; i < item_types.size(); ++i) {
            const auto& type = item_types[i];
            int w = type.w, h = type.h;
            instances.push_back({w, h, type.v, 0, (int)i, (double)type.v / (w * h), std::max(w, h), w * h});
            if (allow_rotate && w != h) {
                instances.push_back({h, w, type.v, 1, (int)i, (double)type.v / (w * h), std::max(w, h), w * h});
            }
        }
        
        std::vector<Placement> best_placements;
        long long best_profit = -1;

        auto evaluate = [&](const std::vector<Placement>& placements) {
            long long profit = 0;
            for(const auto& p : placements) profit += p.v;
            if(profit > best_profit) {
                best_profit = profit;
                best_placements = placements;
            }
        };

        std::vector<std::function<bool(const ItemInstance&, const ItemInstance&)>> sorters;
        sorters.push_back([](const auto& a, const auto& b){ return a.density > b.density; });
        sorters.push_back([](const auto& a, const auto& b){ return a.v > b.v; });
        sorters.push_back([](const auto& a, const auto& b){ return a.max_dim > b.max_dim; });
        sorters.push_back([](const auto& a, const auto& b){ return a.area > b.area; });
        sorters.push_back([](const auto& a, const auto& b){ return a.h > b.h; });

        for (auto& sorter : sorters) {
            std::vector<ItemInstance> sorted_instances = instances;
            std::sort(sorted_instances.begin(), sorted_instances.end(), sorter);
            
            auto placements = run_solver(sorted_instances, W_orig, H_orig);
            evaluate(placements);

            if (allow_rotate && W_orig != H_orig) {
                auto placements_rot_bin = run_solver(sorted_instances, H_orig, W_orig);
                std::vector<Placement> transformed_placements;
                for(const auto& p : placements_rot_bin) {
                    transformed_placements.push_back({
                        p.type_id,
                        W_orig - p.y - p.h,
                        p.x,
                        1 - p.rot,
                        p.h,
                        p.w,
                        p.v
                    });
                }
                evaluate(transformed_placements);
            }
        }
        return best_placements;
    }

private:
    std::vector<Placement> run_solver(const std::vector<ItemInstance>& instances, int W, int H) {
        std::vector<Placement> placements;
        std::vector<int> limits(item_types.size());
        for (size_t i = 0; i < item_types.size(); ++i) {
            limits[i] = item_types[i].limit;
        }

        std::map<int, int> skyline;
        skyline[0] = 0;

        while (true) {
            std::optional<Placement> best_placement_for_step;
            int min_y = H + 1;
            int min_x = W + 1;

            for (const auto& inst : instances) {
                if (limits[inst.type_idx] == 0) continue;

                int inst_best_x = -1;
                int inst_best_y = H + 1;

                for (auto const& [sx, sy] : skyline) {
                    int x = sx;
                    if (x + inst.w > W) continue;

                    int y_base = 0;
                    auto it = skyline.upper_bound(x);
                    --it;
                    while (it != skyline.end() && it->first < x + inst.w) {
                        y_base = std::max(y_base, it->second);
                        ++it;
                    }
                    
                    if (y_base + inst.h > H) continue;

                    if (y_base < inst_best_y || (y_base == inst_best_y && x < inst_best_x)) {
                        inst_best_y = y_base;
                        inst_best_x = x;
                    }
                }

                if (inst_best_x != -1) {
                     if (inst_best_y < min_y || (inst_best_y == min_y && inst_best_x < min_x)) {
                        min_y = inst_best_y;
                        min_x = inst_best_x;
                        best_placement_for_step = Placement{
                            item_types[inst.type_idx].id,
                            min_x, min_y, inst.rot, inst.w, inst.h, inst.v
                        };
                    }
                }
            }

            if (!best_placement_for_step) {
                break;
            }

            placements.push_back(*best_placement_for_step);
            for(size_t i = 0; i < item_types.size(); ++i) {
                if (item_types[i].id == best_placement_for_step->type_id) {
                    limits[i]--;
                    break;
                }
            }
            
            int px = best_placement_for_step->x;
            int pw = best_placement_for_step->w;
            int new_y = best_placement_for_step->y + best_placement_for_step->h;

            auto it_end = skyline.upper_bound(px + pw);
            --it_end;
            int end_h = it_end->second;

            skyline.erase(skyline.upper_bound(px), skyline.lower_bound(px + pw));

            skyline[px] = new_y;
            skyline[px + pw] = end_h;

            auto it = skyline.find(px);
            if (it != skyline.begin() && std::prev(it)->second == it->second) {
                skyline.erase(it);
            }
            it = skyline.find(px + pw);
            if (it != skyline.end() && it != skyline.begin() && std::prev(it)->second == it->second) {
                skyline.erase(it);
            }
        }
        return placements;
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    json input;
    std::cin >> input;

    Solver solver;
    solver.W_orig = input["bin"]["W"];
    solver.H_orig = input["bin"]["H"];
    solver.allow_rotate = input["bin"]["allow_rotate"];

    for (const auto& item_json : input["items"]) {
        solver.item_types.push_back({
            item_json["type"],
            item_json["w"],
            item_json["h"],
            item_json["v"],
            item_json["limit"]
        });
    }
    
    auto placements = solver.solve();

    json output;
    json placements_json = json::array();
    for (const auto& p : placements) {
        json placement_json;
        placement_json["type"] = p.type_id;
        placement_json["x"] = p.x;
        placement_json["y"] = p.y;
        placement_json["rot"] = p.rot;
        placements_json.push_back(placement_json);
    }
    output["placements"] = placements_json;

    std::cout << output.dump() << std::endl;

    return 0;
}