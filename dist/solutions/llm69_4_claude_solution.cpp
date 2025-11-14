#include <bits/stdc++.h>
using namespace std;

int countDistinctSubstrings(const string& s) {
    set<string> substrings;
    int n = s.length();
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            substrings.insert(s.substr(i, j - i + 1));
        }
    }
    return substrings.size();
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<string> words(n + 1);
    map<int, pair<int, int>> powerToIndices;
    
    // Generate n distinct words
    for (int i = 1; i <= n; i++) {
        string word = "";
        int temp = i;
        while (temp > 0) {
            if (temp % 2 == 0) {
                word = "O" + word;
            } else {
                word = "X" + word;
            }
            temp /= 2;
        }
        words[i] = word;
        cout << word << "\n";
    }
    cout.flush();
    
    // Precompute all possible powers
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            string spell = words[i] + words[j];
            int power = countDistinctSubstrings(spell);
            powerToIndices[power] = {i, j};
        }
    }
    
    int q;
    cin >> q;
    
    for (int query = 0; query < q; query++) {
        int p;
        cin >> p;
        
        auto it = powerToIndices.find(p);
        cout << it->second.first << " " << it->second.second << "\n";
        cout.flush();
    }
    
    return 0;
}