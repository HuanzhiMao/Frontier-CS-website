#include <iostream>
#include <vector>
#include <set>
using namespace std;

int main() {
    int sx, sy;
    cin >> sx >> sy;
    
    int rx = sx, ry = sy;
    set<pair<int, int>> black;
    
    for (int turn = 1; turn <= 1000; turn++) {
        int mx, my;
        
        // Strategy: Try to trap the robot by blocking escape routes
        // Focus on creating barriers around the robot's position
        
        if (turn == 1) {
            // Start by blocking a key position
            if (rx == 1) {
                mx = rx + 1;
                my = max(1, ry - 1);
            } else if (ry == 1) {
                mx = max(1, rx - 1);
                my = ry + 1;
            } else {
                mx = rx - 1;
                my = ry - 1;
            }
        } else {
            // Try to block the most likely escape routes
            vector<pair<int, int>> candidates;
            
            // Add all adjacent positions to robot
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    if (dx == 0 && dy == 0) continue;
                    int nx = rx + dx;
                    int ny = ry + dy;
                    if (nx > 0 && ny > 0 && nx <= 1000 && ny <= 1000) {
                        if (black.find({nx, ny}) == black.end()) {
                            candidates.push_back({nx, ny});
                        }
                    }
                }
            }
            
            if (!candidates.empty()) {
                // Choose the first available adjacent cell
                mx = candidates[0].first;
                my = candidates[0].second;
            } else {
                // Fallback
                mx = min(1000, rx + 1);
                my = min(1000, ry + 1);
            }
        }
        
        // Ensure bounds
        mx = max(1, min(1000, mx));
        my = max(1, min(1000, my));
        
        cout << mx << " " << my << endl;
        cout.flush();
        
        black.insert({mx, my});
        
        int nx, ny;
        cin >> nx >> ny;
        
        if (nx == 0 && ny == 0) {
            break;
        }
        
        rx = nx;
        ry = ny;
    }
    
    return 0;
}