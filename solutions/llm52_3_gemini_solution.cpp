#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <map>

using namespace std;

int n;
long long l1, l2;

map<pair<int, int>, int> query_cache;

int ask(int l, int r) {
    if (l > r) swap(l, r);
    if (l == r) return 1;
    if (query_cache.count({l, r})) {
        return query_cache[{l, r}];
    }
    cout << "1 " << l << " " << r << endl;
    int res;
    cin >> res;
    return query_cache[{l, r}] = res;
}

void swap_op(int i, int j) {
    if (i == j) return;
    cout << "2 " << i << " " << j << endl;
    int res;
    cin >> res;
    query_cache.clear();
}

bool are_adjacent(int u, int v) {
    if (u == v) return false;
    if (u > v) swap(u, v);
    if (v == u + 1) {
        return ask(u, v) == 1;
    }
    
    swap_op(u + 1, v);
    bool result = (ask(u, u + 1) == 1);
    swap_op(u + 1, v);
    return result;
}

vector<int> p;

void solve_primitive(int l, int r, int val_start) {
    int B = r - l + 1;
    if (B == 1) {
        p[l - 1] = val_start;
        return;
    }

    vector<vector<int>> adj_list(r + 1);
    
    if (B <= 250) { // O(B^2) is acceptable for smaller blocks
      for(int i = l; i <= r; ++i) {
        for(int j = i + 1; j <= r; ++j) {
          if(are_adjacent(i, j)) {
            adj_list[i].push_back(j);
            adj_list[j].push_back(i);
          }
        }
      }
    } else { // For larger blocks, assume only adjacent positions can be value-adjacent
         for(int i = l; i < r; ++i) {
             if (ask(i, i+1) == 1) {
                 adj_list[i].push_back(i+1);
                 adj_list[i+1].push_back(i);
             }
         }
    }

    int start_node = -1;
    for (int i = l; i <= r; ++i) {
        if (adj_list[i].size() == 1) {
            start_node = i;
            break;
        }
    }
    if (start_node == -1) { // A cycle or isolated nodes, fallback to l
        start_node = l;
    }


    vector<int> path;
    int curr = start_node;
    int prev = -1;
    for (int i = 0; i < B; ++i) {
        path.push_back(curr);
        int next_node = -1;
        for (int neighbor : adj_list[curr]) {
            if (neighbor != prev) {
                next_node = neighbor;
                break;
            }
        }
        prev = curr;
        curr = next_node;
        if (curr == -1 && i < B-1) { // Path broke, must be error in adjacencies
            // This can happen with the large B heuristic. Try to patch.
            vector<bool> on_path(r + 1, false);
            for(int node : path) on_path[node] = true;
            for(int j=l; j<=r; ++j) {
                if (!on_path[j]) {
                   curr = j;
                   break;
                }
            }
        }
    }

    // Now path[0] and path[B-1] are the endpoints of the path on indices
    // Let's assume p[path[0]] gets the smallest value
    for (int i = 0; i < B; ++i) {
        p[path[i] - 1] = val_start + i;
    }
}

void solve(int l, int r, int val_start) {
    if (l > r) return;
    if (l == r) {
        p[l - 1] = val_start;
        return;
    }

    int root_val = ask(l, r);
    if (root_val == 1) {
        solve_primitive(l, r, val_start);
        return;
    }
    
    int split_k = -1;
    
    int low = l, high = r - 1, mid;
    while(low <= high) {
        mid = low + (high - low) / 2;
        if (ask(l,r) == ask(l, mid) + ask(mid + 1, r)) {
            split_k = mid;
            break;
        }
        if (ask(l, mid) == 1) { // try to extend right
            low = mid + 1;
        } else { // there must be a split inside [l,mid]
            high = mid - 1;
        }
    }

    if (split_k == -1) { // binary search failed, linear scan
        for (int k = l; k < r; ++k) {
            if (root_val == ask(l, k) + ask(k + 1, r)) {
                split_k = k;
                break;
            }
        }
    }

    if (split_k != -1) {
        int left_len = split_k - l + 1;
        solve(l, split_k, val_start);
        solve(split_k + 1, r, val_start + left_len);
    } else { // Should not happen if root_val > 1
        solve_primitive(l, r, val_start);
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n >> l1 >> l2;
    p.resize(n);

    solve(1, n, 1);
    
    if (p[0] > n / 2 + 1) {
        for(int i = 0; i < n; ++i) {
            p[i] = n + 1 - p[i];
        }
    }

    cout << "3";
    for (int i = 0; i < n; ++i) {
        cout << " " << p[i];
    }
    cout << endl;

    return 0;
}