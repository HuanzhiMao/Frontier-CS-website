#include <bits/stdc++.h>
using namespace std;

static inline unsigned long long rng64() {
    static unsigned long long x = 88172645463393265ull;
    x ^= x << 7;
    x ^= x >> 9;
    return x;
}
static inline int randint(int l, int r){ // inclusive
    return (int)(rng64() % (unsigned long long)(r - l + 1)) + l;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N, M;
    if(!(cin >> N >> M)) return 0;
    vector<string> ss(M);
    for(int i=0;i<M;i++) cin >> ss[i];

    // Convert strings to 0..7 ints
    vector<vector<uint8_t>> S(M);
    vector<int> L(M);
    for(int i=0;i<M;i++){
        L[i] = (int)ss[i].size();
        S[i].resize(L[i]);
        for(int j=0;j<L[i];j++) S[i][j] = (uint8_t)(ss[i][j]-'A');
    }

    const int ITER_LIMIT = 200; // upper bound, we'll stop earlier if converged or time
    const auto t_start = chrono::steady_clock::now();
    const double TIME_LIMIT = 1.9; // seconds
    auto elapsed = [&](){
        return chrono::duration<double>(chrono::steady_clock::now() - t_start).count();
    };

    // Initialize grid randomly
    vector<vector<uint8_t>> grid(N, vector<uint8_t>(N));
    for(int i=0;i<N;i++) for(int j=0;j<N;j++) grid[i][j] = (uint8_t)randint(0,7);

    // Structures for chosen alignments per string
    vector<uint8_t> bestDir(M, 0); // 0: horizontal, 1: vertical
    vector<uint8_t> bestRC(M, 0);  // row or col index
    vector<uint8_t> bestStart(M, 0);

    // Counts per cell per letter
    vector<array<int,8>> counts(N*N);
    auto idx = [N](int r, int c){ return r*N + c; };

    // Begin iterations
    for(int it=0; it<ITER_LIMIT; it++){
        if(elapsed() > TIME_LIMIT) break;

        // For each string, find best alignment relative to current grid
        for(int i=0;i<M;i++){
            int k = L[i];
            const uint8_t* s = S[i].data();

            int best_mism = INT_MAX;
            uint8_t bd=0, brc=0, bstart=0;

            // Horizontal
            for(int r=0;r<N;r++){
                for(int st=0;st<N;st++){
                    int mism = 0;
                    int c = st;
                    for(int p=0;p<k;p++){
                        if(grid[r][c] != s[p]) mism++;
                        c++; if(c==N) c=0;
                        if(mism >= best_mism) break;
                    }
                    if(mism < best_mism){
                        best_mism = mism; bd = 0; brc = (uint8_t)r; bstart = (uint8_t)st;
                        if(best_mism==0) goto found_best0; // early exit for this string
                    }
                }
            }
            // Vertical
            for(int c=0;c<N;c++){
                for(int st=0;st<N;st++){
                    int mism = 0;
                    int r = st;
                    for(int p=0;p<k;p++){
                        if(grid[r][c] != s[p]) mism++;
                        r++; if(r==N) r=0;
                        if(mism >= best_mism) break;
                    }
                    if(mism < best_mism){
                        best_mism = mism; bd = 1; brc = (uint8_t)c; bstart = (uint8_t)st;
                        if(best_mism==0) goto found_best0;
                    }
                }
            }
            found_best0:
            bestDir[i] = bd; bestRC[i] = brc; bestStart[i] = bstart;
        }

        // Reset counts
        for(int i=0;i<N*N;i++){
            for(int c=0;c<8;c++) counts[i][c] = 0;
        }

        // Accumulate counts according to chosen alignments
        for(int i=0;i<M;i++){
            int k = L[i];
            const uint8_t* s = S[i].data();
            if(bestDir[i] == 0){
                int r = bestRC[i];
                int c = bestStart[i];
                for(int p=0;p<k;p++){
                    counts[idx(r,c)][s[p]]++;
                    c++; if(c==N) c=0;
                }
            }else{
                int c = bestRC[i];
                int r = bestStart[i];
                for(int p=0;p<k;p++){
                    counts[idx(r,c)][s[p]]++;
                    r++; if(r==N) r=0;
                }
            }
        }

        // Update grid by majority vote; prefer keeping current letter on ties
        int changed = 0;
        for(int r=0;r<N;r++){
            for(int c=0;c<N;c++){
                int id = idx(r,c);
                int cur = grid[r][c];
                int bestChar = cur;
                int bestCnt = counts[id][bestChar];
                for(int ch=0; ch<8; ch++){
                    int cnt = counts[id][ch];
                    if(cnt > bestCnt){
                        bestCnt = cnt;
                        bestChar = ch;
                    }
                }
                if(bestChar != cur){
                    grid[r][c] = (uint8_t)bestChar;
                    changed++;
                }
            }
        }
        if(changed == 0) break;
    }

    // Output final grid
    for(int r=0;r<N;r++){
        string out;
        out.resize(N);
        for(int c=0;c<N;c++){
            out[c] = char('A' + grid[r][c]);
        }
        cout << out << "\n";
    }
    return 0;
}