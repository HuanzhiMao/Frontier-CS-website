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
    
    // Simple greedy approach: assign transitions based on target frequencies
    vector<pair<int, int>> freq_idx;
    for (int i = 0; i < N; i++) {
        freq_idx.push_back({T[i], i});
    }
    sort(freq_idx.rbegin(), freq_idx.rend());
    
    // Initialize with simple strategy
    for (int i = 0; i < N; i++) {
        if (T[i] == 0) {
            a[i] = b[i] = i; // Stay at self if not supposed to be assigned
        } else {
            // Find employees with high target counts to transition to
            int best_a = 0, best_b = 0;
            int max_target = -1;
            for (int j = 0; j < N; j++) {
                if (T[j] > max_target) {
                    max_target = T[j];
                    best_a = j;
                }
            }
            
            max_target = -1;
            for (int j = 0; j < N; j++) {
                if (j != best_a && T[j] > max_target) {
                    max_target = T[j];
                    best_b = j;
                }
            }
            
            a[i] = best_a;
            b[i] = (best_b == -1) ? best_a : best_b;
        }
    }
    
    // Simulate and adjust
    for (int iter = 0; iter < 1000; iter++) {
        vector<int> count(N, 0);
        
        // Simulate the cleaning assignment
        int current = 0;
        count[0]++;
        
        for (int week = 1; week < L; week++) {
            int times_assigned = count[current];
            if (times_assigned % 2 == 1) {
                current = a[current];
            } else {
                current = b[current];
            }
            count[current]++;
        }
        
        // Find the employee with maximum error
        int max_error = 0;
        int worst_employee = -1;
        for (int i = 0; i < N; i++) {
            int error = abs(count[i] - T[i]);
            if (error > max_error) {
                max_error = error;
                worst_employee = i;
            }
        }
        
        if (max_error == 0) break;
        
        // Try to improve by modifying transitions
        bool improved = false;
        for (int i = 0; i < N && !improved; i++) {
            for (int choice = 0; choice < 2 && !improved; choice++) {
                int old_val = (choice == 0) ? a[i] : b[i];
                
                for (int new_val = 0; new_val < N; new_val++) {
                    if (new_val == old_val) continue;
                    
                    if (choice == 0) a[i] = new_val;
                    else b[i] = new_val;
                    
                    // Quick simulation check
                    vector<int> new_count(N, 0);
                    int curr = 0;
                    new_count[0]++;
                    
                    for (int week = 1; week < L; week++) {
                        int times = new_count[curr];
                        if (times % 2 == 1) {
                            curr = a[curr];
                        } else {
                            curr = b[curr];
                        }
                        new_count[curr]++;
                    }
                    
                    int new_error = 0;
                    for (int j = 0; j < N; j++) {
                        new_error += abs(new_count[j] - T[j]);
                    }
                    
                    int old_error = 0;
                    for (int j = 0; j < N; j++) {
                        old_error += abs(count[j] - T[j]);
                    }
                    
                    if (new_error < old_error) {
                        improved = true;
                        break;
                    } else {
                        // Revert
                        if (choice == 0) a[i] = old_val;
                        else b[i] = old_val;
                    }
                }
            }
        }
    }
    
    for (int i = 0; i < N; i++) {
        cout << a[i] << " " << b[i] << "\n";
    }
    
    return 0;
}