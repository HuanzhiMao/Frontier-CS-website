#include <bits/stdc++.h>
using namespace std;

struct Timer {
    chrono::high_resolution_clock::time_point st;
    Timer() { reset(); }
    void reset() { st = chrono::high_resolution_clock::now(); }
    double elapsed_ms() const {
        auto now = chrono::high_resolution_clock::now();
        return chrono::duration<double, std::milli>(now - st).count();
    }
};

struct Board {
    int N;
    vector<int> a; // size N*N, 0 is empty
    int z; // index of empty
    Board() {}
    Board(int N_, const vector<int>& a_, int z_) : N(N_), a(a_), z(z_) {}
    inline bool can(char mv) const {
        int r = z / N, c = z % N;
        if (mv == 'U') return r > 0;
        if (mv == 'D') return r < N - 1;
        if (mv == 'L') return c > 0;
        if (mv == 'R') return c < N - 1;
        return false;
    }
    inline void apply(char mv) {
        int r = z / N, c = z % N;
        int nz = z;
        if (mv == 'U') nz = z - N;
        else if (mv == 'D') nz = z + N;
        else if (mv == 'L') nz = z - 1;
        else if (mv == 'R') nz = z + 1;
        swap(a[z], a[nz]);
        z = nz;
    }
    static inline char opp(char mv) {
        if (mv == 'U') return 'D';
        if (mv == 'D') return 'U';
        if (mv == 'L') return 'R';
        if (mv == 'R') return 'L';
        return '?';
    }
};

struct Scorer {
    int N;
    Scorer(int N_) : N(N_) {}
    inline bool hasEdgeRight(const vector<int>& b, int idx) const {
        int r = idx / N, c = idx % N;
        if (c >= N - 1) return false;
        int x = b[idx], y = b[idx + 1];
        if (x == 0 || y == 0) return false;
        return (x & 4) && (y & 1);
    }
    inline bool hasEdgeDown(const vector<int>& b, int idx) const {
        int r = idx / N, c = idx % N;
        if (r >= N - 1) return false;
        int x = b[idx], y = b[idx + N];
        if (x == 0 || y == 0) return false;
        return (x & 8) && (y & 2);
    }
    inline bool hasEdgeUp(const vector<int>& b, int idx) const {
        int r = idx / N;
        if (r == 0) return false;
        int x = b[idx], y = b[idx - N];
        if (x == 0 || y == 0) return false;
        return (x & 2) && (y & 8);
    }
    inline bool hasEdgeLeft(const vector<int>& b, int idx) const {
        int c = idx % N;
        if (c == 0) return false;
        int x = b[idx], y = b[idx - 1];
        if (x == 0 || y == 0) return false;
        return (x & 1) && (y & 4);
    }
    int largestTreeSize(const vector<int>& b) const {
        int NN = N * N;
        vector<char> vis(NN, 0);
        vector<int> compId(NN, -1);
        int best = 0;
        int cid = 0;
        vector<int> q;
        q.reserve(NN);
        for (int i = 0; i < NN; ++i) {
            if (b[i] == 0 || vis[i]) continue;
            // BFS to get component members
            vector<int> members;
            members.reserve(16);
            deque<int> dq;
            dq.push_back(i);
            vis[i] = 1;
            compId[i] = cid;
            while (!dq.empty()) {
                int v = dq.front(); dq.pop_front();
                members.push_back(v);
                // neighbors by edges
                // Up
                if (hasEdgeUp(b, v)) {
                    int u = v - N;
                    if (!vis[u]) { vis[u] = 1; compId[u] = cid; dq.push_back(u); }
                }
                // Down
                if (hasEdgeDown(b, v)) {
                    int u = v + N;
                    if (!vis[u]) { vis[u] = 1; compId[u] = cid; dq.push_back(u); }
                }
                // Left
                if (hasEdgeLeft(b, v)) {
                    int u = v - 1;
                    if (!vis[u]) { vis[u] = 1; compId[u] = cid; dq.push_back(u); }
                }
                // Right
                if (hasEdgeRight(b, v)) {
                    int u = v + 1;
                    if (!vis[u]) { vis[u] = 1; compId[u] = cid; dq.push_back(u); }
                }
            }
            // Count edges within this component
            long long edges = 0;
            for (int v : members) {
                int r = v / N, c = v % N;
                if (c + 1 < N && hasEdgeRight(b, v) && compId[v + 1] == cid) edges++;
                if (r + 1 < N && hasEdgeDown(b, v) && compId[v + N] == cid) edges++;
            }
            int V = (int)members.size();
            if ((int)edges == V - 1) {
                if (V > best) best = V;
            }
            cid++;
        }
        return best;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N;
    int T;
    if (!(cin >> N >> T)) {
        return 0;
    }
    vector<int> a(N * N);
    int z = -1;
    for (int i = 0; i < N; ++i) {
        string s; cin >> s;
        for (int j = 0; j < N; ++j) {
            char ch = s[j];
            int val;
            if ('0' <= ch && ch <= '9') val = ch - '0';
            else val = 10 + (ch - 'a');
            a[i * N + j] = val;
            if (val == 0) z = i * N + j;
        }
    }
    Board init(N, a, z);
    Scorer scorer(N);

    Timer timer;
    // Time limit: keep some safety margin
    double TIME_LIMIT_MS = 1900.0;
    mt19937_64 rng(chrono::high_resolution_clock::now().time_since_epoch().count());

    int globalBestS = scorer.largestTreeSize(init.a);
    string globalBestMoves = "";
    int globalBestLen = 0; // length to achieve globalBestS

    // Multiple restarts
    while (timer.elapsed_ms() < TIME_LIMIT_MS) {
        Board cur = init;
        string path;
        path.reserve(T);
        int bestS = scorer.largestTreeSize(cur.a);
        int bestLen = 0;

        char lastMove = '?';
        for (int step = 1; step <= T; ++step) {
            // Candidate moves
            array<char, 4> dirs = {'U','D','L','R'};
            vector<char> cand;
            cand.reserve(4);
            for (char d : dirs) if (cur.can(d)) cand.push_back(d);
            // avoid immediate reverse if possible
            if (lastMove != '?') {
                char rev = Board::opp(lastMove);
                if ((int)cand.size() >= 2) {
                    // erase reverse move
                    auto it = find(cand.begin(), cand.end(), rev);
                    if (it != cand.end()) {
                        // with small prob keep it
                        if (uniform_real_distribution<double>(0.0,1.0)(rng) < 0.1) {
                            // keep
                        } else {
                            cand.erase(it);
                        }
                    }
                }
            }
            // Random exploration chance
            bool doRandom = (uniform_real_distribution<double>(0.0,1.0)(rng) < 0.05);
            char chosen = cand[uniform_int_distribution<int>(0, (int)cand.size()-1)(rng)];
            int chosenScore = -1;

            if (!doRandom) {
                int bestLocalS = -1;
                vector<char> bestDirs;
                int evalCount = 0;
                for (char d : cand) {
                    cur.apply(d);
                    int s = scorer.largestTreeSize(cur.a);
                    evalCount++;
                    // revert
                    cur.apply(Board::opp(d));
                    if (s > bestLocalS) {
                        bestLocalS = s;
                        bestDirs.clear();
                        bestDirs.push_back(d);
                    } else if (s == bestLocalS) {
                        bestDirs.push_back(d);
                    }
                }
                chosen = bestDirs[uniform_int_distribution<int>(0, (int)bestDirs.size()-1)(rng)];
                chosenScore = bestLocalS;
            }
            // apply chosen
            cur.apply(chosen);
            path.push_back(chosen);
            lastMove = chosen;

            int currS;
            if (doRandom || chosenScore < 0) currS = scorer.largestTreeSize(cur.a);
            else currS = chosenScore;

            if (currS > bestS) {
                bestS = currS;
                bestLen = step;
            }
            // Early stop if perfect tree achieved
            if (bestS >= N*N - 1) break;
            if (timer.elapsed_ms() > TIME_LIMIT_MS) break;
        }
        // Compare with global best
        if (bestS > globalBestS) {
            globalBestS = bestS;
            globalBestLen = bestLen;
            globalBestMoves = path.substr(0, bestLen);
        } else if (bestS == globalBestS) {
            // Prefer shorter if full tree
            if (bestS == N*N - 1 && bestLen < globalBestLen) {
                globalBestLen = bestLen;
                globalBestMoves = path.substr(0, bestLen);
            }
        }
        if (timer.elapsed_ms() > TIME_LIMIT_MS) break;
    }

    // Ensure not exceeding T
    if ((int)globalBestMoves.size() > T) {
        globalBestMoves.resize(T);
    }
    cout << globalBestMoves << '\n';
    return 0;
}