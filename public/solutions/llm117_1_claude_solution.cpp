#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<pair<int, int>> lines;
    
    // Query points to find lines
    vector<pair<long long, long long>> query_points;
    vector<double> distances;
    
    // Use a grid of points to sample
    for (int i = -20; i <= 20; i++) {
        for (int j = -20; j <= 20; j++) {
            long long x = (long long)i * 50000000000LL;
            long long y = (long long)j * 50000000000LL;
            
            if (abs(x) <= 1000000000000LL && abs(y) <= 1000000000000LL) {
                query_points.push_back({x, y});
                
                cout << "? " << x << " " << y << endl;
                cout.flush();
                
                double dist;
                cin >> dist;
                distances.push_back(dist);
                
                if (query_points.size() >= 400) break;
            }
        }
        if (query_points.size() >= 400) break;
    }
    
    // Try all possible combinations of a and b values
    vector<pair<int, int>> candidates;
    
    for (int a = -10000; a <= 10000; a++) {
        for (int b = -10000; b <= 10000; b++) {
            bool valid = true;
            
            // Check if this line is consistent with some queries
            for (int i = 0; i < min(10, (int)query_points.size()) && valid; i++) {
                long long x = query_points[i].first;
                long long y = query_points[i].second;
                
                double line_dist = abs((long long)a * x - y + b) / sqrt((long long)a * a + 1);
                
                if (line_dist > distances[i] + 1e-6) {
                    valid = false;
                }
            }
            
            if (valid) {
                candidates.push_back({a, b});
            }
            
            if (candidates.size() >= n * 3) break;
        }
        if (candidates.size() >= n * 3) break;
    }
    
    // Select best n lines using greedy approach
    vector<pair<int, int>> result;
    vector<bool> used(candidates.size(), false);
    
    while (result.size() < n && result.size() < candidates.size()) {
        int best_idx = -1;
        double best_error = 1e18;
        
        for (int i = 0; i < candidates.size(); i++) {
            if (used[i]) continue;
            
            vector<pair<int, int>> temp_result = result;
            temp_result.push_back(candidates[i]);
            
            double total_error = 0;
            for (int j = 0; j < min(20, (int)query_points.size()); j++) {
                long long x = query_points[j].first;
                long long y = query_points[j].second;
                
                double calc_dist = 0;
                for (auto& line : temp_result) {
                    calc_dist += abs((long long)line.first * x - y + line.second) / sqrt((long long)line.first * line.first + 1);
                }
                
                total_error += abs(calc_dist - distances[j]);
            }
            
            if (total_error < best_error) {
                best_error = total_error;
                best_idx = i;
            }
        }
        
        if (best_idx != -1) {
            result.push_back(candidates[best_idx]);
            used[best_idx] = true;
        } else {
            break;
        }
    }
    
    // Fill remaining with first unused candidates
    while (result.size() < n) {
        for (int i = 0; i < candidates.size() && result.size() < n; i++) {
            if (!used[i]) {
                result.push_back(candidates[i]);
                used[i] = true;
            }
        }
        if (result.size() == candidates.size()) break;
    }
    
    cout << "!";
    for (auto& line : result) {
        cout << " " << line.first;
    }
    for (auto& line : result) {
        cout << " " << line.second;
    }
    cout << endl;
    cout.flush();
    
    return 0;
}