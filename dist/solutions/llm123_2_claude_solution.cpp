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
        
        // First query: first half
        cout << "? " << mid;
        for (int i = 0; i < mid; i++) {
            cout << " " << candidates[i];
        }
        cout << endl;
        cout.flush();
        
        string response1;
        cin >> response1;
        
        // Second query: second half
        cout << "? " << (candidates.size() - mid);
        for (int i = mid; i < candidates.size(); i++) {
            cout << " " << candidates[i];
        }
        cout << endl;
        cout.flush();
        
        string response2;
        cin >> response2;
        
        // Since at least one of the two consecutive answers is correct
        if (response1 == "YES" && response2 == "NO") {
            // First half has the answer
            candidates.erase(candidates.begin() + mid, candidates.end());
        } else if (response1 == "NO" && response2 == "YES") {
            // Second half has the answer
            candidates.erase(candidates.begin(), candidates.begin() + mid);
        } else if (response1 == "YES" && response2 == "YES") {
            // At least one is true, so we can't determine which half
            // Let's assume the second answer is true (second half)
            candidates.erase(candidates.begin(), candidates.begin() + mid);
        } else {
            // Both NO - at least one is true, so this shouldn't happen
            // But if it does, assume second is true (neither half has answer - contradiction)
            // Let's keep first half
            candidates.erase(candidates.begin() + mid, candidates.end());
        }
    }
    
    if (candidates.size() == 1) {
        cout << "! " << candidates[0] << endl;
        cout.flush();
        string result;
        cin >> result;
        return 0;
    }
    
    // candidates.size() == 2
    cout << "! " << candidates[0] << endl;
    cout.flush();
    string result;
    cin >> result;
    
    if (result == ":)") {
        return 0;
    }
    
    cout << "! " << candidates[1] << endl;
    cout.flush();
    cin >> result;
    
    return 0;
}