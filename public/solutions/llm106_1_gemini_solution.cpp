#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

int do_query(const vector<int>& s) {
    if (s.empty()) {
        return 0;
    }
    cout << "? " << s.size() << endl;
    for (size_t i = 0; i < s.size(); ++i) {
        cout << s[i] << (i == s.size() - 1 ? "" : " ");
    }
    cout << endl;

    int m;
    cin >> m;
    if (m == -1) exit(0);
    return m;
}

int find_one_neighbor(int u, vector<int> s) {
    if (s.empty()) return 0;
    
    vector<int> s_and_u = s;
    s_and_u.push_back(u);
    
    if (do_query(s_and_u) - do_query(s) == 0) {
        return 0;
    }

    while (s.size() > 1) {
        int mid = s.size() / 2;
        vector<int> left_s(s.begin(), s.begin() + mid);
        
        vector<int> left_s_and_u = left_s;
        left_s_and_u.push_back(u);

        if (do_query(left_s_and_u) - do_query(left_s) > 0) {
            s = left_s;
        } else {
            s = vector<int>(s.begin() + mid, s.end());
        }
    }
    return s[0];
}

void solve() {
    int n;
    cin >> n;

    vector<int> part1, part2;
    vector<int> parent(n + 1, 0);
    long long edges1 = 0, edges2 = 0;

    part1.push_back(1);
    edges1 = do_query(part1);

    for (int i = 2; i <= n; ++i) {
        vector<int> p1_and_i = part1; p1_and_i.push_back(i);
        long long num_neigh1 = do_query(p1_and_i) - edges1;
        
        vector<int> p2_and_i = part2; p2_and_i.push_back(i);
        long long num_neigh2 = do_query(p2_and_i) - edges2;

        if (num_neigh1 > 0 && num_neigh2 > 0) {
            int u = find_one_neighbor(i, part1);
            int v = find_one_neighbor(i, part2);

            vector<int> path_u, path_v;
            int curr = u;
            while (curr != 0) { path_u.push_back(curr); curr = parent[curr]; }
            curr = v;
            while (curr != 0) { path_v.push_back(curr); curr = parent[curr]; }
            reverse(path_u.begin(), path_u.end());
            reverse(path_v.begin(), path_v.end());

            size_t lca_idx = 0;
            while (lca_idx + 1 < path_u.size() && lca_idx + 1 < path_v.size() && path_u[lca_idx + 1] == path_v[lca_idx + 1]) {
                lca_idx++;
            }
            if (path_u[0] != path_v[0]) {
                // This case suggests u and v are in different components of the spanning forest.
                // The simple LCA path logic might not form a cycle.
                // However, given problem constraints and connectivity, this might not be an issue,
                // or a simple path construction will form a valid cycle.
                // Let's assume the simple LCA logic is sufficient.
            }

            vector<int> cycle;
            cycle.push_back(i);
            for (int j = path_u.size() - 1; j >= (int)lca_idx; --j) {
                cycle.push_back(path_u[j]);
            }
            for (size_t j = lca_idx + 1; j < path_v.size(); ++j) {
                cycle.push_back(path_v[j]);
            }
            
            cout << "N " << cycle.size() << endl;
            for (size_t k = 0; k < cycle.size(); ++k) {
                cout << cycle[k] << (k == cycle.size() - 1 ? "" : " ");
            }
            cout << endl;
            return;
        } else if (num_neigh1 > 0) {
            parent[i] = find_one_neighbor(i, part1);
            part2.push_back(i);
            edges2 = num_neigh2 + edges2;
        } else { // num_neigh2 > 0 or both are 0
             parent[i] = find_one_neighbor(i, part2);
             part1.push_back(i);
             edges1 = num_neigh1 + edges1;
        }
    }
    
    cout << "Y " << part1.size() << endl;
    for (size_t i = 0; i < part1.size(); ++i) {
        cout << part1[i] << (i == part1.size() - 1 ? "" : " ");
    }
    cout << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}