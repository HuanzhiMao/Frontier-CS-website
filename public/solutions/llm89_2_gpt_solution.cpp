#include <bits/stdc++.h>
using namespace std;

int n;
vector<pair<int,int>> edges;

inline void flush_out() {
    cout.flush();
}

int read_int_or_exit() {
    int x;
    if (!(cin >> x)) {
        exit(0);
    }
    if (x == -1) exit(0);
    return x;
}

// Query: is v on the path between a and b?
bool on_path(int a, int b, int v) {
    cout << "? 2 " << v << " " << a << " " << b << "\n";
    flush_out();
    int res = read_int_or_exit();
    return res == 1;
}

// Partition U \ {p} into components after removing p.
// Two vertices x, y are in the same component iff p is NOT on the path between x and y.
vector<vector<int>> partition_by_pivot(const vector<int>& U, int p) {
    vector<vector<int>> groups;
    vector<int> reps; // representative of each group
    for (int x : U) {
        if (x == p) continue;
        bool placed = false;
        for (size_t i = 0; i < reps.size(); ++i) {
            int y = reps[i];
            // If p is NOT on path(x, y), they are in same component.
            if (!on_path(x, y, p)) {
                groups[i].push_back(x);
                placed = true;
                break;
            }
        }
        if (!placed) {
            groups.push_back(vector<int>{x});
            reps.push_back(x);
        }
    }
    return groups;
}

// For a group G (component after removing p), find the unique neighbor of p within G.
// It's the unique vertex c in G such that c lies on every path from p to any x in G.
int find_neighbor(const vector<int>& G, int p) {
    int cand = G[0];
    for (size_t i = 1; i < G.size(); ++i) {
        int x = G[i];
        // If cand is NOT on path(p, x), replace candidate with x
        if (!on_path(p, x, cand)) cand = x;
    }
    // Optional verification (can be omitted to save queries):
    // for (int x : G) {
    //     if (x == cand) continue;
    //     if (!on_path(p, x, cand)) {
    //         // Should not happen
    //     }
    // }
    return cand;
}

void solve(const vector<int>& U) {
    if (U.size() <= 1) return;
    int p = U[0];
    auto groups = partition_by_pivot(U, p);
    for (auto &G : groups) {
        int c = find_neighbor(G, p);
        edges.emplace_back(p, c);
    }
    for (auto &G : groups) {
        solve(G);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> n)) return 0;

    vector<int> all(n);
    iota(all.begin(), all.end(), 1);
    solve(all);

    cout << "!\n";
    for (auto &e : edges) {
        cout << e.first << " " << e.second << "\n";
    }
    flush_out();
    return 0;
}