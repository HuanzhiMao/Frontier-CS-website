#include <bits/stdc++.h>
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
            if (a_mid == a_min && a_mid == a_max) {
                b_max = b_mid - 1;
            } else if (b_mid == b_min && b_mid == b_max) {
                a_max = a_mid - 1;
            } else {
                cout << a_min << " " << b_mid << endl;
                cout.flush();
                
                int response2;
                cin >> response2;
                
                if (response2 == 0) {
                    break;
                } else if (response2 == 1) {
                    a_min = a_min + 1;
                } else if (response2 == 2) {
                    b_min = b_mid + 1;
                } else if (response2 == 3) {
                    a_max = a_mid - 1;
                }
            }
        }
    }
    
    return 0;
}