#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <json/json.h>

struct Item {
    std::string type;
    int w, h, v, limit;
    double density;
};

struct Placement {
    std::string type;
    int x, y, rot;
};

struct Rect {
    int x, y, w, h;
};

class Packer {
private:
    int W, H;
    bool allow_rotate;
    std::vector<Item> items;
    std::vector<Placement> placements;
    std::vector<std::vector<bool>> grid;
    std::vector<int> used_count;

    bool canPlace(int x, int y, int w, int h) {
        if (x + w > W || y + h > H) return false;
        
        for (int i = x; i < x + w; i++) {
            for (int j = y; j < y + h; j++) {
                if (grid[i][j]) return false;
            }
        }
        return true;
    }
    
    void placeItem(int x, int y, int w, int h, const std::string& type, int rot) {
        for (int i = x; i < x + w; i++) {
            for (int j = y; j < y + h; j++) {
                grid[i][j] = true;
            }
        }
        placements.push_back({type, x, y, rot});
    }
    
    std::vector<std::pair<int, int>> findPlacements() {
        std::vector<std::pair<int, int>> positions;
        
        // Add skyline-based positions
        for (int x = 0; x < W; x++) {
            for (int y = 0; y < H; y++) {
                if (!grid[x][y]) {
                    positions.push_back({x, y});
                    if (positions.size() > 1000) break;
                }
            }
            if (positions.size() > 1000) break;
        }
        
        return positions;
    }

public:
    Packer(int w, int h, bool rotate) : W(w), H(h), allow_rotate(rotate) {
        grid.assign(W, std::vector<bool>(H, false));
    }
    
    void addItem(const Item& item) {
        items.push_back(item);
    }
    
    void pack() {
        used_count.assign(items.size(), 0);
        
        // Sort items by value density (descending)
        std::vector<int> order;
        for (int i = 0; i < items.size(); i++) {
            order.push_back(i);
        }
        
        std::sort(order.begin(), order.end(), [&](int a, int b) {
            return items[a].density > items[b].density;
        });
        
        // Greedy placement
        bool placed = true;
        while (placed) {
            placed = false;
            auto positions = findPlacements();
            
            for (int idx : order) {
                if (used_count[idx] >= items[idx].limit) continue;
                
                const Item& item = items[idx];
                
                for (auto pos : positions) {
                    int x = pos.first, y = pos.second;
                    
                    // Try normal orientation
                    if (canPlace(x, y, item.w, item.h)) {
                        placeItem(x, y, item.w, item.h, item.type, 0);
                        used_count[idx]++;
                        placed = true;
                        break;
                    }
                    
                    // Try rotated orientation
                    if (allow_rotate && canPlace(x, y, item.h, item.w)) {
                        placeItem(x, y, item.h, item.w, item.type, 1);
                        used_count[idx]++;
                        placed = true;
                        break;
                    }
                }
                
                if (placed) break;
            }
        }
    }
    
    const std::vector<Placement>& getPlacements() const {
        return placements;
    }
};

int main() {
    Json::Value root;
    std::cin >> root;
    
    int W = root["bin"]["W"].asInt();
    int H = root["bin"]["H"].asInt();
    bool allow_rotate = root["bin"]["allow_rotate"].asBool();
    
    Packer packer(W, H, allow_rotate);
    
    for (const auto& item_json : root["items"]) {
        Item item;
        item.type = item_json["type"].asString();
        item.w = item_json["w"].asInt();
        item.h = item_json["h"].asInt();
        item.v = item_json["v"].asInt();
        item.limit = item_json["limit"].asInt();
        item.density = (double)item.v / (item.w * item.h);
        
        packer.addItem(item);
    }
    
    packer.pack();
    
    Json::Value output;
    Json::Value placements_json(Json::arrayValue);
    
    for (const auto& placement : packer.getPlacements()) {
        Json::Value p;
        p["type"] = placement.type;
        p["x"] = placement.x;
        p["y"] = placement.y;
        p["rot"] = placement.rot;
        placements_json.append(p);
    }
    
    output["placements"] = placements_json;
    std::cout << output << std::endl;
    
    return 0;
}