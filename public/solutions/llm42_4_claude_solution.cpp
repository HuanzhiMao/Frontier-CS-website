#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    // Simple baseline approach: place squares on grid without rotation
    int grid_size = ceil(sqrt(n));
    double L = grid_size;
    
    cout << fixed << setprecision(6) << L << endl;
    
    int count = 0;
    for (int i = 0; i < grid_size && count < n; i++) {
        for (int j = 0; j < grid_size && count < n; j++) {
            double x = 0.5 + j;
            double y = 0.5 + i;
            double angle = 0.0;
            
            cout << fixed << setprecision(6) << x << " " << y << " " << angle << endl;
            count++;
        }
    }
    
    return 0;
}