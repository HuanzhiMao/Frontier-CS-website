#include <iostream>
#include <set>
#include <vector>
using namespace std;

int main() {
    int sx, sy;
    cin >> sx >> sy;
    
    set<pair<int, int>> black;
    int rx = sx, ry = sy;
    
    for (int turn = 1; turn <= 1000; turn++) {
        // Strategy: Try to corner the robot by blocking escape routes
        int mx, my;
        
        // If robot is at edge, block the adjacent cells
        if (rx == 1) {
            // Robot is at left edge, block cells to the right
            mx = rx + 1;
            my = ry;
            if (black.count({mx, my})) {
                // Try different position
                mx = rx + 1;
                my = max(1, ry - 1);
                if (black.count({mx, my})) {
                    mx = rx + 1;
                    my = ry + 1;
                }
            }
        } else if (ry == 1) {
            // Robot is at bottom edge, block cells above
            mx = rx;
            my = ry + 1;
            if (black.count({mx, my})) {
                mx = max(1, rx - 1);
                my = ry + 1;
                if (black.count({mx, my})) {
                    mx = rx + 1;
                    my = ry + 1;
                }
            }
        } else {
            // Robot is not at edge, try to push it towards corner
            // Block the path away from corner (1,1)
            if (rx > ry) {
                // Block leftward movement
                mx = max(1, rx - 1);
                my = ry;
            } else {
                // Block downward movement
                mx = rx;
                my = max(1, ry - 1);
            }
            
            // If already blocked, try adjacent
            if (black.count({mx, my})) {
                mx = max(1, rx - 1);
                my = max(1, ry - 1);
                if (black.count({mx, my})) {
                    mx = rx + 1;
                    my = max(1, ry - 1);
                    if (black.count({mx, my})) {
                        mx = max(1, rx - 1);
                        my = ry + 1;
                    }
                }
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