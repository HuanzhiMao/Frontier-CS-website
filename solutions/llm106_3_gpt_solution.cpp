#include <bits/stdc++.h>
using namespace std;

static const long long INF = (1LL<<60);

int n;

// Ask the judge for the number of edges inside set S
long long ask(const vector<int>& S) {
    if (S.empty()) return 0; // do not query empty set
    cout << "? " << (int)S.size() << "\n";
    for (int i = 0; i < (int)S.size(); ++i) {
        if (i) cout << ' ';
        cout << S[i];
    }
    cout << "\n";
    cout.flush();
    long long ans;
    if (!(cin >> ans)) exit(0);
    if (ans == -1) exit(0);
    return ans;
}

vector<int> union_disjoint(const vector<int>& A, const vector<int>& B) {
    // A and B are disjoint in our algorithm; just concatenate
    vector<int> U;
    U.reserve(A.size() + B.size());
    U.insert(U.end(), A.begin(), A.end());
    U.insert(U.end(), B.begin(), B.end());
    return U;
}

// Find a vertex v in U which has at least one edge to A
int find_vertex_with_edge(const vector<int>& A, long long eA, const vector<int>& U) {
    vector<int> cur = U;
    while (cur.size() > 1) {
        int mid = (int)cur.size() / 2;
        vector<int> L(cur.begin(), cur.begin() + mid);
        vector<int> R(cur.begin() + mid, cur.end());

        long long eL = ask(L);
        vector<int> AL = union_disjoint(A, L);
        long long eAL = ask(AL);
        long long cross = eAL - eA - eL;
        if (cross > 0) {
            cur.swap(L);
        } else {
            cur.swap(R);
        }
    }
    return cur[0];
}

// Find a neighbor u in A of vertex v (i.e., {v,u} is an edge)
int find_neighbor_in_A_for_v(int v, const vector<int>& A) {
    vector<int> cur = A;
    while (cur.size() > 1) {
        int mid = (int)cur.size() / 2;
        vector<int> L(cur.begin(), cur.begin() + mid);
        vector<int> R(cur.begin() + mid, cur.end());

        long long eL = ask(L);
        vector<int> Lv = L;
        Lv.push_back(v);
        long long eLv = ask(Lv);
        long long deg = eLv - eL;
        if (deg > 0) {
            cur.swap(L);
        } else {
            cur.swap(R);
        }
    }
    return cur[0];
}

// Find a vertex in L that has at least one edge to R (both disjoint)
int find_vertex_with_edge_between(const vector<int>& L, const vector<int>& R, long long eR) {
    vector<int> cur = L;
    while (cur.size() > 1) {
        int mid = (int)cur.size() / 2;
        vector<int> L1(cur.begin(), cur.begin() + mid);
        vector<int> L2(cur.begin() + mid, cur.end());
        long long eL1 = ask(L1);
        vector<int> L1R = union_disjoint(L1, R);
        long long eL1R = ask(L1R);
        long long cross = eL1R - eL1 - eR;
        if (cross > 0) {
            cur.swap(L1);
        } else {
            cur.swap(L2);
        }
    }
    return cur[0];
}

// Given x and set R, find y in R such that {x,y} is an edge
int find_neighbor_of_x_in_R(int x, const vector<int>& R) {
    vector<int> cur = R;
    while (cur.size() > 1) {
        int mid = (int)cur.size() / 2;
        vector<int> L(cur.begin(), cur.begin() + mid);
        vector<int> RR(cur.begin() + mid, cur.end());
        long long eL = ask(L);
        vector<int> Lx = L;
        Lx.push_back(x);
        long long eLx = ask(Lx);
        long long deg = eLx - eL;
        if (deg > 0) {
            cur.swap(L);
        } else {
            cur.swap(RR);
        }
    }
    return cur[0];
}

// Find a pair (x, y) inside S with an edge between them (S is a set with E(S) > 0)
pair<int,int> find_internal_edge(vector<int> S) {
    long long eS = ask(S);
    while (true) {
        if ((int)S.size() <= 1) break; // should not happen if eS > 0
        int mid = (int)S.size() / 2;
        vector<int> L(S.begin(), S.begin() + mid);
        vector<int> R(S.begin() + mid, S.end());
        long long eL = ask(L);
        long long eR = ask(R);
        if (eL > 0) {
            S.swap(L);
            eS = eL;
            continue;
        }
        if (eR > 0) {
            S.swap(R);
            eS = eR;
            continue;
        }
        // Both eL and eR are zero; so there must be a cross edge between L and R
        int x = find_vertex_with_edge_between(L, R, eR);
        int y = find_neighbor_of_x_in_R(x, R);
        return {x, y};
    }
    // Fallback (should not reach here)
    return {-1, -1};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> n)) return 0;

    vector<vector<int>> tree(n + 1);
    vector<int> color(n + 1, -1), parent(n + 1, -1);
    vector<char> used(n + 1, 0);
    vector<int> visited;

    // Start from vertex 1
    used[1] = 1;
    color[1] = 0;
    parent[1] = -1;
    visited.push_back(1);

    while ((int)visited.size() < n) {
        vector<int> U;
        U.reserve(n);
        for (int i = 1; i <= n; ++i) if (!used[i]) U.push_back(i);

        long long eA = ask(visited);

        int v = find_vertex_with_edge(visited, eA, U);
        int u;
        if ((int)visited.size() == 1) {
            u = visited[0];
        } else {
            u = find_neighbor_in_A_for_v(v, visited);
        }

        // add edge u - v to the tree
        tree[u].push_back(v);
        tree[v].push_back(u);
        parent[v] = u;
        color[v] = 1 - color[u];
        used[v] = 1;
        visited.push_back(v);
    }

    // Now verify bipartiteness by checking internal edges within color classes
    vector<int> C0, C1;
    for (int i = 1; i <= n; ++i) {
        if (color[i] == 0) C0.push_back(i);
        else C1.push_back(i);
    }

    long long e0 = ask(C0);
    long long e1 = ask(C1);

    if (e0 == 0 && e1 == 0) {
        cout << "Y " << (int)C0.size() << "\n";
        for (int i = 0; i < (int)C0.size(); ++i) {
            if (i) cout << ' ';
            cout << C0[i];
        }
        cout << "\n";
        cout.flush();
        return 0;
    } else {
        vector<int> S = (e0 > 0 ? C0 : C1);
        pair<int,int> edge = find_internal_edge(S);
        int x = edge.first, y = edge.second;

        // Find path between x and y in the spanning tree
        vector<int> prev(n + 1, -1);
        queue<int> q;
        q.push(x);
        prev[x] = x;
        while (!q.empty() && prev[y] == -1) {
            int u = q.front(); q.pop();
            for (int w : tree[u]) {
                if (prev[w] == -1) {
                    prev[w] = u;
                    q.push(w);
                }
            }
        }

        vector<int> path;
        int cur = y;
        while (cur != x) {
            path.push_back(cur);
            cur = prev[cur];
            if (cur == -1) break; // safety
        }
        path.push_back(x);
        reverse(path.begin(), path.end());

        cout << "N " << (int)path.size() << "\n";
        for (int i = 0; i < (int)path.size(); ++i) {
            if (i) cout << ' ';
            cout << path[i];
        }
        cout << "\n";
        cout.flush();
    }

    return 0;
}