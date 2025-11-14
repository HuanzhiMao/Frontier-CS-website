#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

struct Treasure {
    string name;
    ll q, v, m, l;
};

vector<Treasure> items(12);
vector<int> best_counts(12, 0);
ll best_val = 0;

ll M = 20000000LL;
ll V = 25000000LL;

void update_best(const vector<int>& curr, ll val) {
    if (val > best_val) {
        best_val = val;
        best_counts = curr;
    }
}

double compute_upper(int start, ll rm, ll rv) {
    if (rm <= 0 || rv <= 0) return 0;
    double alpha = 1.0 / rm;
    double beta = 1.0 / rv;
    double W = alpha * rm + beta * rv;
    vector<pair<double, int>> dens;
    for (int i = start; i < 12; ++i) {
        double wi = alpha * items[i].m + beta * items[i].l;
        if (wi <= 0) continue;
        double den = (double)items[i].v / wi;
        dens.emplace_back(-den, i);
    }
    sort(dens.begin(), dens.end());
    double total = 0;
    double curw = 0;
    for (auto& p : dens) {
        int i = p.second;
        double wi = alpha * items[i].m + beta * items[i].l;
        double remw = W - curw;
        if (remw <= 0) break;
        double x = min((double)items[i].q, remw / wi);
        total += x * items[i].v;
        curw += x * wi;
    }
    return total;
}

void rec(int idx, ll cm, ll cv, ll cval, vector<int>& curr) {
    if (idx == 12) {
        update_best(curr, cval);
        return;
    }
    double up = compute_upper(idx, M - cm, V - cv);
    if (cval + up <= best_val) return;
    Treasure& it = items[idx];
    ll maxk = min(it.q, min((M - cm) / it.m, (V - cv) / it.l));
    for (ll k = maxk; k >= 0; --k) {
        ll nm = cm + k * it.m;
        ll nv = cv + k * it.l;
        ll nval = cval + k * it.v;
        double nup = compute_upper(idx + 1, M - nm, V - nv);
        if (nval + nup <= best_val) continue;
        curr[idx] = k;
        rec(idx + 1, nm, nv, nval, curr);
    }
}

pair<ll, vector<int>> do_greedy(function<double(const Treasure&)> get_den) {
    vector<pair<double, int>> gdens(12);
    for (int j = 0; j < 12; j++) {
        gdens[j] = { -get_den(items[j]), j };
    }
    sort(gdens.begin(), gdens.end());
    ll gm = 0, gv = 0, gval = 0;
    vector<int> gc(12, 0);
    for (auto p : gdens) {
        int j = p.second;
        auto& it = items[j];
        ll mk = min(it.q, (M - gm) / it.m);
        mk = min(mk, (V - gv) / it.l);
        gc[j] = mk;
        gval += mk * it.v;
        gm += mk * it.m;
        gv += mk * it.l;
    }
    return {gval, gc};
}

int main() {
    string full;
    string line;
    while (getline(cin, line)) {
        full += line;
    }
    string json;
    for (char c : full) {
        if (!isspace(c)) json += c;
    }
    map<string, vector<ll>> data;
    size_t pos = 1;
    for (int cat = 0; cat < 12; ++cat) {
        size_t start_key = json.find('"', pos);
        size_t end_key = json.find('"', start_key + 1);
        string key = json.substr(start_key + 1, end_key - start_key - 1);
        pos = end_key + 2; // after ":"
        size_t start_list = pos;
        size_t end_list = json.find(']', start_list);
        string list = json.substr(start_list + 1, end_list - start_list - 1);
        vector<ll> vals(4);
        stringstream ss(list);
        string token;
        for (int i = 0; i < 4; ++i) {
            getline(ss, token, ',');
            vals[i] = stoll(token);
        }
        data[key] = vals;
        pos = end_list + 1;
        if (json[pos] == ',') ++pos;
    }
    int ii = 0;
    for (auto& p : data) {
        items[ii].name = p.first;
        items[ii].q = p.second[0];
        items[ii].v = p.second[1];
        items[ii].m = p.second[2];
        items[ii].l = p.second[3];
        ++ii;
    }
    vector<pair<ll, int>> sortkey(12);
    for (int i = 0; i < 12; i++) {
        ll mk = min(items[i].q, min(M / items[i].m, V / items[i].l)) + 1;
        sortkey[i] = {mk, i};
    }
    sort(sortkey.begin(), sortkey.end());
    vector<Treasure> sorted_items(12);
    for (int i = 0; i < 12; i++) {
        sorted_items[i] = items[sortkey[i].second];
    }
    items = sorted_items;
    auto try1 = do_greedy([](const Treasure& t) { return (double)t.v / t.m; });
    if (try1.first > best_val) {
        best_val = try1.first;
        best_counts = try1.second;
    }
    auto try2 = do_greedy([](const Treasure& t) { return (double)t.v / t.l; });
    if (try2.first > best_val) {
        best_val = try2.first;
        best_counts = try2.second;
    }
    auto try3 = do_greedy([](const Treasure& t) { return (double)t.v / (t.m + t.l); });
    if (try3.first > best_val) {
        best_val = try3.first;
        best_counts = try3.second;
    }
    auto try4 = do_greedy([](const Treasure& t) { return (double)t.v / (t.m / (double)M + t.l / (double)V); });
    if (try4.first > best_val) {
        best_val = try4.first;
        best_counts = try4.second;
    }
    auto try5 = do_greedy([](const Treasure& t) { double d1 = (double)t.v / t.m; double d2 = (double)t.v / t.l; return min(d1, d2); });
    if (try5.first > best_val) {
        best_val = try5.first;
        best_counts = try5.second;
    }
    vector<int> curr(12, 0);
    rec(0, 0, 0, 0, curr);
    cout << "{" << endl;
    bool first = true;
    for (int i = 0; i < 12; i++) {
        if (!first) cout << "," << endl;
        first = false;
        cout << "    \"" << items[i].name << "\": " << best_counts[i];
    }
    cout << endl << "}" << endl;
    return 0;
}