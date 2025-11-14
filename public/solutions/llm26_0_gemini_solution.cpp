#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include <chrono>
#include <map>

// Treap with parent pointers for efficient rank queries
struct Node {
    int val;
    int priority;
    int size;
    Node *l, *r, *p;
    Node(int v) : val(v), size(1), l(nullptr), r(nullptr), p(nullptr) {
        static std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
        priority = rng();
    }
};

int size(Node* t) {
    return t ? t->size : 0;
}

void set_parent(Node* c, Node* p) {
    if (c) c->p = p;
}

void update_size(Node* t) {
    if (t) {
        t->size = 1 + size(t->l) + size(t->r);
    }
}

void split(Node* t, Node*& l, Node*& r, int pos, int add = 0) {
    if (!t) {
        l = r = nullptr;
        return;
    }
    set_parent(t, nullptr);
    int current_pos = add + size(t->l);
    if (pos <= current_pos) {
        split(t->l, l, t->l, pos, add);
        set_parent(t->l, t);
        r = t;
    } else {
        split(t->r, t->r, r, pos, current_pos + 1);
        set_parent(t->r, t);
        l = t;
    }
    update_size(t);
}

Node* merge(Node* l, Node* r) {
    if (!l || !r) return l ? l : r;
    if (l->priority > r->priority) {
        set_parent(l, nullptr);
        l->r = merge(l->r, r);
        set_parent(l->r, l);
        update_size(l);
        return l;
    } else {
        set_parent(r, nullptr);
        r->l = merge(l, r->l);
        set_parent(r->l, r);
        update_size(r);
        return r;
    }
}

int get_rank(Node* t) {
    if (!t) return 0;
    int rank = size(t->l) + 1;
    while (t->p) {
        if (t == t->p->r) {
            rank += size(t->p->l) + 1;
        }
        t = t->p;
    }
    return rank;
}

// For the DP part
struct SegInfo {
    int len;
    long long sum;
    int idx;
};

SegInfo merge_info(const SegInfo& a, const SegInfo& b) {
    if (a.len > b.len) return a;
    if (b.len > a.len) return b;
    if (a.sum >= b.sum) return a;
    return b;
}

std::vector<SegInfo> seg_tree;
int N_seg;

void update_seg(int idx, SegInfo val) {
    idx += N_seg;
    if (seg_tree[idx].len < val.len || (seg_tree[idx].len == val.len && seg_tree[idx].sum < val.sum)) {
        seg_tree[idx] = val;
    }
    while (idx > 1) {
        idx /= 2;
        seg_tree[idx] = merge_info(seg_tree[idx * 2], seg_tree[idx * 2 + 1]);
    }
}

SegInfo query_seg(int l, int r) { // range [l, r)
    SegInfo res = {0, 0, -1};
    for (l += N_seg, r += N_seg; l < r; l /= 2, r /= 2) {
        if (l & 1) res = merge_info(res, seg_tree[l++]);
        if (r & 1) res = merge_info(res, seg_tree[--r]);
    }
    return res;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;
    std::vector<int> v(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> v[i];
    }

    N_seg = 1;
    while (N_seg < n + 2) N_seg *= 2;
    seg_tree.assign(2 * N_seg, {0, 0, -1});

    std::vector<int> parent(n, -1);
    std::vector<long long> max_S_for_len(n + 1, 0);
    std::vector<int> last_idx_for_len(n + 1, -1);

    for (int i = 0; i < n; ++i) {
        int val = v[i];
        SegInfo res = query_seg(0, val);
        int new_len = res.len + 1;
        long long new_sum = res.sum + val;
        parent[i] = res.idx;
        
        update_seg(val, {new_len, new_sum, i});

        if (new_sum > max_S_for_len[new_len]) {
            max_S_for_len[new_len] = new_sum;
            last_idx_for_len[new_len] = i;
        }
    }

    long long min_final_cost = -1;
    int best_L = 0;

    long long total_sum = (long long)n * (n + 1) / 2;

    for (int L = 1; L <= n; ++L) {
        if (last_idx_for_len[L] == -1) continue;

        long long SUM_S = max_S_for_len[L];
        int k = n - L;
        long long SUM_M = total_sum - SUM_S;

        long long move_cost = (long long)k + SUM_M - (long long)k * (k + 1) / 2;
        long long final_cost = (move_cost + 1) * (k + 1);

        if (min_final_cost == -1 || final_cost < min_final_cost) {
            min_final_cost = final_cost;
            best_L = L;
        }
    }
    
    bool sorted = true;
    for (int i = 0; i < n; ++i) if (v[i] != i + 1) sorted = false;
    if (sorted) {
        min_final_cost = 1;
        best_L = n;
    } else if (best_L == 0) {
        int k = n;
        long long SUM_M = total_sum;
        long long move_cost = (long long)k + SUM_M - (long long)k * (k + 1) / 2;
        min_final_cost = (move_cost + 1) * (k + 1);
        best_L = 0;
    }


    std::cout << min_final_cost << " " << n - best_L << std::endl;

    if (n - best_L == 0) {
        return 0;
    }

    std::vector<bool> is_in_S(n + 1, false);
    if (best_L > 0) {
        int curr = last_idx_for_len[best_L];
        while (curr != -1) {
            is_in_S[v[curr]] = true;
            curr = parent[curr];
        }
    }
    
    std::vector<int> S_vals;
    std::vector<int> M_vals;
    for (int i = 1; i <= n; ++i) {
        if (is_in_S[i]) {
            S_vals.push_back(i);
        } else {
            M_vals.push_back(i);
        }
    }
    std::sort(M_vals.rbegin(), M_vals.rend());

    Node* treap_root = nullptr;
    std::vector<Node*> val_to_node(n + 1);
    for (int i = 0; i < n; ++i) {
        Node* new_node = new Node(v[i]);
        val_to_node[v[i]] = new_node;
        treap_root = merge(treap_root, new_node);
    }
    
    for (int m : M_vals) {
        int x = get_rank(val_to_node[m]);
        
        auto it = std::lower_bound(S_vals.begin(), S_vals.end(), m);
        int y = (it - S_vals.begin()) + 1;

        std::cout << x << " " << y << "\n";

        Node *t1, *t2, *t_m;
        split(treap_root, t1, t2, x);
        split(t1, t1, t_m, x - 1);
        treap_root = merge(t1, t2);

        split(treap_root, t1, t2, y - 1);
        treap_root = merge(merge(t1, t_m), t2);
    }

    return 0;
}