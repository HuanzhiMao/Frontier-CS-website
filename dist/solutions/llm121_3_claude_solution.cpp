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
        string& s = strings[str_idx];
        
        // Count question marks
        vector<int> question_positions;
        for (int i = 0; i < n; i++) {
            if (s[i] == '?') {
                question_positions.push_back(i);
            }
        }
        
        int num_questions = question_positions.size();
        int num_combinations = 1;
        for (int i = 0; i < num_questions; i++) {
            num_combinations *= 4;
        }
        
        // Generate all combinations
        for (int comb = 0; comb < num_combinations; comb++) {
            string candidate = s;
            int temp_comb = comb;
            
            for (int i = 0; i < num_questions; i++) {
                int pos = question_positions[i];
                int choice = temp_comb % 4;
                temp_comb /= 4;
                
                if (choice == 0) candidate[pos] = 'A';
                else if (choice == 1) candidate[pos] = 'C';
                else if (choice == 2) candidate[pos] = 'G';
                else candidate[pos] = 'T';
            }
            
            valid_sequences.insert(candidate);
        }
    }
    
    double probability = (double)valid_sequences.size() / total_sequences;
    cout << fixed << setprecision(10) << probability << endl;
    
    return 0;
}