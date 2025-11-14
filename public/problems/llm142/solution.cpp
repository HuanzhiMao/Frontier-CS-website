#include<bits/stdc++.h>
using namespace std;

const int N=50+5;
const int M=400+5;
const int Max=820000+5;

inline int read()
{
	int x=0,f=1;
	char c=getchar();
	while(c<'0'||c>'9')
	{
	    if(c=='-')
	        f=-1;
	    c=getchar();
	}
	while(c>='0'&&c<='9')
	{
	    x=x*10+c-'0';
	    c=getchar();
	}
	return x*f;
}

int n,m;
int a[N][M];
int top[N];
int ans_len,ans1[Max],ans2[Max];
int re[N],len=0;
int cnt[N];
inline void update(int x,int y)
{
	ans1[++ans_len]=x; ans2[ans_len]=y;
	a[y][++top[y]]=a[x][top[x]];
	a[x][top[x]]=0; top[x]--;
}
inline void prework1(int mid)
{
	for(int i=1;i<=len;i++)
	{
		int place=re[i]; cnt[place]=0;
		for(int j=1;j<=m;j++)
			if(a[place][j]<=mid) 
				cnt[place]++;
	}	
}
inline void prework2(int x,int y,int mid)
{
	for(int i=1;i<=cnt[x];i++) update(y,n+1);
	while(top[x])
	{
		if(a[x][top[x]]<=mid) update(x,y);
		else update(x,n+1);
	}
	for(int i=1;i<=m-cnt[x];i++) update(n+1,x);
	for(int i=1;i<=cnt[x];i++) update(y,x);
	while(top[n+1]) update(n+1,y);
}
inline int ask(int x,int y,int mid)
{
	int ans=0;
	for(int i=1;i<=m;i++) if(a[x][i]<=mid) ans++;
	for(int i=1;i<=m;i++) if(a[y][i]<=mid) ans++;
	return ans;
}
inline void prework3(int x,int y,int mid)
{
	int k=ask(x,y,mid);
	if(k>=m)
	{
		while(top[x]) update(x,n+1);
		while(top[y] && a[y][top[y]]<=mid) update(y,x);
		while(top[n+1] && a[n+1][top[n+1]]>mid) update(n+1,y);
		while(top[x]!=m) update(n+1,x);
		while(top[n+1]) update(n+1,y);
	}
	else
	{
		while(top[x] && a[x][top[x]]<=mid) update(x,n+1);
		while(top[y] && a[y][top[y]]<=mid) update(y,n+1);
		while(top[x]!=m) update(y,x);
		while(top[n+1]) update(n+1,y);
	}
}

inline void Work(int l,int r)
{
	if(l==r) return;
	len=0;
	for(int i=1;i<=n;i++) 
		if(a[i][1]>=l && a[i][1]<=r) 
			re[++len]=i;
	int mid=(l+r)>>1;
	prework1(mid);
	for(int i=1;i<=len;i++)
	{
		if(i==1) prework2(re[1],re[2],mid);
		else prework2(re[i],re[1],mid);
	}
	for(int i=1;i<len;i++) prework3(re[i],re[i+1],mid);
	Work(l,mid); Work(mid+1,r);
}

int main()
{
	n=read(); m=read();
	for(int i=1;i<=n;i++) 
		for(int j=1;j<=m;j++)
			a[i][j]=read();
	for(int i=1;i<=n;i++) top[i]=m;
	Work(1,n);
	printf("%d\n",ans_len);
	for(int i=1;i<=ans_len;i++) printf("%d %d\n",ans1[i],ans2[i]);
    return 0;
}