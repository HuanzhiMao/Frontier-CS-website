#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <cstdlib>

using namespace std;

int N, R;
vector<pair<int, int>> adj;
vector<int> parent;

int query(const string& s) {
    cout << "? " << s << endl;
    int result;
    cin >> result;
    return result;
}

void answer(const string& t) {
    cout << "! " << t << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    srand(1337);

    cin >> N >> R;
    adj.resize(N);
    parent.assign(2 * N + 1, -1);
    for (int i = 0; i < N; ++i) {
        cin >> adj[i].first >> adj[i].second;
        parent[adj[i].first] = i;
        parent[adj[i].second] = i;
    }

    int num_queries = 600;
    if (N > 2000) num_queries = 450;
    if (N > 5000) num_queries = 350;

    vector<string> queries(num_queries, string(2 * N + 1, '0'));
    vector<int> query_results(num_queries);

    for (int i = 0; i < num_queries; ++i) {
        for (int j = 0; j < 2 * N + 1; ++j) {
            queries[i][j] = (rand() % 2) ? '1' : '0';
        }
        query_results[i] = query(queries[i]);
    }

    string determined_T(N, '&');
    vector<vector<int>> computed_out(num_queries, vector<int>(2 * N + 1));

    for (int j = 0; j < num_queries; ++j) {
        for (int k = 2 * N; k >= N; --k) {
            computed_out[j][k] = queries[j][k] - '0';
        }
    }

    for (int i = N - 1; i >= 0; --i) {
        int and_agrees = 0;
        int or_agrees = 0;

        string t_and_dummy = determined_T; // All k<i are '&'
        
        for (int j = 0; j < num_queries; ++j) {
            int u_in = computed_out[j][adj[i].first];
            int v_in = computed_out[j][adj[i].second];

            if (u_in == v_in) {
                continue;
            }

            // Calculate output with T_i = AND and dummy T_{<i}
            vector<int> temp_out = computed_out[j];
            
            // Calculate for i
            int slot_out_and = u_in & v_in;
            temp_out[i] = slot_out_and ^ (queries[j][i] - '0');

            // Propagate for k < i
            for(int k = i - 1; k >= 0; --k) {
                 int u_k = adj[k].first;
                 int v_k = adj[k].second;
                 // Dummy value for T_k is '&'
                 int slot_out_k = temp_out[u_k] & temp_out[v_k];
                 temp_out[k] = slot_out_k ^ (queries[j][k] - '0');
            }
            int res_and = temp_out[0];

            int final_diff = 1;
            int curr = i;
            while(curr != 0) {
                int p = parent[curr];
                int other_child = (adj[p].first == curr) ? adj[p].second : adj[p].first;
                int other_in = temp_out[other_child];
                
                // Dummy T_p is '&'
                if (other_in == 0) { 
                    final_diff = 0;
                    break;
                }
                curr = p;
            }
            
            int res_or = res_and ^ final_diff;

            if (res_and == query_results[j]) {
                and_agrees++;
            }
            if (res_or == query_results[j]) {
                or_agrees++;
            }
        }

        if (or_agrees > and_agrees) {
            determined_T[i] = '|';
        }
        
        for (int j = 0; j < num_queries; ++j) {
            int u_in = computed_out[j][adj[i].first];
            int v_in = computed_out[j][adj[i].second];
            int slot_out;
            if (determined_T[i] == '&') {
                slot_out = u_in & v_in;
            } else {
                slot_out = u_in | v_in;
            }
            computed_out[j][i] = slot_out ^ (queries[j][i] - '0');
        }
    }

    answer(determined_T);

    return 0;
}