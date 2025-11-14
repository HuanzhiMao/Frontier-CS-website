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
    for (int str_idx = 0; str_idx < m; str_idx++) {
        string current = strings[str_idx];
        vector<int> question_positions;
        
        // Find positions of '?'
        for (int i = 0; i < n; i++) {
            if (current[i] == '?') {
                question_positions.push_back(i);
            }
        }
        
        int num_questions = question_positions.size();
        int replacements = 1;
        for (int i = 0; i < num_questions; i++) {
            replacements *= 4;
        }
        
        // Generate all possible replacements
        for (int mask = 0; mask < replacements; mask++) {
            string temp = current;
            int temp_mask = mask;
            
            for (int i = 0; i < num_questions; i++) {
                int pos = question_positions[i];
                int char_choice = temp_mask % 4;
                temp_mask /= 4;
                
                if (char_choice == 0) temp[pos] = 'A';
                else if (char_choice == 1) temp[pos] = 'C';
                else if (char_choice == 2) temp[pos] = 'G';
                else temp[pos] = 'T';
            }
            
            valid_sequences.insert(temp);
        }
    }
    
    double probability = (double)valid_sequences.size() / total_sequences;
    cout << fixed << setprecision(10) << probability << endl;
    
    return 0;
}