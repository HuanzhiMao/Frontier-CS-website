#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<pair<int, int>> lines;
    
    // Query points in a grid pattern to gather information
    vector<tuple<double, double, double>> queries;
    
    // Use a smaller grid to stay within query limits
    int grid_size = min(20, (int)sqrt(10000 / n));
    
    for (int i = -grid_size; i <= grid_size; i++) {
        for (int j = -grid_size; j <= grid_size; j++) {
            double x = i * 1e10 / grid_size;
            double y = j * 1e10 / grid_size;
            
            cout << "? " << fixed << setprecision(1) << x << " " << y << endl;
            cout.flush();
            
            double dist;
            cin >> dist;
            
            queries.push_back({x, y, dist});
        }
    }
    
    // Try all possible combinations of slopes and intercepts
    vector<pair<int, int>> candidates;
    
    for (int a = -10000; a <= 10000; a++) {
        for (int b = -10000; b <= 10000; b++) {
            bool valid = true;
            double total_error = 0;
            
            for (auto& q : queries) {
                double x = get<0>(q);
                double y = get<1>(q);
                double expected_dist = get<2>(q);
                
                double line_dist = abs(a * x - y + b) / sqrt(a * a + 1);
                
                if (candidates.size() < n) {
                    // Still building candidates
                    continue;
                }
                
                double calc_dist = 0;
                for (auto& line : candidates) {
                    double d = abs(line.first * x - y + line.second) / sqrt(line.first * line.first + 1);
                    calc_dist += d;
                }
                
                total_error += abs(calc_dist - expected_dist);
            }
            
            if (candidates.size() < n) {
                candidates.push_back({a, b});
            }
            
            if (candidates.size() == n) {
                break;
            }
        }
        if (candidates.size() == n) {
            break;
        }
    }
    
    // Use a simpler approach - random search
    srand(42);
    candidates.clear();
    
    for (int iter = 0; iter < n && candidates.size() < n; iter++) {
        int best_a = 0, best_b = 0;
        double best_error = 1e18;
        
        for (int trial = 0; trial < 1000 && candidates.size() < n; trial++) {
            int a = rand() % 20001 - 10000;
            int b = rand() % 20001 - 10000;
            
            double error = 0;
            for (auto& q : queries) {
                double x = get<0>(q);
                double y = get<1>(q);
                double expected_dist = get<2>(q);
                
                double calc_dist = 0;
                for (auto& line : candidates) {
                    double d = abs(line.first * x - y + line.second) / sqrt(line.first * line.first + 1);
                    calc_dist += d;
                }
                
                double line_dist = abs(a * x - y + b) / sqrt(a * a + 1);
                calc_dist += line_dist;
                
                error += abs(calc_dist - expected_dist);
            }
            
            if (error < best_error) {
                best_error = error;
                best_a = a;
                best_b = b;
            }
        }
        
        candidates.push_back({best_a, best_b});
    }
    
    cout << "!";
    for (auto& line : candidates) {
        cout << " " << line.first;
    }
    for (auto& line : candidates) {
        cout << " " << line.second;
    }
    cout << endl;
    
    return 0;
}