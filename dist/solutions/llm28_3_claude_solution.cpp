#include <bits/stdc++.h>
using namespace stdin;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int T;
    cin >> T;
    
    while (T--) {
        int N;
        cin >> N;
        
        set<string> words;
        map<string, set<string>> prefix_cache;
        
        function<void(string, int)> explore;
        explore = [&](string prefix, int depth) {
            if (words.size() == N || depth > 10) return;
            
            cout << "query " << prefix << " " << N << endl;
            cout.flush();
            
            int count;
            cin >> count;
            
            vector<string> result(count);
            for (int i = 0; i < count; i++) {
                cin >> result[i];
                words.insert(result[i]);
            }
            
            if (count == 0) return;
            
            prefix_cache[prefix] = set<string>(result.begin(), result.end());
            
            if (words.size() == N) return;
            
            set<char> next_chars;
            for (const string& word : result) {
                if (word.length() > prefix.length()) {
                    next_chars.insert(word[prefix.length()]);
                }
            }
            
            for (char c : next_chars) {
                if (words.size() == N) break;
                explore(prefix + c, depth + 1);
            }
        };
        
        explore("", 0);
        
        cout << "answer";
        for (const string& word : words) {
            cout << " " << word;
        }
        cout << endl;
        cout.flush();
    }
    
    return 0;
}