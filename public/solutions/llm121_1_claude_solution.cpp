#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m;
    cin >> n >> m;
    
    vector<string> strings(m);
    for (int i = 0; i < m; i++) {
        cin >> strings[i];
    }
    
    int total_sequences = 1;
    for (int i = 0; i < n; i++) {
        total_sequences *= 4;
    }
    
    set<string> valid_sequences;
    
    // For each string, generate all possible DNA sequences it can match
    for (int i = 0; i < m; i++) {
        string s = strings[i];
        vector<int> question_positions;
        
        // Find positions of '?'
        for (int j = 0; j < n; j++) {
            if (s[j] == '?') {
                question_positions.push_back(j);
            }
        }
        
        int num_questions = question_positions.size();
        int possibilities = 1;
        for (int j = 0; j < num_questions; j++) {
            possibilities *= 4;
        }
        
        // Generate all possible replacements
        for (int mask = 0; mask < possibilities; mask++) {
            string current = s;
            int temp_mask = mask;
            
            for (int j = 0; j < num_questions; j++) {
                int choice = temp_mask % 4;
                temp_mask /= 4;
                
                char replacement;
                if (choice == 0) replacement = 'A';
                else if (choice == 1) replacement = 'C';
                else if (choice == 2) replacement = 'G';
                else replacement = 'T';
                
                current[question_positions[j]] = replacement;
            }
            
            valid_sequences.insert(current);
        }
    }
    
    double probability = (double)valid_sequences.size() / total_sequences;
    cout << fixed << setprecision(10) << probability << endl;
    
    return 0;
}