#include<bits/stdc++.h>
using namespace std;
int T,n,i,j,a,b,px,py,m1,m2;
void get_randab()
{
	a=rand()%n+1;
	b=rand()%(n-1)+1;
	if(b>=a) b++;
}
int main()
{
	srand(20080506);
	cin.tie(0)->sync_with_stdio(0);cout.tie(0);
    cin>>n;px=n/2,py=n/2+1;m1=m2=a=b=0;
    while(m1!=px||m2!=py)
    {
        get_randab();
        cout<<"0 "<<n-2<<" ";
        for(i=1;i<=n;i++) if(i!=a&&i!=b) cout<<i<<" ";
        cout<<endl;
        cin>>m1>>m2;
    }
    vector<int> q;
    for(i=1;i<=n;i++) if(i!=a&&i!=b) q.push_back(i);
    int ix,iy;
    for(i=0;i<n-2;i+=2)
    {
        cout<<"0 4 "<<q[i]<<" "<<q[i+1]<<" "<<a<<" "<<b<<endl;
        cin>>m1>>m2;
        if(m1==px||m2==px) ix=i;
        if(m1==py||m2==py) iy=i;
    }
    int iix=(ix+2)%(n-2);
    cout<<"0 4 "<<a<<" "<<b<<" "<<q[ix]<<" "<<q[iix]<<endl;
    cin>>m1>>m2;
    if(m1!=px&&m2!=px) ix++;
    int iiy=(iy+2)%(n-2);
    cout<<"0 4 "<<a<<" "<<b<<" "<<q[iy]<<" "<<q[iiy]<<endl;
    cin>>m1>>m2;
    if(m1!=py&&m2!=py) iy++;
    cout<<"1 "<<q[ix]<<" "<<q[iy]<<endl;
	return 0;
 } 