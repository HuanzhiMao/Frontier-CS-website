#include <bits/stdc++.h>
using namespace std;

struct FastScanner {
    static const int BUFSIZE = 1 << 20;
    int idx, size;
    char buf[BUFSIZE];
    FastScanner() : idx(0), size(0) {}
    inline char getch() {
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
        c = getch(); if (!c) return false;
        while (c != '-' && (c < '0' || c > '9')) { c = getch(); if (!c) return false; }
        if (c == '-') { sign = -1; c = getch(); }
        for (; c >= '0' && c <= '9'; c = getch()) val = val * 10 + (c - '0');
        out = val * sign;
        return true;
    }
    bool nextDouble(double &out) {
        char c = getch(); if (!c) return false;
        while (c != '-' && c != '.' && (c < '0' || c > '9')) { c = getch(); if (!c) return false; }
        int sign = 1;
        if (c == '-') { sign = -1; c = getch(); }
        long long ip = 0;
        while (c >= '0' && c <= '9') { ip = ip * 10 + (c - '0'); c = getch(); }
        double fp = 0.0, base = 1.0;
        if (c == '.') {
            c = getch();
            while (c >= '0' && c <= '9') {
                fp = fp * 10.0 + (c - '0');
                base *= 10.0;
                c = getch();
            }
        }
        out = sign * (ip + fp / base);
        return true;
    }
};

struct FastOutput {
    static const int BUFSIZE = 1 << 20;
    int idx;
    char buf[BUFSIZE];
    FastOutput() : idx(0) {}
    ~FastOutput() { flush(); }
    inline void pushChar(char c) {
        if (idx >= BUFSIZE) flush();
        buf[idx++] = c;
    }
    inline void flush() {
        if (idx) {
            fwrite(buf, 1, idx, stdout);
            idx = 0;
        }
    }
    inline void printInt(int x) {
        if (x == 0) { pushChar('0'); return; }
        if (x < 0) { pushChar('-'); x = -x; }
        char s[12]; int n = 0;
        while (x) { s[n++] = char('0' + (x % 10)); x /= 10; }
        while (n--) pushChar(s[n]);
    }
};

struct MinHeapParts {
    int k;
    int capacity;
    vector<int> &sizeRef;
    vector<int> heap;      // stores part ids [1..k]
    vector<int> pos;       // position in heap per part id (or -1 if removed)
    vector<uint32_t> key;  // random tie-breaker per part id
    MinHeapParts(int _k, int _cap, vector<int> &sizes) : k(_k), capacity(_cap), sizeRef(sizes) {
        heap.resize(k);
        pos.assign(k + 1, -1);
        key.assign(k + 1, 0);
        // random but deterministic keys
        uint32_t seed = 123456789u;
        auto nextRand = [&]() {
            seed ^= seed << 13;
            seed ^= seed >> 17;
            seed ^= seed << 5;
            return seed;
        };
        for (int i = 1; i <= k; ++i) key[i] = nextRand();
        for (int i = 0; i < k; ++i) {
            heap[i] = i + 1;
            pos[i + 1] = i;
        }
        // build heap
        for (int i = (k / 2) - 1; i >= 0; --i) fixDown(i);
    }
    inline bool lessPart(int a, int b) const {
        int sa = sizeRef[a], sb = sizeRef[b];
        if (sa != sb) return sa < sb;
        return key[a] < key[b];
    }
    inline void swapAt(int i, int j) {
        int a = heap[i], b = heap[j];
        heap[i] = b; heap[j] = a;
        pos[a] = j; pos[b] = i;
    }
    inline void fixDown(int i) {
        int n = (int)heap.size();
        while (true) {
            int l = i * 2 + 1;
            if (l >= n) break;
            int r = l + 1;
            int m = l;
            if (r < n && lessPart(heap[r], heap[l])) m = r;
            if (!lessPart(heap[m], heap[i])) break;
            swapAt(i, m);
            i = m;
        }
    }
    inline void fixUp(int i) {
        while (i > 0) {
            int p = (i - 1) >> 1;
            if (!lessPart(heap[i], heap[p])) break;
            swapAt(i, p);
            i = p;
        }
    }
    inline void touch(int id) {
        int p = pos[id];
        if (p == -1) return; // removed (full)
        // size increased, so only fixDown needed
        fixDown(p);
    }
    inline int getMinAvailable() {
        while (!heap.empty()) {
            int id = heap[0];
            if (sizeRef[id] < capacity) return id;
            // remove full part
            int last = (int)heap.size() - 1;
            swapAt(0, last);
            pos[heap[last]] = -1;
            heap.pop_back();
            if (!heap.empty()) fixDown(0);
        }
        return -1; // should not happen if total capacity >= n
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    FastScanner fs;
    int n; long long mll; int k; double eps;
    if (!fs.nextInt(n)) return 0;
    fs.nextInt(mll);
    fs.nextInt(k);
    fs.nextDouble(eps);
    long long mReserve = mll;
    if (mReserve < 0) mReserve = 0;

    // Read edges, ignore self-loops; store edges for CSR build
    vector<int> deg(n + 1, 0);
    vector<int> U; U.reserve((size_t)min<long long>(mReserve, 40000000LL));
    vector<int> V; V.reserve((size_t)min<long long>(mReserve, 40000000LL));
    for (long long i = 0; i < mll; ++i) {
        int u, v;
        if (!fs.nextInt(u)) u = 0;
        fs.nextInt(v);
        if (u < 1 || u > n || v < 1 || v > n) continue;
        if (u == v) continue; // ignore self-loops
        U.push_back(u);
        V.push_back(v);
        deg[u]++; deg[v]++;
    }
    int64_t sumDeg = 0;
    for (int i = 1; i <= n; ++i) sumDeg += deg[i];
    vector<int> start(n + 1, 0), cur(n + 1, 0);
    for (int i = 2; i <= n; ++i) start[i] = start[i - 1] + deg[i - 1];
    vector<int> adj;
    adj.resize((size_t)sumDeg);
    for (size_t i = 0; i < U.size(); ++i) {
        int u = U[i], v = V[i];
        adj[start[u] + cur[u]++] = v;
        adj[start[v] + cur[v]++] = u;
    }
    // Free edges arrays
    U.clear(); V.clear();
    U.shrink_to_fit(); V.shrink_to_fit();

    int ideal = (n + k - 1) / k;
    int cap = (int)floor((1.0 + eps) * (double)ideal + 1e-9);
    if (cap < ideal) cap = ideal;

    vector<int> part(n + 1, 0);
    vector<int> psize(k + 1, 0);

    MinHeapParts heap(k, cap, psize);

    vector<char> visited(n + 1, 0);
    vector<int> counts(k + 1, 0);
    vector<int> touched; touched.reserve(32);

    // BFS over all components
    deque<int> q;
    for (int s = 1; s <= n; ++s) {
        if (visited[s]) continue;
        visited[s] = 1;
        q.clear();
        q.push_back(s);
        while (!q.empty()) {
            int x = q.front(); q.pop_front();

            // compute neighbor part counts (only already assigned neighbors contribute)
            touched.clear();
            int st = start[x], en = st + deg[x];
            for (int ei = st; ei < en; ++ei) {
                int nb = adj[ei];
                if (!visited[nb]) { visited[nb] = 1; q.push_back(nb); }
                int pb = part[nb];
                if (pb) {
                    if (counts[pb] == 0) touched.push_back(pb);
                    counts[pb]++;
                }
            }

            int bestPart = -1, bestCount = -1;
            for (int pid : touched) {
                if (psize[pid] >= cap) continue;
                int c = counts[pid];
                if (c > bestCount) {
                    bestCount = c;
                    bestPart = pid;
                } else if (c == bestCount && bestPart != -1) {
                    if (psize[pid] < psize[bestPart]) bestPart = pid;
                }
            }

            if (bestPart == -1) {
                int pick = heap.getMinAvailable();
                if (pick == -1) {
                    // Fallback to any part with room (should not happen)
                    int mini = 1;
                    for (int i = 2; i <= k; ++i) if (psize[i] < psize[mini]) mini = i;
                    bestPart = mini;
                } else bestPart = pick;
            }

            part[x] = bestPart;
            psize[bestPart]++;
            heap.touch(bestPart);

            for (int pid : touched) counts[pid] = 0;
        }
    }

    // Output
    FastOutput fo;
    for (int i = 1; i <= n; ++i) {
        fo.printInt(part[i]);
        if (i == n) fo.pushChar('\n');
        else fo.pushChar(' ');
    }
    fo.flush();
    return 0;
}