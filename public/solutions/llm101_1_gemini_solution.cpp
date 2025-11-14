#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>

using namespace std;

int N, R;
vector<pair<int, int>> adj;
vector<int> parent;

void build_tree(int u) {
    if (u < N) {
        int u_child1 = adj[u].first;
        int u_child2 = adj[u].second;
        parent[u_child1] = u;
        parent[u_child2] = u;
        build_tree(u_child1);
        build_tree(u_child2);
    }
}

int query(const string& s) {
    cout << "? " << s << endl;
    int result;
    cin >> result;
    return result;
}

void answer(const string& t) {
    cout << "! " << t << endl;
}

void build_force_val_str(int u, int target_val, string& s) {
    if (u >= N) {
        s[u] = target_val + '0';
        return;
    }
    build_force_val_str(adj[u].first, 0, s);
    build_force_val_str(adj[u].second, 0, s);
    
    // For inputs (0,0), output is 0 for both AND and OR gates
    int gate_out = 0;
    s[u] = (gate_out ^ target_val) + '0';
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> N >> R;

    adj.resize(N);
    parent.assign(2 * N + 1, -1);

    for (int i = 0; i < N; ++i) {
        cin >> adj[i].first >> adj[i].second;
    }
    
    build_tree(0);

    string t(N, '&');
    
    string s_all_zero(2 * N + 1, '0');
    int out_all_zero = query(s_all_zero);
    
    vector<int> flip_effects(2 * N + 1);
    for (int i = 0; i < 2 * N + 1; ++i) {
        string temp_s = s_all_zero;
        temp_s[i] = '1';
        flip_effects[i] = query(temp_s) ^ out_all_zero;
    }

    string s_all_one(2 * N + 1, '1');
    int out_all_one = query(s_all_one);
    vector<int> flip_effects_one(2 * N + 1);
    for (int i = 0; i < 2 * N + 1; ++i) {
        string temp_s = s_all_one;
        temp_s[i] = '0';
        flip_effects_one[i] = query(temp_s) ^ out_all_one;
    }
    
    for (int i = 0; i < N; ++i) {
        int u_child = adj[i].first;
        int v_child = adj[i].second;

        if (flip_effects[u_child] == 1 && flip_effects[v_child] == 1) {
            string s = s_all_zero;
            s[u_child] = '1';
            int out = query(s);
            if ((out ^ out_all_zero) == 1) {
                t[i] = '|';
            } else {
                t[i] = '&';
            }
        } else if (flip_effects_one[u_child] == 1 && flip_effects_one[v_child] == 1) {
            string s = s_all_one;
            s[u_child] = '0';
            int out = query(s);
            if ((out ^ out_all_one) == 1) {
                t[i] = '&';
            } else {
                t[i] = '|';
            }
        } else {
            string s_base_ext(2*N+1, '0');
            string s_force_same = s_base_ext;
            build_force_val_str(adj[i].first, 0, s_force_same);
            build_force_val_str(adj[i].second, 0, s_force_same);
            s_force_same[i] = '0';
            
            string s_force_diff = s_base_ext;
            build_force_val_str(adj[i].first, 0, s_force_diff);
            build_force_val_str(adj[i].second, 1, s_force_diff);
            s_force_diff[i] = '0';
            
            int p = parent[i];
            if (p != -1) {
                s_force_same[p] = '1';
                s_force_diff[p] = '1';
            }

            int out_same = query(s_force_same);
            int out_diff = query(s_force_diff);

            if (out_diff != out_same) {
                if ((out_diff ^ out_same) == 1) t[i] = '|';
                else t[i] = '&';
            } else {
                s_base_ext = string(2*N+1, '1');
                s_force_same = s_base_ext;
                build_force_val_str(adj[i].first, 0, s_force_same);
                build_force_val_str(adj[i].second, 0, s_force_same);
                s_force_same[i] = '0';

                s_force_diff = s_base_ext;
                build_force_val_str(adj[i].first, 0, s_force_diff);
                build_force_val_str(adj[i].second, 1, s_force_diff);
                s_force_diff[i] = '0';
                
                p = parent[i];
                if (p != -1) {
                    s_force_same[p] = '0';
                    s_force_diff[p] = '0';
                }

                out_same = query(s_force_same);
                out_diff = query(s_force_diff);
                if (out_diff != out_same) {
                    if ((out_diff ^ out_same) == 1) t[i] = '|';
                    else t[i] = '&';
                } else {
                    t[i] = '&'; 
                }
            }
        }
    }

    answer(t);

    return 0;
}