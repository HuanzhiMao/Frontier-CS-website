#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
#include <numeric>
#include <functional>
#include <chrono>
#include <random>
#include <cctype>

auto start_time = std::chrono::steady_clock::now();
bool time_limit_exceeded() {
    auto now = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count() > 950;
}

// --- JSON Parser ---
class SimpleJsonParser {
public:
    std::string data;
    size_t pos = 0;

    SimpleJsonParser(std::istream& in) {
        in.seekg(0, std::ios::end);
        data.reserve(in.tellg());
        in.seekg(0, std::ios::beg);
        data.assign((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    }

    void skip_ws() {
        while (pos < data.length() && isspace(data[pos])) pos++;
    }

    void expect(char c) {
        skip_ws();
        if (pos < data.length() && data[pos] == c) {
            pos++;
        }
    }

    std::string read_string() {
        expect('"');
        size_t start = pos;
        while (pos < data.length() && data[pos] != '"') pos++;
        std::string val = data.substr(start, pos - start);
        expect('"');
        return val;
    }

    long long read_long() {
        skip_ws();
        size_t start = pos;
        if (pos < data.length() && (data[pos] == '-' || isdigit(data[pos]))) {
            pos++;
            while (pos < data.length() && isdigit(data[pos])) pos++;
        }
        return std::stoll(data.substr(start, pos - start));
    }
    
    bool read_bool() {
        skip_ws();
        if (data.substr(pos, 4) == "true") {
            pos += 4;
            return true;
        } else if (data.substr(pos, 5) == "false") {
            pos += 5;
            return false;
        }
        return false;
    }
    
    void find_key(const std::string& key) {
        std::string quoted_key = "\"" + key + "\"";
        pos = data.find(quoted_key, pos);
        if (pos != std::string::npos) {
            pos += quoted_key.length();
            expect(':');
        }
    }
};

// --- Data Structures ---
struct Bin { int W, H; bool allow_rotate; };
struct ItemType { std::string id; int w, h, v, limit; double density; };
struct Placement { std::string type; int x, y, rot; };

void parse_input(Bin& bin, std::vector<ItemType>& items, SimpleJsonParser& parser) {
    parser.expect('{');
    parser.find_key("bin");
    parser.expect('{');
    parser.find_key("W"); bin.W = parser.read_long();
    parser.find_key("H"); bin.H = parser.read_long();
    parser.find_key("allow_rotate"); bin.allow_rotate = parser.read_bool();
    parser.expect('}');
    
    parser.find_key("items");
    parser.expect('[');
    while (true) {
        parser.skip_ws();
        if (parser.data[parser.pos] == ']') break;
        parser.expect('{');
        
        ItemType item;
        parser.find_key("type"); item.id = parser.read_string();
        parser.find_key("w"); item.w = parser.read_long();
        parser.find_key("h"); item.h = parser.read_long();
        parser.find_key("v"); item.v = parser.read_long();
        parser.find_key("limit"); item.limit = parser.read_long();
        item.density = (item.w > 0 && item.h > 0) ? (double)item.v / ((long long)item.w * item.h) : 0;
        items.push_back(item);
        
        parser.expect('}');
        parser.skip_ws();
        if (parser.data[parser.pos] == ',') parser.pos++;
        else break;
    }
    parser.expect(']');
    parser.expect('}');
}

// --- Segment Tree ---
std::vector<int> tree, lazy;
int seg_tree_size;
void push(int node, int start, int end) {
    if (lazy[node] != -1) {
        tree[node] = lazy[node];
        if (start != end) {
            lazy[2 * node] = lazy[node];
            lazy[2 * node + 1] = lazy[node];
        }
        lazy[node] = -1;
    }
}
void update_range(int node, int start, int end, int l, int r, int val) {
    push(node, start, end);
    if (start > end || start > r || end < l) return;
    if (l <= start && end <= r) {
        lazy[node] = val;
        push(node, start, end);
        return;
    }
    int mid = start + (end - start) / 2;
    update_range(2 * node, start, mid, l, r, val);
    update_range(2 * node + 1, mid + 1, end, l, r, val);
    push(2*node, start, mid); push(2*node+1, mid+1, end);
    tree[node] = std::max(tree[2 * node], tree[2 * node + 1]);
}
int query_range(int node, int start, int end, int l, int r) {
    if (start > end || start > r || end < l) return 0;
    push(node, start, end);
    if (l <= start && end <= r) return tree[node];
    int mid = start + (end - start) / 2;
    int p1 = query_range(2 * node, start, mid, l, r);
    int p2 = query_range(2 * node + 1, mid + 1, end, l, r);
    return std::max(p1, p2);
}

// --- Solver ---
struct Solver {
    Bin bin;
    const std::vector<ItemType>& item_types;
    std::vector<int> type_indices;

    Solver(Bin b, const std::vector<ItemType>& it) : bin(b), item_types(it) {}

    std::pair<long long, std::vector<Placement>> solve() {
        std::vector<Placement> placements;
        long long total_profit = 0;
        std::vector<int> remaining_limits(item_types.size());
        for (size_t i = 0; i < item_types.size(); ++i) remaining_limits[i] = item_types[i].limit;

        seg_tree_size = bin.W;
        tree.assign(4 * seg_tree_size, 0);
        lazy.assign(4 * seg_tree_size, -1);
        
        std::set<int> x_coords; x_coords.insert(0);

        while (!time_limit_exceeded()) {
            int best_type_idx = -1, best_rot = -1, best_x = -1;
            int min_y = bin.H + 1;

            for (int idx : type_indices) {
                if (remaining_limits[idx] == 0) continue;
                for (int rot = 0; rot < (bin.allow_rotate ? 2 : 1); ++rot) {
                    int w = (rot == 0) ? item_types[idx].w : item_types[idx].h;
                    int h = (rot == 0) ? item_types[idx].h : item_types[idx].w;
                    if (item_types[idx].w == item_types[idx].h && rot == 1) continue;
                    if (w > bin.W || h > bin.H) continue;
                    
                    std::vector<int> candidate_x_vec;
                    candidate_x_vec.reserve(2 * x_coords.size());
                    for (int xc : x_coords) {
                        candidate_x_vec.push_back(xc);
                        if (xc >= w) candidate_x_vec.push_back(xc - w);
                    }
                    std::sort(candidate_x_vec.begin(), candidate_x_vec.end());
                    candidate_x_vec.erase(std::unique(candidate_x_vec.begin(), candidate_x_vec.end()), candidate_x_vec.end());

                    for (int x : candidate_x_vec) {
                        if (x < 0 || x + w > bin.W) continue;
                        int y_base = query_range(1, 0, seg_tree_size - 1, x, x + w - 1);
                        if (y_base + h <= bin.H && y_base < min_y) {
                            min_y = y_base; best_type_idx = idx; best_rot = rot; best_x = x;
                        }
                    }
                }
            }
            if (best_type_idx != -1) {
                int w = (best_rot == 0) ? item_types[best_type_idx].w : item_types[best_type_idx].h;
                int h = (best_rot == 0) ? item_types[best_type_idx].h : item_types[best_type_idx].w;
                update_range(1, 0, seg_tree_size - 1, best_x, best_x + w - 1, min_y + h);
                placements.push_back({item_types[best_type_idx].id, best_x, min_y, best_rot});
                total_profit += item_types[best_type_idx].v;
                remaining_limits[best_type_idx]--;
                x_coords.insert(best_x); if (best_x + w <= bin.W) x_coords.insert(best_x + w);
            } else break;
        }
        return {total_profit, placements};
    }
};

int main() {
    std::ios_base::sync_with_stdio(false); std::cin.tie(NULL);
    SimpleJsonParser parser(std::cin);
    Bin bin;
    std::vector<ItemType> items;
    parse_input(bin, items, parser);
    
    std::vector<Placement> best_placements;
    long long best_profit = -1;

    std::vector<std::function<bool(const ItemType&, const ItemType&)>> sorters = {
        [](const ItemType& a, const ItemType& b){ return a.density > b.density; },
        [](const ItemType& a, const ItemType& b){ return a.v > b.v; },
        [](const ItemType& a, const ItemType& b){ return (long long)a.w * a.h > (long long)b.w * b.h; },
        [](const ItemType& a, const ItemType& b){ return (long long)a.w * a.h < (long long)b.w * b.h; },
        [](const ItemType& a, const ItemType& b){ return std::max(a.w, a.h) > std::max(b.w, b.h); },
        [](const ItemType& a, const ItemType& b){ return std::min(a.w, a.h) > std::min(b.w, b.h); },
    };
    
    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

    for (int i = 0; i < 2; ++i) {
        if (time_limit_exceeded()) break;
        Bin current_bin = bin;
        std::vector<ItemType> current_items = items;
        bool transposed = (i == 1);
        if (transposed) {
            std::swap(current_bin.W, current_bin.H);
            for(auto& item : current_items) std::swap(item.w, item.h);
        }
        
        Solver solver(current_bin, current_items);
        solver.type_indices.resize(items.size());
        std::iota(solver.type_indices.begin(), solver.type_indices.end(), 0);

        for(const auto& sorter : sorters) {
            if (time_limit_exceeded()) break;
            std::sort(solver.type_indices.begin(), solver.type_indices.end(), 
                [&](int a, int b){ return sorter(current_items[a], current_items[b]); });
            auto result = solver.solve();
            if (result.first > best_profit) {
                best_profit = result.first;
                best_placements = result.second;
                if (transposed) for(auto& p : best_placements) std::swap(p.x, p.y);
            }
        }
        if (time_limit_exceeded()) break;

        while (!time_limit_exceeded()) {
            std::shuffle(solver.type_indices.begin(), solver.type_indices.end(), rng);
            auto result = solver.solve();
            if (result.first > best_profit) {
                best_profit = result.first;
                best_placements = result.second;
                if (transposed) for(auto& p : best_placements) std::swap(p.x, p.y);
            }
        }
    }

    std::cout << "{\n  \"placements\": [\n";
    for (size_t i = 0; i < best_placements.size(); ++i) {
        const auto& p = best_placements[i];
        std::cout << "    {\"type\": \"" << p.type << "\", \"x\": " << p.x << ", \"y\": " << p.y << ", \"rot\": " << p.rot << "}"
                  << (i == best_placements.size() - 1 ? "" : ",") << "\n";
    }
    std::cout << "  ]\n}\n";

    return 0;
}