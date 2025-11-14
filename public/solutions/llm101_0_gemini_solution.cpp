#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <random>
#include <algorithm>

using namespace std;

int N, R;
vector<pair<int, int>> connections;
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

int query(const string& s) {
    cout << "? " << s << endl;
    int result;
    cin >> result;
    return result;
}

void answer(const string& t) {
    cout << "! " << t << endl;
}

vector<int> calculate_outputs_memo;
int calculate_output_recursive(int j, const string& s, const string& t) {
    if (calculate_outputs_memo[j] != -1) {
        return calculate_outputs_memo[j];
    }

    if (j >= N) {
        return calculate_outputs_memo[j] = (s[j] - '0');
    }

    int u = connections[j].first;
    int v = connections[j].second;
    int out_u = calculate_output_recursive(u, s, t);
    int out_v = calculate_output_recursive(v, s, t);

    int slot_output;
    if (t[j] == '&') {
        slot_output = out_u & out_v;
    } else {
        slot_output = out_u | out_v;
    }

    return calculate_outputs_memo[j] = (s[j] - '0') ^ slot_output;
}

int get_circuit_output(const string& s, const string& t) {
    fill(calculate_outputs_memo.begin(), calculate_outputs_memo.end(), -1);
    return calculate_output_recursive(0, s, t);
}

void solve_small_n() {
    int num_queries = min(N * 2 + 10, 900);
    vector<string> queries_s;
    vector<int> queries_res;
    
    for (int i = 0; i < num_queries; ++i) {
        string s(2 * N + 1, '0');
        for (int j = N; j <= 2 * N; ++j) {
            if (uniform_int_distribution<int>(0, 1)(rng)) {
                s[j] = '1';
            }
        }
        bool found = false;
        for(const auto& qs : queries_s) if(qs == s) found = true;
        if(found && i > 0) {
            i--; 
            continue;
        }
        queries_s.push_back(s);
        queries_res.push_back(query(s));
    }

    string current_t(N, '&');
    for (int iter = 0; iter < 2; ++iter) {
        for (int i = 0; i < N; ++i) {
            string t_and = current_t;
            t_and[i] = '&';
            string t_or = current_t;
            t_or[i] = '|';

            bool and_consistent = true;
            for (size_t j = 0; j < queries_s.size(); ++j) {
                if (get_circuit_output(queries_s[j], t_and) != queries_res[j]) {
                    and_consistent = false;
                    break;
                }
            }

            if (and_consistent) {
                 bool or_consistent = true;
                 for (size_t j = 0; j < queries_s.size(); ++j) {
                    if (get_circuit_output(queries_s[j], t_or) != queries_res[j]) {
                        or_consistent = false;
                        break;
                    }
                }
                if (!or_consistent) {
                    current_t[i] = '&';
                }
            } else {
                 current_t[i] = '|';
            }
        }
    }
    answer(current_t);
}

void solve_large_n() {
    string final_t(N, '&');
    for (int i = 0; i < N; ++i) {
        string t_try_and = final_t;
        t_try_and[i] = '&';
        string t_try_or = final_t;
        t_try_or[i] = '|';

        for (int k = 0; k < 5; ++k) {
            string s_test(2 * N + 1, '0');
            // A random query
            int num_ones = uniform_int_distribution<int>(1, R)(rng);
            vector<int> p(N+1);
            iota(p.begin(), p.end(), N);
            shuffle(p.begin(), p.end(), rng);
            for(int j = 0; j < num_ones; ++j) {
                s_test[p[j]] = '1';
            }
            
            int res_test = query(s_test);
            int res_and = get_circuit_output(s_test, t_try_and);
            int res_or = get_circuit_output(s_test, t_try_or);

            if (res_and != res_or) {
                if (res_test == res_and) {
                    final_t[i] = '&';
                } else {
                    final_t[i] = '|';
                }
                break;
            }
            if (k == 4) { // all queries were ambiguous
                final_t[i] = '&'; // default guess
            }
        }
    }
    answer(final_t);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> N >> R;
    connections.resize(N);
    calculate_outputs_memo.resize(2 * N + 1);
    for (int i = 0; i < N; ++i) {
        cin >> connections[i].first >> connections[i].second;
    }

    if (N <= 400) {
        solve_small_n();
    } else {
        solve_large_n();
    }

    return 0;
}