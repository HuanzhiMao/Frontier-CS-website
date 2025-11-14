#include <iostream>
#include <vector>
#include <utility>
#include <set>
#include <algorithm>

void make_move(int x, int y, std::set<std::pair<int, int>>& black_cells) {
    std::cout << x << " " << y << std::endl;
    black_cells.insert({x, y});
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int sx, sy;
    std::cin >> sx >> sy;

    std::set<std::pair<int, int>> black_cells;
    std::vector<std::pair<int, int>> targets;

    if (sx > 1) {
        // Priority 1: Trap for (sx, 1), ordered to build bottom-up, outside-in.
        targets.push_back({sx + 1, 1});
        targets.push_back({sx - 1, 1});
        targets.push_back({sx + 1, 2});
        targets.push_back({sx - 1, 2});
        targets.push_back({sx, 2});
        
        // Priority 2: Chute walls. Height s_y + 15 is a safe margin.
        for (int y = 3; y <= sy + 15 && y <= 1000; ++y) {
            targets.push_back({sx - 1, y});
            targets.push_back({sx + 1, y});
        }
        // Priority 3: A ceiling for the chute, just in case.
        for (int x = sx - 1; x <= sx + 1; ++x) {
            if (x > 0 && x <= 1000) {
                targets.push_back({x, sy + 16});
            }
        }
    } else { // sx == 1
        // Priority 1: Trap for (1, 1).
        targets.push_back({2, 1});
        targets.push_back({2, 2});
        targets.push_back({1, 2});
        
        // Priority 2: Chute wall at x=2.
        for (int y = 3; y <= sy + 15 && y <= 1000; ++y) {
            targets.push_back({2, y});
        }
        // Priority 3: Ceiling for the chute.
        for (int x = 1; x <= 2; ++x) {
            if (x > 0 && x <= 1000) {
                targets.push_back({x, sy + 16});
            }
        }
    }

    int target_idx = 0;
    int rx = sx, ry = sy;

    for (int turn = 0; turn < 3000; ++turn) {
        int mx, my;
        
        // Find the next target cell from our predefined list that isn't black yet.
        while (target_idx < targets.size() && black_cells.count(targets[target_idx])) {
            target_idx++;
        }
        
        if (target_idx < targets.size()) {
            mx = targets[target_idx].first;
            my = targets[target_idx].second;
        } else {
            // This fallback case should ideally not be reached if the chute is tall enough.
            // If it is, the robot is somewhere unexpected. We can revert to a reactive
            // strategy of walling it from above its current position.
            bool moved = false;
            if (!black_cells.count({rx, ry + 1})) { mx = rx; my = ry + 1; moved = true; }
            else if (rx > 1 && !black_cells.count({rx - 1, ry + 1})) { mx = rx - 1; my = ry + 1; moved = true; }
            else if (rx < 1000 && !black_cells.count({rx + 1, ry + 1})) { mx = rx + 1; my = ry + 1; moved = true; }
            
            if(!moved){
                // Everything above is blocked. Mark a valid, non-black cell to not time out.
                mx = 1; my = 1;
                while(black_cells.count({mx,my})) {
                    mx++;
                    if(mx > 1000) {
                        mx = 1;
                        my++;
                    }
                }
            }
        }
        
        make_move(mx, my, black_cells);

        std::cin >> rx >> ry;
        if (rx == 0 && ry == 0) {
            break;
        }
    }

    return 0;
}