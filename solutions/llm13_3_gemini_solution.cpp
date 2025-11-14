#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <utility>

void solve() {
    int sx, sy;
    std::cin >> sx >> sy;

    std::set<std::pair<int, int>> visited;
    std::set<std::pair<int, int>> in_queue;
    std::queue<std::pair<int, int>> q;

    int current_rx = sx;
    int current_ry = sy;

    // Initial population of the queue with neighbors of the starting position
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;
            int nx = current_rx + dx;
            int ny = current_ry + dy;
            if (nx >= 1 && nx <= 1000 && ny >= 1 && ny <= 1000) {
                if (visited.find({nx, ny}) == visited.end() && in_queue.find({nx, ny}) == in_queue.end()) {
                    q.push({nx, ny});
                    in_queue.insert({nx, ny});
                }
            }
        }
    }

    for (int turn = 0; turn < 3000; ++turn) {
        // If the queue is empty, the robot is in a region where all neighbors
        // are already processed. This is a robust fallback to re-seed the queue
        // based on the robot's last known position.
        if (q.empty()) {
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    if (dx == 0 && dy == 0) continue;
                    int nx = current_rx + dx;
                    int ny = current_ry + dy;
                    if (nx >= 1 && nx <= 1000 && ny >= 1 && ny <= 1000) {
                        if (visited.find({nx, ny}) == visited.end() && in_queue.find({nx, ny}) == in_queue.end()) {
                            q.push({nx, ny});
                            in_queue.insert({nx, ny});
                        }
                    }
                }
            }
        }
        
        std::pair<int, int> move = q.front();
        q.pop();
        in_queue.erase(move);

        std::cout << move.first << " " << move.second << std::endl;
        visited.insert(move);

        int next_rx, next_ry;
        std::cin >> next_rx >> next_ry;

        if (next_rx <= 0 || next_ry <= 0) {
            // Robot exploded (0,0) or judge error (-1,-1), we are done.
            return;
        }

        current_rx = next_rx;
        current_ry = next_ry;

        // Add new neighbors of the robot's new position to the queue
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                if (dx == 0 && dy == 0) continue;
                int nx = current_rx + dx;
                int ny = current_ry + dy;
                if (nx >= 1 && nx <= 1000 && ny >= 1 && ny <= 1000) {
                    if (visited.find({nx, ny}) == visited.end() && in_queue.find({nx, ny}) == in_queue.end()) {
                        q.push({nx, ny});
                        in_queue.insert({nx, ny});
                    }
                }
            }
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    solve();
    return 0;
}