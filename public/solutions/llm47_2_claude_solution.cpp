#include <bits/stdc++.h>
using namespace std;

struct Item {
    string type;
    int w, h, v, limit;
    double density;
};

struct Placement {
    string type;
    int x, y, rot;
};

struct Rectangle {
    int x1, y1, x2, y2;
    Rectangle(int x, int y, int w, int h) : x1(x), y1(y), x2(x + w), y2(y + h) {}
};

bool overlaps(const Rectangle& a, const Rectangle& b) {
    return !(a.x2 <= b.x1 || b.x2 <= a.x1 || a.y2 <= b.y1 || b.y2 <= a.y1);
}

struct Packer {
    int W, H;
    bool allow_rotate;
    vector<Item> items;
    vector<Placement> placements;
    vector<Rectangle> placed_rects;
    map<string, int> used_count;
    
    bool canPlace(int x, int y, int w, int h) {
        if (x < 0 || y < 0 || x + w > W || y + h > H) return false;
        Rectangle new_rect(x, y, w, h);
        for (const auto& rect : placed_rects) {
            if (overlaps(new_rect, rect)) return false;
        }
        return true;
    }
    
    void place(const string& type, int x, int y, int rot, int w, int h, int v) {
        placements.push_back({type, x, y, rot});
        placed_rects.push_back(Rectangle(x, y, w, h));
        used_count[type]++;
    }
    
    vector<pair<int, int>> findPositions(int w, int h) {
        vector<pair<int, int>> positions;
        
        // Try all possible positions
        for (int y = 0; y <= H - h; y++) {
            for (int x = 0; x <= W - w; x++) {
                if (canPlace(x, y, w, h)) {
                    positions.push_back({x, y});
                }
            }
        }
        
        // Sort by bottom-left preference
        sort(positions.begin(), positions.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
            if (a.second != b.second) return a.second < b.second;
            return a.first < b.first;
        });
        
        return positions;
    }
    
    void pack() {
        // Sort items by value density (descending)
        sort(items.begin(), items.end(), [](const Item& a, const Item& b) {
            return a.density > b.density;
        });
        
        bool improved = true;
        while (improved) {
            improved = false;
            
            for (const auto& item : items) {
                if (used_count[item.type] >= item.limit) continue;
                
                // Try original orientation
                auto positions = findPositions(item.w, item.h);
                if (!positions.empty()) {
                    place(item.type, positions[0].first, positions[0].second, 0, item.w, item.h, item.v);
                    improved = true;
                    continue;
                }
                
                // Try rotated orientation if allowed
                if (allow_rotate && (item.w != item.h)) {
                    positions = findPositions(item.h, item.w);
                    if (!positions.empty()) {
                        place(item.type, positions[0].first, positions[0].second, 1, item.h, item.w, item.v);
                        improved = true;
                        continue;
                    }
                }
            }
        }
        
        // Try to fill remaining space with lower density items
        for (int pass = 0; pass < 3; pass++) {
            for (const auto& item : items) {
                while (used_count[item.type] < item.limit) {
                    bool placed = false;
                    
                    // Try original orientation
                    auto positions = findPositions(item.w, item.h);
                    if (!positions.empty()) {
                        place(item.type, positions[0].first, positions[0].second, 0, item.w, item.h, item.v);
                        placed = true;
                    } else if (allow_rotate && (item.w != item.h)) {
                        // Try rotated orientation
                        positions = findPositions(item.h, item.w);
                        if (!positions.empty()) {
                            place(item.type, positions[0].first, positions[0].second, 1, item.h, item.w, item.v);
                            placed = true;
                        }
                    }
                    
                    if (!placed) break;
                }
            }
        }
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    string line;
    getline(cin, line);
    
    // Parse JSON manually
    size_t w_pos = line.find("\"W\":");
    size_t h_pos = line.find("\"H\":");
    size_t allow_pos = line.find("\"allow_rotate\":");
    
    int W = stoi(line.substr(w_pos + 4, line.find(",", w_pos) - w_pos - 4));
    int H = stoi(line.substr(h_pos + 4, line.find(",", h_pos) - h_pos - 4));
    bool allow_rotate = line.substr(allow_pos + 15, 4) == "true";
    
    Packer packer;
    packer.W = W;
    packer.H = H;
    packer.allow_rotate = allow_rotate;
    
    // Parse items
    size_t items_start = line.find("\"items\":[") + 9;
    size_t pos = items_start;
    
    while (pos < line.length()) {
        size_t item_start = line.find("{", pos);
        if (item_start == string::npos) break;
        size_t item_end = line.find("}", item_start);
        string item_str = line.substr(item_start, item_end - item_start + 1);
        
        Item item;
        
        // Extract type
        size_t type_pos = item_str.find("\"type\":\"") + 8;
        size_t type_end = item_str.find("\"", type_pos);
        item.type = item_str.substr(type_pos, type_end - type_pos);
        
        // Extract w, h, v, limit
        size_t w_pos = item_str.find("\"w\":") + 4;
        item.w = stoi(item_str.substr(w_pos, item_str.find(",", w_pos) - w_pos));
        
        size_t h_pos = item_str.find("\"h\":") + 4;
        item.h = stoi(item_str.substr(h_pos, item_str.find(",", h_pos) - h_pos));
        
        size_t v_pos = item_str.find("\"v\":") + 4;
        item.v = stoi(item_str.substr(v_pos, item_str.find(",", v_pos) - v_pos));
        
        size_t limit_pos = item_str.find("\"limit\":") + 8;
        item.limit = stoi(item_str.substr(limit_pos));
        
        item.density = (double)item.v / (item.w * item.h);
        
        packer.items.push_back(item);
        packer.used_count[item.type] = 0;
        
        pos = item_end + 1;
        if (line.find(",", pos) == string::npos) break;
    }
    
    packer.pack();
    
    cout << "{\"placements\":[";
    for (size_t i = 0; i < packer.placements.size(); i++) {
        if (i > 0) cout << ",";
        const auto& p = packer.placements[i];
        cout << "{\"type\":\"" << p.type << "\",\"x\":" << p.x << ",\"y\":" << p.y << ",\"rot\":" << p.rot << "}";
    }
    cout << "]}" << endl;
    
    return 0;
}