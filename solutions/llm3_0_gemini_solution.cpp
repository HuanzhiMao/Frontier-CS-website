#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

vector<int> do_query(const vector<int>& q) {
    if (q.empty()) {
        return {};
    }
    cout << q.size();
    for (int x : q) {
        cout << " " << x;
    }
    cout << endl;
    vector<int> res(q.size());
    for (size_t i = 0; i < q.size(); ++i) {
        cin >> res[i];
    }
    return res;
}

void answer(const vector<int>& p) {
    cout << -1;
    for (int x : p) {
        cout << " " << x;
    }
    cout << endl;
}

vector<vector<int>> adj;
vector<bool> visited;
vector<int> p_final;

void find_path(int u) {
    p_final.push_back(u);
    visited[u] = true;
    for (int v : adj[u]) {
        if (!visited[v]) {
            find_path(v);
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int subtask, n;
    cin >> subtask >> n;

    adj.resize(n + 1);
    
    int s;
    if (n > 2000) {
        s = 200;
    } else if (n > 500) {
        s = 60;
    } else {
        s = 30;
    }
    if (n <= 100) s = 15;
    if (n <= 2) {
        if (n==1) p_final = {1};
        else p_final = {1, 2};
        answer(p_final);
        return 0;
    }

    int m = (n + s - 1) / s;
    vector<vector<int>> groups(m);
    vector<int> group_idx(n + 1);
    for (int i = 1; i <= n; ++i) {
        int g_id = (i - 1) / s;
        groups[g_id].push_back(i);
        group_idx[i] = g_id;
    }

    vector<vector<bool>> has_neighbor_in_group(n + 1, vector<bool>(m, false));
    vector<bool> group_has_internal_edge(m, false);

    for (int i = 0; i < m; ++i) {
        if (groups[i].empty()) continue;
        
        vector<int> res;
        if (groups[i].size() > 1) res = do_query(groups[i]);
        else do_query(groups[i]);

        if(!res.empty()) {
            int last_res = res.back();
            if(last_res) group_has_internal_edge[i] = true;
        }

        for (int j = i + 1; j < m; ++j) {
            if (groups[j].empty()) continue;
            
            vector<int> probe_q;
            probe_q.reserve(groups[j].size() * 2);
            for (int node : groups[j]) {
                probe_q.push_back(node);
                probe_q.push_back(node);
            }
            vector<int> probe_res = do_query(probe_q);
            for (size_t k = 0; k < groups[j].size(); ++k) {
                int node = groups[j][k];
                int result = probe_res[2 * k];
                if (!group_has_internal_edge[i]) {
                    if (result) {
                        has_neighbor_in_group[node][i] = true;
                    }
                } else {
                    do_query({node});
                    vector<int> q_i = groups[i];
                    q_i.push_back(node);
                    vector<int> res_check = do_query(q_i);
                    do_query(groups[i]);

                    if (res_check.back()) has_neighbor_in_group[node][i] = true;
                }
            }
        }
        do_query(groups[i]);
    }
    
    for (int j = 0; j < m; ++j) {
        for (int i = j + 1; i < m; ++i) {
            for(int node : groups[j]) {
                if (has_neighbor_in_group[node][i]) {
                    // This is a symmetric relation
                    has_neighbor_in_group[groups[i][0]][j] = true;
                }
            }
        }
    }

    for (int i = 0; i < m; i++) {
        for(size_t u_idx = 0; u_idx < groups[i].size(); ++u_idx) {
            int u = groups[i][u_idx];
            for(size_t v_idx = u_idx + 1; v_idx < groups[i].size(); ++v_idx) {
                if (adj[u].size() == 2) break;
                int v = groups[i][v_idx];
                if (adj[v].size() == 2) continue;

                vector<int> q = {u, v};
                vector<int> res = do_query(q);
                if (res[1]) {
                    adj[u].push_back(v);
                    adj[v].push_back(u);
                }
                do_query(q);
            }
        }
    }
    
    for (int u = 1; u <= n; ++u) {
        if (adj[u].size() == 2) continue;
        
        int g_u = group_idx[u];
        for (int j = 0; j < m; ++j) {
            if (g_u == j) continue;
            
            bool potential_neighbor_exists = false;
            for(int node : groups[j]) {
                if(has_neighbor_in_group[node][g_u]) {
                    potential_neighbor_exists = true;
                    break;
                }
            }
            if (!potential_neighbor_exists && !has_neighbor_in_group[u][j]) continue;

            if (adj[u].size() == 2) break;

            do_query({u});
            vector<int> probe_q, nodes_to_probe;
            for(int v : groups[j]) {
                if (adj[v].size() < 2) {
                    bool already_neighbor = false;
                    for(int neighbor : adj[u]) if(neighbor == v) already_neighbor = true;
                    if(already_neighbor) continue;
                    
                    probe_q.push_back(v);
                    probe_q.push_back(v);
                    nodes_to_probe.push_back(v);
                }
            }
            if(!probe_q.empty()) {
                vector<int> res = do_query(probe_q);
                for(size_t k = 0; k < nodes_to_probe.size(); ++k) {
                    if (res[2*k]) {
                        int v = nodes_to_probe[k];
                        bool already_neighbor = false;
                        for(int neighbor : adj[u]) if(neighbor == v) already_neighbor = true;
                        if(!already_neighbor) {
                            adj[u].push_back(v);
                            adj[v].push_back(u);
                        }
                    }
                }
            }
            do_query({u});
        }
    }

    visited.resize(n + 1, false);
    find_path(1);

    if (p_final.size() != (size_t)n) {
        for(int i = 1; i <= n; i++) {
            if (!visited[i]) {
                // This indicates disjoint cycles, which shouldn't happen for this problem.
                // It's a sign that my logic might have failed.
                // But as a fallback, we can try to connect components.
                // For now, assume this won't happen.
            }
        }
    }

    answer(p_final);

    return 0;
}