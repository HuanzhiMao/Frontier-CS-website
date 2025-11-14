#include <bits/stdc++.h>
using namespace std;

struct JSONValue {
    enum Type {NUL, BOOL, NUM, STR, ARR, OBJ} type = NUL;
    bool b = false;
    long long num = 0;
    string str;
    vector<JSONValue> arr;
    unordered_map<string, JSONValue> obj;
};

struct JSONParser {
    string s;
    size_t i = 0;

    JSONParser(const string& str) : s(str), i(0) {}

    void skip_ws() {
        while (i < s.size() && (s[i]==' ' || s[i]=='\n' || s[i]=='\t' || s[i]=='\r')) ++i;
    }

    bool match(const string& t) {
        skip_ws();
        if (s.compare(i, t.size(), t) == 0) { i += t.size(); return true; }
        return false;
    }

    char peek() { skip_ws(); return i < s.size() ? s[i] : '\0'; }
    char get() { return i < s.size() ? s[i++] : '\0'; }

    JSONValue parse_value() {
        skip_ws();
        if (i >= s.size()) return JSONValue();
        char c = s[i];
        if (c == '{') return parse_object();
        if (c == '[') return parse_array();
        if (c == '"') return parse_string();
        if (c == 't' || c == 'f') return parse_bool();
        if (c == '-' || (c >= '0' && c <= '9')) return parse_number();
        // null or invalid
        if (match("null")) {
            JSONValue v; v.type = JSONValue::NUL; return v;
        }
        // Fallback
        JSONValue v; v.type = JSONValue::NUL; return v;
    }

    JSONValue parse_object() {
        JSONValue v; v.type = JSONValue::OBJ;
        get(); // {
        skip_ws();
        if (peek() == '}') { get(); return v; }
        while (true) {
            skip_ws();
            JSONValue key = parse_string();
            skip_ws();
            if (get() != ':') { /* invalid */ }
            JSONValue val = parse_value();
            v.obj.emplace(key.str, std::move(val));
            skip_ws();
            char c = get();
            if (c == '}') break;
            if (c != ',') { /* invalid */ break; }
        }
        return v;
    }

    JSONValue parse_array() {
        JSONValue v; v.type = JSONValue::ARR;
        get(); // [
        skip_ws();
        if (peek() == ']') { get(); return v; }
        while (true) {
            JSONValue elem = parse_value();
            v.arr.emplace_back(std::move(elem));
            skip_ws();
            char c = get();
            if (c == ']') break;
            if (c != ',') { /* invalid */ break; }
        }
        return v;
    }

    JSONValue parse_string() {
        JSONValue v; v.type = JSONValue::STR;
        if (get() != '"') return v;
        string out;
        while (i < s.size()) {
            char c = get();
            if (c == '"') break;
            if (c == '\\') {
                char e = get();
                if (e == '"' || e == '\\' || e == '/') out.push_back(e);
                else if (e == 'b') out.push_back('\b');
                else if (e == 'f') out.push_back('\f');
                else if (e == 'n') out.push_back('\n');
                else if (e == 'r') out.push_back('\r');
                else if (e == 't') out.push_back('\t');
                else if (e == 'u') {
                    // parse 4 hex digits
                    int code = 0;
                    for (int k=0;k<4 && i<s.size();++k) {
                        char h = get();
                        code <<= 4;
                        if (h >= '0' && h <= '9') code += h - '0';
                        else if (h >= 'a' && h <= 'f') code += 10 + (h - 'a');
                        else if (h >= 'A' && h <= 'F') code += 10 + (h - 'A');
                        else { /* invalid */ }
                    }
                    // naive UTF-16 to UTF-8 conversion for BMP only
                    if (code <= 0x7F) out.push_back((char)code);
                    else if (code <= 0x7FF) {
                        out.push_back((char)(0xC0 | ((code >> 6) & 0x1F)));
                        out.push_back((char)(0x80 | (code & 0x3F)));
                    } else {
                        out.push_back((char)(0xE0 | ((code >> 12) & 0x0F)));
                        out.push_back((char)(0x80 | ((code >> 6) & 0x3F)));
                        out.push_back((char)(0x80 | (code & 0x3F)));
                    }
                } else {
                    out.push_back(e);
                }
            } else {
                out.push_back(c);
            }
        }
        v.str = std::move(out);
        return v;
    }

    JSONValue parse_bool() {
        JSONValue v; v.type = JSONValue::BOOL;
        if (match("true")) { v.b = true; return v; }
        match("false"); v.b = false; return v;
    }

    JSONValue parse_number() {
        JSONValue v; v.type = JSONValue::NUM;
        bool neg = false;
        if (peek()=='-') { neg = true; get(); }
        long long n = 0;
        while (i < s.size() && isdigit((unsigned char)s[i])) {
            n = n*10 + (s[i]-'0');
            ++i;
        }
        v.num = neg ? -n : n;
        return v;
    }
};

static string json_escape_string(const string& s) {
    string out;
    out.reserve(s.size() + 8);
    out.push_back('"');
    for (char c : s) {
        switch (c) {
            case '"': out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\b': out += "\\b"; break;
            case '\f': out += "\\f"; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default:
                if ((unsigned char)c < 0x20) {
                    char buf[7];
                    snprintf(buf, sizeof(buf), "\\u%04x", (unsigned char)c);
                    out += buf;
                } else {
                    out.push_back(c);
                }
        }
    }
    out.push_back('"');
    return out;
}

struct ItemType {
    string id;
    int w, h;
    long long v;
    int limit;
};

struct Variant {
    int itemIdx;
    int w, h;
    bool rot; // 1 if rotated
    double density; // v / (w*h)
    double vw; // v / w
};

struct SplItem {
    int varIdx;
    int cnt;
    int weight;
    long long value;
};

struct Placement {
    string type;
    int x, y;
    int rot;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Read entire input
    string input((istreambuf_iterator<char>(cin)), istreambuf_iterator<char>());
    JSONParser parser(input);
    JSONValue root = parser.parse_value();

    // Extract bin
    int W = 0, H = 0;
    bool allow_rotate = false;

    auto it_bin = root.obj.find("bin");
    if (it_bin != root.obj.end() && it_bin->second.type == JSONValue::OBJ) {
        auto &bin = it_bin->second.obj;
        if (bin.find("W") != bin.end()) W = (int)bin["W"].num;
        if (bin.find("H") != bin.end()) H = (int)bin["H"].num;
        if (bin.find("allow_rotate") != bin.end()) allow_rotate = bin["allow_rotate"].b;
    }

    // Extract items
    vector<ItemType> items;
    auto it_items = root.obj.find("items");
    if (it_items != root.obj.end() && it_items->second.type == JSONValue::ARR) {
        for (const auto& jv : it_items->second.arr) {
            if (jv.type != JSONValue::OBJ) continue;
            ItemType it;
            const auto &o = jv.obj;
            auto g = [&](const string& k)->const JSONValue& {
                static JSONValue dummy;
                auto itf = o.find(k);
                if (itf == o.end()) return dummy;
                return itf->second;
            };
            it.id = g("type").str;
            it.w = (int)g("w").num;
            it.h = (int)g("h").num;
            it.v = g("v").num;
            it.limit = (int)g("limit").num;
            items.push_back(it);
        }
    }

    int M = (int)items.size();
    if (M == 0 || W <= 0 || H <= 0) {
        cout << "{\"placements\":[]}\n";
        return 0;
    }

    // Build variants
    vector<Variant> variants;
    variants.reserve(M * 2);
    for (int i = 0; i < M; ++i) {
        // base
        Variant a;
        a.itemIdx = i;
        a.w = items[i].w;
        a.h = items[i].h;
        a.rot = false;
        a.density = (double)items[i].v / (double)(max(1, a.w) * max(1, a.h));
        a.vw = (double)items[i].v / (double)max(1, a.w);
        variants.push_back(a);
        if (allow_rotate && items[i].w != items[i].h) {
            Variant b;
            b.itemIdx = i;
            b.w = items[i].h;
            b.h = items[i].w;
            b.rot = true;
            b.density = (double)items[i].v / (double)(max(1, b.w) * max(1, b.h));
            b.vw = (double)items[i].v / (double)max(1, b.w);
            variants.push_back(b);
        }
    }

    // Group variants by shelf height (their h)
    unordered_map<int, vector<int>> height_groups;
    height_groups.reserve(variants.size()*2);
    vector<int> unique_heights;
    {
        unordered_set<int> seen;
        for (int idx = 0; idx < (int)variants.size(); ++idx) {
            int hgt = variants[idx].h;
            if (hgt <= 0 || hgt > H) continue; // skip impossible heights
            height_groups[hgt].push_back(idx);
            if (!seen.count(hgt)) { unique_heights.push_back(hgt); seen.insert(hgt); }
        }
        sort(unique_heights.begin(), unique_heights.end());
    }

    vector<int> remaining(M);
    for (int i = 0; i < M; ++i) remaining[i] = max(0, items[i].limit);

    vector<Placement> placements;
    placements.reserve(10000);

    auto choose_best_height = [&](int y_rem) -> int {
        double best_score = -1.0;
        int best_h = -1;
        for (int hgt : unique_heights) {
            if (hgt > y_rem) continue;
            auto itg = height_groups.find(hgt);
            if (itg == height_groups.end()) continue;
            const auto &grp = itg->second;
            double best_r = 0.0;
            long long fill = 0;
            bool any = false;
            for (int vidx : grp) {
                const Variant &v = variants[vidx];
                int rem = remaining[v.itemIdx];
                if (rem <= 0) continue;
                if (v.w > W) continue;
                any = true;
                best_r = max(best_r, (double)items[v.itemIdx].v / (double)v.w);
                long long can = 1LL * min(rem, W / v.w) * v.w;
                fill += can;
                if (fill >= W) { fill = W; break; }
            }
            if (!any) continue;
            double fill_frac = (double)fill / (double)W;
            double score = best_r * (0.5 + 0.5 * fill_frac); // slight preference for fuller shelves
            if (score > best_score) {
                best_score = score;
                best_h = hgt;
            }
        }
        return best_h;
    };

    int curY = 0;
    while (curY < H) {
        int y_rem = H - curY;
        int chosen_h = choose_best_height(y_rem);
        if (chosen_h < 0) break;

        // Build candidates for this shelf height
        const auto &grp = height_groups[chosen_h];
        vector<int> cand;
        cand.reserve(grp.size());
        for (int vidx : grp) {
            const Variant &v = variants[vidx];
            if (v.h != chosen_h) continue;
            if (v.w > W) continue;
            if (remaining[v.itemIdx] <= 0) continue;
            cand.push_back(vidx);
        }
        if (cand.empty()) {
            // can't place anything for this height; mark height unusable by removing from groups? just break to avoid infinite loop
            bool any_other = false;
            for (int h2 : unique_heights) {
                if (h2 <= y_rem) {
                    const auto &g2 = height_groups[h2];
                    for (int vidx : g2) {
                        if (variants[vidx].w <= W && remaining[variants[vidx].itemIdx] > 0) { any_other = true; break; }
                    }
                }
                if (any_other) break;
            }
            if (!any_other) break;
            // Otherwise pick next best height next loop
            // But to prevent infinite loop, remove this height temporarily: decrease y_rem by 0? We'll just break as no candidates.
            break;
        }

        // Split items by binary decomposition for bounded knapsack
        vector<SplItem> items_split;
        items_split.reserve(256);
        for (int vidx : cand) {
            const Variant &v = variants[vidx];
            int rem = remaining[v.itemIdx];
            int maxCopies = min(rem, W / v.w);
            if (maxCopies <= 0) continue;
            int k = 1;
            while (maxCopies > 0) {
                int take = min(k, maxCopies);
                SplItem sp;
                sp.varIdx = vidx;
                sp.cnt = take;
                sp.weight = v.w * take;
                sp.value = items[v.itemIdx].v * 1LL * take;
                items_split.push_back(sp);
                maxCopies -= take;
                k <<= 1;
            }
        }
        if (items_split.empty()) {
            // No feasible items; stop
            break;
        }

        // 0/1 knapsack by width up to W
        const long long NEG_INF = LLONG_MIN / 4;
        vector<long long> dp(W + 1, NEG_INF);
        vector<int> from(W + 1, -1);
        vector<int> prevW(W + 1, -1);
        dp[0] = 0;

        for (int idx = 0; idx < (int)items_split.size(); ++idx) {
            int wt = items_split[idx].weight;
            long long val = items_split[idx].value;
            for (int w = W; w >= wt; --w) {
                if (dp[w - wt] != NEG_INF) {
                    long long candv = dp[w - wt] + val;
                    if (candv > dp[w]) {
                        dp[w] = candv;
                        from[w] = idx;
                        prevW[w] = w - wt;
                    }
                }
            }
        }

        // find best w
        long long bestV = 0;
        int bestW = 0;
        for (int w = 0; w <= W; ++w) {
            if (dp[w] > bestV) {
                bestV = dp[w];
                bestW = w;
            }
        }
        if (bestV <= 0 || bestW == 0) {
            // nothing can be placed for this height -> try to find another height
            // Check if any other height possible; if not, break
            bool any_other = false;
            for (int h2 : unique_heights) {
                if (h2 <= y_rem) {
                    const auto &g2 = height_groups[h2];
                    for (int vidx : g2) {
                        if (variants[vidx].w <= W && remaining[variants[vidx].itemIdx] > 0) { any_other = true; break; }
                    }
                }
                if (any_other) break;
            }
            if (!any_other) break;
            // else, attempt pick another height next iteration by continuing loop
            // To avoid infinite loop here, we will break out of while (curY<H) because chosen_h can't place anything.
            break;
        }

        // reconstruct chosen counts per variant
        unordered_map<int,int> chosenCounts; // varIdx -> count
        int ww = bestW;
        while (ww > 0 && from[ww] != -1) {
            int id = from[ww];
            const SplItem &sp = items_split[id];
            chosenCounts[sp.varIdx] += sp.cnt;
            ww = prevW[ww];
        }
        if (chosenCounts.empty()) {
            break;
        }

        // Place along the shelf left-to-right
        int curX = 0;
        // For deterministic order, sort variants by value per width descending then width descending
        vector<pair<double,int>> order;
        order.reserve(chosenCounts.size());
        for (auto &p : chosenCounts) {
            int vidx = p.first;
            order.emplace_back(variants[vidx].vw, vidx);
        }
        sort(order.begin(), order.end(), [&](const pair<double,int>& A, const pair<double,int>& B){
            if (fabs(A.first - B.first) > 1e-12) return A.first > B.first;
            const Variant &va = variants[A.second];
            const Variant &vb = variants[B.second];
            if (va.w != vb.w) return va.w > vb.w;
            if (items[va.itemIdx].v != items[vb.itemIdx].v) return items[va.itemIdx].v > items[vb.itemIdx].v;
            return A.second < B.second;
        });

        for (auto &pr : order) {
            int vidx = pr.second;
            int cnt = chosenCounts[vidx];
            const Variant &v = variants[vidx];
            int use = min(cnt, (W - curX) / v.w);
            if (use <= 0) continue;
            for (int k = 0; k < use; ++k) {
                Placement pl;
                pl.type = items[v.itemIdx].id;
                pl.x = curX;
                pl.y = curY;
                pl.rot = v.rot ? 1 : 0;
                placements.push_back(pl);
                curX += v.w;
            }
            remaining[v.itemIdx] -= use;
        }

        // advance shelf
        curY += chosen_h;
        if (curY > H) break;
        if (curY == H) break;

        // If no items remain at all, break
        bool any_remain = false;
        for (int i = 0; i < M; ++i) {
            if (remaining[i] > 0) { any_remain = true; break; }
        }
        if (!any_remain) break;
    }

    // Output JSON placements
    cout << "{\n  \"placements\": [\n";
    for (size_t i = 0; i < placements.size(); ++i) {
        const auto &p = placements[i];
        cout << "    {\"type\":" << json_escape_string(p.type)
             << ",\"x\":" << p.x
             << ",\"y\":" << p.y
             << ",\"rot\":" << p.rot << "}";
        if (i + 1 != placements.size()) cout << ",";
        cout << "\n";
    }
    cout << "  ]\n}\n";

    return 0;
}