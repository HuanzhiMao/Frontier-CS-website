#include <bits/stdc++.h>
using namespace std;

int query(int a,int b,int c) {
    cout << "0 " << a << " " << b << " " << c << endl;
    int result;
    cin >> result;
    return result;
}

const int N=1000+5;
int n;
vector<int> edge[N];
inline void Remove(int x,int y)
{
    edge[x].erase(remove(edge[x].begin(),edge[x].end(),y), edge[x].end());
    edge[y].erase(remove(edge[y].begin(),edge[y].end(),x), edge[y].end());
}
inline void Add(int x,int y)
{
    edge[x].push_back(y);
    edge[y].push_back(x);
}
bool flag[N][N];
int siz[N];

int minn;
int best_u, best_v;

inline void dfs1(int from,int to)
{
    siz[to]=1;
    for(auto k : edge[to])
    {
        if(!flag[to][k] || k==from) continue;
        dfs1(to,k);
        siz[to]+=siz[k];
    }
}
inline void dfs2(int from,int to,int sz)
{
    for(auto k : edge[to])
    {
        if(!flag[to][k] || k==from) continue;
        int maxx=max(siz[k],sz-siz[k]);
        if(maxx<minn)
        {
            minn=maxx;
            best_u=to;
            best_v=k;
        }
        dfs2(to,k,sz);
    }
}

bool vis[N];
inline void Work(int from,int to,int id)
{
    dfs1(from,to);
    int sz = siz[to];  
    if(sz==1)
    {
        Add(to,id);
        return;
    }
    minn=n+1; 
    dfs2(from,to,sz);
    int res = query(best_u,best_v,id);
    if(res == best_u)
    {
        flag[best_u][best_v] = flag[best_v][best_u] = false;
        Work(best_v, best_u, id);
    }
    else if(res == best_v)
    {
        flag[best_u][best_v] = flag[best_v][best_u] = false;
        Work(best_u, best_v, id);
    }
    else if(res == id)
    {
        Remove(best_u, best_v);
        Add(best_u, id);
        Add(best_v, id);
        vis[id]=true;
    }
    else 
    {
        Remove(best_u, best_v);
        Add(best_u, res);
        Add(best_v, res);
        Add(res, id);
        vis[res]=vis[id]=true;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n;
    vis[1] = vis[2] = true;
    Add(1,2);
    for(int i=3;i<=n;i++)
    {
        if(vis[i]) continue;
        for(int j=1;j<=n;j++)
            for(auto k : edge[j]) 
                flag[j][k]=true;
        Work(0,1,i);
    }
    cout<<"1 ";
    int tot=0;
    for(int i=1;i<=n;i++)
    {
        for(auto k : edge[i])
        {
            if(k<i) continue;
            tot++;
            if(tot!=n-1) cout<<i<<" "<<k<<" ";
            else cout<<i<<" "<<k<<endl;
        }
    }
    return 0;
}