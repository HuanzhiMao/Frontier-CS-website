#include <bits/stdc++.h>
using namespace std;

struct Parser {
    string s;
    size_t i = 0;

    Parser(const string& str) : s(str), i(0) {}

    void skip_ws() {
        while (i < s.size() && isspace((unsigned char)s[i])) i++;
    }

    bool match(char c) {
        skip_ws();
        if (i < s.size() && s[i] == c) { i++; return true; }
        return false;
    }

    void expect(char c) {
        skip_ws();
        if (i >= s.size() || s[i] != c) {
            // Simple error handling: try to continue
            // but in contest setting, input is assumed valid
        } else {
            i++;
        }
    }

    string parse_string() {
        skip_ws();
        string res;
        if (i < s.size() && s[i] == '"') {
            i++;
            while (i < s.size()) {
                char c = s[i++];
                if (c == '\\') {
                    if (i < s.size()) {
                        char esc = s[i++];
                        // Handle basic escapes
                        if (esc == '"' || esc == '\\' || esc == '/') res.push_back(esc);
                        else if (esc == 'b') res.push_back('\b');
                        else if (esc == 'f') res.push_back('\f');
                        else if (esc == 'n') res.push_back('\n');
                        else if (esc == 'r') res.push_back('\r');
                        else if (esc == 't') res.push_back('\t');
                        else res.push_back(esc);
                    }
                } else if (c == '"') {
                    break;
                } else {
                    res.push_back(c);
                }
            }
        }
        return res;
    }

    long long parse_int() {
        skip_ws();
        bool neg = false;
        if (i < s.size() && (s[i] == '-' || s[i] == '+')) {
            neg = (s[i] == '-');
            i++;
        }
        long long num = 0;
        while (i < s.size() && isdigit((unsigned char)s[i])) {
            num = num * 10 + (s[i] - '0');
            i++;
        }
        return neg ? -num : num;
    }

    vector<long long> parse_array_of_ints() {
        vector<long long> a;
        expect('[');
        skip_ws();
        if (match(']')) return a;
        while (true) {
            long long x = parse_int();
            a.push_back(x);
            skip_ws();
            if (match(']')) break;
            expect(',');
        }
        return a;
    }

    void parse_object(vector<string>& names, vector<array<long long,4>>& vals) {
        expect('{');
        skip_ws();
        if (match('}')) return;
        while (true) {
            string key = parse_string();
            expect(':');
            vector<long long> arr = parse_array_of_ints();
            array<long long,4> t = {0,0,0,0};
            for (int k = 0; k < 4 && k < (int)arr.size(); ++k) t[k] = arr[k];
            names.push_back(key);
            vals.push_back(t);
            skip_ws();
            if (match('}')) break;
            expect(',');
        }
    }
};

struct Piece {
    int m; // mg
    int l; // microliters
    long long v; // value
    int cat; // category index
    int cnt; // number of original items aggregated
    int id; // unique piece id
};

struct State {
    int m;
    int l;
    long long v;
    int parent; // index in previous level
    bool taken; // whether current piece was taken
};

static const int M_CAP = 20000000; // 20 kg in mg
static const int L_CAP = 25000000; // 25 liters in uL

// Prune dominated states
static vector<State> prune_states(vector<State>& cand) {
    // Sort by mass asc, volume asc, value desc
    sort(cand.begin(), cand.end(), [](const State& a, const State& b){
        if (a.m != b.m) return a.m < b.m;
        if (a.l != b.l) return a.l < b.l;
        return a.v > b.v;
    });

    vector<State> out;
    // env: map from volume to best value for any state with volume <= key
    // We maintain that values are non-decreasing with volume
    map<int, long long> env;
    env.insert({-1, LLONG_MIN}); // sentinel

    auto getBestUpTo = [&](int vol)->long long{
        auto it = env.upper_bound(vol);
        if (it == env.begin()) return LLONG_MIN;
        --it;
        return it->second;
    };

    for (size_t idx = 0; idx < cand.size(); ++idx) {
        State& s = cand[idx];
        long long best = getBestUpTo(s.l);
        if (best >= s.v) {
            continue; // dominated by previous (mass <= current mass)
        }
        // keep s
        out.push_back(s);
        // update env at volume s.l to value s.v
        auto it = env.lower_bound(s.l);
        if (it != env.end() && it->first == s.l) {
            if (it->second < s.v) {
                it->second = s.v;
                auto jt = next(it);
                while (jt != env.end() && jt->second <= s.v) {
                    jt = env.erase(jt);
                }
            }
        } else {
            // get previous best <= s.l is < s.v by earlier check
            // insert new breakpoint
            it = env.insert({s.l, s.v}).first;
            auto jt = next(it);
            while (jt != env.end() && jt->second <= s.v) {
                jt = env.erase(jt);
            }
        }
    }
    return out;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Read entire input
    string input, line;
    {
        std::ostringstream oss;
        oss << cin.rdbuf();
        input = oss.str();
    }

    vector<string> names;
    vector<array<long long,4>> vals; // q, v, m, l
    Parser parser(input);
    parser.parse_object(names, vals);

    int n = (int)names.size();
    if (n == 0) {
        // Empty input: output empty JSON
        cout << "{}\n";
        return 0;
    }

    // Build pieces using binary decomposition
    vector<Piece> pieces;
    pieces.reserve(n * 16);
    int pid = 0;
    for (int i = 0; i < n; ++i) {
        long long q = vals[i][0];
        long long v = vals[i][1];
        long long m = vals[i][2];
        long long l = vals[i][3];
        if (q <= 0) continue;

        long long remain = q;
        long long k = 1;
        while (remain > 0) {
            long long take = min(k, remain);
            long long mm = m * take;
            long long ll = l * take;
            if (mm <= M_CAP && ll <= L_CAP) {
                Piece pc;
                pc.m = (int)mm;
                pc.l = (int)ll;
                pc.v = v * take;
                pc.cat = i;
                pc.cnt = (int)take;
                pc.id = pid++;
                pieces.push_back(pc);
            }
            remain -= take;
            k <<= 1;
        }
    }

    // Sort pieces by a heuristic to potentially reduce state explosion
    // Use efficiency based on normalized resource usage
    auto eff = [&](const Piece& p)->double{
        double nm = (double)p.m / (double)M_CAP;
        double nl = (double)p.l / (double)L_CAP;
        double denom = nm + nl;
        if (denom == 0.0) denom = 1e-12;
        return (double)p.v / denom;
    };
    sort(pieces.begin(), pieces.end(), [&](const Piece& a, const Piece& b){
        double ea = eff(a), eb = eff(b);
        if (ea != eb) return ea > eb;
        // tie-breakers: higher value first, then smaller mass/vol
        if (a.v != b.v) return a.v > b.v;
        if (a.m != b.m) return a.m < b.m;
        return a.l < b.l;
    });

    // DP levels
    vector<vector<State>> levels;
    levels.reserve(pieces.size() + 1);
    vector<State> init;
    init.push_back({0,0,0, -1, false});
    levels.push_back(std::move(init));

    // DP over pieces
    for (size_t idx = 0; idx < pieces.size(); ++idx) {
        const Piece& it = pieces[idx];
        const vector<State>& prev = levels.back();

        vector<State> cand;
        cand.reserve(prev.size() * 2);

        // Not taking the item: copy prev states
        for (size_t j = 0; j < prev.size(); ++j) {
            State s = prev[j];
            s.parent = (int)j;
            s.taken = false;
            cand.push_back(s);
        }
        // Taking the item: add to prev states if feasible
        for (size_t j = 0; j < prev.size(); ++j) {
            const State& s = prev[j];
            long long nm = (long long)s.m + it.m;
            long long nl = (long long)s.l + it.l;
            if (nm <= M_CAP && nl <= L_CAP) {
                State t;
                t.m = (int)nm;
                t.l = (int)nl;
                t.v = s.v + it.v;
                t.parent = (int)j;
                t.taken = true;
                cand.push_back(t);
            }
        }

        vector<State> out = prune_states(cand);

        // Optional beam limit to ensure performance in worst-case
        const size_t MAX_STATES = 60000; // generous limit
        if (out.size() > MAX_STATES) {
            // Keep diverse high-value states by bucketing on (m,l)
            // Define grid
            int gridM = 180;
            int gridL = 180;
            long long stepM = max(1, M_CAP / gridM);
            long long stepL = max(1, L_CAP / gridL);
            // Use unordered_map with key combining grid indices
            struct Key { int a,b; };
            struct KeyHash {
                size_t operator()(const Key& k) const noexcept {
                    return (size_t)k.a * 1000003u ^ (size_t)k.b;
                }
            };
            struct KeyEq {
                bool operator()(const Key& x, const Key& y) const noexcept {
                    return x.a == y.a && x.b == y.b;
                }
            };
            unordered_map<Key, size_t, KeyHash, KeyEq> bestIdx;
            bestIdx.reserve(MAX_STATES*2);

            vector<State> reduced;
            reduced.reserve(MAX_STATES);

            for (const auto& s : out) {
                int im = (int)min<long long>(gridM - 1, s.m / stepM);
                int il = (int)min<long long>(gridL - 1, s.l / stepL);
                Key key{im, il};
                auto it2 = bestIdx.find(key);
                if (it2 == bestIdx.end()) {
                    bestIdx[key] = reduced.size();
                    reduced.push_back(s);
                } else {
                    size_t idxr = it2->second;
                    if (reduced[idxr].v < s.v ||
                        (reduced[idxr].v == s.v && (reduced[idxr].m > s.m || (reduced[idxr].m == s.m && reduced[idxr].l > s.l)))) {
                        reduced[idxr] = s;
                    }
                }
                if (reduced.size() >= MAX_STATES) {
                    // stop early
                    // Note: this truncation is heuristic
                    break;
                }
            }

            // If still too large, keep top by value
            if (reduced.size() > MAX_STATES) {
                nth_element(reduced.begin(), reduced.begin() + MAX_STATES, reduced.end(),
                    [](const State& a, const State& b){ return a.v > b.v; });
                reduced.resize(MAX_STATES);
            }

            levels.push_back(std::move(reduced));
        } else {
            levels.push_back(std::move(out));
        }
    }

    // Find best final state
    const vector<State>& finalStates = levels.back();
    long long bestVal = -1;
    int bestIdx = -1;
    for (int i = 0; i < (int)finalStates.size(); ++i) {
        if (finalStates[i].v > bestVal) {
            bestVal = finalStates[i].v;
            bestIdx = i;
        }
    }
    if (bestIdx == -1) {
        // fallback: take nothing
        cout << "{\n";
        for (int i = 0; i < n; ++i) {
            cout << " \"" << names[i] << "\": 0";
            if (i + 1 < n) cout << ",\n";
        }
        cout << "\n}\n";
        return 0;
    }

    // Reconstruct selection
    vector<long long> counts(n, 0);
    int curIdx = bestIdx;
    for (int i = (int)pieces.size(); i >= 1; --i) {
        const State& s = levels[i][curIdx];
        if (s.taken) {
            const Piece& pc = pieces[i - 1];
            counts[pc.cat] += pc.cnt;
        }
        curIdx = s.parent;
    }

    // Sanity cap counts to q
    for (int i = 0; i < n; ++i) {
        long long q = vals[i][0];
        if (counts[i] < 0) counts[i] = 0;
        if (counts[i] > q) counts[i] = q;
    }

    // Output JSON
    cout << "{\n";
    for (int i = 0; i < n; ++i) {
        cout << " \"" << names[i] << "\": " << counts[i];
        if (i + 1 < n) cout << ",\n";
    }
    cout << "\n}\n";

    return 0;
}