#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

typedef long long ll;

const ll INF = -3e18; // A very small value for infinity

struct Node {
    ll max_val;
    ll lazy;
};

vector<Node> tree;
int seg_tree_size;

void build(int node, int start, int end) {
    tree[node].max_val = INF;
    tree[node].lazy = 0;
    if (start == end) {
        return;
    }
    int mid = start + (end - start) / 2;
    build(2 * node, start, mid);
    build(2 * node + 1, mid + 1, end);
}

void push(int node, int start, int end) {
    if (tree[node].lazy != 0) {
        tree[node].max_val += tree[node].lazy;
        if (start != end) {
            tree[2 * node].lazy += tree[node].lazy;
            tree[2 * node + 1].lazy += tree[node].lazy;
        }
        tree[node].lazy = 0;
    }
}

void update_range(int node, int start, int end, int l, int r, ll val) {
    push(node, start, end);
    if (start > end || start > r || end < l) {
        return;
    }
    if (l <= start && end <= r) {
        tree[node].lazy += val;
        push(node, start, end);
        return;
    }
    int mid = start + (end - start) / 2;
    update_range(2 * node, start, mid, l, r, val);
    update_range(2 * node + 1, mid + 1, end, l, r, val);
    
    push(2 * node, start, mid);
    push(2 * node + 1, mid + 1, end);
    tree[node].max_val = max(tree[2 * node].max_val, tree[2 * node + 1].max_val);
}

void update_point(int node, int start, int end, int idx, ll val) {
    push(node, start, end);
    if (start == end) {
        tree[node].max_val = val;
        return;
    }
    int mid = start + (end - start) / 2;
    if (idx <= mid) {
        push(2 * node + 1, mid + 1, end);
        update_point(2 * node, start, mid, idx, val);
    } else {
        push(2 * node, start, mid);
        update_point(2 * node + 1, mid + 1, end, idx, val);
    }
    
    push(2*node, start, mid);
    push(2*node+1, mid+1, end);
    tree[node].max_val = max(tree[2*node].max_val, tree[2*node+1].max_val);
}

ll query_range(int node, int start, int end, int l, int r) {
    if (start > end || start > r || end < l) {
        return INF;
    }
    push(node, start, end);
    if (l <= start && end <= r) {
        return tree[node].max_val;
    }
    int mid = start + (end - start) / 2;
    ll p1 = query_range(2 * node, start, mid, l, r);
    ll p2 = query_range(2 * node + 1, mid + 1, end, l, r);
    return max(p1, p2);
}

int get_level(ll exp, const vector<ll>& b_prefix) {
    return upper_bound(b_prefix.begin() + 1, b_prefix.end(), exp) - (b_prefix.begin() + 1);
}

void solve() {
    int n, m;
    ll c;
    cin >> n >> m >> c;
    vector<ll> a(n);
    vector<ll> b(m);
    for (int i = 0; i < n; ++i) cin >> a[i];
    for (int i = 0; i < m; ++i) cin >> b[i];

    vector<ll> p_prefix(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        p_prefix[i + 1] = p_prefix[i] + a[i];
    }

    vector<ll> b_prefix(m + 1, 0);
    for (int i = 0; i < m; ++i) {
        b_prefix[i + 1] = b_prefix[i] + b[i];
    }

    vector<ll> dp(n + 1, INF);
    dp[0] = 0;

    seg_tree_size = n;
    tree.assign(4 * seg_tree_size, {INF, 0});
    build(1, 0, seg_tree_size - 1);

    for (int i = 1; i <= n; ++i) {
        if (i > 1) {
            for (int k = 1; k <= m; ++k) {
                ll upper = p_prefix[i] - b_prefix[k];
                ll lower = p_prefix[i - 1] - b_prefix[k];

                auto it_s = upper_bound(p_prefix.begin(), p_prefix.begin() + i - 1, lower);
                auto it_e = upper_bound(p_prefix.begin(), p_prefix.begin() + i - 1, upper);
                
                int j_s = it_s - p_prefix.begin();
                int j_e = (it_e - p_prefix.begin()) - 1;

                if (j_s <= j_e) {
                    update_range(1, 0, seg_tree_size - 1, j_s, j_e, 1);
                }
            }
        }
        
        ll val_for_new_segment = dp[i - 1] + get_level(a[i - 1], b_prefix);
        update_point(1, 0, seg_tree_size - 1, i - 1, val_for_new_segment);
        
        ll max_val = query_range(1, 0, seg_tree_size - 1, 0, i - 1);
        if (max_val > INF + 1e15) { 
            dp[i] = max_val - c;
        }
    }

    cout << dp[n] << "\n";
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