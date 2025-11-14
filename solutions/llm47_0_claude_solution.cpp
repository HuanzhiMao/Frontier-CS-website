#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include <queue>
#include <cmath>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

struct Item {
    string type;
    int w, h, v, limit;
    double density;
    int used = 0;
    
    Item(string t, int width, int height, int value, int lim) 
        : type(t), w(width), h(height), v(value), limit(lim) {
        density = (double)value / (width * height);
    }
};

struct Placement {
    string type;
    int x, y, rot;
    int w, h, v;
};

struct Rectangle {
    int x1, y1, x2, y2;
    Rectangle(int _x1, int _y1, int _x2, int _y2) : x1(_x1), y1(_y1), x2(_x2), y2(_y2) {}
};

class Packer {
private:
    int W, H;
    bool allow_rotate;
    vector<Item> items;
    vector<Placement> placements;
    vector<Rectangle> occupied;
    
    bool canPlace(int x, int y, int w, int h) {
        if (x + w > W || y + h > H) return false;
        
        Rectangle newRect(x, y, x + w, y + h);
        for (const auto& rect : occupied) {
            if (!(newRect.x2 <= rect.x1 || newRect.x1 >= rect.x2 || 
                  newRect.y2 <= rect.y1 || newRect.y1 >= rect.y2)) {
                return false;
            }
        }
        return true;
    }
    
    void placeItem(const string& type, int x, int y, int rot, int w, int h, int v) {
        placements.push_back({type, x, y, rot, w, h, v});
        occupied.push_back(Rectangle(x, y, x + w, y + h));
    }
    
    vector<pair<int, int>> findPlacements(int w, int h) {
        vector<pair<int, int>> positions;
        
        // Try different placement strategies
        for (int y = 0; y <= H - h; y += max(1, h/4)) {
            for (int x = 0; x <= W - w; x += max(1, w/4)) {
                if (canPlace(x, y, w, h)) {
                    positions.push_back({x, y});
                }
            }
        }
        
        // Also try skyline approach
        for (const auto& rect : occupied) {
            vector<pair<int, int>> candidates = {
                {rect.x2, rect.y1},
                {rect.x1, rect.y2}
            };
            
            for (auto [x, y] : candidates) {
                if (canPlace(x, y, w, h)) {
                    positions.push_back({x, y});
                }
            }
        }
        
        if (positions.empty() && occupied.empty()) {
            positions.push_back({0, 0});
        }
        
        return positions;
    }
    
public:
    Packer(int w, int h, bool rotate) : W(w), H(h), allow_rotate(rotate) {}
    
    void addItem(const Item& item) {
        items.push_back(item);
    }
    
    void pack() {
        // Sort by density descending
        sort(items.begin(), items.end(), [](const Item& a, const Item& b) {
            return a.density > b.density;
        });
        
        bool improved = true;
        int iterations = 0;
        
        while (improved && iterations < 3) {
            improved = false;
            iterations++;
            
            for (auto& item : items) {
                while (item.used < item.limit) {
                    bool placed = false;
                    
                    // Try normal orientation
                    auto positions = findPlacements(item.w, item.h);
                    if (!positions.empty()) {
                        auto [x, y] = positions[0];
                        placeItem(item.type, x, y, 0, item.w, item.h, item.v);
                        item.used++;
                        placed = true;
                        improved = true;
                    }
                    
                    // Try rotated orientation if allowed
                    if (!placed && allow_rotate && item.w != item.h) {
                        positions = findPlacements(item.h, item.w);
                        if (!positions.empty()) {
                            auto [x, y] = positions[0];
                            placeItem(item.type, x, y, 1, item.h, item.w, item.v);
                            item.used++;
                            placed = true;
                            improved = true;
                        }
                    }
                    
                    if (!placed) break;
                }
            }
        }
    }
    
    json getOutput() {
        json result;
        json placementArray = json::array();
        
        for (const auto& p : placements) {
            json placement;
            placement["type"] = p.type;
            placement["x"] = p.x;
            placement["y"] = p.y;
            placement["rot"] = p.rot;
            placementArray.push_back(placement);
        }
        
        result["placements"] = placementArray;
        return result;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    string line, input;
    while (getline(cin, line)) {
        input += line;
    }
    
    json data = json::parse(input);
    
    int W = data["bin"]["W"];
    int H = data["bin"]["H"];
    bool allow_rotate = data["bin"]["allow_rotate"];
    
    Packer packer(W, H, allow_rotate);
    
    for (const auto& itemData : data["items"]) {
        Item item(itemData["type"], itemData["w"], itemData["h"], 
                 itemData["v"], itemData["limit"]);
        packer.addItem(item);
    }
    
    packer.pack();
    
    cout << packer.getOutput().dump() << endl;
    
    return 0;
}