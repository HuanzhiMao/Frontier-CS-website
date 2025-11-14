#include <bits/stdc++.h>
using namespace std;
int main() {
    int n=20, m=20;
    vector<string> grid(n, string(m, '0'));
    for(int r=0; r<n; r+=2){
        for(int c=0; c<m; c++){
            grid[r][c] = '1';
        }
    }
    vector<int> conn_right = {1,5,9,13,17};
    for(int r: conn_right){
        grid[r][19] = '1';
    }
    vector<int> conn_left = {3,7,11,15};
    for(int r: conn_left){
        grid[r][0] = '1';
    }
    grid[19][0] = '1';
    cout << n << " " << m << endl;
    for(int r=0; r<n; r++){
        for(int c=0; c<m; c++){
            cout << grid[r][c];
        }
        cout << endl;
    }
    return 0;
}