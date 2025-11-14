#include <bits/stdc++.h>
using namespace std;

int main() {
    int N;
    cin >> N;
    
    vector<vector<int>> h(N, vector<int>(N));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> h[i][j];
        }
    }
    
    vector<string> operations;
    int load = 0;
    int x = 0, y = 0;
    
    // Simple snake-like path traversal
    for (int i = 0; i < N; i++) {
        if (i % 2 == 0) {
            // Move right
            for (int j = 0; j < N; j++) {
                if (i == 0 && j == 0) {
                    // Starting position
                } else {
                    operations.push_back("R");
                    y++;
                }
                
                // Handle current cell
                if (h[i][j] > 0) {
                    // Load soil
                    operations.push_back("+" + to_string(h[i][j]));
                    load += h[i][j];
                } else if (h[i][j] < 0) {
                    // Unload soil
                    int unload = min(load, -h[i][j]);
                    if (unload > 0) {
                        operations.push_back("-" + to_string(unload));
                        load -= unload;
                        h[i][j] += unload;
                    }
                }
            }
        } else {
            // Move left
            for (int j = N-1; j >= 0; j--) {
                operations.push_back("L");
                y--;
                
                // Handle current cell
                if (h[i][j] > 0) {
                    // Load soil
                    operations.push_back("+" + to_string(h[i][j]));
                    load += h[i][j];
                } else if (h[i][j] < 0) {
                    // Unload soil
                    int unload = min(load, -h[i][j]);
                    if (unload > 0) {
                        operations.push_back("-" + to_string(unload));
                        load -= unload;
                        h[i][j] += unload;
                    }
                }
            }
        }
        
        // Move down to next row (except for last row)
        if (i < N-1) {
            operations.push_back("D");
            x++;
        }
    }
    
    // Second pass to handle remaining negative heights
    for (int i = N-1; i >= 0; i--) {
        if (i % 2 == 0) {
            // Move right
            for (int j = (i == N-1 ? N-1 : 0); j < N; j++) {
                if (i == N-1 && j == N-1) {
                    // Already at position
                } else {
                    if (y < j) {
                        operations.push_back("R");
                        y++;
                    } else if (y > j) {
                        operations.push_back("L");
                        y--;
                    }
                }
                
                if (h[i][j] < 0 && load > 0) {
                    int unload = min(load, -h[i][j]);
                    operations.push_back("-" + to_string(unload));
                    load -= unload;
                    h[i][j] += unload;
                }
            }
        } else {
            // Move left
            for (int j = (i == N-1 ? 0 : N-1); j >= 0; j--) {
                if (y > j) {
                    operations.push_back("L");
                    y--;
                } else if (y < j) {
                    operations.push_back("R");
                    y++;
                }
                
                if (h[i][j] < 0 && load > 0) {
                    int unload = min(load, -h[i][j]);
                    operations.push_back("-" + to_string(unload));
                    load -= unload;
                    h[i][j] += unload;
                }
            }
        }
        
        if (i > 0) {
            operations.push_back("U");
            x--;
        }
    }
    
    for (const string& op : operations) {
        cout << op << "\n";
    }
    
    return 0;
}