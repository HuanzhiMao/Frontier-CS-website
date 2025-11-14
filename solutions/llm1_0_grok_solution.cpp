#include <bits/stdc++.h>
using namespace std;

int main() {
    string s;
    string line;
    while (getline(cin, line)) s += line;
    string input;
    for (char c : s) if (!isspace(c)) input += c;
    map<string, vector<long long>> treasures;
    vector<string> keys;
    size_t pos = 1;
    while (pos < input.size() - 1) {
        size_t quote1 = pos;
        size_t quote2 = input.find('"', quote1 + 1);
        string key = input.substr(quote1 + 1, quote2 - quote1 - 1);
        keys.push_back(key);
        size_t colon = input.find(':', quote2 + 1);
        size_t brack1 = input.find('[', colon + 1);
        size_t start = brack1 + 1;
        vector<long long> vals(4);
        for (int i = 0; i < 4; i++) {
            size_t comma = input.find(i < 3 ? ',' : ']', start);
            string numstr = input.substr(start, comma - start);
            vals[i] = stoll(numstr);
            start = comma + 1;
        }
        treasures[key] = vals;
        size_t next = input.find(',', start);
        if (next == string::npos) break;
        pos = next + 1;
    }
    long long M = 20000000LL;
    long long L = 25000000LL;
    int num_alphas = 101;
    long long best_value = -1;
    map<string, long long> best_chosen;
    for (int i = 0; i < num_alphas; i++) {
        double alpha = i * 1.0 / (num_alphas - 1);
        vector<pair<double, string>> densities;
        for (auto& key : keys) {
            auto& t = treasures[key];
            long long v = t[1];
            double nm = t[2] * 1.0 / M;
            double nl = t[3] * 1.0 / L;
            double combined = alpha * nm + (1 - alpha) * nl + 1e-9;
            double density = v / combined;
            densities.emplace_back(-density, key);
        }
        sort(densities.begin(), densities.end());
        long long cur_m = M;
        long long cur_l = L;
        map<string, long long> cur_chosen;
        for (auto& p : densities) {
            string key = p.second;
            auto& t = treasures[key];
            long long qq = t[0];
            long long mm = t[2];
            long long ll = t[3];
            long long take = min(qq, min(cur_m / mm, cur_l / ll));
            if (take > 0) {
                cur_chosen[key] = take;
                cur_m -= take * mm;
                cur_l -= take * ll;
            }
        }
        long long cur_value = 0;
        for (auto& pr : cur_chosen) {
            string key = pr.first;
            long long cnt = pr.second;
            cur_value += cnt * treasures[key][1];
        }
        if (cur_value > best_value) {
            best_value = cur_value;
            best_chosen = cur_chosen;
        }
    }
    map<string, long long> chosen = best_chosen;
    for (auto& key : keys) {
        if (chosen.find(key) == chosen.end()) chosen[key] = 0;
    }
    long long used_m = 0, used_l = 0;
    for (auto& pr : chosen) {
        string key = pr.first;
        auto& t = treasures[key];
        long long cnt = pr.second;
        used_m += cnt * t[2];
        used_l += cnt * t[3];
    }
    long long remain_m = M - used_m;
    long long remain_l = L - used_l;
    // Improvement loop for swap
    for (;;) {
        long long best_delta = -1;
        string best_A, best_B;
        long long best_k = 0;
        for (auto& A : keys) {
            if (chosen[A] <= 0) continue;
            auto& tA = treasures[A];
            long long vA = tA[1];
            long long mA = tA[2];
            long long lA = tA[3];
            for (auto& B : keys) {
                if (A == B) continue;
                auto& tB = treasures[B];
                if (chosen[B] >= tB[0]) continue;
                long long vB = tB[1];
                long long mB = tB[2];
                long long lB = tB[3];
                long long qb = tB[0] - chosen[B];
                long long k = min(qb, min((remain_m + mA) / mB, (remain_l + lA) / lB));
                if (k >= 1) {
                    long long delta = k * vB - vA;
                    if (delta > best_delta) {
                        best_delta = delta;
                        best_A = A;
                        best_B = B;
                        best_k = k;
                    }
                }
            }
        }
        if (best_delta > 0) {
            chosen[best_A]--;
            chosen[best_B] += best_k;
            remain_m += treasures[best_A][2] - best_k * treasures[best_B][2];
            remain_l += treasures[best_A][3] - best_k * treasures[best_B][3];
        } else {
            break;
        }
    }
    // Add loop
    for (;;) {
        long long best_delta = -1;
        string best_B;
        long long best_kk = 0;
        for (auto& B : keys) {
            auto& tB = treasures[B];
            if (chosen[B] >= tB[0]) continue;
            long long vB = tB[1];
            long long mB = tB[2];
            long long lB = tB[3];
            long long qb = tB[0] - chosen[B];
            long long kk = min(qb, min(remain_m / mB, remain_l / lB));
            if (kk >= 1) {
                long long delta = kk * vB;
                if (delta > best_delta) {
                    best_delta = delta;
                    best_B = B;
                    best_kk = kk;
                }
            }
        }
        if (best_delta > 0) {
            chosen[best_B] += best_kk;
            remain_m -= best_kk * treasures[best_B][2];
            remain_l -= best_kk * treasures[best_B][3];
        } else {
            break;
        }
    }
    // Output
    cout << "{";
    bool first = true;
    for (auto& key : keys) {
        if (!first) cout << ",";
        first = false;
        cout << "\"" << key << "\":" << chosen[key];
    }
    cout << "}" << endl;
    return 0;
}