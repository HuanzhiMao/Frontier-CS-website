#include <bits/stdc++.h>
using namespace std;

struct Item {
    string name;
    long long q, v, m, l;
};

struct Solution {
    vector<long long> x;
    long long value = 0;
    long long remM = 0, remL = 0;
};

static const long long CAP_M = 20000000LL; // mg
static const long long CAP_L = 25000000LL; // uL

// Parse utilities
static inline void skipSpaces(const string &s, size_t &i) {
    while (i < s.size() && isspace((unsigned char)s[i])) i++;
}
static inline bool isNumChar(char c) { return (c >= '0' && c <= '9') || c=='-'; }

vector<Item> parseInput() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string input, line;
    {
        ostringstream oss;
        while (getline(cin, line)) {
            oss << line << '\n';
        }
        input = oss.str();
    }
    vector<Item> items;
    size_t i = 0;
    // Find entries: "name": [ ... four numbers ... ]
    while (true) {
        size_t startKey = input.find('"', i);
        if (startKey == string::npos) break;
        size_t endKey = input.find('"', startKey + 1);
        if (endKey == string::npos) break;
        string key = input.substr(startKey + 1, endKey - startKey - 1);
        size_t arrStart = input.find('[', endKey);
        if (arrStart == string::npos) { i = endKey + 1; continue; }
        size_t arrEnd = input.find(']', arrStart);
        if (arrEnd == string::npos) { i = arrStart + 1; continue; }
        // Parse four numbers within arrStart..arrEnd
        vector<long long> nums;
        size_t j = arrStart + 1;
        while (j < arrEnd) {
            while (j < arrEnd && !isNumChar(input[j])) j++;
            if (j >= arrEnd) break;
            size_t k = j;
            if (input[k] == '-') k++;
            while (k < arrEnd && isdigit((unsigned char)input[k])) k++;
            long long val = stoll(input.substr(j, k - j));
            nums.push_back(val);
            j = k;
            if (nums.size() >= 4) break;
        }
        if (nums.size() >= 4) {
            Item it;
            it.name = key;
            it.q = nums[0];
            it.v = nums[1];
            it.m = nums[2];
            it.l = nums[3];
            items.push_back(it);
        }
        i = arrEnd + 1;
    }
    return items;
}

vector<int> orderByDensityDesc(const vector<Item>& items, double alpha) {
    int n = (int)items.size();
    vector<int> idx(n);
    iota(idx.begin(), idx.end(), 0);
    double M = (double)CAP_M, L = (double)CAP_L;
    vector<double> dens(n);
    for (int i = 0; i < n; ++i) {
        double denom = alpha * (items[i].m / M) + (1.0 - alpha) * (items[i].l / L);
        if (denom <= 0) dens[i] = 0;
        else dens[i] = items[i].v / denom;
    }
    stable_sort(idx.begin(), idx.end(), [&](int a, int b) {
        if (dens[a] == dens[b]) return items[a].v > items[b].v;
        return dens[a] > dens[b];
    });
    return idx;
}

vector<int> orderByDensityAsc(const vector<Item>& items, double alpha) {
    int n = (int)items.size();
    vector<int> idx(n);
    iota(idx.begin(), idx.end(), 0);
    double M = (double)CAP_M, L = (double)CAP_L;
    vector<double> dens(n);
    for (int i = 0; i < n; ++i) {
        double denom = alpha * (items[i].m / M) + (1.0 - alpha) * (items[i].l / L);
        if (denom <= 0) dens[i] = 1e300;
        else dens[i] = items[i].v / denom;
    }
    stable_sort(idx.begin(), idx.end(), [&](int a, int b) {
        if (dens[a] == dens[b]) return items[a].v < items[b].v;
        return dens[a] < dens[b];
    });
    return idx;
}

Solution greedyGivenOrder(const vector<Item>& items, const vector<int>& order) {
    int n = (int)items.size();
    Solution sol;
    sol.x.assign(n, 0);
    sol.value = 0;
    sol.remM = CAP_M;
    sol.remL = CAP_L;
    for (int id : order) {
        if (items[id].m > sol.remM || items[id].l > sol.remL) continue;
        long long k1 = items[id].q;
        long long k2 = sol.remM / items[id].m;
        long long k3 = sol.remL / items[id].l;
        long long k = min(k1, min(k2, k3));
        if (k <= 0) continue;
        sol.x[id] += k;
        sol.remM -= k * items[id].m;
        sol.remL -= k * items[id].l;
        sol.value += k * items[id].v;
    }
    return sol;
}

void fillGreedyAdditional(const vector<Item>& items, const vector<int>& order, Solution& sol) {
    for (int id : order) {
        long long qleft = items[id].q - sol.x[id];
        if (qleft <= 0) continue;
        if (items[id].m > sol.remM || items[id].l > sol.remL) continue;
        long long k2 = sol.remM / items[id].m;
        long long k3 = sol.remL / items[id].l;
        long long k = min(qleft, min(k2, k3));
        if (k <= 0) continue;
        sol.x[id] += k;
        sol.remM -= k * items[id].m;
        sol.remL -= k * items[id].l;
        sol.value += k * items[id].v;
    }
}

double clamp01(double x) {
    if (x < 0.0) return 0.0;
    if (x > 1.0) return 1.0;
    return x;
}

bool attemptAddOneWithRemoval(const vector<Item>& items, int j, Solution& sol) {
    // If already fits, just add and maybe fill more.
    if (sol.x[j] >= items[j].q) return false;
    long long needM = max(0LL, items[j].m - sol.remM);
    long long needL = max(0LL, items[j].l - sol.remL);
    // Choose alpha based on normalized deficits
    double nm = needM / (double)CAP_M;
    double nl = needL / (double)CAP_L;
    double alpha = 0.5;
    if (nm > 0 || nl > 0) alpha = clamp01((nl) / ((nm) + (nl) + 1e-18));
    // Removal candidates: ascending density
    vector<int> remOrder = orderByDensityAsc(items, alpha);
    vector<long long> removeCount(items.size(), 0);
    long long rM = 0, rL = 0;
    long long valRemoved = 0;
    long long reqM = needM, reqL = needL;
    if (reqM > 0 || reqL > 0) {
        for (int id : remOrder) {
            if (id == j) continue; // do not remove same type to avoid thrash
            if (sol.x[id] <= 0) continue;
            if (reqM <= 0 && reqL <= 0) break;
            long long cm = items[id].m, cl = items[id].l;
            long long kNeedM = (reqM > 0 ? (reqM + cm - 1) / cm : 0);
            long long kNeedL = (reqL > 0 ? (reqL + cl - 1) / cl : 0);
            long long k = max(kNeedM, kNeedL);
            if (k <= 0) continue;
            if (k > sol.x[id]) k = sol.x[id];
            if (k <= 0) continue;
            removeCount[id] += k;
            rM += k * cm;
            rL += k * cl;
            valRemoved += k * items[id].v;
            reqM = max(0LL, reqM - k * cm);
            reqL = max(0LL, reqL - k * cl);
        }
        if (reqM > 0 || reqL > 0) {
            // Not enough removable items to make room (shouldn't happen realistically), abort
            return false;
        }
    }
    // Simulate change and subsequent greedy fill
    Solution tmp = sol;
    // Apply removals
    for (size_t id = 0; id < items.size(); ++id) {
        if (removeCount[id] > 0) {
            tmp.x[id] -= removeCount[id];
            tmp.remM += removeCount[id] * items[id].m;
            tmp.remL += removeCount[id] * items[id].l;
            tmp.value -= removeCount[id] * items[id].v;
        }
    }
    // Add one j
    if (tmp.x[j] >= items[j].q) return false;
    if (tmp.remM < items[j].m || tmp.remL < items[j].l) return false; // Should have space
    tmp.x[j] += 1;
    tmp.remM -= items[j].m;
    tmp.remL -= items[j].l;
    tmp.value += items[j].v;
    // Greedy fill more with same alpha priority
    vector<int> addOrder = orderByDensityDesc(items, alpha);
    fillGreedyAdditional(items, addOrder, tmp);
    if (tmp.value > sol.value) {
        sol = tmp;
        return true;
    }
    return false;
}

Solution improveSolution(const vector<Item>& items, Solution sol, const chrono::steady_clock::time_point &t0, double timeLimitSec) {
    int n = (int)items.size();
    // Iteratively try to improve
    int iter = 0;
    while (true) {
        if (chrono::duration<double>(chrono::steady_clock::now() - t0).count() > timeLimitSec) break;
        bool improved = false;
        // Choose focus alpha from current remaining capacities
        double rm = sol.remM / (double)CAP_M;
        double rl = sol.remL / (double)CAP_L;
        double alphaFocus = clamp01( (rl) / (rm + rl + 1e-18) );
        vector<int> candOrder = orderByDensityDesc(items, alphaFocus);
        for (int id : candOrder) {
            if (sol.x[id] >= items[id].q) continue;
            if (chrono::duration<double>(chrono::steady_clock::now() - t0).count() > timeLimitSec) break;
            if (attemptAddOneWithRemoval(items, id, sol)) {
                improved = true;
                break; // restart loop after improvement
            }
        }
        if (!improved) {
            // Try extremes to escape local optima
            for (double aTry : {0.0, 1.0, 0.3, 0.7}) {
                if (chrono::duration<double>(chrono::steady_clock::now() - t0).count() > timeLimitSec) break;
                vector<int> cand = orderByDensityDesc(items, aTry);
                for (int id : cand) {
                    if (sol.x[id] >= items[id].q) continue;
                    if (attemptAddOneWithRemoval(items, id, sol)) {
                        improved = true;
                        break;
                    }
                }
                if (improved) break;
            }
        }
        if (!improved) break;
        iter++;
        if (iter > 200) break; // safety cap
    }
    return sol;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<Item> items = parseInput();
    int n = (int)items.size();
    if (n == 0) {
        cout << "{\n}\n";
        return 0;
    }

    auto t0 = chrono::steady_clock::now();
    double timeLimitSec = 0.95; // keep some margin for I/O

    // Alpha candidates
    vector<double> alphas;
    for (int i = 0; i <= 10; ++i) alphas.push_back(i / 10.0);
    // Add item-based alphas
    for (const auto &it : items) {
        double nm = it.m / (double)CAP_M;
        double nl = it.l / (double)CAP_L;
        if (nm + nl > 0) {
            double a = clamp01(nl / (nm + nl));
            alphas.push_back(a);
        }
    }
    // Deduplicate
    sort(alphas.begin(), alphas.end());
    alphas.erase(unique(alphas.begin(), alphas.end(), [](double a, double b){ return fabs(a-b) < 1e-6; }), alphas.end());

    // Generate initial candidates
    vector<Solution> candidates;
    for (double a : alphas) {
        if (chrono::duration<double>(chrono::steady_clock::now() - t0).count() > timeLimitSec * 0.5) break;
        auto order = orderByDensityDesc(items, a);
        Solution sol = greedyGivenOrder(items, order);
        candidates.push_back(sol);
    }
    // Ensure at least one candidate
    if (candidates.empty()) {
        auto order = orderByDensityDesc(items, 0.5);
        candidates.push_back(greedyGivenOrder(items, order));
    }
    // Keep top K candidates
    sort(candidates.begin(), candidates.end(), [](const Solution& a, const Solution& b){ return a.value > b.value; });
    int K = min((int)candidates.size(), 4);
    vector<Solution> topCandidates(candidates.begin(), candidates.begin() + K);

    // Improve top candidates within time limit
    Solution best = topCandidates[0];
    for (int i = 0; i < K; ++i) {
        if (chrono::duration<double>(chrono::steady_clock::now() - t0).count() > timeLimitSec) break;
        Solution improved = improveSolution(items, topCandidates[i], t0, timeLimitSec);
        if (improved.value > best.value) best = improved;
    }

    // Output JSON with same keys order as input
    cout << "{\n";
    for (int i = 0; i < n; ++i) {
        cout << " \"" << items[i].name << "\": " << best.x[i];
        if (i + 1 < n) cout << ",\n";
        else cout << "\n";
    }
    cout << "}\n";
    return 0;
}