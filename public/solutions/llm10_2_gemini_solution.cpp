#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <map>
#include <tuple>

using namespace std;

map<pair<int, int>, long long> query_cache;

long long do_query(int u, int v) {
    if (u == v) return 0;
    if (u > v) swap(u, v);
    if (query_cache.count({u, v})) {
        return query_cache[{u, v}];
    }
    cout << "? " << u << " " << v << endl;
    long long d;
    cin >> d;
    return query_cache[{u, v}] = d;
}

void solve() {
    int n;
    cin >> n;
    query_cache.clear();

    if (n == 1) {
        cout << "!" << endl;
        return;
    }

    int A = 1;
    if (n > 2) {
        vector<long long> dist1(n + 1, 0);
        for (int i = 2; i <= n; ++i) {
            dist1[i] = do_query(1, i);
        }

        long long max_dist_from_1 = 0;
        for (int i = 1; i <= n; ++i) {
            if (dist1[i] > max_dist_from_1) {
                max_dist_from_1 = dist1[i];
                A = i;
            }
        }
    }

    vector<long long> distA(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        if (i == A) continue;
        distA[i] = do_query(A, i);
    }
    
    vector<int> v(n);
    iota(v.begin(), v.end(), 1);
    sort(v.begin(), v.end(), [&](int i, int j) {
        return distA[i] < distA[j];
    });
    
    map<long long, int> dist_to_node;
    for (int i = 1; i <= n; ++i) {
        dist_to_node[distA[i]] = i;
    }

    vector<int> parent(n + 1, 0);
    vector<tuple<int, int, long long>> edges;

    for (int i = 1; i < n; ++i) {
        int u = v[i];
        int prev_v = v[i - 1];
        
        long long d_u_prev = do_query(u, prev_v);
        long long d_lca_val = (distA[u] + distA[prev_v] - d_u_prev) / 2;
        int p_cand = dist_to_node[d_lca_val];

        vector<int> path_to_A;
        int curr = p_cand;
        while (curr != 0) {
            path_to_A.push_back(curr);
            curr = parent[curr];
        }
        reverse(path_to_A.begin(), path_to_A.end());

        int low = 0, high = path_to_A.size() - 1;
        int p_idx = 0;
        
        while(low <= high) {
            int mid_idx = low + (high - low) / 2;
            int mid_node = path_to_A[mid_idx];
            long long d_u_mid = do_query(u, mid_node);
            if (distA[u] == distA[mid_node] + d_u_mid) {
                p_idx = mid_idx;
                low = mid_idx + 1;
            } else {
                high = mid_idx - 1;
            }
        }
        parent[u] = path_to_A[p_idx];

        edges.emplace_back(u, parent[u], distA[u] - distA[parent[u]]);
    }

    cout << "! ";
    for (size_t i = 0; i < edges.size(); ++i) {
        cout << get<0>(edges[i]) << " " << get<1>(edges[i]) << " " << get<2>(edges[i]) << (i == edges.size() - 1 ? "" : " ");
    }
    cout << endl;
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