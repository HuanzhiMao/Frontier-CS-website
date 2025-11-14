#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N, M;
    if (!(cin >> N >> M)) return 0;
    vector<string> S(M);
    for (int i = 0; i < M; ++i) cin >> S[i];

    const int n2 = N * N;
    vector<array<int, 8>> counts(n2);
    for (int i = 0; i < n2; ++i) counts[i].fill(0);

    auto idx = [&](int r, int c) { return r * N + c; };
    auto ch2i = [&](char c) { return c - 'A'; };

    // Precompute frequency counts for each cell and character
    for (int si = 0; si < M; ++si) {
        const string &t = S[si];
        int k = (int)t.size();
        // Horizontal placements
        for (int r = 0; r < N; ++r) {
            for (int start = 0; start < N; ++start) {
                for (int p = 0; p < k; ++p) {
                    int c = (start + p) % N;
                    int id = idx(r, c);
                    counts[id][ch2i(t[p])] += 1;
                }
            }
        }
        // Vertical placements
        for (int c = 0; c < N; ++c) {
            for (int start = 0; start < N; ++start) {
                for (int p = 0; p < k; ++p) {
                    int r = (start + p) % N;
                    int id = idx(r, c);
                    counts[id][ch2i(t[p])] += 1;
                }
            }
        }
    }

    // Default character per cell: argmax counts
    vector<char> defChar(n2, 'A');
    for (int id = 0; id < n2; ++id) {
        int best = 0;
        int besti = 0;
        for (int c = 0; c < 8; ++c) {
            if (counts[id][c] > best) {
                best = counts[id][c];
                besti = c;
            }
        }
        defChar[id] = char('A' + besti);
    }

    // Greedy placement with zero-conflict constraint
    vector<char> grid(n2, '?');

    vector<int> order(M);
    iota(order.begin(), order.end(), 0);
    stable_sort(order.begin(), order.end(), [&](int a, int b) {
        if (S[a].size() != S[b].size()) return S[a].size() > S[b].size();
        return a < b;
    });

    for (int idxS = 0; idxS < M; ++idxS) {
        int si = order[idxS];
        const string &t = S[si];
        int k = (int)t.size();
        int bestAdd = -1;
        long long bestScore = -1;
        bool bestDir = false; // false: horizontal, true: vertical
        int bestR = -1, bestC = -1;

        // Horizontal
        for (int r = 0; r < N; ++r) {
            for (int start = 0; start < N; ++start) {
                bool conflict = false;
                int add = 0;
                long long score = 0;
                for (int p = 0; p < k; ++p) {
                    int c = (start + p) % N;
                    int id = r * N + c;
                    char g = grid[id];
                    char ch = t[p];
                    if (g != '?' && g != ch) { conflict = true; break; }
                    if (g == '?') add++;
                    score += counts[id][ch2i(ch)];
                }
                if (!conflict) {
                    if (add > bestAdd || (add == bestAdd && score > bestScore)) {
                        bestAdd = add; bestScore = score; bestDir = false; bestR = r; bestC = start;
                    }
                }
            }
        }
        // Vertical
        for (int c = 0; c < N; ++c) {
            for (int start = 0; start < N; ++start) {
                bool conflict = false;
                int add = 0;
                long long score = 0;
                for (int p = 0; p < k; ++p) {
                    int r = (start + p) % N;
                    int id = r * N + c;
                    char g = grid[id];
                    char ch = t[p];
                    if (g != '?' && g != ch) { conflict = true; break; }
                    if (g == '?') add++;
                    score += counts[id][ch2i(ch)];
                }
                if (!conflict) {
                    if (add > bestAdd || (add == bestAdd && score > bestScore)) {
                        bestAdd = add; bestScore = score; bestDir = true; bestR = start; bestC = c;
                    }
                }
            }
        }

        if (bestAdd >= 0) {
            if (!bestDir) {
                int r = bestR, start = bestC;
                for (int p = 0; p < k; ++p) {
                    int c = (start + p) % N;
                    int id = r * N + c;
                    if (grid[id] == '?') grid[id] = t[p];
                }
            } else {
                int c = bestC, start = bestR;
                for (int p = 0; p < k; ++p) {
                    int r = (start + p) % N;
                    int id = r * N + c;
                    if (grid[id] == '?') grid[id] = t[p];
                }
            }
        }
    }

    // Fill remaining cells with default character
    for (int id = 0; id < n2; ++id) {
        if (grid[id] == '?') grid[id] = defChar[id];
    }

    // Output
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            cout << grid[r * N + c];
        }
        cout << '\n';
    }
    return 0;
}