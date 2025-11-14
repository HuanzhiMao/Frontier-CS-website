#include <bits/stdc++.h>
using namespace std;

struct FastScanner {
    static const int BUFSIZE = 1 << 20;
    int idx, size;
    char buf[BUFSIZE];
    FastScanner(): idx(0), size(0) {}
    inline char getChar() {
        if (idx >= size) {
            size = (int)fread(buf, 1, BUFSIZE, stdin);
            idx = 0;
            if (size == 0) return 0;
        }
        return buf[idx++];
    }
    template<typename T>
    bool nextInt(T &out) {
        char c; T sign = 1; T val = 0;
        c = getChar();
        if (!c) return false;
        while (c != '-' && (c < '0' || c > '9')) {
            c = getChar();
            if (!c) return false;
        }
        if (c == '-') { sign = -1; c = getChar(); }
        for (; c >= '0' && c <= '9'; c = getChar()) val = val * 10 + (c - '0');
        out = val * sign;
        return true;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    FastScanner fs;
    int n, m;
    if (!fs.nextInt(n)) return 0;
    fs.nextInt(m);
    int ai;
    for (int i = 0; i < 10; ++i) fs.nextInt(ai); // read and ignore
    vector<int> U(m), V(m);
    // Build adjacency for left part (1..n) to right part (1..n)
    vector<int> head(n + 1, -1);
    vector<int> to(m), nxt(m);
    for (int i = 0; i < m; ++i) {
        int u, v;
        fs.nextInt(u); fs.nextInt(v);
        U[i] = u; V[i] = v;
        to[i] = v;
        nxt[i] = head[u];
        head[u] = i;
    }

    // Hopcroft-Karp
    const int INF = 1e9;
    vector<int> pairU(n + 1, 0), pairV(n + 1, 0), dist(n + 1, INF);

    // Greedy init
    for (int u = 1; u <= n; ++u) {
        if (pairU[u] != 0) continue;
        for (int e = head[u]; e != -1; e = nxt[e]) {
            int v = to[e];
            if (pairV[v] == 0) {
                pairU[u] = v;
                pairV[v] = u;
                break;
            }
        }
    }

    vector<int> q(n + 5);
    auto bfs = [&]() -> bool {
        int qs = 0, qe = 0;
        bool found = false;
        for (int u = 1; u <= n; ++u) {
            if (pairU[u] == 0) {
                dist[u] = 0;
                q[qe++] = u;
            } else dist[u] = INF;
        }
        while (qs < qe) {
            int u = q[qs++];
            for (int e = head[u]; e != -1; e = nxt[e]) {
                int v = to[e];
                int u2 = pairV[v];
                if (u2 == 0) {
                    found = true;
                } else if (dist[u2] == INF) {
                    dist[u2] = dist[u] + 1;
                    q[qe++] = u2;
                }
            }
        }
        return found;
    };

    // Iterative DFS per BFS phase
    vector<int> work(n + 1, -1), workPhase(n + 1, -1);
    int phase = 0;
    auto dfs_iter = [&](int start) -> bool {
        ++phase;
        vector<int> stackU;
        vector<int> stackE;
        int u = start;
        workPhase[u] = phase;
        work[u] = head[u];
        while (true) {
            bool advanced = false;
            for (int &it = work[u]; it != -1; it = nxt[it]) {
                int v = to[it];
                if (pairV[v] == 0) {
                    pairU[u] = v;
                    pairV[v] = u;
                    while (!stackU.empty()) {
                        int pu = stackU.back(); stackU.pop_back();
                        int eUsed = stackE.back(); stackE.pop_back();
                        int vv = to[eUsed];
                        // vv was matched to some node deeper; now reassign
                        pairU[pu] = vv;
                        pairV[vv] = pu;
                    }
                    return true;
                } else {
                    int u2 = pairV[v];
                    if (dist[u2] == dist[u] + 1) {
                        int used = it;
                        it = nxt[it]; // advance parent's iterator (as recursion would)
                        stackU.push_back(u);
                        stackE.push_back(used);
                        u = u2;
                        if (workPhase[u] != phase) {
                            workPhase[u] = phase;
                            work[u] = head[u];
                        }
                        advanced = true;
                        break;
                    }
                }
            }
            if (advanced) continue;
            dist[u] = INF;
            if (stackU.empty()) return false;
            // backtrack
            u = stackU.back(); stackU.pop_back();
            stackE.pop_back(); // we already advanced work[u] past used edge
        }
    };

    while (bfs()) {
        for (int u = 1; u <= n; ++u) {
            if (pairU[u] == 0) {
                dfs_iter(u);
            }
        }
    }

    // Build succ/pred from matching
    vector<int> succ(n + 1, 0), pred(n + 1, 0);
    for (int u = 1; u <= n; ++u) {
        int v = pairU[u];
        if (v != 0) {
            succ[u] = v;
            pred[v] = u;
        }
    }

    // Build components from succ/pred:
    vector<int> compID(n + 1, 0);
    vector<int> compHead(n + 1, 0), compTail(n + 1, 0), compParent(n + 1, 0), compSize(n + 1, 0);
    int compCnt = 0;

    // Paths starting at vertices with pred == 0
    for (int u = 1; u <= n; ++u) {
        if (pred[u] == 0) {
            int x = u, last = 0, cnt = 0;
            ++compCnt;
            while (x != 0 && compID[x] == 0) {
                compID[x] = compCnt;
                ++cnt;
                last = x;
                x = succ[x];
            }
            compHead[compCnt] = u;
            compTail[compCnt] = last ? last : u;
            compSize[compCnt] = cnt == 0 ? 1 : cnt;
        }
    }
    // Handle cycles: break one edge per cycle
    vector<int> seen(n + 1, 0);
    int visitMark = 1;
    for (int u = 1; u <= n; ++u) {
        if (compID[u] == 0) {
            int x = u;
            ++visitMark;
            while (seen[x] != visitMark) {
                seen[x] = visitMark;
                x = succ[x];
            }
            int y = x;
            int p = pred[y];
            if (p != 0) {
                succ[p] = 0;
                pred[y] = 0;
            }
            // Now traverse path starting at y
            ++compCnt;
            int last = 0, cnt = 0;
            x = y;
            while (x != 0 && compID[x] == 0) {
                compID[x] = compCnt;
                ++cnt;
                last = x;
                x = succ[x];
            }
            compHead[compCnt] = y;
            compTail[compCnt] = last ? last : y;
            compSize[compCnt] = cnt == 0 ? 1 : cnt;
        }
    }

    // DSU for components
    auto findp = [&](auto self, int a) -> int {
        if (compParent[a] == a) return a;
        return compParent[a] = self(self, compParent[a]);
    };
    for (int i = 1; i <= compCnt; ++i) compParent[i] = i;

    // Merge components using edges tail->head
    for (int i = 0; i < m; ++i) {
        int u = U[i], v = V[i];
        int cu = findp(findp, compID[u]);
        int cv = findp(findp, compID[v]);
        if (cu == cv) continue;
        if (compTail[cu] == u && compHead[cv] == v) {
            // connect u->v
            succ[u] = v;
            pred[v] = u;
            // merge cu and cv
            int ru = cu, rv = cv;
            // union by size
            if (compSize[ru] < compSize[rv]) swap(ru, rv);
            compParent[rv] = ru;
            compSize[ru] += compSize[rv];
            // new head is head of cu (ru after swap), new tail is tail of cv (rv before swap)
            int newHead = compHead[cu];
            int newTail = compTail[cv];
            compHead[ru] = newHead;
            compTail[ru] = newTail;
        }
    }

    // Find the largest component
    int bestRoot = -1, bestSize = -1;
    for (int i = 1; i <= compCnt; ++i) {
        if (compParent[i] == i) {
            if (compSize[i] > bestSize) {
                bestSize = compSize[i];
                bestRoot = i;
            }
        }
    }
    int start = compHead[bestRoot];
    // Build the answer path
    vector<int> ans;
    ans.reserve(bestSize);
    int x = start;
    vector<char> used(n + 1, 0);
    while (x != 0 && !used[x]) {
        used[x] = 1;
        ans.push_back(x);
        x = succ[x];
    }

    // Output
    cout << (int)ans.size() << '\n';
    for (size_t i = 0; i < ans.size(); ++i) {
        if (i) cout << ' ';
        cout << ans[i];
    }
    cout << '\n';
    return 0;
}