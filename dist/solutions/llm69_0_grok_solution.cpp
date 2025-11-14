#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
ll compute_f(ll p, ll q, ll r) {
    ll mx = max({p, q, r});
    ll min_pq = min(p, q);
    ll min_qr = min(q, r);
    ll one = (p + 1) * (q + 1) + (q + 1) * (r + 1) - (min_pq + 1) * (min_qr + 1);
    ll two = (p + 1) * (r + 1);
    return mx + one + two;
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int n;
    cin >> n;
    vector<ll> a(n+1);
    for(int i=1; i<=n; i++) a[i] = i-1;
    int max_c = 2000;
    map<ll, pair<int,int>> mp;
    int chosen_c = -1;
    for(int cc=0; cc<=max_c; cc++) {
        unordered_map<ll, int> cnt;
        bool has_coll = false;
        for(int u=1; u<=n && !has_coll; u++){
            for(int v=1; v<=n; v++){
                ll pp = compute_f(a[u], (ll)cc + a[v], cc);
                cnt[pp]++;
                if(cnt[pp] >1){ has_coll=true; break;}
            }
        }
        if(!has_coll){
            chosen_c = cc;
            for(int u=1; u<=n; u++){
                for(int v=1; v<=n; v++){
                    ll pp = compute_f(a[u], (ll)chosen_c + a[v], chosen_c);
                    mp[pp] = {u,v};
                }
            }
            break;
        }
    }
    assert(chosen_c != -1);
    for(int i=1; i<=n; i++){
        string s = "";
        for(ll j=0; j<a[i]; j++) s += 'X';
        s += 'O';
        for(ll j=0; j<chosen_c; j++) s += 'X';
        cout << s << '\n';
    }
    cout << flush;
    int q;
    cin >> q;
    for(int qq=0; qq<q; qq++){
        ll pj;
        cin >> pj;
        auto [uj, vj] = mp[pj];
        cout << uj << " " << vj << '\n';
        cout << flush;
    }
    return 0;
}