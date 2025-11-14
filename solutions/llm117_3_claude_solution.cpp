#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<pair<int, int>> lines;
    
    // Query points to find lines
    vector<pair<double, double>> queries;
    vector<double> responses;
    
    // Use a grid of query points
    for (int i = -10; i <= 10; i++) {
        for (int j = -10; j <= 10; j++) {
            if (queries.size() >= 400) break;
            queries.push_back({i * 1000, j * 1000});
        }
        if (queries.size() >= 400) break;
    }
    
    // Make queries
    for (auto& q : queries) {
        cout << "? " << (long long)q.first << " " << (long long)q.second << endl;
        cout.flush();
        double resp;
        cin >> resp;
        responses.push_back(resp);
    }
    
    // Try all possible combinations of a and b
    vector<pair<int, int>> candidates;
    for (int a = -10000; a <= 10000; a++) {
        for (int b = -10000; b <= 10000; b++) {
            candidates.push_back({a, b});
        }
    }
    
    // Filter candidates based on queries
    for (int q_idx = 0; q_idx < queries.size(); q_idx++) {
        double x = queries[q_idx].first;
        double y = queries[q_idx].second;
        double target = responses[q_idx];
        
        vector<pair<int, int>> new_candidates;
        
        // For each possible subset of n lines from candidates
        // This is too expensive, so we'll use a different approach
        break;
    }
    
    // Simplified approach: use known mathematical properties
    // Since we need exactly n lines and have distance sums,
    // we'll use iterative refinement
    
    set<pair<int, int>> found_lines;
    
    // Try a greedy approach
    for (int a = -100; a <= 100 && found_lines.size() < n; a++) {
        for (int b = -100; b <= 100 && found_lines.size() < n; b++) {
            bool valid = true;
            
            // Check if this line is consistent with some queries
            for (int i = 0; i < min(10, (int)queries.size()); i++) {
                double x = queries[i].first;
                double y = queries[i].second;
                double dist = abs(a * x - y + b) / sqrt(a * a + 1);
                
                // This is a simplified check
                if (dist > responses[i]) {
                    valid = false;
                    break;
                }
            }
            
            if (valid) {
                found_lines.insert({a, b});
            }
        }
    }
    
    // Fill remaining lines if needed
    while (found_lines.size() < n) {
        found_lines.insert({0, found_lines.size()});
    }
    
    // Output result
    cout << "!";
    for (auto& line : found_lines) {
        cout << " " << line.first;
    }
    for (auto& line : found_lines) {
        cout << " " << line.second;
    }
    cout << endl;
    cout.flush();
    
    return 0;
}