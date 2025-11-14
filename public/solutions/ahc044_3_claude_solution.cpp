#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, L;
    cin >> N >> L;
    
    vector<int> T(N);
    for (int i = 0; i < N; i++) {
        cin >> T[i];
    }
    
    vector<int> a(N), b(N);
    
    // Simple greedy approach: for each employee i, set a[i] and b[i] to employees
    // that need more cleaning assignments
    for (int i = 0; i < N; i++) {
        // Find two employees with highest target counts
        vector<pair<int, int>> targets;
        for (int j = 0; j < N; j++) {
            targets.push_back({T[j], j});
        }
        sort(targets.rbegin(), targets.rend());
        
        a[i] = targets[0].second;
        b[i] = targets[1].second;
    }
    
    // Try to optimize by simulation
    for (int iter = 0; iter < 1000; iter++) {
        vector<int> count(N, 0);
        
        // Simulate the cleaning assignment
        int current = 0;
        count[0] = 1;
        
        for (int week = 1; week < L; week++) {
            int times = count[current];
            if (times % 2 == 1) {
                current = a[current];
            } else {
                current = b[current];
            }
            count[current]++;
        }
        
        // Calculate error
        int error = 0;
        for (int i = 0; i < N; i++) {
            error += abs(count[i] - T[i]);
        }
        
        // Try to improve by modifying assignments
        bool improved = false;
        for (int i = 0; i < N && !improved; i++) {
            for (int choice = 0; choice < 2 && !improved; choice++) {
                for (int new_val = 0; new_val < N && !improved; new_val++) {
                    int old_val = (choice == 0) ? a[i] : b[i];
                    if (choice == 0) a[i] = new_val;
                    else b[i] = new_val;
                    
                    // Simulate again
                    vector<int> new_count(N, 0);
                    int new_current = 0;
                    new_count[0] = 1;
                    
                    for (int week = 1; week < L; week++) {
                        int times = new_count[new_current];
                        if (times % 2 == 1) {
                            new_current = a[new_current];
                        } else {
                            new_current = b[new_current];
                        }
                        new_count[new_current]++;
                    }
                    
                    int new_error = 0;
                    for (int j = 0; j < N; j++) {
                        new_error += abs(new_count[j] - T[j]);
                    }
                    
                    if (new_error < error) {
                        improved = true;
                        count = new_count;
                        error = new_error;
                    } else {
                        // Revert
                        if (choice == 0) a[i] = old_val;
                        else b[i] = old_val;
                    }
                }
            }
        }
        
        if (!improved) break;
    }
    
    for (int i = 0; i < N; i++) {
        cout << a[i] << " " << b[i] << "\n";
    }
    
    return 0;
}