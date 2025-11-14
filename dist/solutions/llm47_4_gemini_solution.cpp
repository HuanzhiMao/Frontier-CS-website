#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
#include <functional>
#include <nlohmann/json.hpp>

// Fast I/O
static const bool FAST_IO = []() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    return false;
}();

using json = nlohmann::json;

struct Item {
    std::string id;
    int w, h;
    long long v;
    int limit;
    int original_idx;
};

struct Placement {
    std::string id;
    int x, y, rot;
    long long v;
};

// Segment tree for skyline maintenance
// Supports range max update and range max query
struct SegTree {
    int W;
    std::vector<int> tree;
    std::vector<int> lazy;

    SegTree(int width) : W(width) {
        tree.assign(4 * W, 0);
        lazy.assign(4 * W, 0);
    }

    void push(int v) {
        if (lazy[v] > 0 && v * 2 + 1 < tree.size()) {
            tree[2*v] = std::max(tree[2*v], lazy[v]);
            lazy[2*v] = std::max(lazy[2*v], lazy[v]);
            tree[2*v+1] = std::max(tree[2*v+1], lazy[v]);
            lazy[2*v+1] = std::max(lazy[2*v+1], lazy[v]);
            lazy[v] = 0;
        }
    }

    void update(int v, int tl, int tr, int l, int r, int new_h) {
        if (l > r) return;
        if (l == tl && r == tr) {
            tree[v] = std::max(tree[v], new_h);
            lazy[v] = std::max(lazy[v], new_h);
        } else {
            push(v);
            int tm = tl + (tr - tl) / 2;
            update(v*2, tl, tm, l, std::min(r, tm), new_h);
            update(v*2+1, tm+1, tr, std::max(l, tm+1), r, new_h);
            tree[v] = std::max(tree[v*2], tree[v*2+1]);
        }
    }

    void update(int l, int r, int new_h) {
        if (l > r) return;
        update(1, 0, W - 1, l, r, new_h);
    }

    int query(int v, int tl, int tr, int l, int r) {
        if (l > r) return 0;
        if (l <= tl && tr <= r) return tree[v];
        push(v);
        int tm = tl + (tr - tl) / 2;
        int left_max = query(v*2, tl, tm, l, std::min(r, tm));
        int right_max = query(v*2+1, tm+1, tr, std::max(l, tm+1), r);
        return std::max(left_max, right_max);
    }

    int query(int l, int r) {
        if (l > r) return 0;
        return query(1, 0, W - 1, l, r);
    }
};

struct PlacementResult {
    bool possible;
    int x, y;
};

PlacementResult find_best_placement(int item_w, int item_h, int W, int H, SegTree& segtree, const std::set<int>& candidate_x_coords) {
    int best_y = H + 1, best_x = W + 1;
    bool possible = false;

    for (int x_cand : candidate_x_coords) {
        // Case 1: item starts at x_cand (left-aligned)
        int x = x_cand;
        if (x + item_w <= W) {
            int y_base = segtree.query(x, x + item_w - 1);
            if (y_base + item_h <= H) {
                if (y_base < best_y || (y_base == best_y && x < best_x)) {
                    best_y = y_base;
                    best_x = x;
                    possible = true;
                }
            }
        }
        
        // Case 2: item ends at x_cand (right-aligned)
        x = x_cand - item_w;
        if (x >= 0) {
            int y_base = segtree.query(x, x + item_w - 1);
            if (y_base + item_h <= H) {
                if (y_base < best_y || (y_base == best_y && x < best_x)) {
                    best_y = y_base;
                    best_x = x;
                    possible = true;
                }
            }
        }
    }

    if(possible) return {true, best_x, best_y};
    return {false, -1, -1};
}

std::vector<Placement> solve(int W, int H, bool allow_rotate, const std::vector<Item>& items_orig, const std::function<bool(const Item&, const Item&)>& sorter) {
    std::vector<Item> items = items_orig;
    std::sort(items.begin(), items.end(), sorter);

    std::vector<int> limits(items_orig.size());
    for(const auto& item : items_orig) {
        limits[item.original_idx] = item.limit;
    }

    SegTree segtree(W);
    std::set<int> candidate_x_coords = {0};
    std::vector<Placement> placements;

    bool progress = true;
    while (progress) {
        progress = false;
        for (const auto& item : items) {
            while (limits[item.original_idx] > 0) {
                PlacementResult res1 = find_best_placement(item.w, item.h, W, H, segtree, candidate_x_coords);
                
                PlacementResult res2 = {false, -1, -1};
                if (allow_rotate && item.w != item.h) {
                    res2 = find_best_placement(item.h, item.w, W, H, segtree, candidate_x_coords);
                }

                bool place_item = false;
                int final_x, final_y, final_w, final_h, rot;

                if (res1.possible && res2.possible) {
                    if (res1.y < res2.y || (res1.y == res2.y && res1.x <= res2.x)) {
                        final_x = res1.x; final_y = res1.y; final_w = item.w; final_h = item.h; rot = 0;
                    } else {
                        final_x = res2.x; final_y = res2.y; final_w = item.h; final_h = item.w; rot = 1;
                    }
                    place_item = true;
                } else if (res1.possible) {
                    final_x = res1.x; final_y = res1.y; final_w = item.w; final_h = item.h; rot = 0;
                    place_item = true;
                } else if (res2.possible) {
                    final_x = res2.x; final_y = res2.y; final_w = item.h; final_h = item.w; rot = 1;
                    place_item = true;
                }

                if (place_item) {
                    segtree.update(final_x, final_x + final_w - 1, final_y + final_h);
                    candidate_x_coords.insert(final_x);
                    if (final_x + final_w <= W) {
                        candidate_x_coords.insert(final_x + final_w);
                    }
                    placements.push_back({item.id, final_x, final_y, rot, item.v});
                    limits[item.original_idx]--;
                    progress = true;
                } else {
                    break;
                }
            }
        }
    }
    return placements;
}

int main() {
    json j;
    std::cin >> j;

    int W = j["bin"]["W"];
    int H = j["bin"]["H"];
    bool allow_rotate = j["bin"]["allow_rotate"];

    std::vector<Item> items;
    int original_idx_counter = 0;
    for (const auto& item_json : j["items"]) {
        items.push_back({
            item_json["type"],
            (int)item_json["w"],
            (int)item_json["h"],
            (long long)item_json["v"],
            (int)item_json["limit"],
            original_idx_counter++
        });
    }

    std::vector<std::function<bool(const Item&, const Item&)>> sorters;
    sorters.push_back([](const Item& a, const Item& b) { return (double)a.v / (1.0 * a.w * a.h) > (double)b.v / (1.0 * b.w * b.h); });
    sorters.push_back([](const Item& a, const Item& b) { return a.v > b.v; });
    sorters.push_back([](const Item& a, const Item& b) { return a.h < b.h; });
    sorters.push_back([](const Item& a, const Item& b) { return a.h > b.h; });
    sorters.push_back([](const Item& a, const Item& b) { return a.w < b.w; });
    sorters.push_back([](const Item& a, const Item& b) { return a.w > b.w; });
    sorters.push_back([](const Item& a, const Item& b) { return 1LL*a.w*a.h < 1LL*b.w*b.h; });
    sorters.push_back([](const Item& a, const Item& b) { return 1LL*a.w*a.h > 1LL*b.w*b.h; });
    sorters.push_back([](const Item& a, const Item& b) { return std::max(a.w, a.h) < std::max(b.w, b.h); });
    sorters.push_back([](const Item& a, const Item& b) { return std::max(a.w, a.h) > std::max(b.w, b.h); });
    sorters.push_back([](const Item& a, const Item& b) { return std::min(a.w, a.h) < std::min(b.w, b.h); });
    sorters.push_back([](const Item& a, const Item& b) { return std::min(a.w, a.h) > std::min(b.w, b.h); });

    std::vector<Placement> best_placements;
    long long max_profit = -1;

    for (const auto& sorter : sorters) {
        std::vector<Placement> current_placements = solve(W, H, allow_rotate, items, sorter);
        long long current_profit = 0;
        for (const auto& p : current_placements) {
            current_profit += p.v;
        }
        if (current_profit > max_profit) {
            max_profit = current_profit;
            best_placements = current_placements;
        }
    }

    json out_json;
    json placements_json = json::array();
    for (const auto& p : best_placements) {
        placements_json.push_back({
            {"type", p.id},
            {"x", p.x},
            {"y", p.y},
            {"rot", p.rot}
        });
    }
    out_json["placements"] = placements_json;

    std::cout << out_json.dump() << std::endl;

    return 0;
}