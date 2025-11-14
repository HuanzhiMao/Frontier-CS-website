#include <bits/stdc++.h>
using namespace std;

void hanoi(long long n, int a, int b, int c, vector<pair<int,int>>& moves){
    if(n==0) return;
    hanoi(n-1, a, c, b, moves);
    moves.emplace_back(a, b);
    hanoi(n-1, c, b, a, moves);
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    long long N; 
    if(!(cin >> N)) return 0;
    vector<pair<int,int>> moves;
    moves.reserve((N>=0 && N<=60) ? (1ull<<min(30ll,N)) : 0);
    hanoi(N, 1, 3, 2, moves);
    cout << moves.size() << "\n";
    for(auto &p: moves){
        cout << p.first << " " << p.second << "\n";
    }
    return 0;
}