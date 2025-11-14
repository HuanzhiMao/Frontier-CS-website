#include <iostream>
#include <vector>
#include <algorithm>
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
        
        // Ask about first half
        cout << "? " << mid;
        for (int i = 0; i < mid; i++) {
            cout << " " << candidates[i];
        }
        cout << endl;
        cout.flush();
        
        string response1;
        cin >> response1;
        
        // Ask about second half
        cout << "? " << candidates.size() - mid;
        for (int i = mid; i < candidates.size(); i++) {
            cout << " " << candidates[i];
        }
        cout << endl;
        cout.flush();
        
        string response2;
        cin >> response2;
        
        // Since at least one of the two consecutive answers is correct
        // We need to consider both possibilities
        vector<int> new_candidates;
        
        if (response1 == "YES" && response2 == "NO") {
            // First half contains answer
            for (int i = 0; i < mid; i++) {
                new_candidates.push_back(candidates[i]);
            }
        } else if (response1 == "NO" && response2 == "YES") {
            // Second half contains answer
            for (int i = mid; i < candidates.size(); i++) {
                new_candidates.push_back(candidates[i]);
            }
        } else if (response1 == "YES" && response2 == "YES") {
            // At least one is correct, so answer could be in either half
            // But we need to make a choice - let's go with first half
            for (int i = 0; i < mid; i++) {
                new_candidates.push_back(candidates[i]);
            }
        } else {
            // Both NO - at least one is correct, so this shouldn't happen
            // But if it does, answer must be outside both sets (impossible)
            // This means one answer is wrong, let's assume first is wrong
            for (int i = mid; i < candidates.size(); i++) {
                new_candidates.push_back(candidates[i]);
            }
        }
        
        candidates = new_candidates;
    }
    
    // Now we have at most 2 candidates
    if (candidates.size() == 1) {
        cout << "! " << candidates[0] << endl;
        cout.flush();
        string result;
        cin >> result;
    } else {
        // Try first candidate
        cout << "! " << candidates[0] << endl;
        cout.flush();
        string result;
        cin >> result;
        
        if (result == ":(") {
            // Try second candidate
            cout << "! " << candidates[1] << endl;
            cout.flush();
            cin >> result;
        }
    }
    
    return 0;
}