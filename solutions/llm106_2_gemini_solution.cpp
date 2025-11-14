#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <set>
#include <queue>

using namespace std;

int n;
vector<vector<int>> adj;
vector<int> color;
vector<int> parent;

long long ask(const set<int>& s) {
    if (s.empty()) {
        return 0;
    }
    cout << "? " << s.size() << endl;
    bool first = true;
    for (int v : s) {
        if (!first) {
            cout << " ";
        }
        cout << v;
        first = false;
    }
    cout << endl;
    long long res;
    cin >> res;
    if (res == -1) exit(0);
    return res;
}

// Finds one neighbor of u in C
int find_neighbor(int u, const vector<int>& C) {
    if (C.empty()) return -1;
    
    int low = 0, high = C.size() - 1;
    int neighbor_idx = -1;

    while(low <= high) {
        int mid = low + (high - low) / 2;
        set<int> prefix_set;
        for (int i = 0; i <= mid; ++i) {
            prefix_set.insert(C[i]);
        }
        
        long long q_prefix = ask(prefix_set);
        prefix_set.insert(u);
        long long q_u_prefix = ask(prefix_set);

        if (q_u_prefix > q_prefix) {
            neighbor_idx = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return C[neighbor_idx];
}

// Finds one edge in S, returns {-1, -1} if none
pair<int, int> find_edge_in_set(const set<int>& S) {
    if (S.size() < 2) return {-1, -1};
    long long q_S = ask(S);
    if (q_S == 0) return {-1, -1};
    
    vector<int> s_vec(S.begin(), S.end());
    if (s_vec.size() == 2) return {s_vec[0], s_vec[1]};
    
    int u = s_vec[0];
    set<int> s_rest = S;
    s_rest.erase(u);
    
    vector<int> s_rest_vec(s_rest.begin(), s_rest.end());

    set<int> temp_set = s_rest;
    long long q_s_rest = ask(temp_set);
    temp_set.insert(u);
    long long q_u_s_rest = ask(temp_set);

    if (q_u_s_rest > q_s_rest) {
        int v = find_neighbor(u, s_rest_vec);
        return {u, v};
    } else {
        return find_edge_in_set(s_rest);
    }
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n;

    adj.resize(n + 1);

    set<int> V_T;
    V_T.insert(1);
    set<int> U;
    for (int i = 2; i <= n; ++i) {
        U.insert(i);
    }

    while (V_T.size() < n) {
        long long q_VT = ask(V_T);
        vector<int> u_vec(U.begin(), U.end());
        
        int v_idx = -1;
        int low = 0, high = u_vec.size() - 1;
        while(low <= high) {
            int mid = low + (high-low)/2;
            set<int> prefix_u;
            for(int i = 0; i <= mid; ++i) prefix_u.insert(u_vec[i]);
            
            long long q_prefix_u = ask(prefix_u);
            set<int> union_set = V_T;
            union_set.insert(prefix_u.begin(), prefix_u.end());
            long long q_union = ask(union_set);

            if (q_union - q_VT - q_prefix_u > 0) {
                v_idx = mid;
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }
        int v = u_vec[v_idx];

        vector<int> vt_vec(V_T.begin(), V_T.end());
        int u = find_neighbor(v, vt_vec);

        adj[u].push_back(v);
        adj[v].push_back(u);
        V_T.insert(v);
        U.erase(v);
    }

    color.assign(n + 1, -1);
    parent.assign(n + 1, 0);
    queue<int> q;

    color[1] = 0;
    q.push(1);
    
    set<int> P0, P1;
    P0.insert(1);

    queue<int> bfs_q;
    bfs_q.push(1);
    vector<bool> visited(n + 1, false);
    visited[1] = true;

    while (!bfs_q.empty()) {
        int u_node = bfs_q.front();
        bfs_q.pop();

        for (int v_node : adj[u_node]) {
            if (!visited[v_node]) {
                visited[v_node] = true;
                parent[v_node] = u_node;
                color[v_node] = 1 - color[u_node];
                if (color[v_node] == 0) P0.insert(v_node);
                else P1.insert(v_node);
                bfs_q.push(v_node);
            }
        }
    }
    
    long long q_P0 = ask(P0);
    if (q_P0 == 0) {
        long long q_P1 = ask(P1);
        if (q_P1 == 0) {
            cout << "Y " << P0.size() << endl;
            bool first = true;
            for (int node : P0) {
                if (!first) cout << " ";
                cout << node;
                first = false;
            }
            cout << endl;
            return 0;
        }
    }
    
    set<int> faulty_set = (q_P0 > 0) ? P0 : P1;
    pair<int, int> edge = find_edge_in_set(faulty_set);
    int u = edge.first;
    int v = edge.second;

    vector<int> path_u, path_v;
    set<int> u_ancestors;
    int curr = u;
    while(curr != 0) {
        path_u.push_back(curr);
        u_ancestors.insert(curr);
        curr = parent[curr];
    }
    curr = v;
    while(u_ancestors.find(curr) == u_ancestors.end()) {
        path_v.push_back(curr);
        curr = parent[curr];
    }
    int lca = curr;

    vector<int> cycle;
    cycle.push_back(lca);
    for(size_t i = 0; i < path_v.size(); ++i) {
        if (path_v[i] == v) {
            for(size_t j = i; j < path_v.size(); ++j) {
                 // this is wrong, path_v is from v to parent. just push v and reverse
            }
        }
    }
    reverse(path_v.begin(), path_v.end());
    for(int node : path_v) cycle.push_back(node);

    int lca_idx_in_u_path = -1;
    for(size_t i = 0; i < path_u.size(); ++i) {
        if(path_u[i] == lca) {
            lca_idx_in_u_path = i;
            break;
        }
    }
    for (int i = lca_idx_in_u_path - 1; i >= 0; --i) {
        cycle.push_back(path_u[i]);
    }
    
    cout << "N " << cycle.size() << endl;
    bool first = true;
    for (int node : cycle) {
        if (!first) cout << " ";
        cout << node;
        first = false;
    }
    cout << endl;
    

    return 0;
}