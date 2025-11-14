#include <bits/stdc++.h>
using namespace std;

const int N=100+5;
const int M=20000+5;
int n,m,root;
int st[N],ed[N],cur[N],tmp[N],depth[N],fa[N];
vector<int> edge[N];
int ans_len=0;
int ans[M][N];
int siz[N];

inline void dfs2(int from,int to)
{
    if(ed[to]==0) siz[to]=1;
    for(auto k : edge[to])
    {
        if(k==from || fa[k]!=to) continue;
        dfs2(to,k);
        siz[to]+=siz[k];
    }
}

inline void dfs1(int from,int to)
{
    fa[to]=from;
    depth[to]=depth[from]+1;
    for(auto k : edge[to]) 
        if(k!=from && fa[k]==-1)
            dfs1(to,k);
}
inline void update()
{
    ans_len++;
    for(int i=1;i<=n;i++) ans[ans_len][i]=cur[i];
}

int main() {
    cin>>n>>m;
    memset(fa,-1,sizeof(fa));
    for(int i=1;i<=n;i++) cin>>st[i], cur[i]=st[i];
    for(int i=1;i<=n;i++) cin>>ed[i];
    int u,v;
    for(int i=1;i<=m;i++)
    {
        cin>>u>>v;
        edge[u].push_back(v);
        edge[v].push_back(u);
    }
    for(int i=1;i<=n;i++)
        if(ed[i]==0)
            root=i;
    // cout<<"root = "<<root<<endl;
    update();
    dfs1(0,root);
    int now=-1;
    for(int i=1;i<=n;i++)
        if(cur[i]==0 && (now==-1 || depth[i]<depth[now])) 
            now=i;
    while(now!=root)
    {
        now=fa[now];
        cur[now]=0;
        update();
    }
    for(int i=1;i<=n;i++)
    {
        bool flag=false;
        for(int j=1;j<=n;j++) tmp[j]=cur[j];
        for(int j=1;j<=n;j++)
            if(cur[j]==1)
                for(auto k : edge[j])
                    if(k!=root && cur[k]==0)
                    {
                        flag=true;
                        tmp[k]=1;
                    }
        if(!flag) break;
        for(int j=1;j<=n;j++) cur[j]=tmp[j];
        update();
    }
    int cnt=0;
    for(int i=1;i<=n;i++) cnt+=cur[i];
    if(cnt!=n-1)
    {
        for(int i=1;i<=n;i++)
            if(cur[i]==1 && fa[i]==root)
                now=i;
        swap(cur[now],cur[root]);
        update();
        for(int i=1;i<=n;i++)
        {
            bool flag=false;
            for(int j=1;j<=n;j++) tmp[j]=cur[j];
            for(int j=1;j<=n;j++)
                if(cur[j]==1)
                    for(auto k : edge[j])
                        if(k!=root && k!=now && cur[k]==0)
                        {
                            flag=true;
                            tmp[k]=1;
                        }
            if(!flag) break;
            for(int j=1;j<=n;j++) cur[j]=tmp[j];
            update();
        }
        swap(cur[now],cur[root]);
        update();
    }
    dfs2(0,root);
    // for(int i=1;i<=n;i++) cout<<siz[i]<<" ";
    // cout<<endl;
    for(int i=1;i<=n;i++)
    {
        bool flag=false;
        for(int j=1;j<=n;j++)
        {
            if(depth[j]!=i || cur[j]==1) continue;
            for(auto k : edge[j])
            {
                if(fa[k]!=j) continue;
                if(siz[k])
                {
                    cur[k]=0;
                    flag=true;
                }
            }
            if(ed[j]==1) 
            {
                cur[j]=1;
                flag=true;
            }
        }
        if(!flag) break;
        update(); 
    }
    cout<<ans_len-1<<endl;
    for(int i=1;i<=ans_len;i++)
    {
        for(int j=1;j<=n;j++)
        {
            if(j!=n) cout<<ans[i][j]<<" ";
            else cout<<ans[i][j];
        }
        if(i!=ans_len) cout<<endl;
    }
    return 0;
}