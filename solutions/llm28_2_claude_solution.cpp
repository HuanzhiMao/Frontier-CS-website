#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <algorithm>
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
        vector<string> prefixes;
        prefixes.push_back("");
        
        while (words.size() < N) {
            for (string prefix : prefixes) {
                if (words.size() >= N) break;
                
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
                
                // Generate new prefixes
                vector<string> new_prefixes;
                for (char c = 'a'; c <= 'z'; c++) {
                    string new_prefix = prefix + c;
                    if (new_prefix.length() <= 10) {
                        bool found = false;
                        for (const string& word : result) {
                            if (word.substr(0, new_prefix.length()) == new_prefix) {
                                found = true;
                                break;
                            }
                        }
                        if (found) {
                            new_prefixes.push_back(new_prefix);
                        }
                    }
                }
                
                for (const string& np : new_prefixes) {
                    prefixes.push_back(np);
                }
            }
            
            // Try binary search approach for remaining words
            if (words.size() < N) {
                for (char c = 'a'; c <= 'z' && words.size() < N; c++) {
                    string prefix = "";
                    prefix += c;
                    
                    cout << "query " << prefix << " " << N << endl;
                    cout.flush();
                    
                    int k;
                    cin >> k;
                    
                    for (int i = 0; i < k; i++) {
                        string word;
                        cin >> word;
                        words.insert(word);
                    }
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