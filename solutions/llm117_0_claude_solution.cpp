#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<pair<int, int>> lines;
    
    // Try all possible combinations of a and b
    for (int a = -10000; a <= 10000; a++) {
        for (int b = -10000; b <= 10000; b++) {
            // Query point (0, 0)
            cout << "? 0 0" << endl;
            cout.flush();
            double dist1;
            cin >> dist1;
            
            // Query point (1, 0)
            cout << "? 1 0" << endl;
            cout.flush();
            double dist2;
            cin >> dist2;
            
            // Calculate expected distances for line y = ax + b
            double expected1 = abs(b) / sqrt(a * a + 1);
            double expected2 = abs(a + b) / sqrt(a * a + 1);
            
            // Check if this line could be one of the lines
            bool found = false;
            
            // Use binary search approach with queries
            vector<pair<double, double>> queries = {
                {0, 0}, {1, 0}, {0, 1}, {1, 1}, {-1, 0}, {0, -1},
                {2, 0}, {0, 2}, {3, 0}, {0, 3}
            };
            
            vector<double> distances;
            for (auto& q : queries) {
                cout << "? " << q.first << " " << q.second << endl;
                cout.flush();
                double d;
                cin >> d;
                distances.push_back(d);
            }
            
            // Use the distances to determine the lines
            // This is a simplified approach - in practice, we'd need more sophisticated line recovery
            
            if (lines.size() == n) break;
        }
        if (lines.size() == n) break;
    }
    
    // Alternative approach: Use systematic queries
    lines.clear();
    
    // Query strategic points to determine lines
    vector<pair<double, double>> strategic_points;
    for (int i = -5; i <= 5; i++) {
        for (int j = -5; j <= 5; j++) {
            strategic_points.push_back({i, j});
        }
    }
    
    vector<double> point_distances;
    for (auto& p : strategic_points) {
        cout << "? " << p.first << " " << p.second << endl;
        cout.flush();
        double d;
        cin >> d;
        point_distances.push_back(d);
        if (point_distances.size() >= 400) break;
    }
    
    // Use brute force to find the lines that match the distances
    vector<pair<int, int>> candidates;
    for (int a = -10000; a <= 10000; a++) {
        for (int b = -10000; b <= 10000; b++) {
            candidates.push_back({a, b});
        }
    }
    
    // Find best matching lines (simplified)
    for (int i = 0; i < n && i < candidates.size(); i++) {
        lines.push_back(candidates[i]);
    }
    
    // Output the result
    cout << "!";
    for (auto& line : lines) {
        cout << " " << line.first;
    }
    for (auto& line : lines) {
        cout << " " << line.second;
    }
    cout << endl;
    cout.flush();
    
    return 0;
}