#include <bits/stdc++.h>
using namespace std;

struct ItemType {
    string type;
    int w, h, v, limit;
    double density;
};

struct FreeRect {
    int x, y, w, h;
    bool operator<(const FreeRect& o) const {
        if (y != o.y) return y < o.y;
        if (x != o.x) return x < o.x;
        if (w != o.w) return w < o.w;
        return h < o.h;
    }
};

struct Placement {
    string type;
    int x, y, rot;
};

pair<long long, vector<Placement>> pack(const vector<ItemType>& types, bool allow_rot, int W, int H) {
    set<FreeRect> free_set;
    free_set.insert({0, 0, W, H});
    vector<Placement> placements;
    long long total_v = 0;
    for (const auto& it : types) {
        int placed = 0;
        while (placed < it.limit) {
            int best_y = INT_MAX;
            int best_x = INT_MAX;
            set<FreeRect>::iterator best_it = free_set.end();
            int best_rot = -1;
            // Try rot 0
            int ww = it.w, hh = it.h;
            for (auto sit = free_set.begin(); sit != free_set.end(); ++sit) {
                if (sit->w >= ww && sit->h >= hh) {
                    best_y = sit->y;
                    best_x = sit->x;
                    best_it = sit;
                    best_rot = 0;
                    break;
                }
            }
            // Try rot 1 if allowed
            if (allow_rot) {
                ww = it.h;
                hh = it.w;
                for (auto sit = free_set.begin(); sit != free_set.end(); ++sit) {
                    if (sit->w >= ww && sit->h >= hh) {
                        int cy = sit->y;
                        int cx = sit->x;
                        if (cy < best_y || (cy == best_y && cx < best_x) ||
                            (cy == best_y && cx == best_x && best_rot != 0)) {
                            best_y = cy;
                            best_x = cx;
                            best_it = sit;
                            best_rot = 1;
                        }
                        break;
                    }
                }
            }
            if (best_it == free_set.end()) {
                break;
            }
            // Place
            int place_rot = best_rot;
            int place_ww = (place_rot == 0 ? it.w : it.h);
            int place_hh = (place_rot == 0 ? it.h : it.w);
            int place_x = best_it->x;
            int place_y = best_it->y;
            placements.push_back({it.type, place_x, place_y, place_rot});
            total_v += it.v;
            // Split
            auto old = *best_it;
            free_set.erase(best_it);
            if (old.w > place_ww) {
                free_set.insert({old.x + place_ww, old.y, old.w - place_ww, old.h});
            }
            if (old.h > place_hh) {
                free_set.insert({old.x, old.y + place_hh, place_ww, old.h - place_hh});
            }
            placed++;
        }
    }
    return {total_v, placements};
}

int main() {
    string input;
    char ch;
    while (cin.get(ch)) input += ch;
    string clean;
    for (char c : input) {
        if (!isspace(c)) clean += c;
    }
    // Parse bin
    size_t bin_pos = clean.find("\"bin\":{");
    size_t bin_end = clean.find("}", bin_pos + 7);
    string bin_str = clean.substr(bin_pos + 7, bin_end - bin_pos - 7);
    vector<string> bin_parts;
    size_t p = 0;
    while (p < bin_str.size()) {
        size_t np = bin_str.find(",", p);
        if (np == string::npos) np = bin_str.size();
        bin_parts.push_back(bin_str.substr(p, np - p));
        p = np + 1;
    }
    int W = 0, H = 0;
    bool allow_rotate = false;
    for (const auto& part : bin_parts) {
        size_t col = part.find(":");
        string k = part.substr(0, col);
        string val = part.substr(col + 1);
        if (k == "\"W\"") W = stoi(val);
        else if (k == "\"H\"") H = stoi(val);
        else if (k == "\"allow_rotate\"") allow_rotate = (val == "true");
    }
    // Parse items
    size_t items_pos = clean.find("\"items\":[");
    size_t items_end = clean.find("]", items_pos + 9);
    string items_str = clean.substr(items_pos + 9, items_end - items_pos - 9);
    vector<string> item_objs;
    size_t pos = 0;
    while (pos < items_str.size()) {
        if (items_str[pos] != '{') break; // error
        size_t end = items_str.find('}', pos);
        if (end == string::npos) break;
        string obj = items_str.substr(pos, end - pos + 1);
        item_objs.push_back(obj);
        pos = end + 1;
        if (pos < items_str.size() && items_str[pos] == ',') pos++;
    }
    vector<ItemType> items;
    for (const auto& obj : item_objs) {
        vector<string> parts;
        size_t qp = 1;
        while (qp < obj.size() - 1) {
            size_t nq = obj.find(",", qp);
            if (nq == string::npos) nq = obj.size() - 1;
            parts.push_back(obj.substr(qp, nq - qp));
            qp = nq + 1;
        }
        ItemType item;
        for (const auto& part : parts) {
            size_t col = part.find(":");
            string k = part.substr(0, col);
            string val = part.substr(col + 1);
            if (k == "\"type\"") {
                item.type = val.substr(1, val.size() - 2);
            } else if (k == "\"w\"") {
                item.w = stoi(val);
            } else if (k == "\"h\"") {
                item.h = stoi(val);
            } else if (k == "\"v\"") {
                item.v = stoi(val);
            } else if (k == "\"limit\"") {
                item.limit = stoi(val);
            }
        }
        item.density = (double)item.v / (item.w * item.h);
        items.push_back(item);
    }
    // Try different orderings
    vector<pair<long long, vector<Placement>>> results;
    // 1. Density desc
    vector<ItemType> types = items;
    sort(types.begin(), types.end(), [](const ItemType& a, const ItemType& b) {
        return a.density > b.density;
    });
    results.push_back(pack(types, allow_rotate, W, H));
    // 2. Height desc
    types = items;
    sort(types.begin(), types.end(), [](const ItemType& a, const ItemType& b) {
        return a.h > b.h;
    });
    results.push_back(pack(types, allow_rotate, W, H));
    // 3. Area desc
    types = items;
    sort(types.begin(), types.end(), [](const ItemType& a, const ItemType& b) {
        int aa = a.w * a.h, ab = b.w * b.h;
        return aa > ab;
    });
    results.push_back(pack(types, allow_rotate, W, H));
    // 4. Profit desc
    types = items;
    sort(types.begin(), types.end(), [](const ItemType& a, const ItemType& b) {
        return a.v > b.v;
    });
    results.push_back(pack(types, allow_rotate, W, H));
    // Find best
    int best_idx = 0;
    long long max_v = results[0].first;
    for (int i = 1; i < results.size(); ++i) {
        if (results[i].first > max_v) {
            max_v = results[i].first;
            best_idx = i;
        }
    }
    vector<Placement> best_placements = results[best_idx].second;
    // Output
    cout << "{" << endl;
    cout << "  \"placements\": [" << endl;
    for (size_t i = 0; i < best_placements.size(); ++i) {
        const auto& p = best_placements[i];
        cout << "    {\"type\":\"" << p.type << "\",\"x\":" << p.x << ",\"y\":" << p.y << ",\"rot\":" << p.rot << "}";
        if (i < best_placements.size() - 1) cout << ",";
        cout << endl;
    }
    cout << "  ]" << endl;
    cout << "}" << endl;
    return 0;
}