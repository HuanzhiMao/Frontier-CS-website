#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    if (n == -1) exit(0);
    long long E;
    cout << "? " << n << endl;
    for (int i = 1; i <= n; ++i) cout << i << " ";
    cout << endl;
    cout.flush();
    cin >> E;
    if (E == -1) exit(0);
    vector<long long> deg(n + 1);
    for (int v = 1; v <= n; ++v) {
        cout << "? " << n - 1 << endl;
        for (int i = 1; i <= n; ++i) if (i != v) cout << i << " ";
        cout << endl;
        cout.flush();
        long long Ev;
        cin >> Ev;
        if (Ev == -1) exit(0);
        deg[v] = E - Ev;
    }
    vector<int> parent(n + 1, 0);
    vector<int> C = {1};
    set<int> outside;
    for (int i = 2; i <= n; ++i) outside.insert(i);
    vector<pair<int, int>> tree_edges;
    while (!outside.empty()) {
        int u;
        vector<int> curr = C;
        if (C.size() == 1) {
            u = C[0];
        } else {
            vector<int> out_list(outside.begin(), outside.end());
            cout << "? " << outside.size() << endl;
            for (int x : out_list) cout << x << " ";
            cout << endl;
            cout.flush();
            long long e_out;
            cin >> e_out;
            if (e_out == -1) exit(0);
            while (curr.size() > 1) {
                int mid = curr.size() / 2;
                vector<int> A1(curr.begin(), curr.begin() + mid);
                vector<int> union_list = A1;
                union_list.insert(union_list.end(), out_list.begin(), out_list.end());
                sort(union_list.begin(), union_list.end());
                cout << "? " << union_list.size() << endl;
                for (int x : union_list) cout << x << " ";
                cout << endl;
                cout.flush();
                long long e_union;
                cin >> e_union;
                if (e_union == -1) exit(0);
                cout << "? " << A1.size() << endl;
                for (int x : A1) cout << x << " ";
                cout << endl;
                cout.flush();
                long long e_A1;
                cin >> e_A1;
                if (e_A1 == -1) exit(0);
                long long bet = e_union - e_A1 - e_out;
                if (bet > 0) {
                    curr = A1;
                } else {
                    curr = vector<int>(curr.begin() + mid, curr.end());
                }
            }
            u = curr[0];
        }
        vector<int> curr_B(outside.begin(), outside.end());
        while (curr_B.size() > 1) {
            int mid = curr_B.size() / 2;
            vector<int> B1(curr_B.begin(), curr_B.begin() + mid);
            vector<int> union_list = B1;
            union_list.push_back(u);
            sort(union_list.begin(), union_list.end());
            cout << "? " << union_list.size() << endl;
            for (int x : union_list) cout << x << " ";
            cout << endl;
            cout.flush();
            long long e_union2;
            cin >> e_union2;
            if (e_union2 == -1) exit(0);
            cout << "? " << B1.size() << endl;
            for (int x : B1) cout << x << " ";
            cout << endl;
            cout.flush();
            long long e_B1;
            cin >> e_B1;
            if (e_B1 == -1) exit(0);
            long long num = e_union2 - e_B1;
            if (num > 0) {
                curr_B = B1;
            } else {
                curr_B = vector<int>(curr_B.begin() + mid, curr_B.end());
            }
        }
        int v = curr_B[0];
        parent[v] = u;
        tree_edges.emplace_back(u, v);
        C.push_back(v);
        outside.erase(v);
    }
    vector<int> dist(n + 1, -1);
    dist[1] = 0;
    queue<int> q;
    q.push(1);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        // find children
        for (auto [x, y] : tree_edges) {
            if (x == u && dist[y] == -1) {
                dist[y] = dist[u] + 1;
                q.push(y);
            } else if (y == u && dist[x] == -1) {
                dist[x] = dist[u] + 1;
                q.push(x);
            }
        }
    }
    vector<int> color0, color1;
    for (int i = 1; i <= n; ++i) {
        if (dist[i] % 2 == 0) color0.push_back(i);
        else color1.push_back(i);
    }
    // query m0
    cout << "? " << color0.size() << endl;
    for (int x : color0) cout << x << " ";
    cout << endl;
    cout.flush();
    long long m0;
    cin >> m0;
    if (m0 == -1) exit(0);
    // query m1
    cout << "? " << color1.size() << endl;
    for (int x : color1) cout << x << " ";
    cout << endl;
    cout.flush();
    long long m1;
    cin >> m1;
    if (m1 == -1) exit(0);
    if (m0 == 0 && m1 == 0) {
        cout << "Y " << color0.size() << endl;
        for (int x : color0) cout << x << " ";
        cout << endl;
        cout.flush();
        return 0;
    }
    // find odd cycle
    vector<int> col;
    long long mm;
    if (m0 > 0) {
        col = color0;
        mm = m0;
    } else {
        col = color1;
        mm = m1;
    }
    // find one w in col with positive neighbor in col
    vector<int> K = col;
    long long e_K = mm;
    vector<int> curr_K = K;
    while (curr_K.size() > 1) {
        int mid = curr_K.size() / 2;
        vector<int> K1(curr_K.begin(), curr_K.begin() + mid);
        vector<int> K2(curr_K.begin() + mid, curr_K.end());
        cout << "? " << K1.size() << endl;
        for (int x : K1) cout << x << " ";
        cout << endl;
        cout.flush();
        long long e_K1;
        cin >> e_K1;
        if (e_K1 == -1) exit(0);
        cout << "? " << K2.size() << endl;
        for (int x : K2) cout << x << " ";
        cout << endl;
        cout.flush();
        long long e_K2;
        cin >> e_K2;
        if (e_K2 == -1) exit(0);
        long long summ = e_K + e_K1 - e_K2;
        if (summ > 0) {
            curr_K = K1;
        } else {
            curr_K = K2;
        }
    }
    int w = curr_K[0];
    // now find one v in col connected to w
    vector<int> curr_col = col;
    while (curr_col.size() > 1) {
        int mid = curr_col.size() / 2;
        vector<int> B1(curr_col.begin(), curr_col.begin() + mid);
        vector<int> union_list = B1;
        union_list.push_back(w);
        sort(union_list.begin(), union_list.end());
        auto it = unique(union_list.begin(), union_list.end());
        union_list.resize(distance(union_list.begin(), it));
        cout << "? " << union_list.size() << endl;
        for (int x : union_list) cout << x << " ";
        cout << endl;
        cout.flush();
        long long e_union2;
        cin >> e_union2;
        if (e_union2 == -1) exit(0);
        cout << "? " << B1.size() << endl;
        for (int x : B1) cout << x << " ";
        cout << endl;
        cout.flush();
        long long e_B1;
        cin >> e_B1;
        if (e_B1 == -1) exit(0);
        long long num = e_union2 - e_B1;
        if (num > 0) {
            curr_col = B1;
        } else {
            curr_col = vector<int>(curr_col.begin() + mid, curr_col.end());
        }
    }
    int v = curr_col[0];
    if (v == w) {
        // should not happen
        exit(1);
    }
    // now find tree path from w to v
    vector<int> path_w;
    int cur = w;
    while (cur != 0) {
        path_w.push_back(cur);
        cur = parent[cur];
    }
    reverse(path_w.begin(), path_w.end());
    vector<int> path_v;
    cur = v;
    while (cur != 0) {
        path_v.push_back(cur);
        cur = parent[cur];
    }
    reverse(path_v.begin(), path_v.end());
    // find lca
    int lca = 1;
    int min_len = min(path_w.size(), path_v.size());
    for (int i = 0; i < min_len; ++i) {
        if (path_w[i] == path_v[i]) lca = path_w[i];
        else break;
    }
    // build path from w to lca to v
    vector<int> cycle;
    for (int i = 0; path_w[i] != lca; ++i) {
        cycle.push_back(path_w[i]);
    }
    cycle.push_back(lca);
    vector<int> from_lca_to_v;
    for (int i = 0; path_v[i] != lca; ++i) {
        from_lca_to_v.push_back(path_v[i]);
    }
    from_lca_to_v.push_back(lca);
    reverse(from_lca_to_v.begin(), from_lca_to_v.end());
    for (size_t i = 1; i < from_lca_to_v.size(); ++i) {  // start from 1 to avoid duplicate lca
        cycle.push_back(from_lca_to_v[i]);
    }
    // now cycle is w ... lca ... v
    // output N length cycle w ... v
    cout << "N " << cycle.size() << endl;
    for (int x : cycle) cout << x << " ";
    cout << endl;
    cout.flush();
    return 0;
}