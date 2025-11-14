#include <iostream>
#include <algorithm>
using namespace std;

int main() {
    long long n;
    cin >> n;
    
    long long a_min = 1, a_max = n;
    long long b_min = 1, b_max = n;
    
    while (true) {
        long long a_mid = (a_min + a_max) / 2;
        long long b_mid = (b_min + b_max) / 2;
        
        cout << a_mid << " " << b_mid << endl;
        cout.flush();
        
        int response;
        cin >> response;
        
        if (response == 0) {
            break;
        } else if (response == 1) {
            a_min = a_mid + 1;
        } else if (response == 2) {
            b_min = b_mid + 1;
        } else if (response == 3) {
            if (a_mid > 1 && b_mid > 1) {
                // Try to determine which one is too big
                cout << (a_min + a_mid - 1) / 2 << " " << b_mid << endl;
                cout.flush();
                
                int sub_response;
                cin >> sub_response;
                
                if (sub_response == 0) {
                    break;
                } else if (sub_response == 1) {
                    a_min = (a_min + a_mid) / 2;
                    a_max = a_mid - 1;
                } else if (sub_response == 2) {
                    b_min = b_mid + 1;
                } else {
                    a_max = a_mid - 1;
                }
            } else {
                if (a_mid == 1) {
                    b_max = b_mid - 1;
                } else {
                    a_max = a_mid - 1;
                }
            }
        }
        
        if (a_min > a_max) a_min = a_max;
        if (b_min > b_max) b_min = b_max;
    }
    
    return 0;
}