#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N, L;
    cin >> N >> L;
    
    vector<int> T(N);
    for (int i = 0; i < N; i++) {
        cin >> T[i];
    }
    
    vector<int> a(N), b(N);
    
    // Simple greedy approach: try to balance the assignments
    // For each employee, point to employees with high target values
    vector<pair<int, int>> targets;
    for (int i = 0; i < N; i++) {
        targets.push_back({T[i], i});
    }
    sort(targets.rbegin(), targets.rend());
    
    // Initialize with pointing to highest target employees
    for (int i = 0; i < N; i++) {
        a[i] = targets[0].second;
        b[i] = targets[min(1, N-1)].second;
    }
    
    // Try to improve with local search
    for (int iter = 0; iter < 1000; iter++) {
        // Simulate current assignment
        vector<int> count(N, 0);
        int current = 0;
        count[0] = 1;
        
        for (int week = 1; week < L; week++) {
            int t = count[current];
            if (t % 2 == 1) {
                current = a[current];
            } else {
                current = b[current];
            }
            count[current]++;
        }
        
        // Calculate current error
        int current_error = 0;
        for (int i = 0; i < N; i++) {
            current_error += abs(count[i] - T[i]);
        }
        
        // Try random changes
        bool improved = false;
        for (int tries = 0; tries < 100; tries++) {
            int emp = rand() % N;
            int old_a = a[emp], old_b = b[emp];
            
            if (rand() % 2 == 0) {
                a[emp] = rand() % N;
            } else {
                b[emp] = rand() % N;
            }
            
            // Quick simulation to check improvement
            vector<int> new_count(N, 0);
            int new_current = 0;
            new_count[0] = 1;
            
            for (int week = 1; week < L; week++) {
                int t = new_count[new_current];
                if (t % 2 == 1) {
                    new_current = a[new_current];
                } else {
                    new_current = b[new_current];
                }
                new_count[new_current]++;
            }
            
            int new_error = 0;
            for (int i = 0; i < N; i++) {
                new_error += abs(new_count[i] - T[i]);
            }
            
            if (new_error < current_error) {
                improved = true;
                break;
            } else {
                a[emp] = old_a;
                b[emp] = old_b;
            }
        }
        
        if (!improved) break;
    }
    
    for (int i = 0; i < N; i++) {
        cout << a[i] << " " << b[i] << "\n";
    }
    
    return 0;
}