#include <bits/stdc++.h>
using namespace std;

struct Op {
    int op, x, y;
};

int charIndex(char c){
    if ('a' <= c && c <= 'z') return c - 'a';
    if ('A' <= c && c <= 'Z') return 26 + (c - 'A');
    if ('0' <= c && c <= '9') return 52 + (c - '0');
    return -1;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m, k;
    if(!(cin >> n >> m >> k)) return 0;
    vector<string> cur(n), target(n);
    for(int i=0;i<n;i++) cin >> cur[i];
    for(int i=0;i<n;i++) cin >> target[i];

    struct Preset { int r, c; vector<string> a; vector<int> cnt; };
    vector<Preset> presets(k);
    for(int p=0;p<k;p++){
        int r,c; cin >> r >> c;
        presets[p].r = r; presets[p].c = c;
        presets[p].a.resize(r);
        for(int i=0;i<r;i++) cin >> presets[p].a[i];
        presets[p].cnt.assign(62,0);
        for(int i=0;i<r;i++){
            for(int j=0;j<c;j++){
                int id = charIndex(presets[p].a[i][j]);
                if(id>=0) presets[p].cnt[id]++;
            }
        }
    }

    auto countBoard = [&](const vector<string>& b){
        vector<int> cnt(62,0);
        for(int i=0;i<n;i++)
            for(int j=0;j<m;j++)
                cnt[charIndex(b[i][j])]++;
        return cnt;
    };

    vector<int> cntCur = countBoard(cur);
    vector<int> cntTarget = countBoard(target);

    auto equalCounts = [&](){
        for(int i=0;i<62;i++) if(cntCur[i]!=cntTarget[i]) return false;
        return true;
    };

    vector<Op> ops;
    int presetOps = 0;
    const int MAX_PRESET_OPS = 400;
    const int MAX_OPS = 400000;

    auto buildPrefix = [&](const vector<string>& b){
        vector<vector<vector<int>>> pref(62, vector<vector<int>>(n+1, vector<int>(m+1,0)));
        for(int d=0; d<62; d++){
            for(int i=0;i<n;i++){
                int rowSum = 0;
                for(int j=0;j<m;j++){
                    rowSum += (charIndex(b[i][j])==d);
                    pref[d][i+1][j+1] = pref[d][i][j+1] + rowSum;
                }
            }
        }
        return pref;
    };

    auto getSubCnt = [&](const vector<vector<vector<int>>>& pref, int d, int x, int y, int r, int c){
        int x2 = x + r, y2 = y + c;
        return pref[d][x2][y2] - pref[d][x][y2] - pref[d][x2][y] + pref[d][x][y];
    };

    // Try to adjust counts using presets greedily
    if(!equalCounts() && k>0){
        for(int iter=0; iter<MAX_PRESET_OPS; iter++){
            if(equalCounts()) break;
            vector<int> D(62);
            int L1 = 0;
            for(int d=0; d<62; d++){ D[d] = cntTarget[d] - cntCur[d]; L1 += abs(D[d]); }
            auto pref = buildPrefix(cur);

            long long bestImprove = LLONG_MIN;
            long long bestDot = LLONG_MIN;
            int bestP = -1, bestX = -1, bestY = -1;
            int bestArea = INT_MAX;
            long long bestNextL1 = LLONG_MAX;

            for(int p=0;p<k;p++){
                int r = presets[p].r, c = presets[p].c;
                for(int x=0;x<=n-r;x++){
                    for(int y=0;y<=m-c;y++){
                        // compute delta
                        long long dot = 0;
                        long long l1Next = 0;
                        long long improve = 0;
                        // To compute l1Next and dot, we need Δ for each d
                        long long localImprove = 0;
                        long long localDot = 0;
                        // But computing Δ vector fully
                        long long tmpL1Next = 0;
                        for(int d=0; d<62; d++){
                            int subc = getSubCnt(pref, d, x, y, r, c);
                            int delta = presets[p].cnt[d] - subc;
                            localDot += 1LL*D[d]*delta;
                        }
                        // Compute l1Next
                        for(int d=0; d<62; d++){
                            int subc = getSubCnt(pref, d, x, y, r, c);
                            int delta = presets[p].cnt[d] - subc;
                            int newD = D[d] - delta;
                            tmpL1Next += llabs((long long)newD);
                        }
                        long long tmpImprove = (long long)L1 - tmpL1Next;
                        int area = r*c;

                        bool better = false;
                        if(tmpImprove > bestImprove) better = true;
                        else if(tmpImprove == bestImprove){
                            if(localDot > bestDot) better = true;
                            else if(localDot == bestDot){
                                if(tmpL1Next < bestNextL1) better = true;
                                else if(tmpL1Next == bestNextL1){
                                    if(area < bestArea) better = true;
                                }
                            }
                        }
                        if(better){
                            bestImprove = tmpImprove;
                            bestDot = localDot;
                            bestNextL1 = tmpL1Next;
                            bestP = p; bestX = x; bestY = y;
                            bestArea = area;
                        }
                    }
                }
            }

            if(bestP == -1) break;

            // If the best choice doesn't change anything (Δ==0), break
            bool anyChange = false;
            {
                for(int d=0; d<62; d++){
                    int subc = 0; // compute via prefix
                }
            }
            // compute Δ vector and apply
            vector<int> delta(62,0);
            for(int d=0; d<62; d++){
                int subc = getSubCnt(pref, d, bestX, bestY, presets[bestP].r, presets[bestP].c);
                delta[d] = presets[bestP].cnt[d] - subc;
                if(delta[d]!=0) anyChange = true;
            }
            if(!anyChange){
                // No preset can change anything further
                break;
            }
            // Apply preset on board
            for(int i=0;i<presets[bestP].r;i++){
                for(int j=0;j<presets[bestP].c;j++){
                    cur[bestX+i][bestY+j] = presets[bestP].a[i][j];
                }
            }
            // Update counts
            for(int d=0; d<62; d++) cntCur[d] += delta[d];
            // Record op
            ops.push_back({bestP+1, bestX+1, bestY+1});
            presetOps++;
            if((int)ops.size() > MAX_OPS){ cout << -1 << "\n"; return 0; }
            if(presetOps >= MAX_PRESET_OPS) break;
        }
    }

    if(!equalCounts()){
        cout << -1 << "\n";
        return 0;
    }

    // Now transform cur to target using only adjacent swaps within the unfixed area
    auto doSwapLeft = [&](int x, int y){ // swap (x,y) with (x,y-1), 0-based
        swap(cur[x][y], cur[x][y-1]);
        ops.push_back({-2, x+1, y+1});
    };
    auto doSwapUp = [&](int x, int y){ // swap (x,y) with (x-1,y)
        swap(cur[x][y], cur[x-1][y]);
        ops.push_back({-3, x+1, y+1});
    };

    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            char need = target[i][j];
            if(cur[i][j] == need) continue;
            int bi=-1, bj=-1;
            int bestDist = INT_MAX;
            for(int x=i;x<n;x++){
                for(int y=j;y<m;y++){
                    if(cur[x][y]==need){
                        int dist = (x - i) + (y - j);
                        if(dist < bestDist){
                            bestDist = dist;
                            bi=x; bj=y;
                            if(dist==0) break;
                        }
                    }
                }
                if(bestDist==0) break;
            }
            if(bi==-1){
                // Should not happen because counts match in remainder
                cout << -1 << "\n";
                return 0;
            }
            // Move (bi,bj) to (i,j): first left to column j, then up to row i
            while(bj > j){
                if((int)ops.size() >= MAX_OPS){ cout << -1 << "\n"; return 0; }
                doSwapLeft(bi, bj);
                bj--;
            }
            while(bi > i){
                if((int)ops.size() >= MAX_OPS){ cout << -1 << "\n"; return 0; }
                doSwapUp(bi, bj);
                bi--;
            }
        }
    }

    if((int)ops.size() > MAX_OPS){
        cout << -1 << "\n";
        return 0;
    }
    cout << ops.size() << "\n";
    for(auto &op : ops){
        cout << op.op << " " << op.x << " " << op.y << "\n";
    }
    return 0;
}