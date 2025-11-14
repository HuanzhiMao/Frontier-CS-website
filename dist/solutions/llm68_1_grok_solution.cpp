#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int t;
    cin >> t;
    for(int test = 0; test < t; test++) {
        int n;
        cin >> n;
        int k = (n - 2) / 2;
        int pen = 0;
        bool found = false;
        int sac_pen = -1;
        while(!found) {
            assert(pen < n);
            int curr_pen = pen;
            int success = 0;
            found = true;
            for(int j = 0; j < k; j++) {
                cout << "0 " << curr_pen << endl << flush;
                int x;
                cin >> x;
                if(x == 0) {
                    found = false;
                    break;
                }
                success++;
            }
            if(found) {
                sac_pen = curr_pen;
            }
            pen++;
        }
        cout << "1 " << pen << " " << (pen + 1) << endl << flush;
    }
    return 0;
}