#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int t;
    cin >> t;
    
    while (t--) {
        int n;
        cin >> n;
        
        vector<int> ink(n, 0);
        
        // Test each pen once to get initial information
        for (int i = 0; i < n; i++) {
            cout << "0 " << i << endl;
            cout.flush();
            int response;
            cin >> response;
            ink[i] = response;
        }
        
        // Find pens that had ink initially
        vector<pair<int, int>> pens_with_ink;
        for (int i = 0; i < n; i++) {
            if (ink[i] == 1) {
                pens_with_ink.push_back({i, 1});
            }
        }
        
        // We need to find two pens with total remaining ink >= n
        // Since it's a permutation of 0 to n-1, the two largest values are n-1 and n-2
        // We need to identify these pens
        
        // Continue testing pens that had ink to find the ones with most ink
        while (true) {
            // Sort by current known ink amount
            sort(pens_with_ink.begin(), pens_with_ink.end(), 
                 [](const pair<int, int>& a, const pair<int, int>& b) {
                     return a.second > b.second;
                 });
            
            // If we have at least 2 pens and the top 2 have enough ink
            if (pens_with_ink.size() >= 2 && 
                pens_with_ink[0].second + pens_with_ink[1].second >= n) {
                cout << "1 " << pens_with_ink[0].first << " " << pens_with_ink[1].first << endl;
                cout.flush();
                break;
            }
            
            // Test the pen with most known ink
            if (pens_with_ink.empty()) break;
            
            int best_pen = pens_with_ink[0].first;
            cout << "0 " << best_pen << endl;
            cout.flush();
            int response;
            cin >> response;
            
            if (response == 1) {
                pens_with_ink[0].second++;
            } else {
                // Pen is empty, remove it
                pens_with_ink.erase(pens_with_ink.begin());
            }
        }
    }
    
    return 0;
}