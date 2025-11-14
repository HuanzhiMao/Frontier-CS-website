#include <bits/stdc++.h>
using namespace std;

static inline int ch2i(char c){ return c - 'A'; } // 'A'..'H' -> 0..7

uint64_t encode_str(const string& s, int l, int start){
    uint64_t code = 0;
    for(int i=0;i<l;i++){
        code = (code<<3) | (uint64_t)ch2i(s[start+i]);
    }
    return code;
}
uint64_t encode_prefix(const string& s, int l){
    return encode_str(s, l, 0);
}
uint64_t encode_suffix(const string& s, int l){
    return encode_str(s, l, (int)s.size()-l);
}

int N, M;
vector<string> reads;

const int MIN_OVL = 3;
const int MAX_OVL = 12;

// prefix_maps[l][code] -> vector of read indices having prefix of length l == code
// suffix_maps[l][code] -> vector of read indices having suffix of length l == code
vector<unordered_map<uint64_t, vector<int>>> prefix_maps, suffix_maps;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

struct Candidate {
    string row;
    vector<uint64_t> bits;
    int cov = 0;
};

bool cyclic_contains(const string& T, const string& s){
    // T length N, s length <=12
    int n = (int)T.size();
    int m = (int)s.size();
    // naive search on T+T
    for(int start=0; start<n; start++){
        int k=0;
        while(k<m && T[(start+k)%n]==s[k]) k++;
        if(k==m) return true;
    }
    return false;
}

string repeat_to_N(const string& s, int N){
    if(s.empty()) return string(N,'A');
    string t; t.reserve(N);
    for(int i=0;i<N;i++) t.push_back(s[i % s.size()]);
    return t;
}

pair<int,string> right_extension(const string& cur){
    // returns appended length and appended string
    for(int l=min(MAX_OVL, (int)cur.size()); l>=MIN_OVL; --l){
        uint64_t key = encode_suffix(cur, l);
        auto it = prefix_maps[l].find(key);
        if(it == prefix_maps[l].end()) continue;
        const auto& vec = it->second;
        int bestIdx = -1;
        int bestGain = -1;
        // choose candidate with maximum gain; tiebreak random
        for(int idx: vec){
            const string& r = reads[idx];
            int gain = (int)r.size() - l;
            if(gain <= 0) continue;
            if(gain > bestGain){
                bestGain = gain;
                bestIdx = idx;
            } else if(gain == bestGain && uniform_int_distribution<int>(0,1)(rng)){
                bestIdx = idx;
            }
        }
        if(bestIdx != -1){
            const string& r = reads[bestIdx];
            return {bestGain, r.substr(l)};
        }
    }
    return {0, ""};
}

pair<int,string> left_extension(const string& cur){
    // returns prepended length and string to prepend
    for(int l=min(MAX_OVL, (int)cur.size()); l>=MIN_OVL; --l){
        uint64_t key = encode_prefix(cur, l);
        auto it = suffix_maps[l].find(key);
        if(it == suffix_maps[l].end()) continue;
        const auto& vec = it->second;
        int bestIdx = -1;
        int bestGain = -1;
        for(int idx: vec){
            const string& r = reads[idx];
            int gain = (int)r.size() - l;
            if(gain <= 0) continue;
            if(gain > bestGain){
                bestGain = gain;
                bestIdx = idx;
            } else if(gain == bestGain && uniform_int_distribution<int>(0,1)(rng)){
                bestIdx = idx;
            }
        }
        if(bestIdx != -1){
            const string& r = reads[bestIdx];
            return {bestGain, r.substr(0, (int)r.size()-l)};
        }
    }
    return {0, ""};
}

string extend_right_until(const string& seed, int target){
    string cur = seed;
    int tries = 0;
    while((int)cur.size() < target && tries < 100){
        tries++;
        auto pr = right_extension(cur);
        if(pr.first <= 0) break;
        cur += pr.second;
        if((int)cur.size() > target + 12) break;
    }
    return cur;
}

string extend_left_until(const string& seed, int target){
    string cur = seed;
    int tries = 0;
    while((int)cur.size() < target && tries < 100){
        tries++;
        auto pr = left_extension(cur);
        if(pr.first <= 0) break;
        cur = pr.second + cur;
        if((int)cur.size() > target + 12) break;
    }
    return cur;
}

string extend_both_until(const string& seed, int target){
    string cur = seed;
    int tries = 0;
    while((int)cur.size() < target && tries < 120){
        tries++;
        auto rr = right_extension(cur);
        auto ll = left_extension(cur);
        if(rr.first<=0 && ll.first<=0) break;
        if(rr.first >= ll.first){
            cur += rr.second;
        }else{
            cur = ll.second + cur;
        }
        if((int)cur.size() > target + 12) break;
    }
    return cur;
}

vector<string> generate_markov_candidates(int k, int count){
    unordered_map<uint64_t, array<int,8>> trans;
    vector<uint64_t> seeds;
    // collect transitions
    for(const string& r: reads){
        if((int)r.size() <= k) continue;
        for(int i=0;i + k < (int)r.size(); ++i){
            uint64_t key = encode_str(r, k, i);
            int nx = ch2i(r[i+k]);
            auto& arr = trans[key];
            arr[nx] += 1;
        }
        // also collect seeds
        for(int i=0;i + k <= (int)r.size(); ++i){
            uint64_t key = encode_str(r, k, i);
            seeds.push_back(key);
        }
    }
    vector<pair<uint64_t,int>> seedScore;
    seedScore.reserve(trans.size());
    for(auto& kv: trans){
        int sum = 0;
        for(int c=0;c<8;c++) sum += kv.second[c];
        seedScore.emplace_back(kv.first, sum);
    }
    sort(seedScore.begin(), seedScore.end(), [](auto& a, auto& b){return a.second > b.second;});
    vector<string> result;
    if(seedScore.empty()){
        // fall back: random rows
        for(int t=0;t<count;t++){
            string row(N,'A');
            for(int i=0;i<N;i++) row[i] = 'A' + (rng()%8);
            result.push_back(row);
        }
        return result;
    }
    uniform_int_distribution<int> pickTop(0, max(0, (int)min<size_t>(seedScore.size(), 200) - 1));
    for(int t=0;t<count;t++){
        uint64_t state = seedScore[pickTop(rng)].first;
        string row;
        row.reserve(N);
        // decode state into k chars
        uint64_t tmp = state;
        char buf[16];
        for(int i=k-1;i>=0;i--){
            buf[i] = char('A' + (tmp & 7));
            tmp >>= 3;
        }
        for(int i=0;i<k;i++) row.push_back(buf[i]);
        for(int i=k;i<N;i++){
            auto it = trans.find(state);
            int nxt;
            if(it==trans.end()){
                nxt = rng()%8;
            }else{
                auto& arr = it->second;
                int bestc = -1, bestv = -1;
                for(int c=0;c<8;c++){
                    if(arr[c] > bestv){
                        bestv = arr[c]; bestc = c;
                    }
                }
                if(bestv <= 0) nxt = rng()%8; else nxt = bestc;
            }
            row.push_back(char('A'+nxt));
            // update state: shift left by 3 bits and add nxt; keep k letters
            state = ((state << 3) & ((1ULL<<(3*k))-1)) | (uint64_t)nxt;
        }
        result.push_back(row);
    }
    return result;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    if(!(cin>>N>>M)){
        return 0;
    }
    reads.resize(M);
    for(int i=0;i<M;i++) cin>>reads[i];

    prefix_maps.assign(MAX_OVL+1, {});
    suffix_maps.assign(MAX_OVL+1, {});

    for(int i=0;i<M;i++){
        const string& r = reads[i];
        int L = (int)r.size();
        for(int l=MIN_OVL;l<=min(MAX_OVL, L); ++l){
            uint64_t pre = encode_prefix(r, l);
            uint64_t suf = encode_suffix(r, l);
            prefix_maps[l][pre].push_back(i);
            suffix_maps[l][suf].push_back(i);
        }
    }

    // generate candidate rows
    unordered_set<string> uniq;
    vector<string> candRows;
    candRows.reserve(512);

    // select seeds: indices sorted by length desc
    vector<int> idx(M);
    iota(idx.begin(), idx.end(), 0);
    sort(idx.begin(), idx.end(), [&](int a, int b){
        if((int)reads[a].size() != (int)reads[b].size())
            return reads[a].size() > reads[b].size();
        return a < b;
    });
    int seedLimit = min(M, 140); // number of seeds to try

    // 1) Assembly-based candidates
    for(int t=0; t<seedLimit; ++t){
        int i = idx[t];
        const string& seed = reads[i];
        // extend both sides
        string cur = extend_both_until(seed, N+12);
        int L = (int)cur.size();
        if(L < N){
            string row = repeat_to_N(cur, N);
            if(uniq.insert(row).second) candRows.push_back(row);
        }else{
            // add last window and center window
            int start1 = max(0, L - N);
            int start2 = max(0, (L - N)/2);
            string row1 = cur.substr(start1, N);
            if(uniq.insert(row1).second) candRows.push_back(row1);
            string row2 = cur.substr(start2, N);
            if(uniq.insert(row2).second) candRows.push_back(row2);
        }
        if((int)candRows.size() > 320) break;
    }

    // 2) Right/Left-only extensions for more diversity
    for(int t=0; t<min(seedLimit, 80); ++t){
        int i = idx[t];
        const string& seed = reads[i];
        string curR = extend_right_until(seed, N+12);
        if((int)curR.size() < N){
            string row = repeat_to_N(curR, N);
            if(uniq.insert(row).second) candRows.push_back(row);
        }else{
            string row = curR.substr((int)curR.size()-N, N);
            if(uniq.insert(row).second) candRows.push_back(row);
        }
        string curL = extend_left_until(seed, N+12);
        if((int)curL.size() < N){
            string row = repeat_to_N(curL, N);
            if(uniq.insert(row).second) candRows.push_back(row);
        }else{
            string row = curL.substr(0, N);
            if(uniq.insert(row).second) candRows.push_back(row);
        }
        if((int)candRows.size() > 420) break;
    }

    // 3) Periodic repeats of long seeds
    for(int t=0; t<min(seedLimit, 40); ++t){
        int i = idx[t];
        string row = repeat_to_N(reads[i], N);
        if(uniq.insert(row).second) candRows.push_back(row);
    }

    // 4) Markov-based generation
    auto markovRows = generate_markov_candidates(3, 80);
    for(auto& row: markovRows){
        if(uniq.insert(row).second) candRows.push_back(row);
        if((int)candRows.size() > 500) break;
    }

    if(candRows.empty()){
        // fallback: random rows
        for(int i=0;i<max(20,N);++i){
            string row(N,'A');
            for(int j=0;j<N;j++) row[j] = 'A' + (rng()%8);
            if(uniq.insert(row).second) candRows.push_back(row);
        }
    }

    // Precompute coverage bitsets for candidates
    int W = (M + 63) / 64;
    vector<Candidate> candidates;
    candidates.reserve(candRows.size());

    for(const string& row : candRows){
        Candidate cnd;
        cnd.row = row;
        cnd.bits.assign(W, 0ULL);
        int covcnt = 0;
        for(int i=0;i<M;i++){
            if(cyclic_contains(row, reads[i])){
                cnd.bits[i>>6] |= (1ULL << (i & 63));
                covcnt++;
            }
        }
        cnd.cov = covcnt;
        candidates.push_back(move(cnd));
    }

    // Greedy set cover to select up to N rows
    vector<uint64_t> covered(W, 0ULL);
    vector<string> outputRows;
    vector<char> used(candidates.size(), 0);

    for(int r=0;r<N;r++){
        int bestIdx = -1;
        int bestGain = -1;
        for(int i=0;i<(int)candidates.size();++i){
            if(used[i]) continue;
            int gain = 0;
            for(int w=0; w<W; ++w){
                uint64_t x = candidates[i].bits[w] & (~covered[w]);
                gain += __builtin_popcountll(x);
            }
            if(gain > bestGain){
                bestGain = gain;
                bestIdx = i;
            }
        }
        if(bestIdx == -1){
            // no candidates? use random
            string row(N,'A');
            for(int j=0;j<N;j++) row[j] = 'A' + (rng()%8);
            outputRows.push_back(row);
        }else{
            used[bestIdx] = 1;
            for(int w=0; w<W; ++w){
                covered[w] |= candidates[bestIdx].bits[w];
            }
            outputRows.push_back(candidates[bestIdx].row);
        }
    }

    // Output
    for(int i=0;i<N;i++){
        cout << outputRows[i] << "\n";
    }
    return 0;
}