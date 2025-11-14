#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

// Global state for black cells and robot position
set<pair<int, int>> black_cells;
int r_x, r_y;

// Helper function to make a move and read the robot's response
void make_move(int x, int y) {
    // Ensure the move is within the allowed bounds
    if (x <= 0 || y <= 0 || x > 1000 || y > 1000) {
        // Fallback for invalid coordinates, though logic should prevent this.
        // Find a valid uncolored cell to be safe
        x = 1; y = 1;
        while(black_cells.count({x, y})) {
            x++;
            if (x > 1000) {
                x = 1; y++;
            }
        }
    }
    cout << x << " " << y << endl;
    black_cells.insert({x, y});
    cin >> r_x >> r_y;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> r_x >> r_y;

    // Phase 1: Build a trap around (2, 2)
    vector<pair<int, int>> trap_cells;
    for (int i = 1; i <= 3; ++i) {
        for (int j = 1; j <= 3; ++j) {
            if (i == 2 && j == 2) continue;
            trap_cells.push_back({i, j});
        }
    }

    for (const auto& cell : trap_cells) {
        if (black_cells.count(cell)) continue;
        make_move(cell.first, cell.second);
        if (r_x == 0 && r_y == 0) return 0;
    }
    
    // Phase 2: Herd the robot to (2, 2)
    vector<pair<int,int>> wishlist;

    while (r_x != 0 || r_y != 0) {
        // If the wishlist is empty, create a new one based on robot's position.
        if (wishlist.empty()) {
            if (r_x == 2 && r_y == 2) {
                // Robot is trapped. Any move will result in 0 0.
                // We can make a dummy move on its current location.
                make_move(r_x, r_y);
                if (r_x == 0 && r_y == 0) return 0;
                continue;
            }

            if (r_y > 2) { // Priority 1: Push down
                for (int dx = -1; dx <= 1; ++dx) {
                    wishlist.push_back({r_x + dx, r_y + 1});
                }
            } else if (r_x > 2) { // Priority 2: Push left
                for (int dy = -1; dy <= 1; ++dy) {
                     wishlist.push_back({r_x + 1, r_y + dy});
                }
            } else { // Robot is in the trap zone [1,2]x[1,2]
                // It should get caught soon. Just block a random valid neighbor.
                bool moved = false;
                for(int dx = -1; dx <= 1; ++dx) {
                    for(int dy = -1; dy <= 1; ++dy) {
                        if(dx == 0 && dy == 0) continue;
                        int nx = r_x + dx, ny = r_y + dy;
                        if(nx > 0 && ny > 0 && !black_cells.count({nx,ny})) {
                            make_move(nx, ny);
                            moved = true;
                            goto next_iteration;
                        }
                    }
                }
                // If all neighbors are black, it is trapped. Make a dummy move.
                if(!moved) make_move(r_x, r_y);
            }
        }
        
        // Process the current wishlist: find a cell that is not black and mark it.
        if (!wishlist.empty()) {
            pair<int, int> to_paint = {-1,-1};
            
            // Find first uncolored cell in wishlist
            auto it = wishlist.begin();
            while(it != wishlist.end()){
                if(!black_cells.count(*it)){
                    to_paint = *it;
                    wishlist.erase(it);
                    break;
                }
                it = wishlist.erase(it);
            }
            
            if(to_paint.first != -1){
                make_move(to_paint.first, to_paint.second);
            } else {
                 // Wishlist contained only already black cells. Clear it to get a new one.
                 wishlist.clear();
            }
        }
        
        next_iteration:;
        if (r_x == 0 && r_y == 0) return 0;
    }

    return 0;
}