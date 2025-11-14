#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    // Binary search approach
    int left = 0, right = n - 1;
    
    while (left < right) {
        int mid = (left + right) / 2;
        
        cout << "? " << mid << endl;
        cout.flush();
        
        int a0, a1;
        cin >> a0 >> a1;
        
        // If a0 = 0 and a1 = 0, we found the diamond
        if (a0 == 0 && a1 == 0) {
            cout << "! " << mid << endl;
            cout.flush();
            return 0;
        }
        
        // If there are more expensive prizes on the left, search left
        if (a0 > 0) {
            right = mid - 1;
        }
        // Otherwise, search right
        else {
            left = mid + 1;
        }
    }
    
    // Check the remaining position
    cout << "? " << left << endl;
    cout.flush();
    
    int a0, a1;
    cin >> a0 >> a1;
    
    cout << "! " << left << endl;
    cout.flush();
    
    return 0;
}