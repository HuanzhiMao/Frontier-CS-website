#include <bits/stdc++.h>

using namespace std;

struct Item {
    string name;
    long long q, v, m, l;
};

long long MAX_M = 20000000LL;
long long MAX_L = 25000000LL;

void improve(vector<long long>& cnt, const vector<Item>& ts, long long& val) {
    long long cur_m = 0, cur_l = 0, cur_v = 0;
    for (int k = 0; k < 12; k++) {
        cur_m += cnt[k] * ts[k].m;
        cur_l += cnt[k] * ts[k].l;
        cur_v += cnt[k] * ts[k].v;
    }
    val = cur_v;

    bool changed = true;
    while (changed) {
        changed = false;
        long long max_dv = 0;
        int best_i = -1, best_j = -1;

        // Try adds
        for (int j = 0; j < 12; j++) {
            if (cnt[j] >= ts[j].q) continue;
            long long dm = ts[j].m;
            long long dl = ts[j].l;
            long long dv = ts[j].v;
            if (cur_m + dm <= MAX_M && cur_l + dl <= MAX_L && dv > max_dv) {
                max_dv = dv;
                best_i = -1;
                best_j = j;
            }
        }

        // Try swaps
        for (int i = 0; i < 12; i++) {
            if (cnt[i] == 0) continue;
            for (int j = 0; j < 12; j++) {
                if (cnt[j] >= ts[j].q) continue;
                long long dm = ts[j].m - ts[i].m;
                long long dl = ts[j].l - ts[i].l;
                long long dv = ts[j].v - ts[i].v;
                if (dv > max_dv && cur_m + dm <= MAX_M && cur_l + dl <= MAX_L) {
                    max_dv = dv;
                    best_i = i;
                    best_j = j;
                }
            }
        }

        if (max_dv > 0) {
            changed = true;
            if (best_i == -1) {
                // add
                cnt[best_j]++;
                cur_m += ts[best_j].m;
                cur_l += ts[best_j].l;
                cur_v += ts[best_j].v;
            } else {
                // swap
                cnt[best_i]--;
                cnt[best_j]++;
                cur_m += ts[best_j].m - ts[best_i].m;
                cur_l += ts[best_j].l - ts[best_i].l;
                cur_v += ts[best_j].v - ts[best_i].v;
            }
        }
    }
    val = cur_v;
}

int main() {
    string s;
    string temp;
    while (getline(cin, temp)) {
        s += temp;
    }
    string input;
    for (char c : s) {
        if (!isspace(c)) input += c;
    }

    vector<string> keys;
    map<string, vector<long long>> data;
    size_t pos = 1; // after {
    while (pos < input.size() - 1) {
        assert(input[pos] == '"');
        pos++;
        size_t start = pos;
        while (input[pos] != '"') pos++;
        string key = input.substr(start, pos - start);
        pos++;
        assert(input[pos] == ':');
        pos++;
        assert(input[pos] == '[');
        pos++;
        vector<long long> vals;
        for (int i = 0; i < 4; i++) {
            start = pos;
            while (isdigit(input[pos])) pos++;
            string numstr = input.substr(start, pos - start);
            vals.push_back(stoll(numstr));
            if (i < 3) assert(input[pos] == ',');
            pos++;
        }
        pos--;
        assert(input[pos] == ']');
        pos++;
        data[key] = vals;
        keys.push_back(key);
        if (input[pos] == ',') pos++;
    }

    vector<Item> ts(12);
    for (int i = 0; i < 12; i++) {
        ts[i].name = keys[i];
        auto& vec = data[keys[i]];
        ts[i].q = vec[0];
        ts[i].v = vec[1];
        ts[i].m = vec[2];
        ts[i].l = vec[3];
    }

    long long best_val = 0;
    vector<long long> best_cnt(12, 0);
    int num_alphas = 101;
    for (int aa = 0; aa <= num_alphas; aa++) {
        double alpha = aa * 1.0 / num_alphas;
        vector<pair<double, int>> order;
        for (int i = 0; i < 12; i++) {
            double norm_m = ts[i].m * 1.0 / MAX_M;
            double norm_l = ts[i].l * 1.0 / MAX_L;
            double denom = alpha * norm_m + (1 - alpha) * norm_l;
            double density = ts[i].v / denom;
            order.emplace_back(-density, i);
        }
        sort(order.begin(), order.end());

        vector<long long> cnt(12, 0);
        long long rem_m = MAX_M;
        long long rem_l = MAX_L;
        for (auto& p : order) {
            int i = p.second;
            long long maxk = ts[i].q;
            if (ts[i].m > 0) maxk = min(maxk, rem_m / ts[i].m);
            if (ts[i].l > 0) maxk = min(maxk, rem_l / ts[i].l);
            cnt[i] = maxk;
            rem_m -= maxk * ts[i].m;
            rem_l -= maxk * ts[i].l;
        }

        long long this_val = 0;
        improve(cnt, ts, this_val);
        if (this_val > best_val) {
            best_val = this_val;
            best_cnt = cnt;
        }
    }

    cout << "{" << endl;
    for (int ii = 0; ii < 12; ii++) {
        cout << " \"" << ts[ii].name << "\": " << best_cnt[ii];
        if (ii < 11) cout << ",";
        cout << endl;
    }
    cout << "}" << endl;

    return 0;
}