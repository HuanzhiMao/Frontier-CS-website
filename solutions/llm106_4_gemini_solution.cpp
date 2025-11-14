#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <map>

using namespace std;

int n;
map<vector<int>, int> memo;

int ask(vector<int> s) {
    if (s.empty()) {
        return 0;
    }
    sort(s.begin(), s.end());
    if (memo.count(s)) {
        return memo[s];
    }

    cout << "? " << s.size() << endl;
    for (size_t i = 0; i < s.size(); ++i) {
        cout << s[i] << (i == s.size() - 1 ? "" : " ");
    }
    cout << endl;
    cout.flush();

    int m;
    cin >> m;
    if (m == -1) exit(0);
    return memo[s] = m;
}

int count_edges_between(const vector<int>& A, const vector<int>& B) {
    if (A.empty() || B.empty()) return 0;
    vector<int> AB = A;
    AB.insert(AB.end(), B.begin(), B.end());
    return ask(AB) - ask(A) - ask(B);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n;

    vector<int> p1, p2;
    vector<int> color(n + 1, 0);
    vector<int> parent(n + 1, 0);

    color[1] = 1;
    p1.push_back(1);

    vector<int> colored_nodes = {1};
    vector<int> uncolored_nodes;
    for (int i = 2; i <= n; ++i) {
        uncolored_nodes.push_back(i);
    }

    while (!uncolored_nodes.empty()) {
        int u_to_color;

        int l = 0, r = uncolored_nodes.size() - 1;
        while (l < r) {
            int mid = l + (r - l) / 2;
            vector<int> U_left;
            for (int j = l; j <= mid; ++j) U_left.push_back(uncolored_nodes[j]);
            if (count_edges_between(U_left, colored_nodes) > 0) {
                r = mid;
            } else {
                l = mid + 1;
            }
        }
        u_to_color = uncolored_nodes[l];
        
        swap(uncolored_nodes[l], uncolored_nodes.back());
        uncolored_nodes.pop_back();
        
        int p_node;
        l = 0, r = colored_nodes.size() - 1;
        while (l < r) {
            int mid = l + (r - l) / 2;
            vector<int> C_left;
            for (int j = l; j <= mid; ++j) C_left.push_back(colored_nodes[j]);
            if (count_edges_between({u_to_color}, C_left) > 0) {
                r = mid;
            } else {
                l = mid + 1;
            }
        }
        p_node = colored_nodes[l];

        parent[u_to_color] = p_node;
        color[u_to_color] = 3 - color[p_node];
        if (color[u_to_color] == 1) p1.push_back(u_to_color);
        else p2.push_back(u_to_color);
        
        colored_nodes.push_back(u_to_color);
    }
    
    if (ask(p1) == 0 && ask(p2) == 0) {
        cout << "Y " << p1.size() << endl;
        sort(p1.begin(), p1.end());
        for (size_t i = 0; i < p1.size(); ++i) {
            cout << p1[i] << (i == p1.size() - 1 ? "" : " ");
        }
        cout << endl;
        cout.flush();
    } else {
        vector<int> S_bad;
        if (ask(p1) > 0) S_bad = p1;
        else S_bad = p2;
        
        int u = -1, v = -1;
        
        while (true) {
            if (S_bad.size() == 2) {
                u = S_bad[0]; v = S_bad[1];
                break;
            }
            vector<int> S_L, S_R;
            for (size_t i = 0; i < S_bad.size() / 2; ++i) S_L.push_back(S_bad[i]);
            for (size_t i = S_bad.size() / 2; i < S_bad.size(); ++i) S_R.push_back(S_bad[i]);

            if (ask(S_L) > 0) {
                S_bad = S_L;
                continue;
            }
            if (ask(S_R) > 0) {
                S_bad = S_R;
                continue;
            }

            vector<int> T = S_L;
            while (T.size() > 1) {
                vector<int> T_L, T_R;
                for (size_t i = 0; i < T.size() / 2; ++i) T_L.push_back(T[i]);
                for (size_t i = T.size() / 2; i < T.size(); ++i) T_R.push_back(T[i]);
                if (count_edges_between(T_L, S_R) > 0) {
                    T = T_L;
                } else {
                    T = T_R;
                }
            }
            u = T[0];

            T = S_R;
            while (T.size() > 1) {
                vector<int> T_L, T_R;
                for (size_t i = 0; i < T.size() / 2; ++i) T_L.push_back(T[i]);
                for (size_t i = T.size() / 2; i < T.size(); ++i) T_R.push_back(T[i]);
                if (count_edges_between({u}, T_L) > 0) {
                    T = T_L;
                } else {
                    T = T_R;
                }
            }
            v = T[0];
            break;
        }

        vector<int> path_u, path_v;
        int curr = u;
        while (curr != 0) {
            path_u.push_back(curr);
            curr = parent[curr];
        }
        curr = v;
        while (curr != 0) {
            path_v.push_back(curr);
            curr = parent[curr];
        }
        reverse(path_u.begin(), path_u.end());
        reverse(path_v.begin(), path_v.end());

        size_t lca_idx = 0;
        while (lca_idx + 1 < path_u.size() && lca_idx + 1 < path_v.size() && path_u[lca_idx+1] == path_v[lca_idx+1]) {
            lca_idx++;
        }
        
        vector<int> cycle;
        for (int i = path_u.size() - 1; i > (int)lca_idx; --i) {
            cycle.push_back(path_u[i]);
        }
        for (size_t i = lca_idx; i < path_v.size(); ++i) {
            cycle.push_back(path_v[i]);
        }

        cout << "N " << cycle.size() << endl;
        for (size_t i = 0; i < cycle.size(); ++i) {
            cout << cycle[i] << (i == cycle.size() - 1 ? "" : " ");
        }
        cout << endl;
        cout.flush();
    }

    return 0;
}