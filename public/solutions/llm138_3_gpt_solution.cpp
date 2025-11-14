#include <bits/stdc++.h>
using namespace std;

static const int MAXNM = 400;
static const int CH = 62;
static const int OP_LIMIT = 400000;

int n, m, k;

int chIdx(char c){
    if(c>='a'&&c<='z') return c-'a';
    if(c>='A'&&c<='Z') return 26 + (c-'A');
    return 52 + (c-'0');
}

struct Formula {
    int np, mp;
    vector<string> a;
    array<int,CH> cnt;
};

struct Use {
    int fid;
    int x, y; // 0-based
    bitset<MAXNM> mask;
    array<int,CH> d;
    int score;
};

vector<string> cur, tar;
vector<Formula> F;
vector<tuple<int,int,int>> ops; // (op, x, y)

bool tooManyOps() {
    return (int)ops.size() > OP_LIMIT;
}

void add_op(int op, int x, int y){
    if(!tooManyOps())
        ops.emplace_back(op, x, y);
}

void swapRight(int x, int y){ // move (x,y) <-> (x,y+1), x,y 1-based, y<m
    add_op(-1,x,y);
    if(tooManyOps()) return;
    swap(cur[x-1][y-1], cur[x-1][y]);
}
void swapLeft(int x, int y){ // move (x,y) <-> (x,y-1), y>=2
    add_op(-2,x,y);
    if(tooManyOps()) return;
    swap(cur[x-1][y-1], cur[x-1][y-2]);
}
void swapUp(int x, int y){ // (x,y) <-> (x-1,y), x>=2
    add_op(-3,x,y);
    if(tooManyOps()) return;
    swap(cur[x-1][y-1], cur[x-2][y-1]);
}
void swapDown(int x, int y){ // (x,y) <-> (x+1,y), x<n
    add_op(-4,x,y);
    if(tooManyOps()) return;
    swap(cur[x-1][y-1], cur[x][y-1]);
}

array<int,CH> countsOf(const vector<string>& g){
    array<int,CH> res{}; res.fill(0);
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            res[chIdx(g[i][j])]++;
        }
    }
    return res;
}

bool vecZero(const array<int,CH>& v){
    for(int i=0;i<CH;i++) if(v[i]!=0) return false;
    return true;
}

array<int,CH> vecSub(const array<int,CH>& a, const array<int,CH>& b){
    array<int,CH> r;
    for(int i=0;i<CH;i++) r[i] = a[i]-b[i];
    return r;
}

array<int,CH> vecAdd(const array<int,CH>& a, const array<int,CH>& b){
    array<int,CH> r;
    for(int i=0;i<CH;i++) r[i] = a[i]+b[i];
    return r;
}

bool vecEq(const array<int,CH>& a, const array<int,CH>& b){
    for(int i=0;i<CH;i++) if(a[i]!=b[i]) return false;
    return true;
}

int L1(const array<int,CH>& a){
    long long s=0;
    for(int i=0;i<CH;i++) s += llabs((long long)a[i]);
    if(s>INT_MAX) return INT_MAX;
    return (int)s;
}

string vecKey(const array<int,CH>& v){
    string s;
    s.resize(CH*2);
    for(int i=0;i<CH;i++){
        int val = v[i];
        int16_t t = (int16_t)val;
        uint16_t u = (uint16_t)t;
        s[2*i] = char(u & 0xFF);
        s[2*i+1] = char((u>>8) & 0xFF);
    }
    return s;
}

int cellIdx(int r, int c){ // 0-based
    return r*m + c;
}

void buildUses(const array<int,CH>& diff, vector<Use>& uses){
    uses.clear();
    // Precompute formula counts
    for(auto &form : F){
        form.cnt.fill(0);
        for(int i=0;i<form.np;i++)
            for(int j=0;j<form.mp;j++)
                form.cnt[chIdx(form.a[i][j])]++;
    }
    // Enumerate placements
    for(int fid=0; fid<k; fid++){
        int np = F[fid].np, mp = F[fid].mp;
        for(int x=0; x+np<=n; x++){
            for(int y=0; y+mp<=m; y++){
                array<int,CH> sub{}; sub.fill(0);
                bitset<MAXNM> mask;
                for(int i=0;i<np;i++){
                    for(int j=0;j<mp;j++){
                        char ch = cur[x+i][y+j];
                        sub[chIdx(ch)]++;
                        mask.set(cellIdx(x+i,y+j));
                    }
                }
                Use u;
                u.fid = fid;
                u.x = x; u.y = y;
                u.mask = mask;
                for(int i=0;i<CH;i++){
                    u.d[i] = F[fid].cnt[i] - sub[i];
                }
                array<int,CH> resid = vecSub(diff, u.d);
                u.score = L1(resid);
                uses.push_back(u);
            }
        }
    }
}

bool selectPresets(const array<int,CH>& diff, const vector<Use>& uses, vector<int>& selIdx){
    selIdx.clear();
    if(uses.empty()) return false;
    // If any single use matches diff
    for(size_t i=0;i<uses.size();i++){
        bool ok = true;
        for(int c=0;c<CH;c++) if(uses[i].d[c]!=diff[c]) { ok=false; break; }
        if(ok){
            selIdx = {(int)i};
            return true;
        }
    }
    // choose top T by score
    int Tpair = min<int>((int)uses.size(), 1500);
    vector<int> ord(uses.size());
    iota(ord.begin(), ord.end(), 0);
    nth_element(ord.begin(), ord.begin()+Tpair, ord.end(), [&](int a, int b){
        return uses[a].score < uses[b].score;
    });
    ord.resize(Tpair);
    // Map d -> list of indices
    unordered_map<string, vector<int>> mp;
    mp.reserve(Tpair*2);
    for(int idx : ord){
        string key = vecKey(uses[idx].d);
        mp[key].push_back(idx);
    }
    // Two-sum
    for(int idx : ord){
        array<int,CH> need = vecSub(diff, uses[idx].d);
        string key = vecKey(need);
        auto it = mp.find(key);
        if(it != mp.end()){
            for(int jdx : it->second){
                if(jdx == idx) continue;
                if((uses[idx].mask & uses[jdx].mask).any()) continue;
                selIdx = {idx, jdx};
                return true;
            }
        }
    }
    // Three-sum: pick smaller top
    int Ttri = min<int>((int)uses.size(), 220);
    nth_element(ord.begin(), ord.begin()+Ttri, ord.end(), [&](int a, int b){
        return uses[a].score < uses[b].score;
    });
    ord.resize(Ttri);
    unordered_map<string, vector<pair<int,int>>> pairMap;
    pairMap.reserve(Ttri*Ttri/4+1);
    for(int i=0;i<Ttri;i++){
        for(int j=i+1;j<Ttri;j++){
            int a = ord[i], b = ord[j];
            if((uses[a].mask & uses[b].mask).any()) continue;
            array<int,CH> sum;
            for(int c=0;c<CH;c++) sum[c] = uses[a].d[c] + uses[b].d[c];
            string key = vecKey(sum);
            pairMap[key].push_back({a,b});
        }
    }
    for(int idx : ord){
        array<int,CH> need = vecSub(diff, uses[idx].d);
        string key = vecKey(need);
        auto it = pairMap.find(key);
        if(it != pairMap.end()){
            for(auto pr : it->second){
                int a = pr.first, b = pr.second;
                if((uses[idx].mask & uses[a].mask).any()) continue;
                if((uses[idx].mask & uses[b].mask).any()) continue;
                selIdx = {idx, a, b};
                return true;
            }
        }
    }
    // Four-sum: split top into two groups
    int Tfour = min<int>((int)uses.size(), 120);
    nth_element(ord.begin(), ord.begin()+Tfour, ord.end(), [&](int a, int b){
        return uses[a].score < uses[b].score;
    });
    ord.resize(Tfour);
    vector<int> A, B;
    for(int i=0;i<Tfour;i++){
        if(i%2==0) A.push_back(ord[i]);
        else B.push_back(ord[i]);
    }
    unordered_map<string, vector<pair<int,int>>> sumA, sumB;
    sumA.reserve(A.size()*A.size()/4+1);
    sumB.reserve(B.size()*B.size()/4+1);
    // Pairs in A
    for(size_t i=0;i<A.size();i++){
        for(size_t j=i+1;j<A.size();j++){
            int a = A[i], b = A[j];
            if((uses[a].mask & uses[b].mask).any()) continue;
            array<int,CH> sum;
            for(int c=0;c<CH;c++) sum[c] = uses[a].d[c] + uses[b].d[c];
            string key = vecKey(sum);
            sumA[key].push_back({a,b});
        }
    }
    // Pairs in B
    for(size_t i=0;i<B.size();i++){
        for(size_t j=i+1;j<B.size();j++){
            int a = B[i], b = B[j];
            if((uses[a].mask & uses[b].mask).any()) continue;
            array<int,CH> sum;
            for(int c=0;c<CH;c++) sum[c] = uses[a].d[c] + uses[b].d[c];
            string key = vecKey(sum);
            sumB[key].push_back({a,b});
        }
    }
    for(auto &it : sumA){
        const string &key = it.first;
        // need sumB to be diff - sumA
        // decode key to array<int>
        array<int,CH> sumA_vec;
        for(int c=0;c<CH;c++){
            uint8_t lo = (uint8_t)key[2*c];
            uint8_t hi = (uint8_t)key[2*c+1];
            uint16_t us = ((uint16_t)hi<<8) | lo;
            int16_t ss = (int16_t)us;
            sumA_vec[c] = (int)ss;
        }
        array<int,CH> need = vecSub(diff, sumA_vec);
        string keyNeed = vecKey(need);
        auto it2 = sumB.find(keyNeed);
        if(it2 == sumB.end()) continue;
        for(auto prA : it.second){
            for(auto prB : it2->second){
                int a1=prA.first, a2=prA.second, b1=prB.first, b2=prB.second;
                bitset<MAXNM> msk = uses[a1].mask | uses[a2].mask;
                if((msk & uses[b1].mask).any()) continue;
                if((msk & uses[b2].mask).any()) continue;
                selIdx = {a1, a2, b1, b2};
                return true;
            }
        }
    }
    return false;
}

bool applySelected(const vector<Use>& uses, const vector<int>& selIdx){
    // Apply presets in the given order
    for(int idx : selIdx){
        int fid = uses[idx].fid;
        int x = uses[idx].x;
        int y = uses[idx].y;
        add_op(fid+1, x+1, y+1); // op numbering: 1..k
        if(tooManyOps()) return false;
        int np = F[fid].np, mp = F[fid].mp;
        for(int i=0;i<np;i++){
            for(int j=0;j<mp;j++){
                cur[x+i][y+j] = F[fid].a[i][j];
            }
        }
    }
    return true;
}

bool reorderToTarget(){
    // Place tiles one by one
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            if(i==n-1 && j==m-1) break;
            char need = tar[i][j];
            // Find need in non-fixed region
            int fx=-1, fy=-1;
            for(int r=i;r<n;r++){
                int cj = (r==i? j: 0);
                for(int c=cj;c<m;c++){
                    if(cur[r][c]==need){
                        fx=r; fy=c; break;
                    }
                }
                if(fx!=-1) break;
            }
            if(fx==-1){
                // Should not happen if counts match
                return false;
            }
            // Move (fx,fy) to (i,j)
            int x = fx, y = fy;
            // Ensure we do not exceed op limit
            while(y < j){
                swapRight(x+1, y+1);
                if(tooManyOps()) return false;
                y++;
            }
            while(x > i){
                swapUp(x+1, y+1);
                if(tooManyOps()) return false;
                x--;
            }
            while(y > j){
                swapLeft(x+1, y+1);
                if(tooManyOps()) return false;
                y--;
            }
            // Now cur[i][j] == need
        }
    }
    // Optionally ensure last cell matches
    return cur == tar;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m >> k;
    cur.resize(n);
    for(int i=0;i<n;i++) cin >> cur[i];
    tar.resize(n);
    for(int i=0;i<n;i++) cin >> tar[i];
    F.resize(k);
    for(int id=0; id<k; id++){
        cin >> F[id].np >> F[id].mp;
        F[id].a.resize(F[id].np);
        for(int i=0;i<F[id].np;i++){
            cin >> F[id].a[i];
        }
    }
    auto cntCur = countsOf(cur);
    auto cntTar = countsOf(tar);
    auto diff = vecSub(cntTar, cntCur);
    vector<Use> uses;
    vector<int> selIdx;
    if(!vecZero(diff)){
        buildUses(diff, uses);
        bool ok = selectPresets(diff, uses, selIdx);
        if(!ok){
            cout << -1 << "\n";
            return 0;
        }
        // Verify sum of selected equals diff and masks disjoint
        array<int,CH> sum{}; sum.fill(0);
        bitset<MAXNM> allmask;
        bool disjoint = true;
        for(int idx : selIdx){
            sum = vecAdd(sum, uses[idx].d);
            if((allmask & uses[idx].mask).any()) disjoint=false;
            allmask |= uses[idx].mask;
        }
        if(!vecEq(sum, diff) || !disjoint){
            cout << -1 << "\n";
            return 0;
        }
        if(!applySelected(uses, selIdx)){
            cout << -1 << "\n";
            return 0;
        }
        cntCur = countsOf(cur);
        diff = vecSub(cntTar, cntCur);
        if(!vecZero(diff)){
            cout << -1 << "\n";
            return 0;
        }
    }
    bool ok = reorderToTarget();
    if(!ok || tooManyOps()){
        cout << -1 << "\n";
        return 0;
    }
    cout << (int)ops.size() << "\n";
    for(auto &t : ops){
        int op,x,y;
        tie(op,x,y)=t;
        cout << op << " " << x << " " << y << "\n";
    }
    return 0;
}