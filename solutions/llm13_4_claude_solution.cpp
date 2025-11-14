#include <iostream>
#include <vector>
#include <set>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int sx, sy;
    cin >> sx >> sy;
    
    int rx = sx, ry = sy;
    set<pair<int, int>> black_cells;
    
    for (int turn = 1; turn <= 1000; turn++) {
        // Strategy: Try to corner the robot by blocking escape routes
        // Focus on creating walls and forcing the robot into trapped positions
        
        int target_x, target_y;
        
        // If robot is near boundary, try to trap it there
        if (rx == 1) {
            // Robot is at left boundary, block vertically adjacent cells
            if (ry > 1) {
                target_x = 1;
                target_y = ry - 1;
            } else {
                target_x = 2;
                target_y = 1;
            }
        } else if (ry == 1) {
            // Robot is at bottom boundary, block horizontally adjacent cells
            if (rx > 1) {
                target_x = rx - 1;
                target_y = 1;
            } else {
                target_x = 1;
                target_y = 2;
            }
        } else {
            // General strategy: try to block the robot's most likely escape routes
            // Create a systematic approach to corner the robot
            
            // Try to create barriers around the robot
            if (turn % 4 == 1) {
                target_x = max(1, rx - 1);
                target_y = max(1, ry - 1);
            } else if (turn % 4 == 2) {
                target_x = min(1000, rx + 1);
                target_y = max(1, ry - 1);
            } else if (turn % 4 == 3) {
                target_x = max(1, rx - 1);
                target_y = min(1000, ry + 1);
            } else {
                target_x = min(1000, rx + 1);
                target_y = min(1000, ry + 1);
            }
        }
        
        // Make sure we don't repeat the same cell
        while (black_cells.count({target_x, target_y})) {
            target_x = max(1, min(1000, target_x + 1));
            if (target_x > 1000) {
                target_x = 1;
                target_y = max(1, min(1000, target_y + 1));
            }
        }
        
        cout << target_x << " " << target_y << endl;
        cout.flush();
        
        black_cells.insert({target_x, target_y});
        
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