#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int prev_result = 0;
    
    for (int k = 0; k < 1000; k++) {
        int si, sj, ti, tj;
        cin >> si >> sj >> ti >> tj;
        
        string path = "";
        
        // Simple greedy path - move vertically first, then horizontally
        int ci = si, cj = sj;
        
        // Move vertically
        while (ci != ti) {
            if (ci < ti) {
                path += "D";
                ci++;
            } else {
                path += "U";
                ci--;
            }
        }
        
        // Move horizontally
        while (cj != tj) {
            if (cj < tj) {
                path += "R";
                cj++;
            } else {
                path += "L";
                cj--;
            }
        }
        
        cout << path << endl;
        cout.flush();
        
        cin >> prev_result;
    }
    
    return 0;
}