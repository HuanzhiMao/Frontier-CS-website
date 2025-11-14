#include <bits/stdc++.h>
using namespace std;

struct DSU {
    vector<int> p, r;
    DSU() {}
    DSU(int n): p(n), r(n,0) { iota(p.begin(), p.end(), 0); }
    int find(int x){ return p[x]==x? x : p[x]=find(p[x]); }
    void unite(int a, int b){
        a = find(a); b = find(b);
        if(a==b) return;
        if(r[a]<r[b]) swap(a,b);
        p[b]=a;
        if(r[a]==r[b]) r[a]++;
    }
};

struct Pattern {
    vector<int> pos;            // positions (compressed global)
    vector<unsigned char> let;  // letters 0..3
};

struct VecHash {
    size_t operator()(const vector<int>& v) const noexcept {
        uint64_t h = 1469598103934665603ull;
        for (int x : v) {
            h ^= (uint64_t)(uint32_t)x + 0x9e3779b97f4a7c15ull;
            h *= 1099511628211ull;
        }
        return (size_t)h;
    }
};

static inline int letterCode(char c) {
    if (c=='A') return 0;
    if (c=='C') return 1;
    if (c=='G') return 2;
    if (c=='T') return 3;
    return -1; // '?'
}

struct CompSolver {
    vector<Pattern> pats;         // patterns in component (local positions)
    vector<int> assign;           // -1 unassigned, else 0..3
    vector<int> stackPos;         // positions changed for backtrack
    int assignedCount;
    const vector<long double>& pow4;
    long double ans;
    int SUBSET_PRUNE_THRESHOLD = 16;

    CompSolver(const vector<Pattern>& p, const vector<long double>& pw)
        : pats(p), assign(), stackPos(), assignedCount(0), pow4(pw), ans(0) {}

    // Remove dominated patterns: if there exists another pattern that is a proper subset (same letters), remove current.
    void removeDominated() {
        int m = (int)pats.size();
        // Build set of existing codes
        unordered_set<vector<int>, VecHash> exist;
        exist.reserve(m*2+7);
        vector<vector<int>> codes(m);
        for (int i=0;i<m;i++){
            const auto& P = pats[i];
            vector<int> code; code.reserve(P.pos.size());
            for (size_t k=0;k<P.pos.size();k++){
                code.push_back((P.pos[k]<<2) | (int)P.let[k]);
            }
            codes[i] = move(code);
            exist.insert(codes[i]);
        }
        vector<Pattern> kept;
        for (int i=0;i<m;i++){
            const auto& P = pats[i];
            int k = (int)P.pos.size();
            bool dominated = false;
            if (k == 0) {
                // empty pattern exists -> global probability is 1, but we handle before calling component solver
                dominated = false;
            } else if (k <= SUBSET_PRUNE_THRESHOLD) {
                // Enumerate proper subsets
                int subsets = (1<<k);
                vector<int> subcode; subcode.reserve(k);
                for (int mask=1; mask<subsets-1; mask++) { // proper and non-empty subsets
                    subcode.clear();
                    for (int b=0;b<k;b++){
                        if (mask & (1<<b)){
                            subcode.push_back((P.pos[b]<<2) | (int)P.let[b]);
                        }
                    }
                    if (exist.find(subcode) != exist.end()) {
                        dominated = true;
                        break;
                    }
                }
            }
            if (!dominated) kept.push_back(pats[i]);
        }
        pats.swap(kept);
    }

    // Sort patterns by decreasing size (more constrained first)
    void sortPatterns() {
        sort(pats.begin(), pats.end(), [](const Pattern& a, const Pattern& b){
            if (a.pos.size() != b.pos.size()) return a.pos.size() > b.pos.size();
            return a.pos < b.pos;
        });
    }

    bool applyPattern(const Pattern& P) {
        size_t before = stackPos.size();
        for (size_t i=0;i<P.pos.size();i++){
            int p = P.pos[i];
            int l = (int)P.let[i];
            int cur = assign[p];
            if (cur == -1) {
                assign[p] = l;
                stackPos.push_back(p);
                assignedCount++;
            } else if (cur != l) {
                // conflict: revert changes of this apply
                for (size_t t=stackPos.size(); t>before; t--) {
                    int pos = stackPos[t-1];
                    assign[pos] = -1;
                    assignedCount--;
                }
                stackPos.resize(before);
                return false;
            }
        }
        return true;
    }

    void revertTo(size_t before) {
        for (size_t t=stackPos.size(); t>before; t--) {
            int pos = stackPos[t-1];
            assign[pos] = -1;
            assignedCount--;
        }
        stackPos.resize(before);
    }

    void dfs(int start, int selectedCount) {
        int m = (int)pats.size();
        for (int i=start;i<m;i++){
            size_t before = stackPos.size();
            if (!applyPattern(pats[i])) {
                continue;
            }
            // include this pattern
            long double term = pow4[assignedCount];
            if ((selectedCount % 2) == 0) ans += term; else ans -= term;
            dfs(i+1, selectedCount+1);
            revertTo(before);
        }
    }

    long double solve(int compPositionsCount) {
        // Optional pruning (may be costly if many big patterns)
        removeDominated();
        // Optional: if after pruning no patterns
        if (pats.empty()) return 0.0L;
        // Sort patterns by decreasing k to get early conflicts
        sortPatterns();
        // Initialize assignment array (local to this component)
        assign.assign(compPositionsCount, -1);
        stackPos.clear();
        assignedCount = 0;
        ans = 0.0L;
        dfs(0, 0);
        return ans;
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<vector<pair<int,int>>> raw(m); // pairs of (pos, letterCode)
    vector<char> used(n, 0);
    bool hasEmptyPattern = false;
    for (int i=0;i<m;i++){
        string s; cin >> s;
        for (int j=0;j<n;j++){
            int c = letterCode(s[j]);
            if (c != -1) {
                raw[i].push_back({j, c});
                used[j] = 1;
            }
        }
        if (raw[i].empty()) hasEmptyPattern = true;
    }

    if (hasEmptyPattern) {
        cout.setf(std::ios::fixed); cout<<setprecision(15)<<1.0<<"\n";
        return 0;
    }

    // Compress positions that appear at least once
    vector<int> mapPos(n, -1);
    int P = 0;
    for (int j=0;j<n;j++){
        if (used[j]) mapPos[j] = P++;
    }

    // Deduplicate patterns (identical assignments)
    unordered_map<vector<int>, int, VecHash> uniqMap; // code vector -> index in patterns
    uniqMap.reserve(m*2+7);
    vector<Pattern> patterns;
    patterns.reserve(m);
    vector<vector<int>> pos_to_patterns(P);
    for (int i=0;i<m;i++){
        auto &v = raw[i];
        if (v.empty()) continue; // handled above
        sort(v.begin(), v.end()); // sort by original pos
        vector<int> code; code.reserve(v.size());
        Pattern pat;
        for (auto &pr : v){
            int gp = mapPos[pr.first];
            int lc = pr.second;
            code.push_back((gp<<2) | lc);
            pat.pos.push_back(gp);
            pat.let.push_back((unsigned char)lc);
        }
        // Already canonical order by sorted v -> pat.pos sorted.
        if (uniqMap.find(code) == uniqMap.end()){
            int idx = (int)patterns.size();
            uniqMap.emplace(code, idx);
            patterns.push_back(move(pat));
        }
    }

    int M = (int)patterns.size();
    if (M == 0) {
        cout.setf(std::ios::fixed); cout<<setprecision(15)<<0.0<<"\n";
        return 0;
    }

    // Build DSU over patterns connected by shared positions
    vector<vector<int>> posPatternList(P);
    for (int i=0;i<M;i++){
        for (int p : patterns[i].pos){
            posPatternList[p].push_back(i);
        }
    }
    DSU dsu(M);
    for (int p=0;p<P;p++){
        auto &L = posPatternList[p];
        for (size_t t=1;t<L.size();t++){
            dsu.unite(L[0], L[t]);
        }
    }

    // Group patterns by component
    unordered_map<int, vector<int>> compMap;
    compMap.reserve(M*2+7);
    for (int i=0;i<M;i++){
        compMap[dsu.find(i)].push_back(i);
    }

    // Precompute pow4 up to total distinct positions P
    vector<long double> pow4(P+1, 1.0L);
    for (int i=1;i<=P;i++) pow4[i] = pow4[i-1] * (1.0L/4.0L);

    // Solve each component independently and combine: 1 - prod(1 - p_comp)
    long double prod = 1.0L;
    vector<int> g2l(P, -1);
    vector<int> touched;
    touched.reserve(P);

    for (auto &kv : compMap){
        auto &idxs = kv.second;
        // Collect distinct positions in this component
        vector<int> compPos;
        compPos.reserve(64);
        for (int id : idxs){
            for (int gp : patterns[id].pos){
                if (g2l[gp] == -1) {
                    g2l[gp] = (int)compPos.size();
                    compPos.push_back(gp);
                    touched.push_back(gp);
                }
            }
        }
        int Pc = (int)compPos.size();

        // Build local patterns for this component
        vector<Pattern> compPats;
        compPats.reserve(idxs.size());
        for (int id : idxs){
            Pattern np;
            np.pos.reserve(patterns[id].pos.size());
            np.let.reserve(patterns[id].let.size());
            for (size_t t=0;t<patterns[id].pos.size();t++){
                int gp = patterns[id].pos[t];
                int lp = g2l[gp];
                np.pos.push_back(lp);
                np.let.push_back(patterns[id].let[t]);
            }
            // sort by local pos (should already be sorted globally but ensure)
            vector<int> order(np.pos.size());
            iota(order.begin(), order.end(), 0);
            sort(order.begin(), order.end(), [&](int a, int b){ return np.pos[a] < np.pos[b]; });
            vector<int> newpos; newpos.reserve(np.pos.size());
            vector<unsigned char> newlet; newlet.reserve(np.let.size());
            for (int q : order){
                newpos.push_back(np.pos[q]);
                newlet.push_back(np.let[q]);
            }
            np.pos.swap(newpos);
            np.let.swap(newlet);
            compPats.push_back(move(np));
        }

        // Clear g2l marks for next component
        for (int gp : touched) g2l[gp] = -1;
        touched.clear();

        // Solve component
        CompSolver solver(compPats, pow4);
        long double pc = solver.solve(Pc); // union probability within component
        prod *= (1.0L - pc);
    }

    long double answer = 1.0L - prod;
    cout.setf(std::ios::fixed);
    cout << setprecision(15) << (double)answer << "\n";
    return 0;
}