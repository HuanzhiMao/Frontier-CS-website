#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

using ll = long long;

int n, m;
ll c;
vector<ll> pa;
vector<ll> pb;

ll get_level(ll exp) {
    if (exp < 0) return -2e18; 
    auto it = upper_bound(pb.begin() + 1, pb.end(), exp);
    return distance(pb.begin(), it) - 1;
}

struct Func {
    ll dp_val;
    ll A_val;
    bool is_empty;

    Func() : dp_val(0), A_val(0), is_empty(true) {}
    Func(ll d, ll a) : dp_val(d), A_val(a), is_empty(false) {}

    ll eval(ll V) const {
        if (is_empty) return -2e18;
        ll level = get_level(V - A_val);
        if (level < 0) return -2e18;
        return dp_val + level;
    }
};

vector<Func> lichao_tree;
vector<ll> coords;

void insert_func(int node, int l, int r, Func new_func) {
    if (new_func.is_empty || l > r) {
        return;
    }

    if (lichao_tree[node].is_empty) {
        lichao_tree[node] = new_func;
        return;
    }

    int mid = l + (r - l) / 2;
    ll mid_coord = coords[mid];

    bool new_is_better_mid = new_func.eval(mid_coord) > lichao_tree[node].eval(mid_coord);
    if (new_is_better_mid) {
        swap(lichao_tree[node], new_func);
    }
    
    bool new_is_better_l = new_func.eval(coords[l]) > lichao_tree[node].eval(coords[l]);
    if (new_is_better_l != new_is_better_mid) {
        insert_func(2 * node, l, mid - 1, new_func);
    }
    
    bool new_is_better_r = new_func.eval(coords[r]) > lichao_tree[node].eval(coords[r]);
    if (new_is_better_r != new_is_better_mid) {
        insert_func(2 * node + 1, mid + 1, r, new_func);
    }
}

ll query(int node, int l, int r, int target_idx) {
    if (l > r) return -2e18;
    
    ll res = lichao_tree[node].eval(coords[target_idx]);
    
    int mid = l + (r - l) / 2;
    if (target_idx <= mid) {
        res = max(res, query(2 * node, l, mid, target_idx));
    } else {
        res = max(res, query(2 * node + 1, mid + 1, r, target_idx));
    }
    return res;
}

void solve() {
    cin >> n >> m >> c;
    vector<ll> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    vector<ll> b(m);
    for (int i = 0; i < m; ++i) cin >> b[i];

    pa.assign(n + 1, 0);
    for (int i = 0; i < n; ++i) pa[i + 1] = pa[i] + a[i];

    pb.assign(m + 1, 0);
    for (int i = 0; i < m; ++i) pb[i + 1] = pb[i] + b[i];

    coords = pa;
    sort(coords.begin(), coords.end());
    coords.erase(unique(coords.begin(), coords.end()), coords.end());
    
    int num_coords = coords.size();
    lichao_tree.assign(4 * num_coords + 4, Func());

    vector<ll> dp(n + 1, 0);
    
    Func f0(dp[0], pa[0]);
    insert_func(1, 0, num_coords - 1, f0);

    for (int i = 1; i <= n; ++i) {
        int coord_idx = lower_bound(coords.begin(), coords.end(), pa[i]) - coords.begin();
        ll val = query(1, 0, num_coords - 1, coord_idx);
        if (val <= -1e18) {
             dp[i] = -2e18; // Should not happen with dp[0]=0
        } else {
            dp[i] = val - c;
        }
        
        Func fi(dp[i], pa[i]);
        insert_func(1, 0, num_coords - 1, fi);
    }

    cout << dp[n] << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}