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
    
    // Initialize with simple strategy - point to employees with highest targets
    vector<pair<int, int>> targets;
    for (int i = 0; i < N; i++) {
        targets.push_back({T[i], i});
    }
    sort(targets.rbegin(), targets.rend());
    
    for (int i = 0; i < N; i++) {
        a[i] = targets[0].second;
        b[i] = targets[min(1, N-1)].second;
    }
    
    // Simulate and improve iteratively
    for (int iter = 0; iter < 1000; iter++) {
        vector<int> count(N, 0);
        vector<int> times_assigned(N, 0);
        
        // Simulate the cleaning schedule
        int current = 0;
        count[0] = 1;
        times_assigned[0] = 1;
        
        for (int week = 1; week < L; week++) {
            int next;
            if (times_assigned[current] % 2 == 1) {
                next = a[current];
            } else {
                next = b[current];
            }
            count[next]++;
            times_assigned[current]++;
            current = next;
        }
        
        // Calculate current error
        int error = 0;
        for (int i = 0; i < N; i++) {
            error += abs(count[i] - T[i]);
        }
        
        if (error == 0) break;
        
        // Try to improve by adjusting transitions
        bool improved = false;
        for (int i = 0; i < N && !improved; i++) {
            for (int choice = 0; choice < 2 && !improved; choice++) {
                int old_val = (choice == 0) ? a[i] : b[i];
                
                for (int new_val = 0; new_val < N; new_val++) {
                    if (new_val == old_val) continue;
                    
                    // Try this change
                    if (choice == 0) {
                        a[i] = new_val;
                    } else {
                        b[i] = new_val;
                    }
                    
                    // Quick simulation to check if it's better
                    vector<int> new_count(N, 0);
                    vector<int> new_times_assigned(N, 0);
                    
                    int curr = 0;
                    new_count[0] = 1;
                    new_times_assigned[0] = 1;
                    
                    for (int week = 1; week < min(L, 50000); week++) {
                        int next;
                        if (new_times_assigned[curr] % 2 == 1) {
                            next = a[curr];
                        } else {
                            next = b[curr];
                        }
                        new_count[next]++;
                        new_times_assigned[curr]++;
                        curr = next;
                    }
                    
                    int new_error = 0;
                    for (int j = 0; j < N; j++) {
                        new_error += abs(new_count[j] * L / 50000 - T[j]);
                    }
                    
                    if (new_error < error * 50000 / L) {
                        improved = true;
                        break;
                    }
                    
                    // Revert change
                    if (choice == 0) {
                        a[i] = old_val;
                    } else {
                        b[i] = old_val;
                    }
                }
            }
        }
        
        if (!improved) break;
    }
    
    // Output the result
    for (int i = 0; i < N; i++) {
        cout << a[i] << " " << b[i] << "\n";
    }
    
    return 0;
}