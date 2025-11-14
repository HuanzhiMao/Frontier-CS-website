#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <set>
#include <iomanip>

// For competitive programming, the nlohmann/json.hpp header is often
// pasted directly into the source file. For brevity, it is included here.
#include "json.hpp"

using json = nlohmann::json;

// Data structures for problem entities
struct Bin {
    int W, H;
    bool allow_rotate;
};

struct ItemType {
    std::string id;
    int w, h, v, limit;
};

struct Placement {
    std::string type_id;
    int x, y, rot;
};

// Represents a potential placement to evaluate
struct Candidate {
    int item_type_idx;
    int x, y, w, h, rot;
    double score;
};

void solve(const Bin& bin, const std::vector<ItemType>& item_types, std::vector<Placement>& placements) {
    std::vector<int> limits_left(item_types.size());
    for (size_t i = 0; i < item_types.size(); ++i) {
        limits_left[i] = item_types[i].limit;
    }

    // The skyline is represented by a map from an x-coordinate to a height.
    // A point (x, y) means the skyline has height y for the interval [x, next_x).
    std::map<int, int> skyline;
    skyline[0] = 0;
    skyline[bin.W] = 0; // Sentinel at the end for boundary checks

    while (true) {
        Candidate best_candidate = {-1, 0, 0, 0, 0, 0, -1.0};

        // In each step, find the best possible placement among all item types
        for (size_t i = 0; i < item_types.size(); ++i) {
            if (limits_left[i] == 0) {
                continue;
            }
            const auto& item = item_types[i];

            int rotations = bin.allow_rotate ? 2 : 1;
            if (item.w == item.h && bin.allow_rotate) rotations = 1;

            for (int rot = 0; rot < rotations; ++rot) {
                int cw = (rot == 0) ? item.w : item.h;
                int ch = (rot == 0) ? item.h : item.w;

                if (cw > bin.W || ch > bin.H) {
                    continue;
                }
                
                // Find the best Bottom-Left position for this item configuration
                int best_x = -1, best_y = bin.H + 1;
                
                // Candidate x-positions for placing the item's left edge
                std::set<int> candidate_x_coords;
                for(auto const& [x_coord, y_coord] : skyline) {
                    candidate_x_coords.insert(x_coord);
                    if (x_coord >= cw) {
                        candidate_x_coords.insert(x_coord - cw);
                    }
                }

                for (int px : candidate_x_coords) {
                    if (px < 0 || px + cw > bin.W) {
                        continue;
                    }

                    // Calculate the required base height for placement at px
                    int py = 0;
                    auto it = skyline.upper_bound(px);
                    it--; 
                    
                    while (it != skyline.end() && it->first < px + cw) {
                        py = std::max(py, it->second);
                        it++;
                    }

                    if (py + ch <= bin.H) {
                        if (py < best_y || (py == best_y && px < best_x)) {
                            best_y = py;
                            best_x = px;
                        }
                    }
                }

                // If a valid placement is found, consider it as a candidate
                if (best_x != -1) {
                    double score = (double)item.v / (double)(cw * ch);
                    
                    bool is_better = false;
                    if (best_candidate.item_type_idx == -1 || score > best_candidate.score) {
                        is_better = true;
                    } else if (std::abs(score - best_candidate.score) < 1e-9) { // score is double
                        if (best_y < best_candidate.y) {
                            is_better = true;
                        } else if (best_y == best_candidate.y && best_x < best_candidate.x) {
                            is_better = true;
                        }
                    }

                    if (is_better) {
                        best_candidate = {
                            (int)i, best_x, best_y, cw, ch, rot, score
                        };
                    }
                }
            }
        }

        if (best_candidate.item_type_idx == -1) {
            break; // No more items can be placed
        }

        // Perform the placement of the best candidate found
        int item_idx = best_candidate.item_type_idx;
        placements.push_back({item_types[item_idx].id, best_candidate.x, best_candidate.y, best_candidate.rot});
        limits_left[item_idx]--;

        // Update the skyline
        int x = best_candidate.x;
        int y = best_candidate.y;
        int w = best_candidate.w;
        int h = best_candidate.h;
        int new_y = y + h;

        auto it = skyline.upper_bound(x + w);
        it--;
        int y_after = it->second;

        it = skyline.upper_bound(x);
        while(it != skyline.end() && it->first < x + w) {
            it = skyline.erase(it);
        }
        
        skyline[x] = new_y;
        skyline[x+w] = y_after;

        // Clean up redundant skyline points to keep the map small
        it = skyline.find(x);
        if (it != skyline.begin()) {
            auto prev_it = std::prev(it);
            if (prev_it->second == it->second) {
                skyline.erase(it);
            }
        }
        
        it = skyline.find(x+w);
        if (it != skyline.end() && it != skyline.begin()) {
             auto prev_it = std::prev(it);
             if (prev_it->second == it->second) {
                skyline.erase(it);
            }
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    json input;
    std::cin >> input;

    Bin bin;
    bin.W = input["bin"]["W"];
    bin.H = input["bin"]["H"];
    bin.allow_rotate = input["bin"]["allow_rotate"];

    std::vector<ItemType> item_types;
    for (const auto& item_json : input["items"]) {
        item_types.push_back({
            item_json["type"],
            item_json["w"],
            item_json["h"],
            item_json["v"],
            item_json["limit"]
        });
    }

    std::vector<Placement> placements;
    solve(bin, item_types, placements);

    json output;
    json placements_json = json::array();
    for (const auto& p : placements) {
        placements_json.push_back({
            {"type", p.type_id},
            {"x", p.x},
            {"y", p.y},
            {"rot", p.rot}
        });
    }
    output["placements"] = placements_json;

    std::cout << output.dump() << std::endl;

    return 0;
}