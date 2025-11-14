#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    long long n;
    cin >> n;
    
    long long a_low = 1, a_high = n;
    long long b_low = 1, b_high = n;
    
    while (true) {
        long long mid_a = (a_low + a_high) / 2;
        long long mid_b = (b_low + b_high) / 2;
        
        cout << mid_a << " " << mid_b << endl;
        cout.flush();
        
        int response;
        cin >> response;
        
        if (response == 0) {
            break;
        } else if (response == 1) {
            // x < a, so a > mid_a
            a_low = mid_a + 1;
        } else if (response == 2) {
            // y < b, so b > mid_b
            b_low = mid_b + 1;
        } else if (response == 3) {
            // x > a OR y > b
            // This means NOT(x <= a AND y <= b)
            // We need to be more careful here
            // Let's try to narrow down one variable at a time
            
            // First try to determine if x > a by querying with a small y
            cout << mid_a << " " << b_low << endl;
            cout.flush();
            
            int response2;
            cin >> response2;
            
            if (response2 == 0) {
                break;
            } else if (response2 == 1) {
                // x < a
                a_low = mid_a + 1;
            } else if (response2 == 2) {
                // y < b
                b_low = b_low + 1;
            } else if (response2 == 3) {
                // x > a OR y > b
                // Since y = b_low (minimum possible), if we get 3, then likely x > a
                a_high = mid_a - 1;
            }
        }
    }
    
    return 0;
}