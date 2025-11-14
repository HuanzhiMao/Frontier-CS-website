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
    
    while (left <= right) {
        int mid = (left + right) / 2;
        
        cout << "? " << mid << endl;
        cout.flush();
        
        int a0, a1;
        cin >> a0 >> a1;
        
        // If both a0 and a1 are 0, this is the diamond
        if (a0 == 0 && a1 == 0) {
            cout << "! " << mid << endl;
            cout.flush();
            return 0;
        }
        
        // If a0 == 0, the diamond is to the right
        if (a0 == 0) {
            left = mid + 1;
        }
        // If a1 == 0, the diamond is to the left
        else if (a1 == 0) {
            right = mid - 1;
        }
        // Otherwise, choose the side with fewer expensive prizes
        else if (a0 <= a1) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    
    // Fallback - should not reach here in valid cases
    cout << "! 0" << endl;
    cout.flush();
    return 0;
}