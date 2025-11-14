#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<int> candidates;
    for (int i = 1; i <= n; i++) {
        candidates.push_back(i);
    }
    
    while (candidates.size() > 2) {
        int mid = candidates.size() / 2;
        
        // First query - ask about first half
        cout << "? " << mid;
        for (int i = 0; i < mid; i++) {
            cout << " " << candidates[i];
        }
        cout << endl;
        cout.flush();
        
        string response1;
        cin >> response1;
        
        // Second query - ask about second half
        cout << "? " << (candidates.size() - mid);
        for (int i = mid; i < candidates.size(); i++) {
            cout << " " << candidates[i];
        }
        cout << endl;
        cout.flush();
        
        string response2;
        cin >> response2;
        
        // At least one of the two consecutive answers is correct
        // We need to determine which half contains x
        
        if (response1 == "YES" && response2 == "NO") {
            // Either first answer is correct (x in first half) or second is correct (x not in second half, so in first half)
            candidates.erase(candidates.begin() + mid, candidates.end());
        } else if (response1 == "NO" && response2 == "YES") {
            // Either first answer is correct (x not in first half) or second is correct (x in second half)
            candidates.erase(candidates.begin(), candidates.begin() + mid);
        } else if (response1 == "YES" && response2 == "YES") {
            // At least one is correct, but we can't determine which half
            // Let's make a guess to eliminate one possibility
            cout << "! " << candidates[0] << endl;
            cout.flush();
            
            string guess_result;
            cin >> guess_result;
            
            if (guess_result == ":)") {
                return 0;
            }
            
            // Our guess was wrong, so remove it from candidates
            candidates.erase(candidates.begin());
        } else { // response1 == "NO" && response2 == "NO"
            // At least one is correct, but both say NO
            // This means x is neither in first half nor second half
            // This shouldn't happen in a valid scenario, but let's handle it
            // Make a guess
            cout << "! " << candidates[0] << endl;
            cout.flush();
            
            string guess_result;
            cin >> guess_result;
            
            if (guess_result == ":)") {
                return 0;
            }
            
            candidates.erase(candidates.begin());
        }
    }
    
    // Now we have at most 2 candidates left
    if (candidates.size() == 1) {
        cout << "! " << candidates[0] << endl;
        cout.flush();
    } else {
        // We have 2 candidates, make a guess
        cout << "! " << candidates[0] << endl;
        cout.flush();
        
        string guess_result;
        cin >> guess_result;
        
        if (guess_result == ":)") {
            return 0;
        }
        
        // First guess was wrong, so it must be the second one
        cout << "! " << candidates[1] << endl;
        cout.flush();
    }
    
    return 0;
}