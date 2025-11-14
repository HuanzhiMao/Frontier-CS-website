#include <bits/stdc++.h>
using namespace std;

struct Node {
    int val;
    uint32_t prio;
    int sz;
    Node *l, *r, *p;
    Node(int v, uint32_t pr) : val(v), prio(pr), sz(1), l(nullptr), r(nullptr), p(nullptr) {}
};

inline int getsz(Node* t) { return t ? t->sz : 0; }

inline void pull(Node* t) {
    if (!t) return;
    t->sz = 1 + getsz(t->l) + getsz(t->r);
    if (t->l) t->l->p = t;
    if (t->r) t->r->p = t;
}

Node* merge(Node* a, Node* b) {
    if (!a) { if (b) b->p = nullptr; return b; }
    if (!b) { if (a) a->p = nullptr; return a; }
    if (a->prio < b->prio) {
        a->r = merge(a->r, b);
        if (a->r) a->r->p = a;
        pull(a);
        a->p = nullptr;
        return a;
    } else {
        b->l = merge(a, b->l);
        if (b->l) b->l->p = b;
        pull(b);
        b->p = nullptr;
        return b;
    }
}

void split(Node* t, int k, Node*& a, Node*& b) { // a: first k, b: rest
    if (!t) { a = b = nullptr; return; }
    int lsz = getsz(t->l);
    if (k <= lsz) {
        split(t->l, k, a, t->l);
        if (t->l) t->l->p = t;
        pull(t);
        t->p = nullptr;
        b = t;
    } else {
        split(t->r, k - lsz - 1, t->r, b);
        if (t->r) t->r->p = t;
        pull(t);
        t->p = nullptr;
        a = t;
    }
}

int getIndex(Node* x) { // 1-based
    int idx = getsz(x->l) + 1;
    Node* u = x;
    while (u->p) {
        if (u == u->p->r) {
            idx += getsz(u->p->l) + 1;
        }
        u = u->p;
    }
    return idx;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    if (!(cin >> n)) return 0;
    vector<int> v(n + 1);
    for (int i = 1; i <= n; ++i) cin >> v[i];

    // Build treap
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    Node* root = nullptr;
    vector<Node*> ptr(n + 1, nullptr);
    for (int i = 1; i <= n; ++i) {
        int val = v[i];
        Node* nd = new Node(val, rng());
        ptr[val] = nd;
        root = merge(root, nd);
    }

    // Determine largest suffix [cur+1..n] we can keep
    int cur = n;
    for (int i = n; i >= 1; --i) {
        if (v[i] == cur) {
            --cur;
        }
    }
    // Move values from cur down to 1 to front (y=1)
    vector<pair<int,int>> moves;
    moves.reserve(cur);
    long long sum_y = 0;
    for (int t = cur; t >= 1; --t) {
        Node* nd = ptr[t];
        int x = getIndex(nd);
        moves.emplace_back(x, 1);
        sum_y += 1;
        Node *L, *R, *M, *R2;
        split(root, x - 1, L, R);
        split(R, 1, M, R2); // M is nd
        root = merge(L, R2); // remove nd
        root = merge(M, root); // insert at front (y=1)
    }

    long long k = (long long)moves.size();
    long long final_cost = (sum_y + 1) * (k + 1);

    cout << final_cost << " " << moves.size() << "\n";
    for (auto &mv : moves) {
        cout << mv.first << " " << mv.second << "\n";
    }
    return 0;
}