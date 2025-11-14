#include <bits/stdc++.h>
using namespace std;

int main() {
    vector<vector<int>> pyramid(30);
    for (int i = 0; i < 30; i++) {
        pyramid[i].resize(i + 1);
        for (int j = 0; j <= i; j++) {
            cin >> pyramid[i][j];
        }
    }
    
    vector<tuple<int, int, int, int>> operations;
    
    // Bubble sort approach - repeatedly fix violations
    bool improved = true;
    while (improved && operations.size() < 9000) {
        improved = false;
        
        // Check all parent-child relationships
        for (int x = 0; x < 29; x++) {
            for (int y = 0; y <= x; y++) {
                // Check left child (x+1, y)
                if (pyramid[x][y] > pyramid[x+1][y]) {
                    swap(pyramid[x][y], pyramid[x+1][y]);
                    operations.push_back({x, y, x+1, y});
                    improved = true;
                }
                
                // Check right child (x+1, y+1)
                if (pyramid[x][y] > pyramid[x+1][y+1]) {
                    swap(pyramid[x][y], pyramid[x+1][y+1]);
                    operations.push_back({x, y, x+1, y+1});
                    improved = true;
                }
            }
        }
        
        // Also try horizontal swaps to help with rearrangement
        for (int x = 0; x < 30; x++) {
            for (int y = 0; y < x; y++) {
                if (pyramid[x][y] > pyramid[x][y+1]) {
                    bool should_swap = false;
                    
                    // Check if this swap helps with parent constraints
                    if (x > 0) {
                        if (y > 0 && pyramid[x-1][y-1] > pyramid[x][y+1]) should_swap = true;
                        if (pyramid[x-1][y] > pyramid[x][y+1]) should_swap = true;
                    }
                    
                    // Check if this swap helps with child constraints
                    if (x < 29) {
                        if (pyramid[x][y] > pyramid[x+1][y]) should_swap = true;
                        if (pyramid[x][y] > pyramid[x+1][y+1]) should_swap = true;
                    }
                    
                    if (should_swap) {
                        swap(pyramid[x][y], pyramid[x][y+1]);
                        operations.push_back({x, y, x, y+1});
                        improved = true;
                    }
                }
            }
        }
    }
    
    cout << operations.size() << "\n";
    for (auto& op : operations) {
        cout << get<0>(op) << " " << get<1>(op) << " " << get<2>(op) << " " << get<3>(op) << "\n";
    }
    
    return 0;
}