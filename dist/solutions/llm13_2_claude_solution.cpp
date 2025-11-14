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
        
        // Try to block escape routes systematically
        // Focus on creating barriers around the robot's current position
        
        if (turn == 1) {
            // Start by blocking a key escape route
            if (rx > 1) {
                mx = rx - 1;
                my = max(1, ry - 1);
            } else {
                mx = rx + 1;
                my = max(1, ry - 1);
            }
        } else {
            // Create a containment strategy
            // Block cells in a pattern that forces the robot into a corner
            
            // Priority: block cells that are adjacent to robot's likely moves
            vector<pair<int, int>> candidates;
            
            // Generate all possible adjacent cells to current robot position
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    if (dx == 0 && dy == 0) continue;
                    int nx = rx + dx;
                    int ny = ry + dy;
                    if (nx > 0 && ny > 0 && nx <= 1000 && ny <= 1000) {
                        candidates.push_back({nx, ny});
                    }
                }
            }
            
            // Choose a strategic cell to block
            bool found = false;
            
            // Try to block cells that would give robot fewer options
            for (auto& cell : candidates) {
                if (black.find(cell) == black.end()) {
                    mx = cell.first;
                    my = cell.second;
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                // Fallback: block a cell near the robot
                mx = max(1, rx - 1);
                my = max(1, ry - 1);
            }
        }
        
        cout << mx << " " << my << endl;
        cout.flush();
        
        black.insert({mx, my});
        
        int nx, ny;
        cin >> nx >> ny;
        
        if (nx == 0 && ny == 0) {
            // Robot exploded
            break;
        }
        
        rx = nx;
        ry = ny;
    }
    
    return 0;
}