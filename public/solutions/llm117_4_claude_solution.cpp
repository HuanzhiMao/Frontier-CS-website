#include <bits/stdc++.h>
using namespace std;

double query(double x, double y) {
    cout << "? " << fixed << setprecision(10) << x << " " << y << endl;
    double result;
    cin >> result;
    return result;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<pair<int, int>> lines;
    
    // Try all possible combinations of a and b
    for (int a = -10000; a <= 10000; a++) {
        for (int b = -10000; b <= 10000; b++) {
            if (lines.size() == n) break;
            
            // Test if line y = ax + b is one of the lines
            double x1 = 0, y1 = b;
            double x2 = 1, y2 = a + b;
            double x3 = -1, y3 = -a + b;
            
            // Make sure points are within bounds
            if (abs(y1) > 1e12 || abs(y2) > 1e12 || abs(y3) > 1e12) continue;
            
            double d1 = query(x1, y1);
            double d2 = query(x2, y2);
            double d3 = query(x3, y3);
            
            // If this line exists, the distances should be very small at these points
            if (d1 < 1e-6 || d2 < 1e-6 || d3 < 1e-6) {
                lines.push_back({a, b});
            }
            
            if (lines.size() == n) break;
        }
        if (lines.size() == n) break;
    }
    
    // If we couldn't find enough lines with the simple approach,
    // use a more sophisticated method
    if (lines.size() < n) {
        lines.clear();
        
        // Use random sampling approach
        for (int iter = 0; iter < 1000 && lines.size() < n; iter++) {
            double x = (rand() % 2001 - 1000) * 1e9;
            double y = (rand() % 2001 - 1000) * 1e9;
            
            double dist = query(x, y);
            
            // Try to find lines that pass close to this point
            for (int a = -100; a <= 100; a++) {
                for (int b = -100; b <= 100; b++) {
                    double line_dist = abs(a * x - y + b) / sqrt(a * a + 1);
                    if (line_dist < dist * 0.1) {
                        bool found = false;
                        for (auto& line : lines) {
                            if (line.first == a && line.second == b) {
                                found = true;
                                break;
                            }
                        }
                        if (!found) {
                            lines.push_back({a, b});
                            if (lines.size() == n) break;
                        }
                    }
                }
                if (lines.size() == n) break;
            }
        }
    }
    
    // Fill remaining with default values if needed
    while (lines.size() < n) {
        lines.push_back({0, 0});
    }
    
    cout << "!";
    for (int i = 0; i < n; i++) {
        cout << " " << lines[i].first;
    }
    for (int i = 0; i < n; i++) {
        cout << " " << lines[i].second;
    }
    cout << endl;
    
    return 0;
}