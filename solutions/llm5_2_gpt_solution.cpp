#include <bits/stdc++.h>
using namespace std;

static inline void fast_io() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
}

struct GreedyPathBuilder {
    int n;
    const vector<vector<int>> *adj; // adjacency list (outgoing)
    vector<int> parent, head, tail, nxt;
    vector<unsigned char> indeg;

    GreedyPathBuilder(int n_, const vector<vector<int>> *adj_) : n(n_), adj(adj_) {
        parent.resize(n + 1);
        head.resize(n + 1);
        tail.resize(n + 1);
        nxt.resize(n + 1);
        indeg.resize(n + 1);
    }

    inline int findp(int x) {
        while (parent[x] != x) {
            parent[x] = parent[parent[x]];
            x = parent[x];
        }
        return x;
    }

    int run(const vector<int> &order, vector<int> &outPath) {
        for (int i = 1; i <= n; ++i) {
            parent[i] = i;
            head[i] = i;
            tail[i] = i;
            nxt[i] = 0;
            indeg[i] = 0;
        }

        for (int u : order) {
            int ru = findp(u);
            if (tail[ru] != u) continue; // not a current tail; cannot extend from u
            const auto &neis = (*adj)[u];
            for (int v : neis) {
                int rv = findp(v);
                if (ru == rv) continue;
                if (head[rv] != v) continue; // v must be the head of its component
                // Merge ru -> rv using edge u->v
                parent[rv] = ru;
                nxt[u] = v;
                indeg[v] = 1;
                tail[ru] = tail[rv];
                break; // u no longer tail; move to next u
            }
        }

        int bestLen = 0;
        int bestHead = -1;

        // Traverse chains starting at nodes with indeg == 0
        vector<char> used(n + 1, 0);
        for (int i = 1; i <= n; ++i) {
            if (indeg[i] == 0) {
                int len = 0;
                int cur = i;
                while (cur != 0 && !used[cur]) {
                    used[cur] = 1;
                    ++len;
                    cur = nxt[cur];
                }
                if (len > bestLen) {
                    bestLen = len;
                    bestHead = i;
                }
            }
        }
        if (bestHead == -1) {
            // All nodes have indeg 1? Shouldn't happen with our merging preventing cycles,
            // but fallback: pick any node.
            bestHead = 1;
            bestLen = 1;
        }

        outPath.clear();
        outPath.reserve(bestLen);
        int cur = bestHead;
        while (cur != 0 && (int)outPath.size() < bestLen) {
            outPath.push_back(cur);
            cur = nxt[cur];
        }
        return (int)outPath.size();
    }
};

struct TournamentHeuristic {
    int n;
    const vector<vector<int>> *outAdj; // sorted for binary search

    TournamentHeuristic(int n_, const vector<vector<int>> *outAdj_) : n(n_), outAdj(outAdj_) {}

    inline bool hasEdge(int u, int v) const {
        const auto &vec = (*outAdj)[u];
        return binary_search(vec.begin(), vec.end(), v);
    }

    // Try to construct Hamiltonian path via bubble insertion approach (works on tournaments)
    bool try_build(const vector<int> &perm, vector<int> &path) {
        path.clear();
        path.reserve(n);
        for (int v : perm) {
            path.push_back(v);
            int j = (int)path.size() - 2;
            while (j >= 0) {
                int a = path[j], b = path[j + 1];
                if (hasEdge(a, b)) {
                    break; // ok for this adjacent pair
                } else if (hasEdge(b, a)) {
                    swap(path[j], path[j + 1]);
                    --j;
                } else {
                    // No edge between a and b in either direction: cannot fix further
                    break;
                }
            }
        }
        // Validate adjacency in final path
        for (int i = 0; i + 1 < (int)path.size(); ++i) {
            if (!hasEdge(path[i], path[i + 1])) return false;
        }
        return true;
    }
};

int main() {
    fast_io();

    int n, m;
    if (!(cin >> n >> m)) {
        return 0;
    }
    // Read scoring parameters (unused)
    for (int i = 0; i < 10; ++i) {
        int tmp; cin >> tmp;
    }

    vector<vector<int>> out(n + 1), in(n + 1);
    out.shrink_to_fit();
    in.shrink_to_fit();

    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        out[u].push_back(v);
        in[v].push_back(u);
    }

    // For small n, try tournament heuristic with sorted adjacency.
    vector<int> bestPath;
    bestPath.reserve(n);
    int bestLen = 0;

    // Sort adjacency for binary search in tournament heuristic
    bool tryTournament = (n <= 2000);
    if (tryTournament) {
        for (int i = 1; i <= n; ++i) {
            sort(out[i].begin(), out[i].end());
        }
        TournamentHeuristic th(n, &out);
        vector<int> perm(n);
        iota(perm.begin(), perm.end(), 1);

        // Multiple attempts with different insert orders
        vector<int> cand;
        // Attempt 1: natural order
        if (th.try_build(perm, cand)) {
            if ((int)cand.size() > bestLen) {
                bestLen = (int)cand.size();
                bestPath = cand;
                if (bestLen == n) {
                    cout << bestLen << "\n";
                    for (int i = 0; i < bestLen; ++i) {
                        if (i) cout << ' ';
                        cout << bestPath[i];
                    }
                    cout << "\n";
                    return 0;
                }
            }
        }
        // Attempt 2: reverse order
        reverse(perm.begin(), perm.end());
        if (th.try_build(perm, cand)) {
            if ((int)cand.size() > bestLen) {
                bestLen = (int)cand.size();
                bestPath = cand;
                if (bestLen == n) {
                    cout << bestLen << "\n";
                    for (int i = 0; i < bestLen; ++i) {
                        if (i) cout << ' ';
                        cout << bestPath[i];
                    }
                    cout << "\n";
                    return 0;
                }
            }
        }
        // Attempt 3 and 4: random permutations
        mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
        iota(perm.begin(), perm.end(), 1);
        for (int t = 0; t < 2; ++t) {
            shuffle(perm.begin(), perm.end(), rng);
            if (th.try_build(perm, cand)) {
                if ((int)cand.size() > bestLen) {
                    bestLen = (int)cand.size();
                    bestPath = cand;
                    if (bestLen == n) {
                        cout << bestLen << "\n";
                        for (int i = 0; i < bestLen; ++i) {
                            if (i) cout << ' ';
                            cout << bestPath[i];
                        }
                        cout << "\n";
                        return 0;
                    }
                }
            }
        }
    }

    // Greedy DSU-based path cover attempts on out and in adjacency
    GreedyPathBuilder builderOut(n, &out);
    GreedyPathBuilder builderIn(n, &in);

    vector<int> order(n);
    iota(order.begin(), order.end(), 1);

    vector<int> path;

    auto consider_path = [&](const vector<int> &p) {
        if ((int)p.size() > bestLen) {
            bestLen = (int)p.size();
            bestPath = p;
        }
    };

    // Attempts with various orders on outgoing edges
    // 1) natural
    builderOut.run(order, path);
    consider_path(path);
    if (bestLen == n) {
        cout << bestLen << "\n";
        for (int i = 0; i < bestLen; ++i) {
            if (i) cout << ' ';
            cout << bestPath[i];
        }
        cout << "\n";
        return 0;
    }
    // 2) reverse
    reverse(order.begin(), order.end());
    builderOut.run(order, path);
    consider_path(path);
    if (bestLen == n) {
        cout << bestLen << "\n";
        for (int i = 0; i < bestLen; ++i) {
            if (i) cout << ' ';
            cout << bestPath[i];
        }
        cout << "\n";
        return 0;
    }
    // 3-4) random shuffles
    mt19937_64 rng2(chrono::steady_clock::now().time_since_epoch().count() ^ 0x9e3779b97f4a7c15ULL);
    iota(order.begin(), order.end(), 1);
    for (int t = 0; t < 2; ++t) {
        shuffle(order.begin(), order.end(), rng2);
        builderOut.run(order, path);
        consider_path(path);
        if (bestLen == n) {
            cout << bestLen << "\n";
            for (int i = 0; i < bestLen; ++i) {
                if (i) cout << ' ';
                cout << bestPath[i];
            }
            cout << "\n";
            return 0;
        }
    }

    // Attempts on reversed edges (use in-adjacency), reverse the resulting path to match original directions
    // 1) natural
    iota(order.begin(), order.end(), 1);
    builderIn.run(order, path);
    reverse(path.begin(), path.end());
    consider_path(path);
    if (bestLen == n) {
        cout << bestLen << "\n";
        for (int i = 0; i < bestLen; ++i) {
            if (i) cout << ' ';
            cout << bestPath[i];
        }
        cout << "\n";
        return 0;
    }
    // 2) random
    shuffle(order.begin(), order.end(), rng2);
    builderIn.run(order, path);
    reverse(path.begin(), path.end());
    consider_path(path);

    if (bestLen == 0) {
        // Fallback: output any single vertex
        bestLen = 1;
        bestPath = {1};
    }

    cout << bestLen << "\n";
    for (int i = 0; i < bestLen; ++i) {
        if (i) cout << ' ';
        cout << bestPath[i];
    }
    cout << "\n";
    return 0;
}