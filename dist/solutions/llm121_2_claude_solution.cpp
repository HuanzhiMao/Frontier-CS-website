#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m;
    cin >> n >> m;
    
    vector<string> patterns(m);
    for (int i = 0; i < m; i++) {
        cin >> patterns[i];
    }
    
    int total_sequences = 1;
    for (int i = 0; i < n; i++) {
        total_sequences *= 4;
    }
    
    set<string> valid_sequences;
    
    // For each pattern, generate all possible DNA sequences
    for (int p = 0; p < m; p++) {
        vector<int> question_positions;
        for (int i = 0; i < n; i++) {
            if (patterns[p][i] == '?') {
                question_positions.push_back(i);
            }
        }
        
        int num_questions = question_positions.size();
        int replacements = 1;
        for (int i = 0; i < num_questions; i++) {
            replacements *= 4;
        }
        
        // Generate all possible replacements
        for (int r = 0; r < replacements; r++) {
            string current = patterns[p];
            int temp = r;
            for (int i = 0; i < num_questions; i++) {
                int char_idx = temp % 4;
                temp /= 4;
                char replacement;
                if (char_idx == 0) replacement = 'A';
                else if (char_idx == 1) replacement = 'C';
                else if (char_idx == 2) replacement = 'G';
                else replacement = 'T';
                
                current[question_positions[i]] = replacement;
            }
            valid_sequences.insert(current);
        }
    }
    
    double probability = (double)valid_sequences.size() / total_sequences;
    cout << fixed << setprecision(10) << probability << endl;
    
    return 0;
}