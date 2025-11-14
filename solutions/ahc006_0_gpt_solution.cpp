#include <bits/stdc++.h>
using namespace std;

struct Order {
    int a, b, c, d;
};

using P = pair<int,int>;

static inline int manh(const P& u, const P& v){
    return abs(u.first - v.first) + abs(u.second - v.second);
}

vector<int> greedyOrderFromStart(const P& start, const vector<P>& pts){
    int m = (int)pts.size();
    vector<int> order;
    order.reserve(m);
    vector<char> used(m, 0);
    P cur = start;
    for(int it=0; it<m; ++it){
        int best = -1;
        int bestd = INT_MAX;
        for(int i=0;i<m;i++){
            if(used[i]) continue;
            int d = manh(cur, pts[i]);
            if(d < bestd){
                bestd = d;
                best = i;
            }
        }
        used[best] = 1;
        order.push_back(best);
        cur = pts[best];
    }
    return order;
}

void twoOptOpen(const P& start, const vector<P>& pts, vector<int>& ord, bool anchoredEnd, const P& endAnchor){
    int m = (int)ord.size();
    if(m <= 2) return;
    bool improved = true;
    int iter = 0;
    while(improved && iter < 10){
        improved = false;
        ++iter;
        for(int i=0;i<m-1;i++){
            for(int j=i+1;j<m;j++){
                // prev and next points
                P prev = (i==0) ? start : pts[ord[i-1]];
                bool hasNext = (j < m-1) || anchoredEnd;
                P next;
                if(j < m-1) next = pts[ord[j+1]];
                else next = endAnchor;
                
                long long before = (long long)manh(prev, pts[ord[i]]);
                long long after  = (long long)manh(prev, pts[ord[j]]);
                if(hasNext){
                    before += manh(pts[ord[j]], next);
                    after  += manh(pts[ord[i]], next);
                }
                if(after + 0 < before){ // strict improvement
                    reverse(ord.begin()+i, ord.begin()+j+1);
                    improved = true;
                }
            }
        }
    }
}

long long evalGreedy(const vector<Order>& orders, const vector<int>& subset){
    const P center = {400,400};
    int m = (int)subset.size();
    vector<P> picks; picks.reserve(m);
    vector<P> dels; dels.reserve(m);
    for(int idx: subset){
        const auto& o = orders[idx];
        picks.emplace_back(o.a, o.b);
        dels.emplace_back(o.c, o.d);
    }

    // pickup greedy
    vector<char> usedPick(m, 0), usedDel(m, 0);
    P cur = center;
    long long T = 0;
    for(int k=0;k<m;k++){
        int best=-1, bestd=INT_MAX;
        for(int i=0;i<m;i++){
            if(usedPick[i]) continue;
            int d = manh(cur, picks[i]);
            if(d < bestd){
                bestd = d; best = i;
            }
        }
        T += bestd;
        cur = picks[best];
        usedPick[best]=1;
    }
    for(int k=0;k<m;k++){
        int best=-1, bestd=INT_MAX;
        for(int i=0;i<m;i++){
            if(usedDel[i]) continue;
            int d = manh(cur, dels[i]);
            if(d < bestd){
                bestd = d; best = i;
            }
        }
        T += bestd;
        cur = dels[best];
        usedDel[best]=1;
    }
    T += manh(cur, center);
    return T;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    const int N = 1000;
    vector<Order> orders(N);
    for(int i=0;i<N;i++){
        int a,b,c,d;
        if(!(cin>>a>>b>>c>>d)) return 0;
        orders[i] = {a,b,c,d};
    }
    const P center = {400,400};

    // Generate pivots grid
    vector<int> xs = {0,200,400,600,800};
    vector<int> ys = {0,200,400,600,800};
    vector<P> pivots;
    for(int x: xs) for(int y: ys) pivots.emplace_back(x,y);

    // Candidate subsets
    long long bestT = (1LL<<60);
    vector<int> bestSubset;

    auto try_subset = [&](const vector<int>& subset){
        if((int)subset.size() < 50) return;
        vector<int> sub50(subset.begin(), subset.begin()+50);
        long long T = evalGreedy(orders, sub50);
        if(T < bestT){
            bestT = T;
            bestSubset = sub50;
        }
    };

    // Pivot pair combinations: score = dist(a, pA)+dist(c, pC)
    for(const auto& pA : pivots){
        for(const auto& pC : pivots){
            vector<pair<int,int>> v; v.reserve(N);
            for(int i=0;i<N;i++){
                const auto& o = orders[i];
                int s = manh({o.a,o.b}, pA) + manh({o.c,o.d}, pC);
                v.emplace_back(s, i);
            }
            nth_element(v.begin(), v.begin()+50, v.end());
            vector<int> subset;
            subset.reserve(50);
            for(int k=0;k<50;k++) subset.push_back(v[k].second);
            try_subset(subset);
        }
    }
    // Weighted center combinations
    vector<pair<int,int>> weights = {{1,1},{2,1},{1,2},{3,1},{1,3}};
    for(auto w: weights){
        int wa = w.first, wc = w.second;
        vector<pair<long long,int>> v; v.reserve(N);
        for(int i=0;i<N;i++){
            const auto& o = orders[i];
            long long s = 1LL*wa*manh({o.a,o.b}, center) + 1LL*wc*manh({o.c,o.d}, center);
            v.emplace_back(s, i);
        }
        nth_element(v.begin(), v.begin()+50, v.end());
        vector<int> subset;
        subset.reserve(50);
        for(int k=0;k<50;k++) subset.push_back(v[k].second);
        try_subset(subset);
    }
    // Deliveries-only pivot
    for(const auto& pC : pivots){
        vector<pair<int,int>> v; v.reserve(N);
        for(int i=0;i<N;i++){
            const auto& o = orders[i];
            int s = manh({o.c,o.d}, pC);
            v.emplace_back(s, i);
        }
        nth_element(v.begin(), v.begin()+50, v.end());
        vector<int> subset;
        subset.reserve(50);
        for(int k=0;k<50;k++) subset.push_back(v[k].second);
        try_subset(subset);
    }
    // Pickups-only pivot
    for(const auto& pA : pivots){
        vector<pair<int,int>> v; v.reserve(N);
        for(int i=0;i<N;i++){
            const auto& o = orders[i];
            int s = manh({o.a,o.b}, pA);
            v.emplace_back(s, i);
        }
        nth_element(v.begin(), v.begin()+50, v.end());
        vector<int> subset;
        subset.reserve(50);
        for(int k=0;k<50;k++) subset.push_back(v[k].second);
        try_subset(subset);
    }

    // Fallback if not found (shouldn't happen)
    if(bestSubset.empty()){
        vector<pair<int,int>> v; v.reserve(N);
        for(int i=0;i<N;i++){
            const auto& o = orders[i];
            int s = manh({o.a,o.b}, center) + manh({o.c,o.d}, center);
            v.emplace_back(s, i);
        }
        nth_element(v.begin(), v.begin()+50, v.end());
        for(int k=0;k<50;k++) bestSubset.push_back(v[k].second);
    }

    // Build final route with 2-opt refinements
    int m = 50;
    vector<P> pickPts; pickPts.reserve(m);
    vector<P> delPts; delPts.reserve(m);
    for(int idx: bestSubset){
        const auto& o = orders[idx];
        pickPts.emplace_back(o.a, o.b);
        delPts.emplace_back(o.c, o.d);
    }

    // Pickups: greedy from center
    vector<int> pickOrd = greedyOrderFromStart(center, pickPts);
    // Deliveries anchored to last pickup (after initial)
    P lastPick = pickPts[pickOrd.back()];
    vector<int> delOrd = greedyOrderFromStart(lastPick, delPts);
    // 2-opt deliveries with start=lastPick and end=center
    twoOptOpen(lastPick, delPts, delOrd, true, center);
    // After this, get first delivery point
    P firstDel = delPts[delOrd.front()];
    // 2-opt pickups with start=center and end=firstDel
    twoOptOpen(center, pickPts, pickOrd, true, firstDel);
    // Recompute lastPick after optimizing pickups
    lastPick = pickPts[pickOrd.back()];
    // Recompute deliveries from new lastPick and 2-opt again
    delOrd = greedyOrderFromStart(lastPick, delPts);
    twoOptOpen(lastPick, delPts, delOrd, true, center);

    // Build final route points
    vector<P> route;
    route.reserve(1 + m + m + 1);
    route.push_back(center);
    for(int id: pickOrd) route.push_back(pickPts[id]);
    for(int id: delOrd) route.push_back(delPts[id]);
    route.push_back(center);

    // Output
    cout << m;
    for(int idx: bestSubset) cout << ' ' << (idx+1);
    cout << '\n';
    cout << route.size();
    for(const auto& p: route){
        cout << ' ' << p.first << ' ' << p.second;
    }
    cout << '\n';
    return 0;
}