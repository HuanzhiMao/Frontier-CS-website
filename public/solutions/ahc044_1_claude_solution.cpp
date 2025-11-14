#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, L;
    cin >> N >> L;
    
    vector<int> T(N);
    for (int i = 0; i < N; i++) {
        cin >> T[i];
    }
    
    vector<int> a(N), b(N);
    
    // Simple greedy approach: for each employee, set a[i] and b[i] to employees with highest targets
    vector<pair<int, int>> targets;
    for (int i = 0; i < N; i++) {
        targets.push_back({T[i], i});
    }
    sort(targets.rbegin(), targets.rend());
    
    for (int i = 0; i < N; i++) {
        if (T[i] > 0) {
            // Point to employees with high targets
            a[i] = targets[0].second;
            b[i] = targets[min(1, N-1)].second;
        } else {
            // Point to self or low target employees
            a[i] = i;
            b[i] = i;
        }
    }
    
    // Simulate and improve
    for (int iter = 0; iter < 1000; iter++) {
        vector<int> count(N, 0);
        int current = 0;
        vector<int> employee_count(N, 0);
        
        // Simulate the process
        for (int week = 0; week < L; week++) {
            count[current]++;
            employee_count[current]++;
            
            if (week < L - 1) {
                if (employee_count[current] % 2 == 1) {
                    current = a[current];
                } else {
                    current = b[current];
                }
            }
        }
        
        // Calculate current error
        int error = 0;
        for (int i = 0; i < N; i++) {
            error += abs(count[i] - T[i]);
        }
        
        // Try to improve by modifying random a[i] or b[i]
        int best_error = error;
        int best_i = -1, best_val = -1, best_type = -1;
        
        for (int i = 0; i < N; i++) {
            for (int new_val = 0; new_val < N; new_val++) {
                // Try changing a[i]
                int old_a = a[i];
                a[i] = new_val;
                
                vector<int> new_count(N, 0);
                vector<int> new_employee_count(N, 0);
                int new_current = 0;
                
                for (int week = 0; week < L; week++) {
                    new_count[new_current]++;
                    new_employee_count[new_current]++;
                    
                    if (week < L - 1) {
                        if (new_employee_count[new_current] % 2 == 1) {
                            new_current = a[new_current];
                        } else {
                            new_current = b[new_current];
                        }
                    }
                }
                
                int new_error = 0;
                for (int j = 0; j < N; j++) {
                    new_error += abs(new_count[j] - T[j]);
                }
                
                if (new_error < best_error) {
                    best_error = new_error;
                    best_i = i;
                    best_val = new_val;
                    best_type = 0;
                }
                
                a[i] = old_a;
                
                // Try changing b[i]
                int old_b = b[i];
                b[i] = new_val;
                
                fill(new_count.begin(), new_count.end(), 0);
                fill(new_employee_count.begin(), new_employee_count.end(), 0);
                new_current = 0;
                
                for (int week = 0; week < L; week++) {
                    new_count[new_current]++;
                    new_employee_count[new_current]++;
                    
                    if (week < L - 1) {
                        if (new_employee_count[new_current] % 2 == 1) {
                            new_current = a[new_current];
                        } else {
                            new_current = b[new_current];
                        }
                    }
                }
                
                new_error = 0;
                for (int j = 0; j < N; j++) {
                    new_error += abs(new_count[j] - T[j]);
                }
                
                if (new_error < best_error) {
                    best_error = new_error;
                    best_i = i;
                    best_val = new_val;
                    best_type = 1;
                }
                
                b[i] = old_b;
            }
        }
        
        if (best_i != -1) {
            if (best_type == 0) {
                a[best_i] = best_val;
            } else {
                b[best_i] = best_val;
            }
        } else {
            break;
        }
    }
    
    for (int i = 0; i < N; i++) {
        cout << a[i] << " " << b[i] << "\n";
    }
    
    return 0;
}