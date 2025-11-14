#include <bits/stdc++.h>
using namespace std;

struct Item {
    string name;
    long long q, v, m, l;
};

static inline void skipWS(const string &s, size_t &i) {
    while (i < s.size() && isspace((unsigned char)s[i])) ++i;
}

static inline bool expectChar(const string &s, size_t &i, char c) {
    skipWS(s, i);
    if (i < s.size() && s[i] == c) { ++i; return true; }
    return false;
}

static inline long long parseInteger(const string &s, size_t &i) {
    skipWS(s, i);
    long long sign = 1;
    if (i < s.size() && s[i] == '-') { sign = -1; ++i; }
    long long x = 0;
    while (i < s.size() && isdigit((unsigned char)s[i])) {
        x = x * 10 + (s[i] - '0');
        ++i;
    }
    return sign * x;
}

static inline string parseString(const string &s, size_t &i) {
    skipWS(s, i);
    string r;
    if (i < s.size() && s[i] == '"') {
        ++i;
        while (i < s.size()) {
            char c = s[i++];
            if (c == '"') break;
            if (c == '\\' && i < s.size()) {
                char e = s[i++];
                // Only handle simple escapes as needed; input is simple ASCII
                if (e == '"' || e == '\\' || e == '/') r.push_back(e);
                else if (e == 'b') r.push_back('\b');
                else if (e == 'f') r.push_back('\f');
                else if (e == 'n') r.push_back('\n');
                else if (e == 'r') r.push_back('\r');
                else if (e == 't') r.push_back('\t');
                else r.push_back(e);
            } else {
                r.push_back(c);
            }
        }
    }
    return r;
}

static inline long long ceil_div(long long a, long long b) {
    if (a <= 0) return 0;
    return (a + b - 1) / b;
}

struct Strategy {
    int type; // 0: linear alpha*m+beta*l, 1: max normalized, 2: v/m, 3: v/l, 4: dynamic normalized
    double alpha, beta;
};

static inline long long totalValue(const vector<Item> &items, const vector<long long> &cnt) {
    long long val = 0;
    for (size_t i = 0; i < items.size(); ++i) val += cnt[i] * items[i].v;
    return val;
}

static inline pair<long long,long long> usedCaps(const vector<Item> &items, const vector<long long> &cnt) {
    long long M = 0, L = 0;
    for (size_t i = 0; i < items.size(); ++i) {
        M += cnt[i] * items[i].m;
        L += cnt[i] * items[i].l;
    }
    return {M, L};
}

static vector<long long> greedyFill(const vector<Item> &items, long long M_CAP, long long L_CAP, const Strategy &st) {
    int n = (int)items.size();
    vector<int> idx(n);
    iota(idx.begin(), idx.end(), 0);
    vector<double> dens(n, 0.0);
    if (st.type == 0) {
        for (int i = 0; i < n; ++i) {
            double w = st.alpha * (double)items[i].m + st.beta * (double)items[i].l;
            if (w <= 0) dens[i] = numeric_limits<double>::infinity();
            else dens[i] = (double)items[i].v / w;
        }
    } else if (st.type == 1) {
        for (int i = 0; i < n; ++i) {
            double w = max((double)items[i].m / (double)M_CAP, (double)items[i].l / (double)L_CAP);
            if (w <= 0) dens[i] = numeric_limits<double>::infinity();
            else dens[i] = (double)items[i].v / w;
        }
    } else if (st.type == 2) {
        for (int i = 0; i < n; ++i) {
            dens[i] = (double)items[i].v / (double)items[i].m;
        }
    } else if (st.type == 3) {
        for (int i = 0; i < n; ++i) {
            dens[i] = (double)items[i].v / (double)items[i].l;
        }
    }
    sort(idx.begin(), idx.end(), [&](int a, int b){
        if (dens[a] != dens[b]) return dens[a] > dens[b];
        if (items[a].v != items[b].v) return items[a].v > items[b].v;
        return items[a].name < items[b].name;
    });
    vector<long long> cnt(n, 0);
    long long M_left = M_CAP, L_left = L_CAP;
    if (st.type == 4) {
        // dynamic normalized greedy
        while (true) {
            int best = -1;
            double bestScore = -1.0;
            // compute best item that can fit at least one more
            for (int i = 0; i < n; ++i) {
                if (cnt[i] >= items[i].q) continue;
                if (items[i].m > M_left || items[i].l > L_left) continue;
                double w = (double)items[i].m / max(1.0, (double)M_left) + (double)items[i].l / max(1.0, (double)L_left);
                if (w <= 0) continue;
                double sc = (double)items[i].v / w;
                if (sc > bestScore) {
                    bestScore = sc;
                    best = i;
                }
            }
            if (best == -1) break;
            long long can = min({ items[best].q - cnt[best], M_left / items[best].m, L_left / items[best].l });
            if (can <= 0) break;
            cnt[best] += can;
            M_left -= can * items[best].m;
            L_left -= can * items[best].l;
        }
        return cnt;
    }
    for (int id : idx) {
        if (M_left <= 0 || L_left <= 0) break;
        long long can = min({ items[id].q, M_left / items[id].m, L_left / items[id].l });
        if (can <= 0) continue;
        cnt[id] = can;
        M_left -= can * items[id].m;
        L_left -= can * items[id].l;
    }
    return cnt;
}

static void fillLeftoverGreedy(const vector<Item> &items, vector<long long> &cnt, long long M_CAP, long long L_CAP) {
    // Single pass fill by normalized linear density
    Strategy st{0, 1.0 / (double)M_CAP, 1.0 / (double)L_CAP};
    int n = (int)items.size();
    vector<int> idx(n);
    iota(idx.begin(), idx.end(), 0);
    vector<double> dens(n, 0.0);
    for (int i = 0; i < n; ++i) {
        double w = st.alpha*(double)items[i].m + st.beta*(double)items[i].l;
        if (w <= 0) dens[i] = numeric_limits<double>::infinity();
        else dens[i] = (double)items[i].v / w;
    }
    sort(idx.begin(), idx.end(), [&](int a, int b){
        if (dens[a] != dens[b]) return dens[a] > dens[b];
        if (items[a].v != items[b].v) return items[a].v > items[b].v;
        return items[a].name < items[b].name;
    });
    long long usedM = 0, usedL = 0;
    for (int i = 0; i < n; ++i) { usedM += cnt[i]*items[i].m; usedL += cnt[i]*items[i].l; }
    long long M_left = max(0LL, M_CAP - usedM);
    long long L_left = max(0LL, L_CAP - usedL);
    for (int id : idx) {
        if (M_left <= 0 || L_left <= 0) break;
        long long can = min({ items[id].q - cnt[id], M_left / items[id].m, L_left / items[id].l });
        if (can <= 0) continue;
        cnt[id] += can;
        M_left -= can * items[id].m;
        L_left -= can * items[id].l;
    }
}

static void localImprove(const vector<Item> &items, vector<long long> &cnt, long long M_CAP, long long L_CAP, chrono::steady_clock::time_point startT, double timeLimitSec) {
    int n = (int)items.size();
    auto now = [&](){ return chrono::duration<double>(chrono::steady_clock::now() - startT).count(); };
    auto getLeft = [&](const vector<long long> &c) {
        long long um = 0, ul = 0;
        for (int i = 0; i < n; ++i) {
            um += c[i]*items[i].m;
            ul += c[i]*items[i].l;
        }
        return pair<long long,long long>(M_CAP - um, L_CAP - ul);
    };
    auto curLeft = getLeft(cnt);
    long long M_left = curLeft.first, L_left = curLeft.second;

    vector<int> order(n);
    iota(order.begin(), order.end(), 0);

    bool improved = true;
    int outerIters = 0;
    while (improved && now() < timeLimitSec) {
        improved = false;
        ++outerIters;
        // Try to add items of each category with greedy removal
        for (int addId = 0; addId < n && now() < timeLimitSec; ++addId) {
            long long remAvail = items[addId].q - cnt[addId];
            if (remAvail <= 0) continue;

            // Build candidate k values: Fibonacci-like up to remAvail
            vector<long long> kList;
            long long a = 1, b = 2;
            kList.push_back(1);
            if (remAvail >= 2) kList.push_back(2);
            while (true) {
                long long c = a + b;
                if (c > remAvail) break;
                kList.push_back(c);
                a = b; b = c;
                if ((int)kList.size() >= 20) break;
            }
            if (kList.back() != remAvail) kList.push_back(remAvail);

            for (long long k : kList) {
                if (k <= 0) continue;
                if (now() >= timeLimitSec) break;

                long long needM = max(0LL, k*items[addId].m - M_left);
                long long needL = max(0LL, k*items[addId].l - L_left);
                if (needM == 0 && needL == 0) {
                    // direct add
                    cnt[addId] += k;
                    M_left -= k*items[addId].m;
                    L_left -= k*items[addId].l;
                    // try fill leftover quickly
                    fillLeftoverGreedy(items, cnt, M_CAP, L_CAP);
                    auto left2 = getLeft(cnt);
                    M_left = left2.first; L_left = left2.second;
                    improved = true;
                    break;
                }

                double alpha = needM > 0 ? 1.0 / (double)needM : 0.0;
                double beta  = needL > 0 ? 1.0 / (double)needL : 0.0;

                vector<int> remIdx;
                remIdx.reserve(n);
                for (int i = 0; i < n; ++i) if (cnt[i] > 0 && i != addId) remIdx.push_back(i);
                if (remIdx.empty()) continue;

                vector<double> penalty(n, 0.0);
                for (int i : remIdx) {
                    double w = alpha * (double)items[i].m + beta * (double)items[i].l;
                    if (w <= 0) penalty[i] = numeric_limits<double>::infinity();
                    else penalty[i] = (double)items[i].v / w;
                }
                sort(remIdx.begin(), remIdx.end(), [&](int a1, int b1){
                    if (penalty[a1] != penalty[b1]) return penalty[a1] < penalty[b1];
                    return items[a1].v < items[b1].v;
                });

                long long freeM = 0, freeL = 0, lost = 0;
                vector<long long> toRemove(n, 0);
                bool possible = false;

                // Remove greedily from lowest penalty items one by one
                size_t ptr = 0;
                while (freeM < needM || freeL < needL) {
                    if (ptr >= remIdx.size()) break;
                    int rid = remIdx[ptr];
                    if (cnt[rid] - toRemove[rid] <= 0) {
                        ++ptr;
                        continue;
                    }
                    // remove one unit
                    toRemove[rid] += 1;
                    freeM += items[rid].m;
                    freeL += items[rid].l;
                    lost += items[rid].v;
                }
                if (freeM >= needM && freeL >= needL) {
                    possible = true;
                }

                if (!possible) continue;

                long long gain = k * items[addId].v - lost;
                if (gain > 0) {
                    // apply
                    cnt[addId] += k;
                    for (int i = 0; i < n; ++i) {
                        if (toRemove[i] > 0) cnt[i] -= toRemove[i];
                    }
                    // update leftovers
                    M_left = M_left - k*items[addId].m + freeM;
                    L_left = L_left - k*items[addId].l + freeL;
                    // sanity
                    if (M_left < 0 || L_left < 0) {
                        // recompute exact leftovers if rounding error; but should not happen
                        auto left2 = getLeft(cnt);
                        M_left = left2.first; L_left = left2.second;
                    }
                    // try to fill leftover greedily
                    fillLeftoverGreedy(items, cnt, M_CAP, L_CAP);
                    auto left2 = getLeft(cnt);
                    M_left = left2.first; L_left = left2.second;
                    improved = true;
                    break;
                }
            }
            if (improved) break;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string input((istreambuf_iterator<char>(cin)), istreambuf_iterator<char>());

    size_t i = 0;
    vector<Item> items;
    if (!expectChar(input, i, '{')) {
        // Fallback: empty
    } else {
        while (true) {
            skipWS(input, i);
            if (i >= input.size()) break;
            if (input[i] == '}') { ++i; break; }
            string key = parseString(input, i);
            expectChar(input, i, ':');
            expectChar(input, i, '[');
            long long q = parseInteger(input, i);
            expectChar(input, i, ',');
            long long v = parseInteger(input, i);
            expectChar(input, i, ',');
            long long m = parseInteger(input, i);
            expectChar(input, i, ',');
            long long l = parseInteger(input, i);
            expectChar(input, i, ']');
            items.push_back({key, q, v, m, l});
            skipWS(input, i);
            if (i < input.size() && input[i] == ',') { ++i; continue; }
            skipWS(input, i);
            if (i < input.size() && input[i] == '}') { ++i; break; }
        }
    }

    const long long M_CAP = 20000000LL;
    const long long L_CAP = 25000000LL;

    int n = (int)items.size();
    if (n == 0) {
        cout << "{\n}\n";
        return 0;
    }

    auto startT = chrono::steady_clock::now();
    double TL = 0.98; // seconds budget

    // Strategies
    vector<Strategy> strategies;
    strategies.push_back({0, 1.0 / (double)M_CAP, 1.0 / (double)L_CAP});
    strategies.push_back({0, 1.0 / (double)M_CAP, 0.0});
    strategies.push_back({0, 0.0, 1.0 / (double)L_CAP});
    strategies.push_back({1, 0.0, 0.0});
    strategies.push_back({2, 0.0, 0.0});
    strategies.push_back({3, 0.0, 0.0});
    for (int t = 0; t <= 10; ++t) {
        double lam = t / 10.0;
        strategies.push_back({0, lam / (double)M_CAP, (1.0 - lam) / (double)L_CAP});
    }
    // dynamic normalized greedy
    strategies.push_back({4, 0.0, 0.0});

    std::mt19937_64 rng((uint64_t)chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<double> U(0.0, 1.0);

    // Explore some random strategies until half time
    while (chrono::duration<double>(chrono::steady_clock::now() - startT).count() < TL * 0.55) {
        double lam = U(rng);
        strategies.push_back({0, lam / (double)M_CAP, (1.0 - lam) / (double)L_CAP});
        // Also some edge tilts
        double s = pow(U(rng), 2.0);
        double t = 1.0 - s;
        strategies.push_back({0, s / (double)M_CAP, t / (double)L_CAP});
    }

    vector<long long> bestCnt(n, 0);
    long long bestVal = 0;

    // Evaluate strategies
    for (const auto &st : strategies) {
        auto cnt = greedyFill(items, M_CAP, L_CAP, st);
        long long val = totalValue(items, cnt);
        if (val > bestVal) {
            bestVal = val;
            bestCnt = cnt;
        }
    }

    // Local improvement
    localImprove(items, bestCnt, M_CAP, L_CAP, startT, TL);

    // As a small extra try, run another dynamic greedy from scratch and pick better
    if (chrono::duration<double>(chrono::steady_clock::now() - startT).count() < TL * 0.85) {
        Strategy dyn{4, 0.0, 0.0};
        auto cnt2 = greedyFill(items, M_CAP, L_CAP, dyn);
        long long val2 = totalValue(items, cnt2);
        if (val2 > bestVal) {
            bestVal = val2;
            bestCnt = cnt2;
            localImprove(items, bestCnt, M_CAP, L_CAP, startT, TL);
        }
    }

    // Output JSON
    cout << "{\n";
    for (int j = 0; j < n; ++j) {
        cout << " \"" << items[j].name << "\": " << bestCnt[j];
        if (j + 1 < n) cout << ",\n";
        else cout << "\n";
    }
    cout << "}\n";

    return 0;
}