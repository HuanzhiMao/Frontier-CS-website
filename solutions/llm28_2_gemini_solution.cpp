#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include <map>
#include <functional>

using namespace std;

set<string> found_words;

// Recursive function to explore prefixes
void explore(string prefix, const vector<string>& words, int n_total) {
    if (words.empty() || prefix.length() >= 10) {
        return;
    }

    // Group words by the next character, forming child prefixes
    map<string, vector<string>> children_words;
    for (const string& w : words) {
        if (w.length() > prefix.length()) {
            children_words[w.substr(0, prefix.length() + 1)].push_back(w);
        }
    }
    
    for (auto const& [child_prefix, child_word_list] : children_words) {
        // Recurse only if it's a branching point.
        // A simple heuristic: if a child prefix doesn't account for all words of the parent prefix,
        // it means there's branching (or some words are shorter). This is a good candidate for a detailed query.
        if (child_word_list.size() < words.size()) {
            cout << "query " << child_prefix << " " << n_total << endl;
            int k;
            cin >> k;
            vector<string> new_words(k);
            for(int i = 0; i < k; ++i) {
                cin >> new_words[i];
                found_words.insert(new_words[i]);
            }
            explore(child_prefix, new_words, n_total);
        }
    }
}

void solve() {
    int n;
    cin >> n;

    found_words.clear();
    
    // Iterate through all possible first letters
    for (char c = 'a'; c <= 'z'; ++c) {
        if (found_words.size() == n) break;
        
        string prefix(1, c);
        
        // Check if any word starts with this prefix
        cout << "query " << prefix << " " << 1 << endl;
        int k1;
        cin >> k1;
        if (k1 > 0) {
            string first_word;
            cin >> first_word; 
            
            // If a word exists, query for all words with this prefix.
            // Using N as K is a safe upper bound.
            cout << "query " << prefix << " " << n << endl;
            int k2;
            cin >> k2;
            vector<string> words(k2);
            for(int i = 0; i < k2; ++i) {
                cin >> words[i];
                found_words.insert(words[i]);
            }
            // Recurse to explore deeper prefixes
            explore(prefix, words, n);
        }
    }

    cout << "answer";
    vector<string> sorted_words(found_words.begin(), found_words.end());
    for (const string& s : sorted_words) {
        cout << " " << s;
    }
    cout << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.flush();
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}