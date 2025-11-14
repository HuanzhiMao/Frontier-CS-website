#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <cmath>

using namespace std;

// A queue to manage the cells we will blacken, ensuring a BFS-like expansion.
queue<pair<int, int>> to_blacken;
// A set to track cells that are either already black or are scheduled to be blackened.
// This prevents redundant operations.
set<pair<int, int>> is_in_queue_or_black;

// Helper function to add a cell to the queue if it's valid and hasn't been processed.
void add_to_queue(int x, int y) {
    if (x > 0 && y > 0 && x <= 1000 && y <= 1000) {
        if (is_in_queue_or_black.find({x, y}) == is_in_queue_or_black.end()) {
            to_blacken.push({x, y});
            is_in_queue_or_black.insert({x, y});
        }
    }
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int rx, ry;
    cin >> rx >> ry;

    // Initial seeding: Create a 5x5 black area (minus the center) around the start.
    // This gives our expanding region a solid base.
    for (int dx = -2; dx <= 2; ++dx) {
        for (int dy = -2; dy <= 2; ++dy) {
             if (dx == 0 && dy == 0) continue;
             add_to_queue(rx + dx, ry + dy);
        }
    }
    
    for (int t = 1; t <= 3000; ++t) {
        // At each step, add the immediate neighbors of the robot's current position to the queue.
        // This reinforces the "wall" around the robot, ensuring it cannot escape.
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                if (dx == 0 && dy == 0) continue;
                add_to_queue(rx + dx, ry + dy);
            }
        }
        
        // If the queue becomes empty, it implies the robot is completely surrounded.
        // We can add more cells further out to be safe, though this case is unlikely
        // to be hit before the robot is trapped.
        if (to_blacken.empty()) {
            add_to_queue(rx + 2, ry);
            add_to_queue(rx - 2, ry);
            add_to_queue(rx, ry + 2);
            add_to_queue(rx, ry - 2);
        }

        // Dequeue the next cell and blacken it.
        pair<int, int> cell = to_blacken.front();
        to_blacken.pop();
        int mx = cell.first;
        int my = cell.second;

        cout << mx << " " << my << endl;

        // Read the robot's next move.
        cin >> rx >> ry;
        if (rx == 0 && ry == 0) {
            // Robot exploded, we win.
            return 0;
        }
    }

    return 0;
}