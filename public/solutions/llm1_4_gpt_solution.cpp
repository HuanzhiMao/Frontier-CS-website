#include <bits/stdc++.h>
using namespace std;

struct Item {
    string name;
    long long q, v, m, l;
};

struct Solution {
    vector<long long> cnt;
    long long value = 0;
    long long usedW = 0;
    long long usedV = 0;
};

static inline bool isdigit_c(char c){ return c >= '0' && c <= '9'; }

long long parseLongLong(const string &s, size_t &pos) {
    size_t n = s.size();
    while (pos < n && !(s[pos] == '-' || isdigit_c(s[pos]))) pos++;
    int sign = 1;
    if (pos < n && s[pos] == '-') { sign = -1; pos++; }
    long long val = 0;
    while (pos < n && isdigit_c(s[pos])) {
        val = val * 10 + (s[pos] - '0');
        pos++;
    }
    return val * sign;
}

vector<Item> parseInput() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string input, line;
    // Read full stdin into string
    {
        std::ostringstream oss;
        oss << cin.rdbuf();
        input = oss.str();
    }
    vector<Item> items;
    size_t pos = 0;
    int countItems = 0;
    while (true) {
        size_t q1 = input.find('"', pos);
        if (q1 == string::npos) break;
        size_t q2 = input.find('"', q1 + 1);
        if (q2 == string::npos) break;
        string name = input.substr(q1 + 1, q2 - q1 - 1);
        pos = q2 + 1;
        size_t br = input.find('[', pos);
        if (br == string::npos) break;
        pos = br + 1;
        long long q = parseLongLong(input, pos);
        long long v = parseLongLong(input, pos);
        long long m = parseLongLong(input, pos);
        long long l = parseLongLong(input, pos);
        Item it{name, q, v, m, l};
        items.push_back(it);
        countItems++;
        pos = input.find(']', pos);
        if (pos == string::npos) break;
        pos++;
    }
    return items;
}

int main() {
    auto items = parseInput();
    const long long W = 20000000LL;
    const long long Vcap = 25000000LL;
    int n = (int)items.size();
    if (n == 0) {
        cout << "{\n}\n";
        return 0;
    }
    // Prepare orders
    vector<vector<int>> orders;
    vector<double> alphas = {0.0, 0.15, 0.35, 0.5, 0.65, 0.85, 1.0};
    // Weighted sum orders
    for (double a : alphas) {
        vector<pair<double,int>> sc;
        sc.reserve(n);
        for (int i = 0; i < n; ++i) {
            double mm = (double)items[i].m / (double)W;
            double ll = (double)items[i].l / (double)Vcap;
            double cost = a * mm + (1.0 - a) * ll;
            double score = (cost > 0) ? ( (double)items[i].v / cost ) : 1e300;
            sc.emplace_back(score, i);
        }
        sort(sc.begin(), sc.end(), [&](const auto& A, const auto& B){
            if (A.first != B.first) return A.first > B.first;
            const Item &ia = items[A.second], &ib = items[B.second];
            if (ia.v != ib.v) return ia.v > ib.v;
            return A.second < B.second;
        });
        vector<int> ord;
        ord.reserve(n);
        for (auto &p : sc) ord.push_back(p.second);
        orders.push_back(ord);
    }
    // Max norm order
    {
        vector<pair<double,int>> sc;
        sc.reserve(n);
        for (int i = 0; i < n; ++i) {
            double mm = (double)items[i].m / (double)W;
            double ll = (double)items[i].l / (double)Vcap;
            double cost = max(mm, ll);
            double score = (cost > 0) ? ((double)items[i].v / cost) : 1e300;
            sc.emplace_back(score, i);
        }
        sort(sc.begin(), sc.end(), [&](const auto& A, const auto& B){
            if (A.first != B.first) return A.first > B.first;
            const Item &ia = items[A.second], &ib = items[B.second];
            if (ia.v != ib.v) return ia.v > ib.v;
            return A.second < B.second;
        });
        vector<int> ord;
        ord.reserve(n);
        for (auto &p : sc) ord.push_back(p.second);
        orders.push_back(ord);
    }
    // Euclidean norm order
    {
        vector<pair<double,int>> sc;
        sc.reserve(n);
        for (int i = 0; i < n; ++i) {
            double mm = (double)items[i].m / (double)W;
            double ll = (double)items[i].l / (double)Vcap;
            double cost = sqrt(mm*mm + ll*ll);
            double score = (cost > 0) ? ((double)items[i].v / cost) : 1e300;
            sc.emplace_back(score, i);
        }
        sort(sc.begin(), sc.end(), [&](const auto& A, const auto& B){
            if (A.first != B.first) return A.first > B.first;
            const Item &ia = items[A.second], &ib = items[B.second];
            if (ia.v != ib.v) return ia.v > ib.v;
            return A.second < B.second;
        });
        vector<int> ord;
        ord.reserve(n);
        for (auto &p : sc) ord.push_back(p.second);
        orders.push_back(ord);
    }
    // Also plain value descending
    {
        vector<int> ord(n);
        iota(ord.begin(), ord.end(), 0);
        sort(ord.begin(), ord.end(), [&](int a, int b){
            if (items[a].v != items[b].v) return items[a].v > items[b].v;
            return a < b;
        });
        orders.push_back(ord);
    }
    // Value per mass, value per volume
    {
        vector<int> ord(n);
        iota(ord.begin(), ord.end(), 0);
        sort(ord.begin(), ord.end(), [&](int a, int b){
            double ra = (double)items[a].v / (double)items[a].m;
            double rb = (double)items[b].v / (double)items[b].m;
            if (ra != rb) return ra > rb;
            return a < b;
        });
        orders.push_back(ord);
    }
    {
        vector<int> ord(n);
        iota(ord.begin(), ord.end(), 0);
        sort(ord.begin(), ord.end(), [&](int a, int b){
            double ra = (double)items[a].v / (double)items[a].l;
            double rb = (double)items[b].v / (double)items[b].l;
            if (ra != rb) return ra > rb;
            return a < b;
        });
        orders.push_back(ord);
    }

    auto startTime = chrono::high_resolution_clock::now();
    auto timeExceeded = [&](){
        auto now = chrono::high_resolution_clock::now();
        double elapsed = chrono::duration<double>(now - startTime).count();
        return elapsed > 0.97; // leave some margin
    };

    auto computeValue = [&](const vector<long long>& cnt, long long &uW, long long &uV){
        __int128 val = 0;
        uW = 0; uV = 0;
        for (int i = 0; i < n; ++i) {
            val += (__int128)cnt[i] * items[i].v;
            uW += cnt[i] * items[i].m;
            uV += cnt[i] * items[i].l;
        }
        return (long long)val;
    };

    auto greedyFill = [&](vector<long long>& cnt, long long &uW, long long &uV, long long &val, const vector<int>& order) {
        for (int idx : order) {
            if (cnt[idx] >= items[idx].q) continue;
            if (items[idx].m > W || items[idx].l > Vcap) continue;
            long long remW = W - uW;
            long long remV = Vcap - uV;
            if (remW <= 0 || remV <= 0) break;
            long long byW = remW / items[idx].m;
            long long byV = remV / items[idx].l;
            long long add = min(items[idx].q - cnt[idx], min(byW, byV));
            if (add > 0) {
                cnt[idx] += add;
                uW += add * items[idx].m;
                uV += add * items[idx].l;
                val += add * items[idx].v;
            }
        }
    };

    auto tryAddOne = [&](vector<long long>& cnt, long long &uW, long long &uV, long long &val, int b)->bool{
        if (cnt[b] >= items[b].q) return false;
        if (uW + items[b].m <= W && uV + items[b].l <= Vcap) {
            cnt[b] += 1;
            uW += items[b].m;
            uV += items[b].l;
            val += items[b].v;
            return true;
        }
        return false;
    };

    auto improvePairwise = [&](vector<long long>& cnt, long long &uW, long long &uV, long long &val)->bool{
        bool changed = false;
        // Precompute an order to try adding in descending "density"
        vector<pair<double,int>> addOrder;
        addOrder.reserve(n);
        for (int i = 0; i < n; ++i) {
            double mm = (double)items[i].m / (double)W;
            double ll = (double)items[i].l / (double)Vcap;
            double cost = (mm + ll) * 0.5;
            double score = (cost > 0) ? ((double)items[i].v / cost) : 1e300;
            addOrder.emplace_back(score, i);
        }
        sort(addOrder.begin(), addOrder.end(), greater<>());

        while (true) {
            if (timeExceeded()) break;
            bool progressed = false;

            // Try to add any single item
            for (auto &p : addOrder) {
                int b = p.second;
                if (tryAddOne(cnt, uW, uV, val, b)) {
                    progressed = true;
                    changed = true;
                    break;
                }
            }
            if (progressed) continue;

            long long bestGain = 0;
            pair<int,int> bestMove = {-1,-1};
            for (auto &p : addOrder) {
                int b = p.second;
                if (cnt[b] >= items[b].q) continue;
                // try removing one item a
                for (int a = 0; a < n; ++a) {
                    if (cnt[a] <= 0) continue;
                    long long newW = uW - items[a].m + items[b].m;
                    long long newV = uV - items[a].l + items[b].l;
                    if (newW <= W && newV <= Vcap) {
                        long long gain = items[b].v - items[a].v;
                        if (gain > bestGain) {
                            bestGain = gain;
                            bestMove = {a, b};
                        }
                    }
                }
            }
            if (bestGain > 0) {
                int a = bestMove.first, b = bestMove.second;
                cnt[a] -= 1;
                cnt[b] += 1;
                uW = uW - items[a].m + items[b].m;
                uV = uV - items[a].l + items[b].l;
                val = val - items[a].v + items[b].v;
                progressed = true;
                changed = true;
            }
            if (!progressed) break;
        }
        return changed;
    };

    auto improveTwoRemovals = [&](vector<long long>& cnt, long long &uW, long long &uV, long long &val)->bool{
        bool changed = false;
        vector<int> idxs(n);
        iota(idxs.begin(), idxs.end(), 0);

        while (true) {
            if (timeExceeded()) break;

            long long bestGain = 0;
            int bestB = -1, bestA1 = -1, bestA2 = -1;

            for (int b = 0; b < n; ++b) {
                if (cnt[b] >= items[b].q) continue;

                // Try removing two items (a1 and a2)
                for (int a1 = 0; a1 < n; ++a1) {
                    if (cnt[a1] <= 0) continue;
                    // remove a1 only as well (but pairwise pass already did); still we keep for coverage
                    long long newW1 = uW - items[a1].m + items[b].m;
                    long long newV1 = uV - items[a1].l + items[b].l;
                    if (newW1 <= W && newV1 <= Vcap) {
                        long long gain = items[b].v - items[a1].v;
                        if (gain > bestGain) {
                            bestGain = gain;
                            bestB = b; bestA1 = a1; bestA2 = -1;
                        }
                    }
                    for (int a2 = a1; a2 < n; ++a2) {
                        if (a1 == a2) {
                            if (cnt[a1] <= 1) continue;
                        } else {
                            if (cnt[a2] <= 0) continue;
                        }
                        long long newW = uW - items[a1].m - items[a2].m + items[b].m;
                        long long newV = uV - items[a1].l - items[a2].l + items[b].l;
                        if (newW <= W && newV <= Vcap) {
                            long long loss = items[a1].v + items[a2].v;
                            long long gain = items[b].v - loss;
                            if (gain > bestGain) {
                                bestGain = gain;
                                bestB = b; bestA1 = a1; bestA2 = a2;
                            }
                        }
                    }
                }
            }

            if (bestGain > 0 && bestB != -1) {
                if (bestA2 == -1) {
                    // remove one
                    cnt[bestA1] -= 1;
                    uW -= items[bestA1].m;
                    uV -= items[bestA1].l;
                    val -= items[bestA1].v;
                } else {
                    // remove two (may be same)
                    if (bestA1 == bestA2) {
                        cnt[bestA1] -= 2;
                        uW -= 2 * items[bestA1].m;
                        uV -= 2 * items[bestA1].l;
                        val -= 2 * items[bestA1].v;
                    } else {
                        cnt[bestA1] -= 1;
                        cnt[bestA2] -= 1;
                        uW -= items[bestA1].m + items[bestA2].m;
                        uV -= items[bestA1].l + items[bestA2].l;
                        val -= items[bestA1].v + items[bestA2].v;
                    }
                }
                // add one b
                cnt[bestB] += 1;
                uW += items[bestB].m;
                uV += items[bestB].l;
                val += items[bestB].v;
                changed = true;
            } else {
                break;
            }
        }
        return changed;
    };

    auto refillByTightConstraint = [&](vector<long long>& cnt, long long &uW, long long &uV, long long &val) {
        double fracW = (double)uW / (double)W;
        double fracV = (double)uV / (double)Vcap;
        vector<int> ord(n);
        iota(ord.begin(), ord.end(), 0);
        if (fracW > fracV) {
            // mass is tighter, prioritize value per mass
            sort(ord.begin(), ord.end(), [&](int a, int b){
                double ra = (double)items[a].v / (double)items[a].m;
                double rb = (double)items[b].v / (double)items[b].m;
                if (ra != rb) return ra > rb;
                return a < b;
            });
        } else {
            // volume is tighter
            sort(ord.begin(), ord.end(), [&](int a, int b){
                double ra = (double)items[a].v / (double)items[a].l;
                double rb = (double)items[b].v / (double)items[b].l;
                if (ra != rb) return ra > rb;
                return a < b;
            });
        }
        // Try to add remaining items by that order
        for (int i : ord) {
            if (cnt[i] >= items[i].q) continue;
            if (items[i].m > W || items[i].l > Vcap) continue;
            long long remW = W - uW;
            long long remV = Vcap - uV;
            if (remW <= 0 || remV <= 0) break;
            long long byW = remW / items[i].m;
            long long byV = remV / items[i].l;
            long long add = min(items[i].q - cnt[i], min(byW, byV));
            if (add > 0) {
                cnt[i] += add;
                uW += add * items[i].m;
                uV += add * items[i].l;
                val += add * items[i].v;
            }
        }
    };

    Solution best;
    best.cnt.assign(n, 0);

    // Evaluate all orders and improvements within time
    for (const auto &ord : orders) {
        if (timeExceeded()) break;

        vector<long long> cnt(n, 0);
        long long uW = 0, uV = 0, val = 0;

        greedyFill(cnt, uW, uV, val, ord);

        // Improvement phases
        bool improved = true;
        int rounds = 0;
        while (improved && rounds < 5 && !timeExceeded()) {
            improved = false;
            if (improvePairwise(cnt, uW, uV, val)) improved = true;
            if (improveTwoRemovals(cnt, uW, uV, val)) improved = true;
            // Refill greedily with different strategies
            greedyFill(cnt, uW, uV, val, ord);
            refillByTightConstraint(cnt, uW, uV, val);
            rounds++;
        }

        if (val > best.value) {
            best.value = val;
            best.usedW = uW;
            best.usedV = uV;
            best.cnt = cnt;
        }
    }

    // As a final pass, try a small random perturbation to escape local opt (optional)
    std::mt19937_64 rng(1234567);
    for (int attempt = 0; attempt < 6 && !timeExceeded(); ++attempt) {
        vector<long long> cnt = best.cnt;
        long long uW = best.usedW, uV = best.usedV, val = best.value;

        // Randomly remove a few items to free space
        int removals = 1 + (rng() % 3);
        for (int r = 0; r < removals; ++r) {
            int i = rng() % n;
            if (cnt[i] > 0) {
                long long rem = 1 + (rng() % (int)min<long long>(cnt[i], 3));
                cnt[i] -= rem;
                uW -= rem * items[i].m;
                uV -= rem * items[i].l;
                val -= rem * items[i].v;
                if (uW < 0) uW = 0;
                if (uV < 0) uV = 0;
                if (val < 0) val = 0;
            }
        }
        // Refill by a random order
        vector<int> ord = orders[rng() % orders.size()];
        greedyFill(cnt, uW, uV, val, ord);
        improvePairwise(cnt, uW, uV, val);
        improveTwoRemovals(cnt, uW, uV, val);
        refillByTightConstraint(cnt, uW, uV, val);

        if (val > best.value) {
            best.value = val;
            best.usedW = uW;
            best.usedV = uV;
            best.cnt = cnt;
        }
    }

    // Output JSON with the same keys as input order
    cout << "{\n";
    for (int i = 0; i < n; ++i) {
        cout << " \"" << items[i].name << "\": " << best.cnt[i];
        if (i + 1 < n) cout << ",";
        cout << "\n";
    }
    cout << "}\n";
    return 0;
}