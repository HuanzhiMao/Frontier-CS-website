#include <bits/stdc++.h>
#define fo(i,l,r) for(int i=(l);i<=(r);++i)
#define fd(i,l,r) for(int i=(l);i>=(r);--i)
#define fu(i,l,r) for(int i=(l);i<(r);++i)
#define pi pair<int,int>
#define eb emplace_back
#define vi vector<int>
#define fi first
#define se second
#define ll long long
using namespace std;

const int N = 1007;
int n, g[N], st;
vi e[N];

int ask(int l, int r) {
    cout << "1 " << l << " " << r << endl;
    int result;
    cin >> result;
    return result;
}

void swp(int i, int j) {
    cout << "2 " << i << " " << j << endl;
    int confirm;
    cin >> confirm;
}

int main() {
    int l1, l2;
    cin >> n >> l1 >> l2;
    
    fo(i, 2, n) {
        fo(j, 1, n) g[j] = 0;
        fd(j, i - 1, 1) {
            for(int k : e[j]) if(k < j) g[k]++;
        }
        fd(j, i - 1, 1) g[j] += g[j + 1];
        
        int l = 1, r = i - 1, s = 0;
        while(l <= r) {
            int mid = (l + r) >> 1;
            if(ask(mid, i) - (i - mid - g[mid]) <= 0) 
                s = mid, l = mid + 1;
            else 
                r = mid - 1;
        }
        if(s) e[i].eb(s), e[s].eb(i);
        
        l = 1, r = s - 1, s = 0;
        while(l <= r) {
            int mid = (l + r) >> 1;
            if(ask(mid, i) - (i - mid - g[mid]) < 0) 
                s = mid, l = mid + 1;
            else 
                r = mid - 1;
        }
        if(s) e[i].eb(s), e[s].eb(i);
    }
    
    fo(i, 1, n) if(e[i].size() == 1) st = i;
    
    vi p(n + 1);
    fo(i, 1, n) {
        p[st] = i;
        for(int j : e[st]) if(!p[j]) {
            st = j;
            break;
        }
    }
    
    cout << "3";
    fo(i, 1, n) {
        cout << " " << p[i];
    }
    cout << endl;
    
    return 0;
}