#include <bits/stdc++.h>
using namespace std;

struct XorShift {
    uint64_t x;
    XorShift(uint64_t seed = 88172645463393265ull) { x = seed; }
    inline uint64_t next() {
        x ^= x << 7;
        x ^= x >> 9;
        return x;
    }
    inline uint32_t next_u32() { return (uint32_t)next(); }
    inline double next_double() { return (next() >> 11) * (1.0 / 9007199254740992.0); }
    inline int randint(int l, int r) { // [l, r)
        return l + (int)(next_u32() % (uint32_t)(r - l));
    }
};

struct Board {
    int N;
    vector<int> a; // size N*N, 0 means empty
    int er, ec;
    Board(int N=0): N(N), a(N*N,0), er(0), ec(0) {}
    inline int idx(int r, int c) const { return r*N + c; }
    inline bool in(int r, int c) const { return (0 <= r && r < N && 0 <= c && c < N); }
    inline bool validMove(char mv) const {
        if (mv=='U') return er > 0;
        if (mv=='D') return er < N-1;
        if (mv=='L') return ec > 0;
        if (mv=='R') return ec < N-1;
        return false;
    }
    inline char opposite(char mv) const {
        if (mv=='U') return 'D';
        if (mv=='D') return 'U';
        if (mv=='L') return 'R';
        if (mv=='R') return 'L';
        return '?';
    }
    // Apply move; assumes valid
    inline void apply(char mv) {
        int nr = er, nc = ec;
        if (mv=='U') nr = er-1;
        else if (mv=='D') nr = er+1;
        else if (mv=='L') nc = ec-1;
        else if (mv=='R') nc = ec+1;
        int ei = idx(er,ec);
        int ni = idx(nr,nc);
        swap(a[ei], a[ni]);
        er = nr; ec = nc;
    }
    inline bool connected(int r1, int c1, int r2, int c2) const {
        if (!in(r1,c1) || !in(r2,c2)) return false;
        int t1 = a[idx(r1,c1)];
        int t2 = a[idx(r2,c2)];
        if (t1==0 || t2==0) return false;
        int dr = r2 - r1;
        int dc = c2 - c1;
        if (dr==0 && dc==1) {
            // right neighbor
            return ((t1 & 4) && (t2 & 1));
        } else if (dr==0 && dc==-1) {
            // left neighbor
            return ((t1 & 1) && (t2 & 4));
        } else if (dr==1 && dc==0) {
            // down neighbor
            return ((t1 & 8) && (t2 & 2));
        } else if (dr==-1 && dc==0) {
            // up neighbor
            return ((t1 & 2) && (t2 & 8));
        }
        return false;
    }
};

struct EvalRes {
    int largestTree;      // size of largest acyclic connected component
    int bestPotential;    // max over components of V - cycles (where cycles = E - V + 1)
};

static inline EvalRes evaluateBoard(const Board& B) {
    int N = B.N;
    int NN = N*N;
    vector<int> vis(NN, 0);
    int bestTree = 0;
    int bestPot = 0;
    vector<int> compList;
    compList.reserve(NN);
    for (int r=0; r<N; r++) for (int c=0; c<N; c++) {
        int id = r*N + c;
        if (B.a[id] == 0 || vis[id]) continue;
        // BFS over adjacency edges
        queue<int> q;
        vis[id] = 1;
        q.push(id);
        compList.clear();
        compList.push_back(id);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            int ur = u / N, uc = u % N;
            // four neighbors
            if (B.in(ur, uc+1) && !vis[u+1] && B.connected(ur, uc, ur, uc+1)) {
                vis[u+1] = 1; q.push(u+1); compList.push_back(u+1);
            }
            if (B.in(ur+1, uc) && !vis[u+N] && B.connected(ur, uc, ur+1, uc)) {
                vis[u+N] = 1; q.push(u+N); compList.push_back(u+N);
            }
            if (B.in(ur, uc-1) && !vis[u-1] && B.connected(ur, uc, ur, uc-1)) {
                vis[u-1] = 1; q.push(u-1); compList.push_back(u-1);
            }
            if (B.in(ur-1, uc) && !vis[u-N] && B.connected(ur, uc, ur-1, uc)) {
                vis[u-N] = 1; q.push(u-N); compList.push_back(u-N);
            }
        }
        int V = (int)compList.size();
        int E = 0;
        for (int idx : compList) {
            int rr = idx / N, cc = idx % N;
            // count right and down to avoid double count
            if (B.in(rr, cc+1) && B.connected(rr, cc, rr, cc+1)) E++;
            if (B.in(rr+1, cc) && B.connected(rr, cc, rr+1, cc)) E++;
        }
        int cycles = E - V + 1; // for connected comp
        if (cycles == 0) {
            if (V > bestTree) bestTree = V;
        }
        int pot = V - max(0, cycles);
        if (pot > bestPot) bestPot = pot;
    }
    return {bestTree, bestPot};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N; int T;
    if (!(cin >> N >> T)) {
        cout << "\n";
        return 0;
    }
    Board B(N);
    for (int i=0;i<N;i++){
        string s; cin >> s;
        for (int j=0;j<N;j++){
            char ch = s[j];
            int val;
            if ('0' <= ch && ch <= '9') val = ch - '0';
            else val = 10 + (ch - 'a');
            B.a[i*N + j] = val;
            if (val == 0) { B.er = i; B.ec = j; }
        }
    }

    XorShift rng(chrono::high_resolution_clock::now().time_since_epoch().count());

    // Evaluate initial state
    EvalRes initEv = evaluateBoard(B);
    int bestL = initEv.largestTree;
    int bestPot = initEv.bestPotential;
    string moves;
    int bestLen = 0;

    // Parameters for softmax temperature
    double tempStart = 0.5;
    double tempEnd = 0.02;

    // Track last move to avoid immediate reversal
    char lastMove = '?';

    // Precompute candidates per step
    auto appendCandidates = [&](vector<char>& cand) {
        cand.clear();
        if (B.validMove('U') && lastMove!='D') cand.push_back('U');
        if (B.validMove('D') && lastMove!='U') cand.push_back('D');
        if (B.validMove('L') && lastMove!='R') cand.push_back('L');
        if (B.validMove('R') && lastMove!='L') cand.push_back('R');
        if (cand.empty()) { // if stuck due to lastMove constraint, allow any valid
            if (B.validMove('U')) cand.push_back('U');
            if (B.validMove('D')) cand.push_back('D');
            if (B.validMove('L')) cand.push_back('L');
            if (B.validMove('R')) cand.push_back('R');
        }
    };

    // Current evaluation
    EvalRes curEval = initEv;

    for (int step = 0; step < T; step++) {
        vector<char> cand;
        appendCandidates(cand);
        if (cand.empty()) break;

        // Evaluate candidates
        int msz = (int)cand.size();
        vector<EvalRes> evs(msz);
        vector<double> scores(msz);
        double bestScore = -1e100;
        double progress = (double)step / max(1, T-1);
        double temp = tempStart + (tempEnd - tempStart) * progress;
        for (int i=0;i<msz;i++){
            char mv = cand[i];
            B.apply(mv);
            evs[i] = evaluateBoard(B);
            // Score: prioritize largestTree, break ties with potential
            double sc = evs[i].largestTree + evs[i].bestPotential / (double)(N*N + 1);
            scores[i] = sc;
            if (sc > bestScore) bestScore = sc;
            B.apply(B.opposite(mv)); // undo
        }

        // Softmax selection
        // To avoid underflow, subtract max
        double sumw = 0.0;
        vector<double> w(msz, 0.0);
        for (int i=0;i<msz;i++){
            double x = (scores[i] - bestScore) / max(1e-9, temp);
            if (x < -50) x = -50;
            double wi = exp(x);
            w[i] = wi;
            sumw += wi;
        }
        int chosenIndex = 0;
        if (sumw <= 0) {
            chosenIndex = rng.randint(0, msz);
        } else {
            double r = rng.next_double() * sumw;
            double acc = 0.0;
            for (int i=0;i<msz;i++){
                acc += w[i];
                if (r <= acc) { chosenIndex = i; break; }
            }
        }

        char chosenMove = cand[chosenIndex];
        B.apply(chosenMove);
        moves.push_back(chosenMove);
        lastMove = chosenMove;
        curEval = evs[chosenIndex];

        // Update best prefix if improved
        if (curEval.largestTree > bestL || (curEval.largestTree == bestL && curEval.bestPotential > bestPot)) {
            bestL = curEval.largestTree;
            bestPot = curEval.bestPotential;
            bestLen = (int)moves.size();
            // Early exit if perfect tree achieved
            if (bestL == N*N - 1) {
                // We could keep going to reduce K for second scoring case, but K only matters if S=N^2-1.
                // However, since we cannot "undo", keep current prefix.
                break;
            }
        }
    }

    if (bestLen == 0) {
        cout << "\n";
    } else {
        cout << moves.substr(0, bestLen) << "\n";
    }
    return 0;
}