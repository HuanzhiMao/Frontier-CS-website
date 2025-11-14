#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> adj;
vector<int> assignment;

void partition(const vector<int>& nodes, int label_start, int num_parts) {
    if (num_parts == 1) {
        for (int v : nodes) assignment[v] = label_start;
        return;
    }
    int total = nodes.size();
    if (total == 0) return;
    int s1 = (total + 1) / 2;
    int tries = 10;
    long long min_cut = LLONG_MAX;
    vector<int> best_A, best_B;
    vector<char> in_group(adj.size(), 0);
    for (int v : nodes) in_group[v] = 1;
    for (int t = 0; t < tries; t++) {
        int idx = rand() % total;
        int first_start = nodes[idx];
        vector<int> order;
        order.reserve(total);
        vector<char> visited(adj.size(), 0);
        bool all_visited = false;
        int start = first_start;
        while (!all_visited) {
            if (visited[start] == 0) {
                queue<int> q;
                q.push(start);
                visited[start] = 1;
                while (!q.empty()) {
                    int u = q.front(); q.pop();
                    order.push_back(u);
                    for (int v : adj[u]) {
                        if (in_group[v] && visited[v] == 0) {
                            visited[v] = 1;
                            q.push(v);
                        }
                    }
                }
            }
            all_visited = true;
            start = -1;
            for (int v : nodes) {
                if (visited[v] == 0) {
                    start = v;
                    all_visited = false;
                    break;
                }
            }
            if (start == -1) break;
        }
        if (order.size() != (size_t)total) continue;
        vector<int> A(order.begin(), order.begin() + s1);
        vector<int> B(order.begin() + s1, order.end());
        long long cut = 0;
        vector<int> temp_part(adj.size(), 0);
        for (int v : A) temp_part[v] = 1;
        for (int v : B) temp_part[v] = 2;
        for (int v : nodes) {
            for (int nei : adj[v]) {
                if (in_group[nei] && temp_part[nei] != temp_part[v]) {
                    cut++;
                }
            }
        }
        cut /= 2;
        if (cut < min_cut) {
            min_cut = cut;
            best_A = std::move(A);
            best_B = std::move(B);
        }
    }
    if (min_cut == LLONG_MAX) {
        best_A.assign(nodes.begin(), nodes.begin() + s1);
        best_B.assign(nodes.begin() + s1, nodes.end());
    }
    partition(best_A, label_start, num_parts / 2);
    partition(best_B, label_start + num_parts / 2, num_parts / 2);
}

int main() {
    srand(time(0));
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m, k;
    double eps;
    cin >> n >> m >> k >> eps;
    adj.resize(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        if (u != v) {
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
    }
    for (int i = 1; i <= n; i++) {
        sort(adj[i].begin(), adj[i].end());
        auto it = unique(adj[i].begin(), adj[i].end());
        adj[i].resize(it - adj[i].begin());
    }
    assignment.resize(n + 1);
    vector<int> all_nodes(n);
    for (int i = 0; i < n; i++) all_nodes[i] = i + 1;
    partition(all_nodes, 1, k);
    for (int i = 1; i <= n; i++) {
        if (i > 1) cout << " ";
        cout << assignment[i];
    }
    cout << "\n";
    return 0;
}