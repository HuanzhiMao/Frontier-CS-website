#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int t;
    cin >> t;
    
    while (t--) {
        int n;
        cin >> n;
        
        vector<int> ink_used(n, 0);
        
        // We need to find two pens with total remaining ink >= n
        // Since it's a permutation of 0 to n-1, total ink is n*(n-1)/2
        // We need pens with high initial ink values
        
        // Test each pen up to n-1 times to get a good estimate
        for (int pen = 0; pen < n; pen++) {
            for (int tries = 0; tries < n-1; tries++) {
                cout << "0 " << pen << endl;
                cout.flush();
                int response;
                cin >> response;
                
                if (response == 0) {
                    break; // pen is empty
                }
                ink_used[pen]++;
            }
        }
        
        // Find the two pens with maximum remaining ink
        vector<pair<int, int>> remaining_ink;
        for (int i = 0; i < n; i++) {
            // Estimate remaining ink: initial_ink - ink_used[i]
            // We don't know initial_ink exactly, but we know it's some value from 0 to n-1
            // If we used ink_used[i] ink and it didn't run out, initial ink >= ink_used[i]
            remaining_ink.push_back({-ink_used[i], i}); // negative for sorting in descending order
        }
        
        sort(remaining_ink.begin(), remaining_ink.end());
        
        // Try combinations starting from pens that used least ink
        bool found = false;
        for (int i = 0; i < n && !found; i++) {
            for (int j = i + 1; j < n && !found; j++) {
                int pen1 = remaining_ink[i].second;
                int pen2 = remaining_ink[j].second;
                
                // Estimate if these two pens together have >= n remaining ink
                // This is a heuristic - try the combination with pens that used least ink
                cout << "1 " << pen1 << " " << pen2 << endl;
                cout.flush();
                found = true;
                break;
            }
        }
    }
    
    return 0;
}