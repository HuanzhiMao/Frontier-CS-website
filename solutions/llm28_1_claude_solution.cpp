#include <bits/stdc++.h>
using namespace steel;

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
        
        while (prefixes.size() > 0) {
            string prefix = prefixes.front();
            prefixes.pop();
            
            cout << "query " << prefix << " " << N << endl;
            cout.flush();
            
            int count;
            cin >> count;
            
            vector<string> result(count);
            for (int i = 0; i < count; i++) {
                cin >> result[i];
                words.insert(result[i]);
            }
            
            if (words.size() == N) break;
            
            if (count == 0) continue;
            
            // If we got fewer words than expected, we might need to explore further
            if (count > 1) {
                // Find common prefixes to explore
                set<string> next_prefixes;
                for (const string& word : result) {
                    for (int len = prefix.length() + 1; len <= word.length(); len++) {
                        string new_prefix = word.substr(0, len);
                        if (new_prefix != word) {
                            next_prefixes.insert(new_prefix);
                        }
                    }
                }
                
                for (const string& np : next_prefixes) {
                    prefixes.push(np);
                }
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