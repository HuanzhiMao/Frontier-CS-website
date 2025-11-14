#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    int left = 0, right = n - 1;
    
    while (left < right) {
        int mid = (left + right) / 2;
        
        cout << "? " << mid << endl;
        cout.flush();
        
        int a0, a1;
        cin >> a0 >> a1;
        
        if (a0 == 0 && a1 == 0) {
            cout << "! " << mid << endl;
            cout.flush();
            return 0;
        }
        
        if (a0 > a1) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    
    cout << "! " << left << endl;
    cout.flush();
    
    return 0;
}