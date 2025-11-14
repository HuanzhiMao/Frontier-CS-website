#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    if (!(cin >> n >> m)) return 0;
    int a[10];
    for (int i = 0; i < 10; ++i) cin >> a[i]; // scoring parameters, unused

    vector<int> U(m), V(m);
    vector<int> degOut(n + 1, 0), degIn(n + 1, 0);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        U[i] = u; V[i] = v;
        degOut[u]++; degIn[v]++;
    }

    // Build adjacency lists with reserved sizes
    vector<vector<int>> out(n + 1), in(n + 1);
    for (int u = 1; u <= n; ++u) {
        out[u].reserve(degOut[u]);
        in[u].reserve(degIn[u]);
    }
    for (int i = 0; i < m; ++i) {
        out[U[i]].push_back(V[i]);
        in[V[i]].push_back(U[i]);
    }
    U.clear(); V.clear();
    U.shrink_to_fit(); V.shrink_to_fit();

    // Build initial disjoint directed paths by greedy double-ended extension
    vector<char> used(n + 1, 0);
    vector<deque<int>> segs;
    segs.reserve(n);
    for (int v = 1; v <= n; ++v) {
        if (used[v]) continue;
        used[v] = 1;
        deque<int> seg;
        seg.push_back(v);

        bool progress = true;
        while (progress) {
            progress = false;

            // Extend at tail as far as possible
            while (true) {
                int tail = seg.back();
                bool extended = false;
                for (int w : out[tail]) {
                    if (!used[w]) {
                        used[w] = 1;
                        seg.push_back(w);
                        extended = true;
                        break;
                    }
                }
                if (!extended) break;
                progress = true;
            }

            // Extend at head as far as possible
            while (true) {
                int head = seg.front();
                bool extended = false;
                for (int u : in[head]) {
                    if (!used[u]) {
                        used[u] = 1;
                        seg.push_front(u);
                        extended = true;
                        break;
                    }
                }
                if (!extended) break;
                progress = true;
            }
        }

        segs.push_back(move(seg));
    }

    int S = (int)segs.size();
    if (S == 0) {
        cout << 0 << "\n\n";
        return 0;
    }

    // Prepare mappings for segment heads and tails
    vector<int> headID(n + 1, -1), tailID(n + 1, -1);
    vector<char> alive(S, 1);
    for (int s = 0; s < S; ++s) {
        if (!segs[s].empty()) {
            headID[segs[s].front()] = s;
            tailID[segs[s].back()] = s;
        } else {
            alive[s] = 0;
        }
    }

    // Pointers for scanning neighbors during merges
    vector<int> mergeOutPtr(n + 1, 0), mergeInPtr(n + 1, 0);

    // Queue segments to process merges
    deque<int> q;
    for (int s = 0; s < S; ++s) if (alive[s]) q.push_back(s);

    while (!q.empty()) {
        int s = q.front(); q.pop_front();
        if (!alive[s]) continue;

        while (true) {
            bool merged = false;

            // Try to merge at tail: tail(s) -> head(other)
            {
                int u = segs[s].back();
                int &ptr = mergeOutPtr[u];
                while (ptr < (int)out[u].size()) {
                    int w = out[u][ptr++];
                    int s2 = (w >= 1 && w <= n) ? headID[w] : -1;
                    if (s2 != -1 && alive[s2] && s2 != s) {
                        // Merge s + s2
                        int oldTail = u;
                        tailID[oldTail] = -1;
                        int oldHeadS2 = segs[s2].front();
                        headID[oldHeadS2] = -1;

                        // Append segs[s2] to segs[s]
                        for (int x : segs[s2]) segs[s].push_back(x);

                        int newHead = segs[s].front();
                        int newTail = segs[s].back();
                        headID[newHead] = s;
                        tailID[newTail] = s;

                        alive[s2] = 0;
                        merged = true;
                        break;
                    }
                }
            }
            if (merged) continue;

            // Try to merge at head: tail(other) -> head(s)
            {
                int h = segs[s].front();
                int &ptr = mergeInPtr[h];
                while (ptr < (int)in[h].size()) {
                    int u = in[h][ptr++];
                    int s2 = (u >= 1 && u <= n) ? tailID[u] : -1;
                    if (s2 != -1 && alive[s2] && s2 != s) {
                        // Merge s2 + s (prepend segs[s] with segs[s2])
                        int oldHeadS = h;
                        headID[oldHeadS] = -1;
                        int oldTailS2 = u;
                        tailID[oldTailS2] = -1;

                        // Prepend segs[s2] into segs[s] keeping order
                        for (auto it = segs[s2].rbegin(); it != segs[s2].rend(); ++it) {
                            segs[s].push_front(*it);
                        }

                        int newHead = segs[s].front();
                        int newTail = segs[s].back();
                        headID[newHead] = s;
                        tailID[newTail] = s;

                        alive[s2] = 0;
                        merged = true;
                        break;
                    }
                }
            }

            if (!merged) break;
        }
    }

    // Choose the longest alive segment
    int best = -1, bestLen = -1;
    for (int s = 0; s < S; ++s) if (alive[s]) {
        int len = (int)segs[s].size();
        if (len > bestLen) {
            bestLen = len;
            best = s;
        }
    }

    if (best == -1) {
        // Fallback: pick any vertex
        cout << 1 << "\n1\n";
        return 0;
    }

    cout << segs[best].size() << "\n";
    bool first = true;
    for (int v : segs[best]) {
        if (!first) cout << ' ';
        cout << v;
        first = false;
    }
    cout << "\n";
    return 0;
}