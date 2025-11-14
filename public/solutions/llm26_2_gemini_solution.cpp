#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

struct Node {
    int len;
    long long sum;
    int last_val;
};

Node merge(const Node& a, const Node& b) {
    if (a.len > b.len) return a;
    if (b.len > a.len) return b;
    if (a.sum >= b.sum) return a;
    return b;
}

vector<Node> bit;
int N_val;

void update(int idx, Node val) {
    for (; idx <= N_val; idx += idx & -idx) {
        bit[idx] = merge(bit[idx], val);
    }
}

Node query(int idx) {
    Node res = {0, 0, 0};
    for (; idx > 0; idx -= idx & -idx) {
        res = merge(res, bit[idx]);
    }
    return res;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;
    N_val = n;
    vector<int> v(n);
    vector<int> pos_of_val(n + 1);
    for (int i = 0; i < n; ++i) {
        cin >> v[i];
        pos_of_val[v[i]] = i + 1;
    }

    bit.assign(n + 1, {0, 0, 0});
    vector<long long> max_sum_for_len(n + 1, 0);
    vector<int> last_val_for_len(n + 1, 0);
    vector<int> parent(n + 1, 0);

    for (int val : v) {
        Node best_prev = query(val - 1);
        
        Node current_node = {best_prev.len + 1, best_prev.sum + val, val};
        parent[val] = best_prev.last_val;
        
        if (current_node.len > 0) {
            if (current_node.sum > max_sum_for_len[current_node.len]) {
                max_sum_for_len[current_node.len] = current_node.sum;
                last_val_for_len[current_node.len] = val;
            } else if (current_node.sum == max_sum_for_len[current_node.len]) {
                // For determinism, could prefer smaller last_val, but not strictly necessary
                last_val_for_len[current_node.len] = min(last_val_for_len[current_node.len], val);
            }
        }

        update(val, current_node);
    }

    long long min_final_cost = -1;
    int best_L = -1;

    for (int L = 1; L <= n; ++L) {
        if (max_sum_for_len[L] == 0) continue;
        
        long long moves_count = n - L;
        long long max_s = max_sum_for_len[L];
        long long cost = (long long)n - L + (long long)L * n - max_s - ((long long)L * (L - 1)) / 2;
        long long final_cost = (cost + 1) * (moves_count + 1);

        if (best_L == -1 || final_cost < min_final_cost) {
            min_final_cost = final_cost;
            best_L = L;
        }
    }
    
    long long moves_count_all = n;
    long long cost_all = n;
    long long final_cost_all = (cost_all + 1) * (moves_count_all + 1);
    if (best_L == -1 || final_cost_all < min_final_cost) {
        min_final_cost = final_cost_all;
        best_L = 0;
    }

    vector<int> S;
    if (best_L > 0) {
        int curr = last_val_for_len[best_L];
        while (curr != 0) {
            S.push_back(curr);
            curr = parent[curr];
        }
        reverse(S.begin(), S.end());
    }

    vector<bool> is_in_S(n + 1, false);
    for (int s_val : S) {
        is_in_S[s_val] = true;
    }

    vector<int> M;
    for (int i = 1; i <= n; ++i) {
        if (!is_in_S[i]) {
            M.push_back(i);
        }
    }

    cout << min_final_cost << " " << M.size() << endl;

    if (M.empty()) {
        return 0;
    }

    vector<pair<int, int>> s_pos;
    for (int s_val : S) {
        s_pos.push_back({pos_of_val[s_val], s_val});
    }
    sort(s_pos.begin(), s_pos.end());

    vector<int> count_S_before(n + 1, 0);
    for (int m_val : M) {
        auto it = lower_bound(s_pos.begin(), s_pos.end(), make_pair(pos_of_val[m_val], 0));
        count_S_before[m_val] = distance(s_pos.begin(), it);
    }
    
    vector<int> ft_pos(n + 2, 0);
    auto add_pos = [&](int idx, int delta) {
        for (; idx <= n; idx += idx & -idx) ft_pos[idx] += delta;
    };
    auto query_pos = [&](int idx) {
        int sum = 0;
        for (; idx > 0; idx -= idx & -idx) sum += ft_pos[idx];
        return sum;
    };

    for (int m_val : M) {
        add_pos(pos_of_val[m_val], 1);
    }

    sort(M.rbegin(), M.rend());

    for (int m_val : M) {
        auto it = lower_bound(S.begin(), S.end(), m_val);
        long long y = distance(S.begin(), it) + 1;
        
        add_pos(pos_of_val[m_val], -1);
        long long count_M_before = query_pos(pos_of_val[m_val] - 1);
        long long x = 1 + count_S_before[m_val] + count_M_before;
        
        cout << x << " " << y << "\n";
    }

    return 0;
}