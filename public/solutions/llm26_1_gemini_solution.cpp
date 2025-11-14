#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include <chrono>

struct Node {
    int val;
    unsigned int priority;
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

void update_size(Node* t) {
    if (t) {
        t->size = 1 + size(t->l) + size(t->r);
    }
}

void set_parent(Node* c, Node* p) {
    if (c) {
        c->p = p;
    }
}

void split(Node* t, Node*& l, Node*& r, int k) {
    if (!t) {
        l = r = nullptr;
        return;
    }
    t->p = nullptr;
    int left_size = size(t->l);
    if (k <= left_size) {
        split(t->l, l, t->l, k);
        r = t;
        set_parent(r->l, r);
    } else {
        split(t->r, t->r, r, k - left_size - 1);
        l = t;
        set_parent(l->r, l);
    }
    update_size(t);
}

Node* merge(Node* l, Node* r) {
    if (!l || !r) {
        return l ? l : r;
    }
    if (l->priority > r->priority) {
        l->r = merge(l->r, r);
        set_parent(l->r, l);
        update_size(l);
        return l;
    } else {
        r->l = merge(l, r->l);
        set_parent(r->l, r);
        update_size(r);
        return r;
    }
}

int get_pos(Node* node) {
    if (!node) return 0;
    int pos = size(node->l) + 1;
    while (node->p) {
        if (node == node->p->r) {
            pos += size(node->p->l) + 1;
        }
        node = node->p;
    }
    return pos;
}


struct FW {
    std::vector<long long> bit_sum;
    std::vector<int> bit_pred_val;
    int n;

    FW(int size) : n(size + 1), bit_sum(size + 1, 0), bit_pred_val(size + 1, 0) {}

    void update(int idx, long long sum, int pred_val) {
        for (; idx < n; idx += idx & -idx) {
            if (sum > bit_sum[idx]) {
                bit_sum[idx] = sum;
                bit_pred_val[idx] = pred_val;
            }
        }
    }

    std::pair<long long, int> query(int idx) {
        long long max_sum = 0;
        int pred_val = 0;
        for (; idx > 0; idx -= idx & -idx) {
            if (bit_sum[idx] > max_sum) {
                max_sum = bit_sum[idx];
                pred_val = bit_pred_val[idx];
            }
        }
        return {max_sum, pred_val};
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;
    std::vector<int> v(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> v[i];
    }

    FW fw(n);
    std::vector<int> pred(n + 1, 0);
    int last_val = 0;
    long long max_w = 0;

    for (int x : v) {
        auto [prev_sum, prev_val] = fw.query(x - 1);
        long long current_sum = prev_sum + x;
        pred[x] = prev_val;
        fw.update(x, current_sum, x);
        if (current_sum > max_w) {
            max_w = current_sum;
            last_val = x;
        }
    }
    
    std::vector<bool> in_lis(n + 1, false);
    std::vector<int> lis_vals;
    int curr = last_val;
    while (curr != 0) {
        in_lis[curr] = true;
        lis_vals.push_back(curr);
        curr = pred[curr];
    }
    std::reverse(lis_vals.begin(), lis_vals.end());

    std::vector<int> to_move;
    for (int i = 0; i < n; ++i) {
        if (!in_lis[v[i]]) {
            to_move.push_back(v[i]);
        }
    }
    std::sort(to_move.rbegin(), to_move.rend());

    std::vector<Node*> ptrs(n + 1);
    Node* root = nullptr;
    for (int val : v) {
        ptrs[val] = new Node(val);
        root = merge(root, ptrs[val]);
    }
    
    std::vector<std::pair<int, int>> moves;
    long long total_cost = 0;

    for (int val_to_move : to_move) {
        int x = get_pos(ptrs[val_to_move]);
        
        auto it = std::upper_bound(lis_vals.begin(), lis_vals.end(), val_to_move);
        int y;
        if (it == lis_vals.end()) {
            int target_val = lis_vals.back();
            y = get_pos(ptrs[target_val]) + 1;
        } else {
            int target_val = *it;
            y = get_pos(ptrs[target_val]);
        }
       
        if (x == y) {
            if (y > 1) y--;
            else y++;
        }
        
        moves.push_back({x, y});
        total_cost += y;

        Node *left, *right, *elem;
        split(root, left, right, x);
        split(left, left, elem, x - 1);
        root = merge(left, right);

        int insert_pos = y;
        if (x < y) {
            insert_pos--;
        }

        split(root, left, right, insert_pos - 1);
        root = merge(merge(left, elem), right);
    }
    
    long long final_cost = (total_cost + 1) * (moves.size() + 1);
    std::cout << final_cost << " " << moves.size() << "\n";
    for (const auto& p : moves) {
        std::cout << p.first << " " << p.second << "\n";
    }

    return 0;
}