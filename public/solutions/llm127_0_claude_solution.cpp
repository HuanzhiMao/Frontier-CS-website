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
        
        if (a0 == 0) {
            // Diamond is at mid or to the left
            right = mid;
        } else {
            // There are more expensive prizes to the left, so diamond is to the left
            right = mid - 1;
        }
    }
    
    cout << "! " << left << endl;
    cout.flush();
    
    return 0;
}