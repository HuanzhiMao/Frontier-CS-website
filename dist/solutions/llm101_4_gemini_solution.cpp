#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <random>

using namespace std;

int N, R;
vector<pair<int, int>> connections;
vector<int> gate_types;
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

string query(const string& s) {
    cout << "? " << s << endl;
    fflush(stdout);
    string result;
    cin >> result;
    return result;
}

int calculate_out(int node_idx, const string& s, const vector<int>& types, vector<int>& memo) {
    if (memo[node_idx] != -1) {
        return memo[node_idx];
    }
    if (node_idx >= N) {
        return memo[node_idx] = s[node_idx] - '0';
    }

    int u = connections[node_idx].first;
    int v = connections[node_idx].second;

    int out_u = calculate_out(u, s, types, memo);
    int out_v = calculate_out(v, s, types, memo);

    int x;
    if (types[node_idx] == 0) { // AND
        x = out_u & out_v;
    } else { // OR
        x = out_u | out_v;
    }
    return memo[node_idx] = x ^ (s[node_idx] - '0');
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> N >> R;
    connections.resize(N);
    for (int i = 0; i < N; ++i) {
        cin >> connections[i].first >> connections[i].second;
    }
    
    gate_types.assign(N, -1);

    string base_s_all_0_int(2 * N + 1, '0');
    
    string s_all_0_ext = base_s_all_0_int;
    for(int i = N; i <= 2 * N; ++i) s_all_0_ext[i] = '0';
    
    string s_all_1_ext = base_s_all_0_int;
    for(int i = N; i <= 2 * N; ++i) s_all_1_ext[i] = '1';

    vector<bool> is_or_path(N, false);
    string s_or_test_base = s_all_0_ext;
    int base_res_or = query(s_or_test_base)[0] - '0';
    for (int i = 0; i < N; ++i) {
        s_or_test_base[i] = '1';
        int res = query(s_or_test_base)[0] - '0';
        if (res != base_res_or) {
            is_or_path[i] = true;
        }
        s_or_test_base[i] = '0';
    }

    vector<bool> is_and_path(N, false);
    string s_and_test_base = s_all_1_ext;
    int base_res_and = query(s_and_test_base)[0] - '0';
    for (int i = 0; i < N; ++i) {
        s_and_test_base[i] = '1';
        int res = query(s_and_test_base)[0] - '0';
        if (res != base_res_and) {
            is_and_path[i] = true;
        }
        s_and_test_base[i] = '0';
    }

    vector<int> parent(2 * N + 1, -1);
    for (int i = 0; i < N; i++) {
       parent[connections[i].first] = i;
       parent[connections[i].second] = i;
    }

    for (int i = 0; i < N; i++) {
        if(is_and_path[i]) gate_types[i] = 0;
        if(is_or_path[i]) gate_types[i] = 1;
    }
    
    bool changed = true;
    while(changed) {
        changed = false;
        for (int i = 0; i < N; ++i) {
            if (gate_types[i] != -1) continue;

            int p = parent[i];
            int curr = i;
            while(p != -1) {
                if (gate_types[p] != -1) break; // Known parent type, no new info
                if (is_and_path[p] && !is_and_path[curr]) {
                    gate_types[curr] = 1; // OR broke the AND path
                    changed = true;
                    break;
                }
                if (is_or_path[p] && !is_or_path[curr]) {
                    gate_types[curr] = 0; // AND broke the OR path
                    changed = true;
                    break;
                }
                curr = p;
                p = parent[p];
            }
        }
    }


    uniform_int_distribution<int> distrib(0, 1);

    for (int i = N-1; i >= 0; --i) {
        if (gate_types[i] == -1) {
            string s_base;
            
            for(int k=0; k < 40; ++k) {
                s_base = string(2*N+1, '0');
                for(int j=N; j<=2*N; ++j) {
                    if (distrib(rng)) s_base[j] = '1';
                }
                for(int j=0; j<i; ++j) {
                    if(distrib(rng)) s_base[j] = '1';
                }
                
                string s_flip = s_base;
                s_flip[i] = '1';
                
                string out_base_str = query(s_base);
                string out_flip_str = query(s_flip);
                
                if (out_base_str != out_flip_str) {
                    vector<int> memo(2 * N + 1);
                    vector<int> temp_types = gate_types;
                    for(int j=0; j<N; ++j) if(temp_types[j] == -1) temp_types[j] = 0;

                    temp_types[i] = 0;
                    fill(memo.begin(), memo.end(), -1);
                    int res_and = calculate_out(0, s_base, temp_types, memo);

                    if (res_and == (out_base_str[0] - '0')) {
                        gate_types[i] = 0;
                    } else {
                        gate_types[i] = 1;
                    }
                    break;
                }
            }
            if (gate_types[i] == -1) {
                 gate_types[i] = 0;
            }
        }
    }

    cout << "! ";
    for (int i = 0; i < N; ++i) {
        if (gate_types[i] == 0) {
            cout << "&";
        } else {
            cout << "|";
        }
    }
    cout << endl;
    fflush(stdout);

    return 0;
}