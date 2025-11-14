#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <algorithm>

// Use a standard namespace to make code cleaner
using namespace std;

// Global set to store all unique words found so far
set<string> found_words;
// The total number of words to find, read at the start of each test case
int N;

// Helper function to send a query to the judge and parse the response.
// It takes a prefix and the number K, prints the query, reads the response,
// and returns a vector of the words received.
vector<string> make_query(const string& prefix, int k) {
    // It's pointless to query with K=0 or less.
    if (k <= 0) {
        return {};
    }
    // Print the query in the specified format and flush the output.
    cout << "query " << prefix << " " << k << endl;
    
    // Read the number of words returned by the judge.
    int count_received;
    cin >> count_received;
    
    // Read the words themselves and store them in a vector.
    vector<string> words(count_received);
    for (int i = 0; i < count_received; ++i) {
        cin >> words[i];
    }
    return words;
}

// This function recursively explores the trie of words.
// It's called for a prefix when a query for it might not have returned all words.
// 'prefix': the prefix we are currently exploring (e.g., "a", "ap", "appl").
// 'words_from_parent': the words starting with 'prefix' that we received from the parent's query.
void explore(const string& prefix, const vector<string>& words_from_parent) {
    // Stop recursion if prefixes get too long (as per problem constraints).
    if (prefix.length() >= 10) {
        return;
    }

    // Group the words received from the parent query by their next character.
    // This identifies the children of the current prefix node in the trie.
    map<char, vector<string>> groups;
    for (const string& word : words_from_parent) {
        if (word.length() > prefix.length()) {
            groups[word[prefix.length()]].push_back(word);
        }
    }

    // Iterate over each child prefix.
    for (auto const& [next_char, child_words] : groups) {
        string next_prefix = prefix + next_char;
        if (next_prefix.length() > 10) continue;
        
        // The number of words we saw for this child is the best upper bound we have for K.
        int k_to_query = child_words.size();
        vector<string> result = make_query(next_prefix, k_to_query);

        // Add any new words found to our global set.
        for (const string& w : result) {
            found_words.insert(w);
        }

        // If the number of words returned equals K, it means there might be even more
        // words with this prefix. We must recurse deeper to find them.
        if (result.size() == k_to_query) {
            explore(next_prefix, result);
        }
    }
}


void solve_test_case() {
    cin >> N;
    found_words.clear();

    int remaining_to_find = N;
    char last_char = 'a' - 1;

    // Phase 1: Bootstrap by finding the first character of all word groups.
    while (remaining_to_find > 0 && last_char < 'z') {
        // Binary search for the lexicographically smallest character (greater than last_char)
        // that starts at least one word. This is more efficient than checking 'a' through 'z'.
        char next_char_to_probe = 0;
        char low = last_char + 1;
        char high = 'z';

        while (low <= high) {
            char mid = low + (high - low) / 2;
            vector<string> res = make_query(string(1, mid), 1);
            if (!res.empty()) {
                // This character exists. It could be the first, so search left to be sure.
                next_char_to_probe = mid;
                high = mid - 1;
            } else {
                // No words start with this character, search right.
                low = mid + 1;
            }
        }

        if (next_char_to_probe == 0) {
            // No more first characters found, we should be done.
            break;
        }

        // We found the next active first character. Now query for all words starting with it.
        // We use 'remaining_to_find' as K, as it's the maximum possible number of such words.
        int k_for_probe = remaining_to_find;
        vector<string> result = make_query(string(1, next_char_to_probe), k_for_probe);
        
        for (const string& w : result) {
            found_words.insert(w);
        }
        
        remaining_to_find -= result.size();
        last_char = next_char_to_probe;

        // If the query returned as many words as we asked for (k == K),
        // it means this branch is a "frontier" and might not be fully explored.
        // We must call explore() to investigate its children.
        if (result.size() == k_for_probe && !result.empty()) {
            explore(string(1, next_char_to_probe), result);
        }
    }

    // Phase 2: Output the answer.
    cout << "answer";
    for (const string& s : found_words) {
        cout << " " << s;
    }
    cout << endl;
}

int main() {
    // Fast I/O is good practice in competitive programming.
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int T;
    cin >> T;
    while (T--) {
        solve_test_case();
    }

    return 0;
}