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

struct Rectangle {
    int x1, y1, x2, y2;
    
    bool overlaps(const Rectangle& other) const {
        return !(x2 <= other.x1 || other.x2 <= x1 || y2 <= other.y1 || other.y2 <= y1);
    }
};

class Packer {
private:
    int W, H;
    bool allow_rotate;
    std::vector<Item> items;
    std::vector<Placement> placements;
    std::vector<Rectangle> placed_rects;
    std::vector<int> type_counts;
    
public:
    void solve() {
        Json::Value root;
        std::cin >> root;
        
        W = root["bin"]["W"].asInt();
        H = root["bin"]["H"].asInt();
        allow_rotate = root["bin"]["allow_rotate"].asBool();
        
        items.clear();
        for (const auto& item : root["items"]) {
            Item i;
            i.type = item["type"].asString();
            i.w = item["w"].asInt();
            i.h = item["h"].asInt();
            i.v = item["v"].asInt();
            i.limit = item["limit"].asInt();
            i.density = (double)i.v / (i.w * i.h);
            items.push_back(i);
        }
        
        type_counts.resize(items.size(), 0);
        
        // Sort by density descending
        std::sort(items.begin(), items.end(), [](const Item& a, const Item& b) {
            return a.density > b.density;
        });
        
        pack();
        
        Json::Value output;
        Json::Value placement_array(Json::arrayValue);
        
        for (const auto& p : placements) {
            Json::Value placement;
            placement["type"] = p.type;
            placement["x"] = p.x;
            placement["y"] = p.y;
            placement["rot"] = p.rot;
            placement_array.append(placement);
        }
        
        output["placements"] = placement_array;
        std::cout << output << std::endl;
    }
    
private:
    void pack() {
        // Try different strategies
        packGreedy();
        
        // Try with rotation if allowed
        if (allow_rotate) {
            auto saved_placements = placements;
            auto saved_rects = placed_rects;
            auto saved_counts = type_counts;
            
            placements.clear();
            placed_rects.clear();
            std::fill(type_counts.begin(), type_counts.end(), 0);
            
            packGreedyWithRotation();
            
            // Keep better solution
            if (calculateProfit(saved_placements) > calculateProfit(placements)) {
                placements = saved_placements;
                placed_rects = saved_rects;
                type_counts = saved_counts;
            }
        }
    }
    
    void packGreedy() {
        std::vector<int> skyline_heights(W, 0);
        
        for (const auto& item : items) {
            int type_idx = getTypeIndex(item.type);
            
            for (int count = 0; count < item.limit; count++) {
                bool placed = tryPlaceItem(item, type_idx, skyline_heights, 0);
                if (!placed) break;
            }
        }
    }
    
    void packGreedyWithRotation() {
        std::vector<int> skyline_heights(W, 0);
        
        for (const auto& item : items) {
            int type_idx = getTypeIndex(item.type);
            
            for (int count = 0; count < item.limit; count++) {
                bool placed = tryPlaceItem(item, type_idx, skyline_heights, 0);
                if (!placed && allow_rotate) {
                    placed = tryPlaceItem(item, type_idx, skyline_heights, 1);
                }
                if (!placed) break;
            }
        }
    }
    
    bool tryPlaceItem(const Item& item, int type_idx, std::vector<int>& skyline_heights, int rotation) {
        int w = rotation ? item.h : item.w;
        int h = rotation ? item.w : item.h;
        
        if (w > W || h > H) return false;
        
        for (int x = 0; x <= W - w; x++) {
            int max_height = 0;
            for (int i = x; i < x + w; i++) {
                max_height = std::max(max_height, skyline_heights[i]);
            }
            
            int y = max_height;
            if (y + h > H) continue;
            
            Rectangle rect = {x, y, x + w, y + h};
            
            // Check for overlaps with existing placements
            bool valid = true;
            for (const auto& existing : placed_rects) {
                if (rect.overlaps(existing)) {
                    valid = false;
                    break;
                }
            }
            
            if (valid) {
                Placement p = {item.type, x, y, rotation};
                placements.push_back(p);
                placed_rects.push_back(rect);
                type_counts[type_idx]++;
                
                // Update skyline
                for (int i = x; i < x + w; i++) {
                    skyline_heights[i] = std::max(skyline_heights[i], y + h);
                }
                
                return true;
            }
        }
        
        return false;
    }
    
    int getTypeIndex(const std::string& type) {
        for (int i = 0; i < items.size(); i++) {
            if (items[i].type == type) return i;
        }
        return -1;
    }
    
    long long calculateProfit(const std::vector<Placement>& placements) {
        long long profit = 0;
        for (const auto& p : placements) {
            for (const auto& item : items) {
                if (item.type == p.type) {
                    profit += item.v;
                    break;
                }
            }
        }
        return profit;
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    Packer packer;
    packer.solve();
    
    return 0;
}