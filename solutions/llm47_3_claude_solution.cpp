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

struct Rect {
    int x1, y1, x2, y2;
};

int W, H;
bool allow_rotate;
vector<Item> items;
vector<Placement> placements;

bool overlaps(const Rect& a, const Rect& b) {
    return !(a.x2 <= b.x1 || b.x2 <= a.x1 || a.y2 <= b.y1 || b.y2 <= a.y1);
}

bool canPlace(int x, int y, int w, int h) {
    if (x < 0 || y < 0 || x + w > W || y + h > H) return false;
    
    Rect newRect = {x, y, x + w, y + h};
    for (const auto& p : placements) {
        int pw = items[0].w, ph = items[0].h;
        for (const auto& item : items) {
            if (item.type == p.type) {
                if (p.rot == 0) {
                    pw = item.w;
                    ph = item.h;
                } else {
                    pw = item.h;
                    ph = item.w;
                }
                break;
            }
        }
        Rect existingRect = {p.x, p.y, p.x + pw, p.y + ph};
        if (overlaps(newRect, existingRect)) return false;
    }
    return true;
}

void greedyPack() {
    vector<Item> sortedItems = items;
    sort(sortedItems.begin(), sortedItems.end(), [](const Item& a, const Item& b) {
        return a.density > b.density;
    });
    
    map<string, int> used;
    
    for (const auto& item : sortedItems) {
        if (used[item.type] >= item.limit) continue;
        
        for (int y = 0; y <= H; y++) {
            for (int x = 0; x <= W; x++) {
                // Try normal orientation
                if (canPlace(x, y, item.w, item.h) && used[item.type] < item.limit) {
                    placements.push_back({item.type, x, y, 0});
                    used[item.type]++;
                }
                
                // Try rotated orientation
                if (allow_rotate && item.w != item.h && canPlace(x, y, item.h, item.w) && used[item.type] < item.limit) {
                    placements.push_back({item.type, x, y, 1});
                    used[item.type]++;
                }
            }
        }
    }
}

void skylinePack() {
    vector<pair<int, int>> skyline; // (x, height)
    skyline.push_back({0, 0});
    skyline.push_back({W, 0});
    
    vector<Item> sortedItems = items;
    sort(sortedItems.begin(), sortedItems.end(), [](const Item& a, const Item& b) {
        return a.density > b.density;
    });
    
    map<string, int> used;
    
    for (const auto& item : sortedItems) {
        while (used[item.type] < item.limit) {
            bool placed = false;
            
            // Try normal orientation
            for (int i = 0; i < skyline.size() - 1 && !placed; i++) {
                int x = skyline[i].first;
                int maxHeight = skyline[i].second;
                
                // Find max height in the range [x, x + item.w)
                for (int j = i; j < skyline.size() && skyline[j].first < x + item.w; j++) {
                    maxHeight = max(maxHeight, skyline[j].second);
                }
                
                if (x + item.w <= W && maxHeight + item.h <= H) {
                    if (canPlace(x, maxHeight, item.w, item.h)) {
                        placements.push_back({item.type, x, maxHeight, 0});
                        used[item.type]++;
                        placed = true;
                    }
                }
            }
            
            // Try rotated orientation
            if (!placed && allow_rotate && item.w != item.h) {
                for (int i = 0; i < skyline.size() - 1 && !placed; i++) {
                    int x = skyline[i].first;
                    int maxHeight = skyline[i].second;
                    
                    // Find max height in the range [x, x + item.h)
                    for (int j = i; j < skyline.size() && skyline[j].first < x + item.h; j++) {
                        maxHeight = max(maxHeight, skyline[j].second);
                    }
                    
                    if (x + item.h <= W && maxHeight + item.w <= H) {
                        if (canPlace(x, maxHeight, item.h, item.w)) {
                            placements.push_back({item.type, x, maxHeight, 1});
                            used[item.type]++;
                            placed = true;
                        }
                    }
                }
            }
            
            if (!placed) break;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    string line;
    string input = "";
    while (getline(cin, line)) {
        input += line;
    }
    
    // Parse JSON manually
    size_t pos = input.find("\"W\":");
    pos = input.find(":", pos) + 1;
    W = stoi(input.substr(pos, input.find(",", pos) - pos));
    
    pos = input.find("\"H\":");
    pos = input.find(":", pos) + 1;
    H = stoi(input.substr(pos, input.find(",", pos) - pos));
    
    pos = input.find("\"allow_rotate\":");
    pos = input.find(":", pos) + 1;
    allow_rotate = input.substr(pos, 4) == "true";
    
    pos = input.find("\"items\":");
    pos = input.find("[", pos) + 1;
    
    while (pos < input.size()) {
        pos = input.find("{", pos);
        if (pos == string::npos) break;
        
        Item item;
        
        size_t typePos = input.find("\"type\":", pos);
        typePos = input.find("\"", typePos + 7) + 1;
        item.type = input.substr(typePos, input.find("\"", typePos) - typePos);
        
        size_t wPos = input.find("\"w\":", pos);
        wPos = input.find(":", wPos) + 1;
        item.w = stoi(input.substr(wPos, input.find(",", wPos) - wPos));
        
        size_t hPos = input.find("\"h\":", pos);
        hPos = input.find(":", hPos) + 1;
        item.h = stoi(input.substr(hPos, input.find(",", hPos) - hPos));
        
        size_t vPos = input.find("\"v\":", pos);
        vPos = input.find(":", vPos) + 1;
        item.v = stoi(input.substr(vPos, input.find(",", vPos) - vPos));
        
        size_t limitPos = input.find("\"limit\":", pos);
        limitPos = input.find(":", limitPos) + 1;
        item.limit = stoi(input.substr(limitPos, input.find("}", limitPos) - limitPos));
        
        item.density = (double)item.v / (item.w * item.h);
        
        items.push_back(item);
        
        pos = input.find("}", pos) + 1;
    }
    
    skylinePack();
    
    cout << "{\"placements\":[";
    for (int i = 0; i < placements.size(); i++) {
        if (i > 0) cout << ",";
        cout << "{\"type\":\"" << placements[i].type << "\",\"x\":" << placements[i].x 
             << ",\"y\":" << placements[i].y << ",\"rot\":" << placements[i].rot << "}";
    }
    cout << "]}" << endl;
    
    return 0;
}