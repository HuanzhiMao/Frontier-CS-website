#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

using ll = long long;
const ll INF = -4e18; // Use a very small negative number for infinity

struct Node {
    ll max_dp;
};

vector<ll> A;
vector<ll> B_prefix;
vector<ll> unique_A;
vector<Node> tree;

// Get adventure rank for a given EXP
int get_level(ll exp) {
    if (exp < 0) return 0;
    // upper_bound finds the first element > exp
    // The index of this element (minus 1) gives the level k such that B[k] <= exp < B[k+1]
    auto it = upper_bound(B_prefix.begin() + 1, B_prefix.end(), exp);
    return (it - B_prefix.begin()) - 1;
}

// Build segment tree, initialize max_dp to INF
void build(int u, int l, int r) {
    tree[u].max_dp = INF;
    if (l == r) {
        return;
    }
    int mid = l + (r - l) / 2;
    build(2 * u, l, mid);
    build(2 * u + 1, mid + 1, r);
}

// Update a point in the segment tree
void update(int u, int l, int r, int pos, ll val) {
    if (l == r) {
        tree[u].max_dp = max(tree[u].max_dp, val);
        return;
    }
    int mid = l + (r - l) / 2;
    if (pos <= mid) {
        update(2 * u, l, mid, pos, val);
    } else {
        update(2 * u + 1, mid + 1, r, pos, val);
    }
    tree[u].max_dp = max(tree[2 * u].max_dp, tree[2 * u + 1].max_dp);
}

// The core query function to find max(dp[j] + level(A[i] - A[j]))
ll query(int u, int l, int r, ll current_A) {
    if (tree[u].max_dp == INF) {
        return INF;
    }

    ll x_l = unique_A[l - 1];
    ll x_r = unique_A[r - 1];
    
    // Level for smallest possible EXP in this node's A-range
    int level_low = get_level(current_A - x_r);
    // Level for largest possible EXP
    int level_high = get_level(current_A - x_l);

    // If all A_j in this node's range yield the same level, we can calculate the result directly
    if (level_low == level_high) {
        return level_low + tree[u].max_dp;
    }

    // Base case: leaf node
    if (l == r) {
        return level_high + tree[u].max_dp;
    }

    // Otherwise, recurse on children
    int mid = l + (r - l) / 2;
    ll res_left = query(2 * u, l, mid, current_A);
    ll res_right = query(2 * u + 1, mid + 1, r, current_A);
    return max(res_left, res_right);
}

void solve() {
    int n, m;
    ll c;
    cin >> n >> m >> c;
    
    vector<ll> a_input(n);
    A.assign(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        cin >> a_input[i];
        A[i + 1] = A[i] + a_input[i];
    }
    
    vector<ll> b_input(m);
    B_prefix.assign(m + 1, 0);
    for (int i = 0; i < m; ++i) {
        cin >> b_input[i];
        B_prefix[i + 1] = B_prefix[i] + b_input[i];
    }
    
    unique_A = A;
    sort(unique_A.begin(), unique_A.end());
    unique_A.erase(unique(unique_A.begin(), unique_A.end()), unique_A.end());

    int unique_A_size = unique_A.size();
    tree.assign(4 * unique_A_size + 4, Node());
    build(1, 1, unique_A_size);

    vector<ll> dp(n + 1, INF);
    dp[0] = 0;

    int pos_A0 = lower_bound(unique_A.begin(), unique_A.end(), A[0]) - unique_A.begin() + 1;
    update(1, 1, unique_A_size, pos_A0, dp[0]);

    for (int i = 1; i <= n; ++i) {
        ll max_val = query(1, 1, unique_A_size, A[i]);
        if (max_val > INF) {
            dp[i] = max_val - c;
        }

        int pos_Ai = lower_bound(unique_A.begin(), unique_A.end(), A[i]) - unique_A.begin() + 1;
        update(1, 1, unique_A_size, pos_Ai, dp[i]);
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