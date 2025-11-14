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
        
        if (n == -1) break;
        
        int answer = -1;
        
        for (int x = 1; x <= n; x++) {
            // Query all positions for value x
            cout << "? " << x << " " << (2*n-1);
            for (int i = 1; i <= 2*n-1; i++) {
                cout << " " << i;
            }
            cout << endl;
            cout.flush();
            
            int response;
            cin >> response;
            
            if (response == -1) return 0;
            
            if (response == 1) {
                // x appears in the array, now check if it appears once or twice
                // We'll use binary search approach to count occurrences
                
                vector<int> positions;
                for (int i = 1; i <= 2*n-1; i++) {
                    positions.push_back(i);
                }
                
                int count = 0;
                
                // Binary search to find positions where x appears
                while (positions.size() > 0 && count < 2) {
                    if (positions.size() == 1) {
                        cout << "? " << x << " 1 " << positions[0] << endl;
                        cout.flush();
                        int resp;
                        cin >> resp;
                        if (resp == -1) return 0;
                        if (resp == 1) count++;
                        break;
                    }
                    
                    int mid = positions.size() / 2;
                    vector<int> left_half(positions.begin(), positions.begin() + mid);
                    
                    cout << "? " << x << " " << left_half.size();
                    for (int pos : left_half) {
                        cout << " " << pos;
                    }
                    cout << endl;
                    cout.flush();
                    
                    int resp;
                    cin >> resp;
                    if (resp == -1) return 0;
                    
                    if (resp == 1) {
                        positions = left_half;
                        count++;
                        if (count == 1) {
                            // Check right half for second occurrence
                            vector<int> right_half(positions.begin() + mid, positions.end());
                            if (right_half.size() > 0) {
                                cout << "? " << x << " " << right_half.size();
                                for (int pos : right_half) {
                                    cout << " " << pos;
                                }
                                cout << endl;
                                cout.flush();
                                
                                int resp2;
                                cin >> resp2;
                                if (resp2 == -1) return 0;
                                if (resp2 == 1) count++;
                            }
                            break;
                        }
                    } else {
                        vector<int> right_half(positions.begin() + mid, positions.end());
                        positions = right_half;
                    }
                }
                
                if (count == 1) {
                    answer = x;
                    break;
                }
            }
        }
        
        cout << "! " << answer << endl;
        cout.flush();
    }
    
    return 0;
}