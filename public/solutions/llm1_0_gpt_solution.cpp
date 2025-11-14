#include <bits/stdc++.h>
using namespace std;

struct Item {
    string name;
    long long q, v, m, l;
};

struct Solution {
    vector<long long> x;
    long long totalValue = 0;
    long long totalM = 0;
    long long totalL = 0;
};

struct Parser {
    string s;
    size_t pos = 0;

    Parser(const string& str) : s(str), pos(0) {}

    void skip() {
        while (pos < s.size() && (s[pos] == ' ' || s[pos] == '\n' || s[pos] == '\t' || s[pos] == '\r')) pos++;
    }

    bool expect(char c) {
        skip();
        if (pos < s.size() && s[pos] == c) { pos++; return true; }
        return false;
    }

    string parseString() {
        skip();
        if (pos >= s.size() || s[pos] != '"') return "";
        pos++;
        string res;
        while (pos < s.size() && s[pos] != '"') {
            // no escape handling needed for given constraints (lowercase ascii)
            res.push_back(s[pos++]);
        }
        if (pos < s.size() && s[pos] == '"') pos++;
        return res;
    }

    long long parseInt() {
        skip();
        bool neg = false;
        if (pos < s.size() && s[pos] == '-') { neg = true; pos++; }
        long long val = 0;
        while (pos < s.size() && s[pos] >= '0' && s[pos] <= '9') {
            val = val * 10 + (s[pos] - '0');
            pos++;
        }
        return neg ? -val : val;
    }

    vector<Item> parseItems() {
        vector<Item> items;
        skip();
        if (!expect('{')) return items;
        while (true) {
            skip();
            if (pos < s.size() && s[pos] == '}') { pos++; break; }
            string key = parseString();
            skip();
            expect(':');
            skip();
            expect('[');
            long long q = parseInt();
            skip(); expect(',');
            long long v = parseInt();
            skip(); expect(',');
            long long m = parseInt();
            skip(); expect(',');
            long long l = parseInt();
            skip(); expect(']');
            items.push_back({key, q, v, m, l});
            skip();
            if (pos < s.size() && s[pos] == ',') { pos++; continue; }
            skip();
            if (pos < s.size() && s[pos] == '}') { pos++; break; }
        }
        return items;
    }
};

static inline long long ceil_div(long long a, long long b) {
    if (b <= 0) return LLONG_MAX/4;
    if (a <= 0) return 0;
    return (a + b - 1) / b;
}

struct Metric {
    long double lam;
    long double mu;
    int mode; // 0 = linear combo; 1 = max normalized
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Read entire input
    string input, line;
    stringstream buffer;
    while (getline(cin, line)) {
        buffer << line << "\n";
    }
    input = buffer.str();

    Parser parser(input);
    vector<Item> items = parser.parseItems();
    int n = (int)items.size();
    if (n == 0) {
        // Fallback: print empty JSON
        cout << "{\n";
        for (int i = 0; i < n; i++) {
            cout << " \"" << items[i].name << "\": 0";
            if (i + 1 < n) cout << ",\n"; else cout << "\n";
        }
        cout << "}\n";
        return 0;
    }

    const long long Mcap = 20LL * 1000 * 1000;
    const long long Lcap = 25LL * 1000 * 1000;

    auto now = [](){ return chrono::steady_clock::now(); };
    auto start = now();
    auto timePassedMs = [&](double ms){ 
        return chrono::duration<double, milli>(now() - start).count() >= ms;
    };

    // Helper to compute solution totals
    auto computeTotals = [&](Solution& sol) {
        sol.totalValue = 0;
        sol.totalM = 0;
        sol.totalL = 0;
        for (int i = 0; i < n; i++) {
            if (sol.x[i] < 0) sol.x[i] = 0;
            if (sol.x[i] > items[i].q) sol.x[i] = items[i].q;
            sol.totalValue += sol.x[i] * items[i].v;
            sol.totalM += sol.x[i] * items[i].m;
            sol.totalL += sol.x[i] * items[i].l;
        }
    };

    // Feasibility check
    auto isFeasible = [&](const Solution& sol)->bool{
        if (sol.totalM > Mcap || sol.totalL > Lcap) return false;
        for (int i = 0; i < n; i++) if (sol.x[i] < 0 || sol.x[i] > items[i].q) return false;
        return true;
    };

    // Greedy by metric
    auto greedyByMetric = [&](const Metric& metric)->Solution {
        vector<int> order(n);
        iota(order.begin(), order.end(), 0);
        vector<long double> dens(n, 0.0L);
        for (int i = 0; i < n; i++) {
            long double nm = (long double)items[i].m / (long double)Mcap;
            long double nl = (long double)items[i].l / (long double)Lcap;
            long double denom;
            if (metric.mode == 1) {
                denom = max(nm, nl);
            } else {
                denom = metric.lam * nm + metric.mu * nl;
            }
            if (denom <= 0) {
                dens[i] = 1e300L; // extremely high density
            } else {
                dens[i] = (long double)items[i].v / denom;
            }
        }
        stable_sort(order.begin(), order.end(), [&](int a, int b){
            if (dens[a] == dens[b]) return items[a].v > items[b].v;
            return dens[a] > dens[b];
        });

        Solution sol;
        sol.x.assign(n, 0);
        long long remM = Mcap, remL = Lcap;
        for (int idx : order) {
            if (items[idx].m > remM || items[idx].l > remL) continue;
            long long kM = items[idx].m > 0 ? remM / items[idx].m : (long long)4e18;
            long long kL = items[idx].l > 0 ? remL / items[idx].l : (long long)4e18;
            long long k = min({ (long long)items[idx].q, kM, kL });
            if (k <= 0) continue;
            sol.x[idx] += k;
            remM -= k * items[idx].m;
            remL -= k * items[idx].l;
            // Early break if bag is "full" for both dimensions (cannot add any item)
        }
        computeTotals(sol);
        return sol;
    };

    // Single item greedy (only one type)
    auto singleTypeGreedy = [&](int idx)->Solution {
        Solution sol;
        sol.x.assign(n, 0);
        long long kM = items[idx].m > 0 ? Mcap / items[idx].m : (long long)4e18;
        long long kL = items[idx].l > 0 ? Lcap / items[idx].l : (long long)4e18;
        long long k = min({ (long long)items[idx].q, kM, kL });
        if (k > 0) {
            sol.x[idx] = k;
        }
        computeTotals(sol);
        return sol;
    };

    // Try to add one item j by possibly removing others greedily; returns true if improved
    auto tryAddImprove = [&](Solution& sol, int j)->bool {
        if (sol.x[j] >= items[j].q) return false;
        long long remM = Mcap - sol.totalM;
        long long remL = Lcap - sol.totalL;
        if (items[j].m <= remM && items[j].l <= remL) {
            // Always beneficial since v > 0
            sol.x[j] += 1;
            sol.totalM += items[j].m;
            sol.totalL += items[j].l;
            sol.totalValue += items[j].v;
            return true;
        }
        // Need to remove items to fit j
        long long needM0 = max(0LL, items[j].m - remM);
        long long needL0 = max(0LL, items[j].l - remL);
        if (needM0 == 0 && needL0 == 0) {
            // Shouldn't get here due to first check
            return false;
        }

        auto attemptRemoval = [&](double mode)->bool {
            // mode 0: weights proportional to deficits; mode 1: binary 0/1
            long double a, b;
            if (mode < 0.5) {
                a = (long double)needM0 / (long double)Mcap;
                b = (long double)needL0 / (long double)Lcap;
            } else {
                a = needM0 > 0 ? 1.0L : 0.0L;
                b = needL0 > 0 ? 1.0L : 0.0L;
            }
            vector<long long> remCnt(n, 0);
            long long removedVal = 0;
            long long curRemM = remM;
            long long curRemL = remL;
            long long needM = needM0;
            long long needL = needL0;
            int guard = 0;
            while ((needM > 0 || needL > 0) && removedVal < items[j].v) {
                int best = -1;
                long double bestScore = 1e300L;
                for (int i = 0; i < n; i++) {
                    if (i == j) continue;
                    long long avail = sol.x[i] - remCnt[i];
                    if (avail <= 0) continue;
                    if (needM > 0 || needL > 0) {
                        // Only consider those freeing at least some needed resource
                        bool useful = false;
                        if (needM > 0 && items[i].m > 0) useful = true;
                        if (needL > 0 && items[i].l > 0) useful = true;
                        if (!useful) continue;
                    }
                    long double denom = a * (long double)items[i].m + b * (long double)items[i].l;
                    if (denom <= 0) continue;
                    long double s = (long double)items[i].v / denom;
                    // Respect value bound: must allow removing at least 1 while still potentially improving
                    long long maxRemByVal = (items[j].v - 1 - removedVal) / items[i].v;
                    if (maxRemByVal <= 0) continue;
                    if (s < bestScore) { bestScore = s; best = i; }
                }
                if (best == -1) {
                    // No removable items without losing more value than gain or not useful
                    break;
                }
                // Remove in batch
                long long maxRemByVal = (items[j].v - 1 - removedVal) / items[best].v;
                long long avail = sol.x[best] - remCnt[best];
                if (avail <= 0 || maxRemByVal <= 0) break;

                long long kNeedM = needM > 0 ? ceil_div(needM, items[best].m) : 0;
                long long kNeedL = needL > 0 ? ceil_div(needL, items[best].l) : 0;
                long long kNeed = max(kNeedM, kNeedL);
                if (kNeed <= 0) kNeed = 1;
                long long k = min({avail, maxRemByVal, kNeed});
                if (k <= 0) k = 1;
                // Apply removal
                remCnt[best] += k;
                removedVal += k * items[best].v;
                curRemM += k * items[best].m;
                curRemL += k * items[best].l;
                needM = max(0LL, items[j].m - curRemM);
                needL = max(0LL, items[j].l - curRemL);

                guard++;
                if (guard > 100000) break; // safety
            }
            if (needM > 0 || needL > 0) return false;
            if (removedVal >= items[j].v) return false;
            // Commit changes
            for (int i = 0; i < n; i++) {
                if (remCnt[i] > 0) {
                    sol.x[i] -= remCnt[i];
                    sol.totalM -= remCnt[i] * items[i].m;
                    sol.totalL -= remCnt[i] * items[i].l;
                    sol.totalValue -= remCnt[i] * items[i].v;
                }
            }
            // Now add j
            sol.x[j] += 1;
            sol.totalM += items[j].m;
            sol.totalL += items[j].l;
            sol.totalValue += items[j].v;
            return true;
        };

        if (attemptRemoval(0.0)) return true;
        if (attemptRemoval(1.0)) return true;
        return false;
    };

    auto localImprove = [&](Solution& sol) {
        // Try several passes, attempt to add items with removal
        // Order by descending value density using equal weights
        vector<int> order(n);
        iota(order.begin(), order.end(), 0);
        vector<long double> density(n, 0.0L);
        for (int i = 0; i < n; i++) {
            long double nm = (long double)items[i].m / (long double)Mcap;
            long double nl = (long double)items[i].l / (long double)Lcap;
            long double denom = nm + nl;
            if (denom <= 0) density[i] = 1e300L;
            else density[i] = (long double)items[i].v / denom;
        }
        stable_sort(order.begin(), order.end(), [&](int a, int b){
            if (density[a] == density[b]) return items[a].v > items[b].v;
            return density[a] > density[b];
        });

        bool improved = true;
        int passes = 0;
        while (improved && passes < 4 && !timePassedMs(900.0)) {
            improved = false;
            for (int idx = 0; idx < n && !timePassedMs(930.0); idx++) {
                int j = order[idx];
                // Try to add as many as possible with improvement
                int innerGuard = 0;
                while (!timePassedMs(950.0)) {
                    bool ok = tryAddImprove(sol, j);
                    if (!ok) break;
                    improved = true;
                    innerGuard++;
                    if (innerGuard > 10000) break; // safety
                }
            }
            passes++;
        }
    };

    // Generate metrics
    vector<Metric> metrics;
    vector<long double> ws = {0.0L, 0.1L, 0.25L, 0.5L, 1.0L, 2.0L, 4.0L};
    for (auto lam : ws) {
        for (auto mu : ws) {
            if (lam == 0.0L && mu == 0.0L) continue;
            metrics.push_back({lam, mu, 0});
        }
    }
    // Special max-normalized metric
    metrics.push_back({1.0L, 1.0L, 1});
    // Additional skewed metrics
    metrics.push_back({1.0L, 0.0L, 0});
    metrics.push_back({0.0L, 1.0L, 0});
    metrics.push_back({1.0L, 3.0L, 0});
    metrics.push_back({3.0L, 1.0L, 0});
    metrics.push_back({1.0L, 5.0L, 0});
    metrics.push_back({5.0L, 1.0L, 0});

    // Random metrics
    mt19937_64 rng((uint64_t)chrono::high_resolution_clock::now().time_since_epoch().count());
    uniform_real_distribution<long double> dist(0.05L, 5.0L);
    for (int r = 0; r < 16; r++) {
        long double lam = dist(rng);
        long double mu = dist(rng);
        metrics.push_back({lam, mu, 0});
    }

    // Evaluate candidates
    Solution best;
    best.x.assign(n, 0);
    computeTotals(best); // zero solution

    // Single-type baseline candidates
    for (int i = 0; i < n; i++) {
        Solution s = singleTypeGreedy(i);
        if (isFeasible(s) && s.totalValue > best.totalValue) best = s;
        if (timePassedMs(100.0)) ; // no-op, just force possible branch prediction
    }

    // Metrics-based greedy + improvement
    for (size_t mi = 0; mi < metrics.size(); mi++) {
        if (timePassedMs(980.0)) break;
        Solution s = greedyByMetric(metrics[mi]);
        if (isFeasible(s)) {
            if (!timePassedMs(960.0)) localImprove(s);
            if (s.totalValue > best.totalValue) best = s;
        }
    }

    // Final safety feasibility
    if (!isFeasible(best)) {
        // Attempt to repair by dropping items with worst value density until feasible
        // Compute density with equal weights
        vector<pair<long double,int>> dens;
        for (int i = 0; i < n; i++) {
            long double nm = (long double)items[i].m / (long double)Mcap;
            long double nl = (long double)items[i].l / (long double)Lcap;
            long double denom = nm + nl;
            long double d = denom > 0 ? (long double)items[i].v / denom : 1e300L;
            dens.push_back({d, i});
        }
        sort(dens.begin(), dens.end(), [](auto& A, auto& B){ return A.first < B.first; }); // remove worst first
        for (auto &p : dens) {
            int i = p.second;
            while ((best.totalM > Mcap || best.totalL > Lcap) && best.x[i] > 0) {
                best.x[i]--;
                best.totalM -= items[i].m;
                best.totalL -= items[i].l;
                best.totalValue -= items[i].v;
            }
            if (best.totalM <= Mcap && best.totalL <= Lcap) break;
        }
        if (!isFeasible(best)) {
            // As last resort, empty solution
            best.x.assign(n, 0);
            computeTotals(best);
        }
    }

    // Output JSON with same keys and order as input
    cout << "{\n";
    for (int i = 0; i < n; i++) {
        cout << " \"" << items[i].name << "\": " << best.x[i];
        if (i + 1 < n) cout << ",\n"; else cout << "\n";
    }
    cout << "}\n";
    return 0;
}