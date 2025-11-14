#include <iostream>
#include <vector>
#include <set>
#include <utility>
#include <algorithm>

// Custom comparator for the set to order cells.
// We prioritize cells with a larger sum of coordinates (x+y),
// as this is a good heuristic for the direction an escaping robot would take.
// Ties are broken by prioritizing larger x, then larger y.
struct ComparePairsDesc {
    bool operator()(const std::pair<int, int>& a, const std::pair<int, int>& b) const {
        int sum_a = a.first + a.second;
        int sum_b = b.first + b.second;
        if (sum_a != sum_b) {
            return sum_a > sum_b;
        }
        if (a.first != b.first) {
            return a.first > b.first;
        }
        return a.second > b.second;
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int sx, sy;
    std::cin >> sx >> sy;

    int rx = sx;
    int ry = sy;

    // A priority queue of cells to consider blackening, ordered by our heuristic.
    std::set<std::pair<int, int>, ComparePairsDesc> to_blacken;
    // A set to keep track of cells that are already black.
    std::set<std::pair<int, int>> is_black;

    while (true) {
        // Define the 8 directions for neighbors.
        int all_dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
        int all_dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

        // Add all valid, non-black neighbors of the robot's current position to our candidate set.
        for (int i = 0; i < 8; ++i) {
            int nx = rx + all_dx[i];
            int ny = ry + all_dy[i];

            if (nx > 0 && ny > 0 && nx <= 1000 && ny <= 1000) {
                if (is_black.find({nx, ny}) == is_black.end()) {
                    to_blacken.insert({nx, ny});
                }
            }
        }
        
        int mx, my;
        
        if (to_blacken.empty()) {
            // This case occurs if all of the robot's neighbors are already black, meaning it's trapped.
            // The judge should send "0 0" on the robot's next move.
            // We must still make a valid move. We can pick any valid neighbor; it will be a wasted move
            // if it's already in `is_black`, but it will satisfy the protocol.
            bool found_move = false;
            for (int i = 0; i < 8; ++i) {
                int nx = rx + all_dx[i];
                int ny = ry + all_dy[i];
                if (nx > 0 && ny > 0 && nx <= 1000 && ny <= 1000) {
                    mx = nx;
                    my = ny;
                    found_move = true;
                    break;
                }
            }
            if (!found_move) { // This can only happen if the robot is at (1,1)
              mx = 2; my = 2;
            }
        } else {
            // Pick the highest priority cell from our candidate set.
            auto it = to_blacken.begin();
            mx = it->first;
            my = it->second;
            to_blacken.erase(it);
        }

        std::cout << mx << " " << my << std::endl;

        is_black.insert({mx, my});

        int nx, ny;
        std::cin >> nx >> ny;

        if (nx == 0 && ny == 0) {
            // Robot exploded, game over.
            break;
        }
        if (nx == -1 && ny == -1) {
            // Judge indicates an error or termination.
            break;
        }

        // Update robot's position for the next turn.
        rx = nx;
        ry = ny;
        
        // The cell the robot moved to is confirmed to be white.
        // If it was in our `to_blacken` set, we should remove it, as there's no need to blacken it now.
        if(to_blacken.count({rx, ry})) {
            to_blacken.erase({rx, ry});
        }
    }

    return 0;
}