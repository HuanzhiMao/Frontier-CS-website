#include <bits/stdc++.h>
using namespace std;

struct Node {
    int val;
    uint32_t prio;
    Node *l, *r, *par;
    int sz;
    Node(int v, uint32_t p): val(v), prio(p), l(nullptr), r(nullptr), par(nullptr), sz(1) {}
};
inline int getsz(Node* t){ return t ? t->sz : 0; }
inline void upd(Node* t){
    if(!t) return;
    t->sz = 1 + getsz(t->l) + getsz(t->r);
    if(t->l) t->l->par = t;
    if(t->r) t->r->par = t;
}
void split(Node* t, int k, Node* &l, Node* &r){
    if(!t){ l = r = nullptr; return; }
    if(getsz(t->l) >= k){
        split(t->l, k, l, t->l);
        if(t->l) t->l->par = t;
        r = t; r->par = nullptr;
        upd(r);
    }else{
        split(t->r, k - getsz(t->l) - 1, t->r, r);
        if(t->r) t->r->par = t;
        l = t; l->par = nullptr;
        upd(l);
    }
}
Node* merge(Node* l, Node* r){
    if(!l){ if(r) r->par = nullptr; return r; }
    if(!r){ if(l) l->par = nullptr; return l; }
    if(l->prio < r->prio){
        l->r = merge(l->r, r);
        if(l->r) l->r->par = l;
        l->par = nullptr;
        upd(l);
        return l;
    }else{
        r->l = merge(l, r->l);
        if(r->l) r->l->par = r;
        r->par = nullptr;
        upd(r);
        return r;
    }
}
int get_index(Node* u){
    int res = getsz(u->l) + 1;
    while(u->par){
        if(u == u->par->r){
            res += getsz(u->par->l) + 1;
        }
        u = u->par;
    }
    return res;
}
Node* do_move(Node* root, int x, int y){
    Node *a, *b, *c, *d;
    split(root, x-1, a, b);
    split(b, 1, c, d); // c is the node
    root = merge(a, d);
    split(root, y-1, a, b);
    root = merge(merge(a, c), b);
    return root;
}

struct Plan {
    long long sum_y = 0;
    int moves = 0;
    vector<pair<int,int>> ops;
    __int128 cost() const {
        __int128 S = sum_y;
        __int128 K = moves;
        return (S + 1) * (K + 1);
    }
};

struct FenwickMax {
    int n;
    vector<pair<int,int>> bit; // (len, idx)
    FenwickMax(int n=0){init(n);}
    void init(int n_){
        n = n_;
        bit.assign(n+1, {0, -1});
    }
    static pair<int,int> better(pair<int,int> a, pair<int,int> b){
        if(a.first != b.first) return (a.first > b.first ? a : b);
        // tie-breaker: smaller idx (arbitrary but deterministic)
        if(a.second == -1) return b;
        if(b.second == -1) return a;
        return (a.second < b.second ? a : b);
    }
    void update(int idx, pair<int,int> val){
        for(int i = idx; i <= n; i += i&-i){
            bit[i] = better(bit[i], val);
        }
    }
    pair<int,int> query(int idx){
        pair<int,int> res = {0, -1};
        for(int i = idx; i > 0; i -= i&-i){
            res = better(res, bit[i]);
        }
        return res;
    }
};

struct Solver {
    int n;
    vector<int> v;         // permutation by positions 1..n
    vector<int> pos;       // pos[val] = index in v
    mt19937 rng;

    Solver(int n, const vector<int>& v): n(n), v(v), pos(n+1), rng(712367) {
        for(int i=1;i<=n;i++) pos[v[i]] = i;
    }

    Node* buildTreap(vector<Node*>& nodes){
        Node* root = nullptr;
        nodes.assign(n+1, nullptr);
        for(int i=1;i<=n;i++){
            nodes[v[i]] = new Node(v[i], rng());
            root = merge(root, nodes[v[i]]);
        }
        return root;
    }

    vector<int> LIS_minpos_keep_values() {
        // Perform LIS on A[i] = pos[i] for i=1..n. Return values (i) included in one LIS (lexicographically minimal by positions).
        vector<int> A(n+1);
        for(int i=1;i<=n;i++) A[i] = pos[i];
        vector<int> tail_val(n+1, 0);
        vector<int> tail_idx(n+1, -1);
        vector<int> prev(n+1, -1);
        int len = 0;
        for(int i=1;i<=n;i++){
            int x = A[i];
            int l = int(lower_bound(tail_val.begin()+1, tail_val.begin()+len+1, x) - tail_val.begin());
            if(l == len+1) len++;
            tail_val[l] = x;
            prev[i] = (l > 1 ? tail_idx[l-1] : -1);
            tail_idx[l] = i;
        }
        vector<int> idxs;
        int k = tail_idx[len];
        while(k != -1){
            idxs.push_back(k);
            k = prev[k];
        }
        reverse(idxs.begin(), idxs.end());
        // idxs are value indices 1..n
        return idxs;
    }

    vector<int> LIS_end_at_n_keep_values() {
        // LIS ending at value n (by values increasing, positions increasing)
        vector<int> A(n+1);
        for(int i=1;i<=n;i++) A[i] = pos[i];
        FenwickMax fw(n);
        vector<int> dp(n+1, 0), pre(n+1, -1);
        for(int i=1;i<=n;i++){
            auto best = fw.query(A[i]-1);
            dp[i] = best.first + 1;
            pre[i] = best.second;
            fw.update(A[i], {dp[i], i});
        }
        int i = n; // value n
        vector<int> idxs;
        while(i != -1){
            idxs.push_back(i);
            i = pre[i];
        }
        reverse(idxs.begin(), idxs.end());
        return idxs;
    }

    vector<int> suffix_keep_values() {
        // Keep values from t..n such that positions increasing.
        int cur = INT_MAX;
        int t = 1;
        for(int val = n; val >= 1; --val){
            if(pos[val] < cur){
                cur = pos[val];
            }else{
                t = val + 1;
                break;
            }
            if(val == 1) t = 1;
        }
        vector<int> keep;
        for(int val = t; val <= n; ++val) keep.push_back(val);
        return keep;
    }

    Plan generate_moves_with_keep(const vector<int>& keepValues) {
        vector<char> inKeep(n+1, 0);
        for(int x: keepValues) inKeep[x] = 1;
        int L = (int)keepValues.size();

        // Build groups by successor K element index
        vector<vector<int>> groups(L + 2); // 1..L for real groups; L+1 sentinel
        for(int val = 1; val <= n; ++val){
            if(inKeep[val]) continue;
            auto it = upper_bound(keepValues.begin(), keepValues.end(), val);
            int gidx;
            if(it == keepValues.end()){
                gidx = L + 1;
            } else {
                gidx = int(it - keepValues.begin()) + 1; // 1-based
            }
            groups[gidx].push_back(val);
        }
        for(int j=1;j<=L+1;j++){
            sort(groups[j].begin(), groups[j].end());
        }

        // Build treap
        vector<Node*> nodes;
        Node* root = buildTreap(nodes);

        Plan plan;
        // Process groups in descending successor index: L+1 (sentinel), L, ..., 1
        for(int j = L+1; j >= 1; --j){
            if(groups[j].empty()) continue;
            for(int val: groups[j]){
                int x = get_index(nodes[val]);
                int y;
                if(j == L+1){
                    y = n;
                } else {
                    int succVal = keepValues[j-1];
                    y = get_index(nodes[succVal]);
                }
                plan.ops.push_back({x, y});
                plan.sum_y += y;
                plan.moves += 1;
                root = do_move(root, x, y);
            }
        }
        return plan;
    }

    Plan generate_moves_suffix_method(const vector<int>& keepValues) {
        // Move all values less than first keep (i.e., t-1 down to 1) to front y=1 in decreasing order
        vector<char> inKeep(n+1, 0);
        for(int x: keepValues) inKeep[x] = 1;

        vector<Node*> nodes;
        Node* root = buildTreap(nodes);

        Plan plan;
        // Determine minimal kept value
        int t = 1;
        if(!keepValues.empty()) t = keepValues.front();
        else t = n+1; // keep nothing; move all to front
        for(int val = t-1; val >= 1; --val){
            int x = get_index(nodes[val]);
            int y = 1;
            plan.ops.push_back({x, y});
            plan.sum_y += y;
            plan.moves += 1;
            root = do_move(root, x, y);
        }
        return plan;
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if(!(cin >> n)) return 0;
    vector<int> v(n+1);
    for(int i=1;i<=n;i++) cin >> v[i];

    Solver solver(n, v);

    // Scenario 1: LIS with lexicographically minimal positions
    vector<int> keep1 = solver.LIS_minpos_keep_values();
    Plan plan1 = solver.generate_moves_with_keep(keep1);

    // Scenario 2: LIS ending at n
    vector<int> keep2 = solver.LIS_end_at_n_keep_values();
    Plan plan2 = solver.generate_moves_with_keep(keep2);

    // Scenario 3: Suffix keep and move all smaller to front
    vector<int> keep3 = solver.suffix_keep_values();
    Plan plan3 = solver.generate_moves_suffix_method(keep3);

    // Choose best by product (sum_y+1)*(moves+1)
    Plan best = plan1;
    vector<pair<int,int>> *best_ops = &plan1.ops;
    __int128 bestCost = plan1.cost();

    if(plan2.cost() < bestCost){
        best = plan2;
        best_ops = &plan2.ops;
        bestCost = plan2.cost();
    }
    if(plan3.cost() < bestCost){
        best = plan3;
        best_ops = &plan3.ops;
        bestCost = plan3.cost();
    }

    unsigned long long outputCost = (unsigned long long)bestCost; // fits in 64-bit for constraints
    cout << outputCost << " " << best.moves << "\n";
    for(auto &op : *best_ops){
        cout << op.first << " " << op.second << "\n";
    }
    return 0;
}