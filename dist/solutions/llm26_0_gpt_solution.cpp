#include <bits/stdc++.h>
using namespace std;

struct BIT {
    int n;
    vector<int> bit;
    BIT(int n=0){init(n);}
    void init(int n_) { n = n_; bit.assign(n+1, 0); }
    void add(int idx, int val){
        for(; idx<=n; idx+=idx&-idx) bit[idx] += val;
    }
    int sum(int idx){
        int s=0;
        for(; idx>0; idx-=idx&-idx) s += bit[idx];
        return s;
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if(!(cin >> n)) return 0;
    vector<int> v(n+1), pos(n+1);
    for(int i=1;i<=n;i++){
        cin >> v[i];
        pos[v[i]] = i;
    }
    int k = n;
    for(int i=n; i>=2; --i){
        if(pos[i-1] < pos[i]) k--;
        else break;
    }
    int m = k - 1;
    int M = m;
    int TOT = n + M + 2;
    BIT ft(TOT);
    vector<int> keyOfValue(n+1);
    for(int val=1; val<=n; ++val){
        int key = M + pos[val];
        keyOfValue[val] = key;
        ft.add(key, 1);
    }
    vector<pair<int,int>> moves;
    moves.reserve(m);
    int curFront = M;
    for(int val = k-1; val >= 1; --val){
        int key = keyOfValue[val];
        int x = ft.sum(key);
        moves.emplace_back(x, 1);
        ft.add(key, -1);
        keyOfValue[val] = curFront;
        ft.add(curFront, +1);
        curFront--;
    }
    long long S = (long long)m; // all y = 1
    long long F = (S + 1) * ( (long long)m + 1 );
    cout << F << " " << moves.size() << "\n";
    for(auto &p : moves){
        cout << p.first << " " << p.second << "\n";
    }
    return 0;
}