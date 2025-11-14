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
        
        // Strategy: Try to block escape routes systematically
        // Focus on creating barriers that force the robot into trapped positions
        
        if (turn == 1) {
            // Start by blocking a key escape route
            if (rx > 1 && ry > 1) {
                mx = rx - 1;
                my = ry - 1;
            } else if (rx == 1) {
                mx = rx + 1;
                my = ry - 1 > 0 ? ry - 1 : ry + 1;
            } else {
                mx = rx - 1 > 0 ? rx - 1 : rx + 1;
                my = ry + 1;
            }
        } else {
            // Try to create a systematic trap
            // Block positions that create corridors leading to traps
            
            bool found = false;
            
            // Try to block adjacent cells that aren't already blocked
            for (int dx = -2; dx <= 2 && !found; dx++) {
                for (int dy = -2; dy <= 2 && !found; dy++) {
                    int nx = rx + dx;
                    int ny = ry + dy;
                    
                    if (nx >= 1 && nx <= 1000 && ny >= 1 && ny <= 1000) {
                        if (black.find({nx, ny}) == black.end()) {
                            // Check if this creates a good blocking position
                            mx = nx;
                            my = ny;
                            found = true;
                        }
                    }
                }
            }
            
            if (!found) {
                // Fallback: block systematically around current position
                mx = rx;
                my = ry > 1 ? ry - 1 : ry + 1;
                if (black.find({mx, my}) != black.end()) {
                    mx = rx > 1 ? rx - 1 : rx + 1;
                    my = ry;
                }
            }
        }
        
        // Ensure move is within bounds
        mx = max(1, min(1000, mx));
        my = max(1, min(1000, my));
        
        cout << mx << " " << my << endl;
        cout.flush();
        
        black.insert({mx, my});
        
        int nx, ny;
        cin >> nx >> ny;
        
        if (nx == 0 && ny == 0) {
            break; // Robot exploded
        }
        
        rx = nx;
        ry = ny;
    }
    
    return 0;
}