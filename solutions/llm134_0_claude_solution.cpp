#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
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
                long long a_range = a_max - a_min + 1;
                long long b_range = b_max - b_min + 1;
                
                if (a_range >= b_range) {
                    a_max = a_mid - 1;
                } else {
                    b_max = b_mid - 1;
                }
            }
        }
    }
    
    return 0;
}