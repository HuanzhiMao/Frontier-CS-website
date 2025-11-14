#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    double L = ceil(sqrt(n));
    cout << fixed << setprecision(6) << L << endl;
    
    int cols = (int)L;
    for (int i = 0; i < n; i++) {
        int row = i / cols;
        int col = i % cols;
        double x = col + 0.5;
        double y = row + 0.5;
        cout << fixed << setprecision(6) << x << " " << y << " 0.000000" << endl;
    }
    
    return 0;
}