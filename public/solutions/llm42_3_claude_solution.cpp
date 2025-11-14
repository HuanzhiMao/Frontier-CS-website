#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    double L = ceil(sqrt(n));
    int grid_size = (int)L;
    
    cout << fixed << setprecision(6) << L << endl;
    
    int idx = 0;
    for (int i = 0; i < grid_size && idx < n; i++) {
        for (int j = 0; j < grid_size && idx < n; j++) {
            double x = j + 0.5;
            double y = i + 0.5;
            double angle = 0.0;
            cout << fixed << setprecision(6) << x << " " << y << " " << angle << endl;
            idx++;
        }
    }
    
    return 0;
}