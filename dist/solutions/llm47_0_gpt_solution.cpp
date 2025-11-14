#include <bits/stdc++.h>
using namespace std;

// Minimal JSON parser tailored for the problem input structure

struct JSONParser {
    string s;
    size_t i = 0;

    JSONParser(const string& str) : s(str), i(0) {}

    void skip_ws() {
        while (i < s.size() && (s[i] == ' ' || s[i] == '\n' || s[i] == '\r' || s[i] == '\t')) i++;
    }

    bool consume(char c) {
        skip_ws();
        if (i < s.size() && s[i] == c) { i++; return true; }
        return false;
    }

    void expect(char c) {
        if (!consume(c)) {
            // Attempt to be robust: if fails, try to proceed
            // but generally input is valid per problem statement
        }
    }

    string parse_string() {
        skip_ws();
        string res;
        if (i >= s.size() || s[i] != '"') return res;
        i++;
        while (i < s.size()) {
            char c = s[i++];
            if (c == '"') break;
            if (c == '\\') {
                if (i >= s.size()) break;
                char e = s[i++];
                switch (e) {
                    case '"': res.push_back('"'); break;
                    case '\\': res.push_back('\\'); break;
                    case '/': res.push_back('/'); break;
                    case 'b': res.push_back('\b'); break;
                    case 'f': res.push_back('\f'); break;
                    case 'n': res.push_back('\n'); break;
                    case 'r': res.push_back('\r'); break;
                    case 't': res.push_back('\t'); break;
                    case 'u': {
                        // Parse 4 hex digits; convert basic ASCII if possible
                        int val = 0;
                        for (int k = 0; k < 4 && i < s.size(); ++k) {
                            char h = s[i++];
                            val <<= 4;
                            if (h >= '0' && h <= '9') val |= (h - '0');
                            else if (h >= 'a' && h <= 'f') val |= (h - 'a' + 10);
                            else if (h >= 'A' && h <= 'F') val |= (h - 'A' + 10);
                            else { /* invalid, ignore */ }
                        }
                        if (val >= 0 && val <= 0x7F) res.push_back((char)val);
                        else res.push_back('?');
                        break;
                    }
                    default: res.push_back(e); break;
                }
            } else {
                res.push_back(c);
            }
        }
        return res;
    }

    long long parse_int() {
        skip_ws();
        long long sign = 1;
        if (i < s.size() && s[i] == '-') { sign = -1; i++; }
        long long val = 0;
        while (i < s.size() && isdigit((unsigned char)s[i])) {
            val = val * 10 + (s[i] - '0');
            i++;
        }
        return sign * val;
    }

    bool parse_bool() {
        skip_ws();
        if (i + 3 < s.size() && s.substr(i, 4) == "true") { i += 4; return true; }
        if (i + 4 < s.size() && s.substr(i, 5) == "false") { i += 5; return false; }
        return false;
    }
};

// Data structures
struct ItemType {
    string id;
    int w, h;
    long long v;
    int limit;
    double density;
    int idx;
};

struct Placement {
    int typeIdx;
    int x, y;
    int rot; // 0 or 1
};

struct Node {
    int x, y, w;
};

struct Candidate {
    bool valid = false;
    int typeIdx = -1;
    int rot = 0;
    int x = 0, y = 0, w = 0, h = 0;
    int nodeIdx = -1;
    double density = 0.0;
    long long v = 0;
};

// Skyline packer
class SkylinePacker {
public:
    int W, H;
    bool allow_rotate;
    vector<ItemType> items;
    vector<Node> nodes;
    vector<int> remain;
    vector<Placement> placements;

    SkylinePacker(int W_, int H_, bool allow_rotate_, const vector<ItemType>& items_)
        : W(W_), H(H_), allow_rotate(allow_rotate_), items(items_) {
        reset();
    }

    void reset() {
        nodes.clear();
        nodes.push_back({0, 0, W});
        placements.clear();
        remain.assign(items.size(), 0);
        for (size_t i = 0; i < items.size(); ++i) remain[i] = items[i].limit;
    }

    // Fit function: return y if rectangle (w,h) fits starting at nodes[idx].x, else -1
    int computeMinYAtIndex(int idx, int w, int h) {
        int x = nodes[idx].x;
        if (x + w > W) return -1;
        int widthLeft = w;
        int j = idx;
        int yCandidate = nodes[idx].y;
        while (widthLeft > 0) {
            if (j >= (int)nodes.size()) return -1;
            yCandidate = max(yCandidate, nodes[j].y);
            if (yCandidate + h > H) return -1;
            widthLeft -= nodes[j].w;
            j++;
        }
        return yCandidate;
    }

    // Find best position (lowest y, then lowest x) for (w,h)
    bool findBestPosition(int w, int h, int& outIdx, int& outX, int& outY) {
        int bestY = INT_MAX;
        int bestX = INT_MAX;
        int bestIdx = -1;
        for (int i = 0; i < (int)nodes.size(); ++i) {
            if (nodes[i].x + w > W) continue;
            int y = computeMinYAtIndex(i, w, h);
            if (y >= 0) {
                if (y < bestY || (y == bestY && nodes[i].x < bestX)) {
                    bestY = y;
                    bestX = nodes[i].x;
                    bestIdx = i;
                }
            }
        }
        if (bestIdx == -1) return false;
        outIdx = bestIdx;
        outX = bestX;
        outY = bestY;
        return true;
    }

    void mergeNeighbors() {
        for (size_t k = 0; k + 1 < nodes.size(); ) {
            if (nodes[k].y == nodes[k + 1].y) {
                nodes[k].w += nodes[k + 1].w;
                nodes.erase(nodes.begin() + (k + 1));
            } else {
                ++k;
            }
        }
    }

    void addSkylineLevel(int idx, int x, int y, int w, int h) {
        Node newNode { x, y + h, w };
        nodes.insert(nodes.begin() + idx, newNode);

        // Fix overlapping nodes to the right
        for (size_t k = idx + 1; k < nodes.size(); ) {
            int overlap = nodes[k - 1].x + nodes[k - 1].w - nodes[k].x;
            if (overlap <= 0) break;
            if (nodes[k].w > overlap) {
                nodes[k].x += overlap;
                nodes[k].w -= overlap;
                break;
            } else {
                nodes.erase(nodes.begin() + k);
            }
        }
        mergeNeighbors();
    }

    enum Policy {
        LOWEST_Y = 0,
        DENSITY_FIRST = 1,
        VALUE_FIRST = 2
    };

    static bool better(const Candidate& a, const Candidate& b, Policy policy) {
        if (!b.valid) return true; // a valid, b invalid
        if (!a.valid) return false;
        switch (policy) {
            case LOWEST_Y:
                if (a.y != b.y) return a.y < b.y;
                if (a.x != b.x) return a.x < b.x;
                if (a.density != b.density) return a.density > b.density;
                if (a.v != b.v) return a.v > b.v;
                if (a.w * a.h != b.w * b.h) return a.w * a.h < b.w * b.h;
                return a.typeIdx < b.typeIdx;
            case DENSITY_FIRST:
                if (a.density != b.density) return a.density > b.density;
                if (a.v != b.v) return a.v > b.v;
                if (a.y != b.y) return a.y < b.y;
                if (a.x != b.x) return a.x < b.x;
                return a.typeIdx < b.typeIdx;
            case VALUE_FIRST:
                if (a.v != b.v) return a.v > b.v;
                if (a.density != b.density) return a.density > b.density;
                if (a.y != b.y) return a.y < b.y;
                if (a.x != b.x) return a.x < b.x;
                return a.typeIdx < b.typeIdx;
        }
        return false;
    }

    vector<Placement> pack_iterative(Policy policy, int maxPlacementsLimit, double timeLimitSec) {
        reset();
        auto tStart = chrono::high_resolution_clock::now();
        int MAX_ITERS = 1000000000; // effectively unlimited; bounded by remain and area
        int iter = 0;

        while (iter < MAX_ITERS) {
            // Time check
            auto tNow = chrono::high_resolution_clock::now();
            double elapsed = chrono::duration<double>(tNow - tStart).count();
            if (elapsed > timeLimitSec) break;

            if ((int)placements.size() >= maxPlacementsLimit) break;

            Candidate best; best.valid = false;

            for (int t = 0; t < (int)items.size(); ++t) {
                if (remain[t] <= 0) continue;

                // orientation 0
                {
                    int w = items[t].w, h = items[t].h;
                    if (w <= W && h <= H) {
                        int idx, x, y;
                        if (findBestPosition(w, h, idx, x, y)) {
                            Candidate cand;
                            cand.valid = true;
                            cand.typeIdx = t;
                            cand.rot = 0;
                            cand.x = x; cand.y = y; cand.w = w; cand.h = h;
                            cand.nodeIdx = idx;
                            cand.density = items[t].density;
                            cand.v = items[t].v;
                            if (better(cand, best, policy)) best = cand;
                        }
                    }
                }
                // orientation 1
                if (allow_rotate) {
                    int w = items[t].h, h = items[t].w;
                    if (w <= W && h <= H) {
                        int idx, x, y;
                        if (findBestPosition(w, h, idx, x, y)) {
                            Candidate cand;
                            cand.valid = true;
                            cand.typeIdx = t;
                            cand.rot = 1;
                            cand.x = x; cand.y = y; cand.w = w; cand.h = h;
                            cand.nodeIdx = idx;
                            cand.density = items[t].density; // same area, same density
                            cand.v = items[t].v;
                            if (better(cand, best, policy)) best = cand;
                        }
                    }
                }
            }

            if (!best.valid) break;

            addSkylineLevel(best.nodeIdx, best.x, best.y, best.w, best.h);
            placements.push_back({best.typeIdx, best.x, best.y, best.rot});
            remain[best.typeIdx]--;
            iter++;
        }

        return placements;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Read entire input
    string input;
    {
        std::ostringstream oss;
        oss << cin.rdbuf();
        input = oss.str();
    }

    JSONParser p(input);

    // Parse top-level object with keys "bin" and "items"
    int W = 0, H = 0;
    bool allow_rotate = false;
    vector<ItemType> items;

    p.skip_ws();
    p.expect('{');

    for (int top = 0; top < 2; ++top) {
        p.skip_ws();
        if (p.consume('}')) break;
        string key = p.parse_string();
        p.skip_ws();
        p.expect(':');
        if (key == "bin") {
            // Parse bin object: { "W": <int>, "H": <int>, "allow_rotate": <bool> }
            p.skip_ws();
            p.expect('{');
            bool gotW = false, gotH = false, gotR = false;
            while (true) {
                p.skip_ws();
                if (p.consume('}')) break;
                string bkey = p.parse_string();
                p.skip_ws();
                p.expect(':');
                if (bkey == "W") {
                    W = (int)p.parse_int(); gotW = true;
                } else if (bkey == "H") {
                    H = (int)p.parse_int(); gotH = true;
                } else if (bkey == "allow_rotate") {
                    allow_rotate = p.parse_bool(); gotR = true;
                } else {
                    // unknown key, try to skip a value robustly
                    // Try to detect value type
                    p.skip_ws();
                    if (p.consume('{')) {
                        int depth = 1;
                        while (depth > 0 && p.i < p.s.size()) {
                            if (p.s[p.i] == '{') depth++;
                            else if (p.s[p.i] == '}') depth--;
                            p.i++;
                        }
                    } else if (p.consume('[')) {
                        int depth = 1;
                        while (depth > 0 && p.i < p.s.size()) {
                            if (p.s[p.i] == '[') depth++;
                            else if (p.s[p.i] == ']') depth--;
                            p.i++;
                        }
                    } else if (p.consume('"')) {
                        while (p.i < p.s.size()) {
                            char c = p.s[p.i++];
                            if (c == '\\') {
                                if (p.i < p.s.size()) p.i++;
                            } else if (c == '"') break;
                        }
                    } else {
                        // number or literal
                        while (p.i < p.s.size() && string(",}] \n\r\t").find(p.s[p.i]) == string::npos) p.i++;
                    }
                }
                p.skip_ws();
                p.consume(',');
            }
            (void)gotW; (void)gotH; (void)gotR;
        } else if (key == "items") {
            // Parse array of item objects
            p.skip_ws();
            p.expect('[');
            bool first = true;
            while (true) {
                p.skip_ws();
                if (p.consume(']')) break;
                p.expect('{');
                ItemType it;
                it.w = it.h = 0;
                it.v = 0;
                it.limit = 0;
                it.id = "";
                while (true) {
                    p.skip_ws();
                    if (p.consume('}')) break;
                    string ik = p.parse_string();
                    p.skip_ws();
                    p.expect(':');
                    if (ik == "type") {
                        it.id = p.parse_string();
                    } else if (ik == "w") {
                        it.w = (int)p.parse_int();
                    } else if (ik == "h") {
                        it.h = (int)p.parse_int();
                    } else if (ik == "v") {
                        it.v = p.parse_int();
                    } else if (ik == "limit") {
                        it.limit = (int)p.parse_int();
                    } else {
                        // skip value
                        p.skip_ws();
                        if (p.consume('{')) {
                            int depth = 1;
                            while (depth > 0 && p.i < p.s.size()) {
                                if (p.s[p.i] == '{') depth++;
                                else if (p.s[p.i] == '}') depth--;
                                p.i++;
                            }
                        } else if (p.consume('[')) {
                            int depth = 1;
                            while (depth > 0 && p.i < p.s.size()) {
                                if (p.s[p.i] == '[') depth++;
                                else if (p.s[p.i] == ']') depth--;
                                p.i++;
                            }
                        } else if (p.consume('"')) {
                            while (p.i < p.s.size()) {
                                char c = p.s[p.i++];
                                if (c == '\\') {
                                    if (p.i < p.s.size()) p.i++;
                                } else if (c == '"') break;
                            }
                        } else {
                            while (p.i < p.s.size() && string(",}] \n\r\t").find(p.s[p.i]) == string::npos) p.i++;
                        }
                    }
                    p.skip_ws();
                    p.consume(',');
                }
                items.push_back(it);
                p.skip_ws();
                p.consume(',');
            }
        } else {
            // skip unknown top-level value
            p.skip_ws();
            if (p.consume('{')) {
                int depth = 1;
                while (depth > 0 && p.i < p.s.size()) {
                    if (p.s[p.i] == '{') depth++;
                    else if (p.s[p.i] == '}') depth--;
                    p.i++;
                }
            } else if (p.consume('[')) {
                int depth = 1;
                while (depth > 0 && p.i < p.s.size()) {
                    if (p.s[p.i] == '[') depth++;
                    else if (p.s[p.i] == ']') depth--;
                    p.i++;
                }
            } else if (p.consume('"')) {
                while (p.i < p.s.size()) {
                    char c = p.s[p.i++];
                    if (c == '\\') {
                        if (p.i < p.s.size()) p.i++;
                    } else if (c == '"') break;
                }
            } else {
                while (p.i < p.s.size() && string(",}] \n\r\t").find(p.s[p.i]) == string::npos) p.i++;
            }
        }
        p.skip_ws();
        p.consume(',');
    }
    // consume trailing
    p.skip_ws();
    p.consume('}');

    // Prepare items
    for (int i = 0; i < (int)items.size(); ++i) {
        items[i].idx = i;
        long long area = 1LL * items[i].w * items[i].h;
        items[i].density = (area > 0 ? (double)items[i].v / (double)area : 0.0);
    }

    SkylinePacker packer(W, H, allow_rotate, items);

    // Time management: total time limit ~1s; keep margin for printing. Use ~0.25s per attempt
    double totalTimeLimit = 0.85; // seconds
    int maxPlacementsLimit = 5000;

    vector<Placement> bestPlacements;
    long long bestProfit = -1;
    auto start = chrono::high_resolution_clock::now();

    vector<SkylinePacker::Policy> policies = {
        SkylinePacker::LOWEST_Y,
        SkylinePacker::DENSITY_FIRST,
        SkylinePacker::VALUE_FIRST
    };

    for (size_t pi = 0; pi < policies.size(); ++pi) {
        auto now = chrono::high_resolution_clock::now();
        double elapsed = chrono::duration<double>(now - start).count();
        double remaining = max(0.05, totalTimeLimit - elapsed);
        double perAttempt = min(remaining, 0.32); // allocate remaining time but cap
        vector<Placement> out = packer.pack_iterative(policies[pi], maxPlacementsLimit, perAttempt);
        long long profit = 0;
        for (const auto& pl : out) profit += items[pl.typeIdx].v;
        if (profit > bestProfit) {
            bestProfit = profit;
            bestPlacements = out;
        }
        now = chrono::high_resolution_clock::now();
        elapsed = chrono::duration<double>(now - start).count();
        if (elapsed > totalTimeLimit) break;
    }

    // Output JSON
    // Format exactly: { "placements": [ { "type": "...", "x": int, "y": int, "rot": 0|1 }, ... ] }
    cout << "{\n  \"placements\": [";
    for (size_t i = 0; i < bestPlacements.size(); ++i) {
        const auto& pl = bestPlacements[i];
        if (i) cout << ",";
        cout << "\n    {\"type\":\"" << items[pl.typeIdx].id << "\",\"x\":" << pl.x << ",\"y\":" << pl.y << ",\"rot\":" << pl.rot << "}";
    }
    if (!bestPlacements.empty()) cout << "\n";
    cout << "  ]\n}\n";

    return 0;
}