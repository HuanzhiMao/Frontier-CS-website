#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<int> candidates;
    for(int i = 1; i <= n; i++) {
        candidates.push_back(i);
    }
    
    while(candidates.size() > 2) {
        int mid = candidates.size() / 2;
        
        // Ask about first half
        cout << "? " << mid;
        for(int i = 0; i < mid; i++) {
            cout << " " << candidates[i];
        }
        cout << endl;
        cout.flush();
        
        string response1;
        cin >> response1;
        
        // Ask about second half
        cout << "? " << (candidates.size() - mid);
        for(int i = mid; i < candidates.size(); i++) {
            cout << " " << candidates[i];
        }
        cout << endl;
        cout.flush();
        
        string response2;
        cin >> response2;
        
        // At least one of these responses is correct
        // We'll use a strategy: try to narrow down based on the responses
        if(response1 == "YES" && response2 == "YES") {
            // At least one is correct, but we don't know which
            // Let's guess the middle of first half
            cout << "! " << candidates[mid/2] << endl;
            cout.flush();
            string result;
            cin >> result;
            if(result == ":)") return 0;
            
            // Wrong guess, so we know this number is not the answer
            // Remove it and continue with remaining candidates
            vector<int> new_candidates;
            for(int x : candidates) {
                if(x != candidates[mid/2]) {
                    new_candidates.push_back(x);
                }
            }
            candidates = new_candidates;
        }
        else if(response1 == "YES" && response2 == "NO") {
            // Keep first half
            vector<int> new_candidates;
            for(int i = 0; i < mid; i++) {
                new_candidates.push_back(candidates[i]);
            }
            candidates = new_candidates;
        }
        else if(response1 == "NO" && response2 == "YES") {
            // Keep second half
            vector<int> new_candidates;
            for(int i = mid; i < candidates.size(); i++) {
                new_candidates.push_back(candidates[i]);
            }
            candidates = new_candidates;
        }
        else { // both NO
            // At least one is correct, so if both say NO, we have a problem
            // This means the answer is not in our current candidates
            // This shouldn't happen with our logic, so let's just guess
            cout << "! " << candidates[0] << endl;
            cout.flush();
            string result;
            cin >> result;
            if(result == ":)") return 0;
            
            // Remove the wrong guess
            vector<int> new_candidates;
            for(int i = 1; i < candidates.size(); i++) {
                new_candidates.push_back(candidates[i]);
            }
            candidates = new_candidates;
        }
    }
    
    // We have at most 2 candidates left
    if(candidates.size() == 1) {
        cout << "! " << candidates[0] << endl;
        cout.flush();
        string result;
        cin >> result;
    }
    else if(candidates.size() == 2) {
        cout << "! " << candidates[0] << endl;
        cout.flush();
        string result;
        cin >> result;
        if(result == ":(") {
            cout << "! " << candidates[1] << endl;
            cout.flush();
            cin >> result;
        }
    }
    
    return 0;
}