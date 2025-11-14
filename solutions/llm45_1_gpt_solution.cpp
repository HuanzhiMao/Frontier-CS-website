#include <bits/stdc++.h>
using namespace std;

struct FastScanner {
    static const size_t BUFSIZE = 1 << 20;
    int fd;
    size_t idx, size_;
    char buf[BUFSIZE];
    FastScanner() : fd(0), idx(0), size_(0) {}
    inline char getChar() {
        if (idx >= size_) {
            size_ = fread(buf, 1, BUFSIZE, stdin);
            idx = 0;
            if (size_ == 0) return 0;
        }
        return buf[idx++];
    }
    template<typename T>
    bool nextInt(T &out) {
        char c;
        T sign = 1;
        T val = 0;
        c = getChar();
        if (!c) return false;
        while (c <= ' ') {
            c = getChar();
            if (!c) return false;
        }
        if (c == '-') {
            sign = -1;
            c = getChar();
        }
        for (; c >= '0' && c <= '9'; c = getChar()) {
            val = val * 10 + (c - '0');
        }
        out = val * sign;
        return true;
    }
    bool nextDouble(double &out) {
        char c = getChar();
        if (!c) return false;
        while (c <= ' ') {
            c = getChar();
            if (!c) return false;
        }
        int sign = 1;
        if (c == '-') {
            sign = -1;
            c = getChar();
        }
        double val = 0.0;
        while (c >= '0' && c <= '9') {
            val = val * 10 + (c - '0');
            c = getChar();
        }
        if (c == '.') {
            double mul = 0.1;
            c = getChar();
            while (c >= '0' && c <= '9') {
                val += (c - '0') * mul;
                mul *= 0.1;
                c = getChar();
            }
        }
        // Optional exponent part
        if (c == 'e' || c == 'E') {
            int esign = 1;
            int expv = 0;
            c = getChar();
            if (c == '-') { esign = -1; c = getChar(); }
            else if (c == '+') { c = getChar(); }
            while (c >= '0' && c <= '9') {
                expv = expv * 10 + (c - '0');
                c = getChar();
            }
            val = val * pow(10.0, esign * expv);
        }
        out = val * sign;
        return true;
    }
};

struct FastOutput {
    static const size_t BUFSIZE = 1 << 20;
    char buf[BUFSIZE];
    size_t idx;
    FastOutput() : idx(0) {}
    ~FastOutput() { flush(); }
    inline void pushChar(char c) {
        if (idx >= BUFSIZE) flush();
        buf[idx++] = c;
    }
    inline void writeInt(int x) {
        if (x == 0) { pushChar('0'); return; }
        if (x < 0) { pushChar('-'); x = -x; }
        char tmp[20];
        int n = 0;
        while (x) { tmp[n++] = char('0' + (x % 10)); x /= 10; }
        while (n--) pushChar(tmp[n]);
    }
    inline void writeSpace() { pushChar(' '); }
    inline void writeNewline() { pushChar('\n'); }
    void flush() {
        if (idx) {
            fwrite(buf, 1, idx, stdout);
            idx = 0;
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    FastScanner fs;
    int n, m, k;
    double eps;
    if (!fs.nextInt(n)) return 0;
    fs.nextInt(m);
    fs.nextInt(k);
    fs.nextDouble(eps);

    // Build adjacency as static arrays (CSR-like linked lists)
    vector<int> head(n, -1);
    vector<int> to;
    vector<int> nxt;
    to.resize((size_t)2 * m);
    nxt.resize((size_t)2 * m);
    int ecnt = 0;
    auto add_edge = [&](int u, int v) {
        to[ecnt] = v;
        nxt[ecnt] = head[u];
        head[u] = ecnt++;
    };

    for (int i = 0; i < m; ++i) {
        int u, v;
        fs.nextInt(u);
        fs.nextInt(v);
        --u; --v;
        if (u == v) continue; // ignore self-loops
        add_edge(u, v);
        add_edge(v, u);
    }

    // Balance capacity
    long long ideal = (n + k - 1) / k;
    long long capLL = (long long)floor((1.0 + eps) * (double)ideal);
    if (capLL > n) capLL = n;
    int cap = (int)capLL;

    vector<int> part(n, -1);
    vector<int> load(k, 0);

    std::mt19937_64 rng(712367821ULL);
    int unassigned = n;

    auto choose_seed = [&](int tries) -> int {
        int best = -1, bestdeg = -1;
        for (int t = 0; t < tries; ++t) {
            int cand = (int)(rng() % (uint64_t)n);
            if (part[cand] == -1) {
                // degree is number of adjacency entries from head
                int deg = 0;
                for (int e = head[cand]; e != -1; e = nxt[e]) ++deg;
                if (deg > bestdeg) { bestdeg = deg; best = cand; }
            }
        }
        return best;
    };

    int scanPtr = 0;
    vector<int> q;
    q.reserve(1024);

    auto fill_part = [&](int pid) {
        q.clear();
        size_t qhead = 0;
        while (unassigned > 0 && load[pid] < cap) {
            if (qhead >= q.size()) {
                int s = choose_seed(32);
                if (s == -1) {
                    while (scanPtr < n && part[scanPtr] != -1) ++scanPtr;
                    if (scanPtr < n) s = scanPtr;
                    else {
                        // wrap-around scan
                        int i = 0;
                        for (; i < n; ++i) if (part[i] == -1) { s = i; break; }
                        if (i == n) break;
                    }
                }
                if (part[s] == -1) {
                    part[s] = pid;
                    load[pid]++;
                    unassigned--;
                    q.push_back(s);
                }
            } else {
                int u = q[qhead++];
                for (int e = head[u]; e != -1 && load[pid] < cap; e = nxt[e]) {
                    int v = to[e];
                    if (part[v] == -1) {
                        part[v] = pid;
                        load[pid]++;
                        unassigned--;
                        q.push_back(v);
                        if (load[pid] >= cap) break;
                    }
                }
            }
        }
    };

    // Grow k regions
    for (int pid = 0; pid < k && unassigned > 0; ++pid) {
        fill_part(pid);
    }

    // Assign any remaining vertices to least-loaded admissible parts
    if (unassigned > 0) {
        for (int v = 0; v < n; ++v) {
            if (part[v] != -1) continue;
            int bestp = -1;
            int bestload = INT_MAX;
            for (int p = 0; p < k; ++p) {
                if (load[p] < cap) {
                    if (load[p] < bestload) {
                        bestload = load[p];
                        bestp = p;
                    }
                }
            }
            if (bestp == -1) {
                // Should not happen; fallback to any part (cap might be >= n so safe)
                bestp = int(min_element(load.begin(), load.end()) - load.begin());
            }
            part[v] = bestp;
            load[bestp]++;
            unassigned--;
            if (unassigned == 0) break;
        }
    }

    // Lightweight refinement on boundary vertices (one pass, time-limited)
    auto t_start = chrono::high_resolution_clock::now();
    double time_budget = 0.75; // seconds for refinement
    bool do_refine = true;

    if (do_refine) {
        vector<char> boundary(n, 0);
        vector<int> boundary_list;
        boundary_list.reserve(n / 4 + 1);
        for (int v = 0; v < n; ++v) {
            int pv = part[v];
            bool isB = false;
            for (int e = head[v]; e != -1; e = nxt[e]) {
                int u = to[e];
                if (part[u] != pv) { isB = true; break; }
            }
            if (isB) {
                boundary[v] = 1;
                boundary_list.push_back(v);
            }
        }

        // Counts per part with timestamp trick
        vector<int> cnt(k, 0), seen(k, -1);
        int stamp = 0;

        // Shuffle boundary to avoid bias
        shuffle(boundary_list.begin(), boundary_list.end(), rng);

        for (int v : boundary_list) {
            // Time check
            auto now = chrono::high_resolution_clock::now();
            double elapsed = chrono::duration<double>(now - t_start).count();
            if (elapsed > time_budget) break;

            int cur = part[v];
            int bestp = cur;
            int bestc = -1;
            int curc = 0;
            stamp++;
            for (int e = head[v]; e != -1; e = nxt[e]) {
                int u = to[e];
                int pu = part[u];
                if (seen[pu] != stamp) { seen[pu] = stamp; cnt[pu] = 0; }
                cnt[pu]++;
            }
            if (seen[cur] == stamp) curc = cnt[cur];
            else curc = 0;

            // Evaluate only touched parts
            for (int e = head[v]; e != -1; e = nxt[e]) {
                int u = to[e];
                int pu = part[u];
                if (pu == cur) continue;
                if (seen[pu] != stamp) continue; // shouldn't happen
                int c = cnt[pu];
                if (c > bestc && load[pu] + 1 <= cap) {
                    bestc = c;
                    bestp = pu;
                }
            }
            // Move if strictly improves internal neighbor count
            if (bestp != cur && bestc > curc) {
                part[v] = bestp;
                load[bestp]++;
                load[cur]--;
            }
        }
    }

    FastOutput fo;
    for (int i = 0; i < n; ++i) {
        fo.writeInt(part[i] + 1);
        if (i + 1 < n) fo.writeSpace();
    }
    fo.writeNewline();
    fo.flush();
    return 0;
}