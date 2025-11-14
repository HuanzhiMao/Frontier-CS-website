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
            
            if (k > 1) {
                set<string> next_prefixes;
                for (const string& word : result) {
                    for (int len = prefix.length() + 1; len <= word.length(); len++) {
                        string new_prefix = word.substr(0, len);
                        if (new_prefix.length() <= 10) {
                            next_prefixes.insert(new_prefix);
                        }
                    }
                }
                for (const string& p : next_prefixes) {
                    prefixes.push(p);
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