#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    // Simple baseline approach: place squares on a grid without rotation
    int side = ceil(sqrt(n));
    double L = (double)side;
    
    cout << fixed << setprecision(6) << L << endl;
    
    int count = 0;
    for (int i = 0; i < side && count < n; i++) {
        for (int j = 0; j < side && count < n; j++) {
            double x = 0.5 + j;
            double y = 0.5 + i;
            double angle = 0.0;
            
            cout << fixed << setprecision(6) << x << " " << y << " " << angle << endl;
            count++;
        }
    }
    
    return 0;
}