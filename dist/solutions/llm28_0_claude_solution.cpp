#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int T;
    cin >> T;
    
    while (T--) {
        int N;
        cin >> N;
        
        set<string> words;
        queue<string> prefixes;
        prefixes.push("");
        
        while (!prefixes.empty() && words.size() < N) {
            string prefix = prefixes.front();
            prefixes.pop();
            
            cout << "query " << prefix << " " << N << endl;
            cout.flush();
            
            int k;
            cin >> k;
            
            vector<string> result(k);
            for (int i = 0; i < k; i++) {
                cin >> result[i];
                words.insert(result[i]);
            }
            
            if (k == 0) continue;
            
            // If we got fewer words than expected for this prefix, we're done with it
            if (words.size() >= N) break;
            
            // Generate next prefixes
            set<string> next_prefixes;
            for (const string& word : result) {
                if (word.length() > prefix.length()) {
                    string next_prefix = word.substr(0, prefix.length() + 1);
                    next_prefixes.insert(next_prefix);
                }
            }
            
            for (const string& next : next_prefixes) {
                prefixes.push(next);
            }
        }
        
        cout << "answer";
        for (const string& word : words) {
            cout << " " << word;
        }
        cout << endl;
        cout.flush();
    }
    
    return 0;
}