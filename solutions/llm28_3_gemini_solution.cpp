#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <queue>
#include <algorithm>
#include <map>

using namespace std;

void solve() {
    int n;
    cin >> n;

    set<string> found_words;
    queue<string> q;
    set<string> q_set;
    
    for (char c = 'a'; c <= 'z'; ++c) {
        string s(1, c);
        q.push(s);
        q_set.insert(s);
    }
    
    map<string, int> num_found_for_prefix;

    while (found_words.size() < n) {
        if (q.empty()) {
            break;
        }

        string prefix = q.front();
        q.pop();
        q_set.erase(prefix);

        int known_count = 0;
        if (num_found_for_prefix.count(prefix)) {
            known_count = num_found_for_prefix[prefix];
        }
        
        int k_query = (known_count == 0) ? 1 : known_count * 2;
        k_query = min(k_query, n);

        cout << "query " << prefix << " " << k_query << endl;

        int k_response;
        cin >> k_response;

        if (k_response == 0) {
            continue;
        }

        vector<string> words(k_response);
        for (int i = 0; i < k_response; ++i) {
            cin >> words[i];
            found_words.insert(words[i]);
        }

        num_found_for_prefix[prefix] = k_response;

        if (k_response == k_query && found_words.size() < n) {
            // Potentially more words with this prefix
            if (q_set.find(prefix) == q_set.end()) {
                q.push(prefix);
                q_set.insert(prefix);
            }

            // Explore child prefix of the last returned word
            const string& last_word = words.back();
            if (last_word.length() > prefix.length() && prefix.length() < 10) {
                string child_prefix = last_word.substr(0, prefix.length() + 1);
                if (q_set.find(child_prefix) == q_set.end()) {
                    q.push(child_prefix);
                    q_set.insert(child_prefix);
                }
            }
        }
    }

    cout << "answer";
    for (const string& word : found_words) {
        cout << " " << word;
    }
    cout << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int t;
    cin >> t;
    while (t--) {
        solve();
    }

    return 0;
}