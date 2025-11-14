#include <iostream>
#include <vector>
#include <cmath>
#include <queue>
#include <algorithm>
#include <set>
#include <tuple>

const int N = 30;

struct Point {
    int x, y;

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

int board[N][N];
Point pos[N * (N + 1) / 2];
std::vector<std::tuple<int, int, int, int>> swaps;
bool finalized[N][N];

void do_swap(Point p1, Point p2) {
    int val1 = board[p1.x][p1.y];
    int val2 = board[p2.x][p2.y];

    std::swap(board[p1.x][p1.y], board[p2.x][p2.y]);
    pos[val1] = p2;
    pos[val2] = p1;
    swaps.emplace_back(p1.x, p1.y, p2.x, p2.y);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j <= i; ++j) {
            std::cin >> board[i][j];
            pos[board[i][j]] = {i, j};
            finalized[i][j] = false;
        }
    }

    for (int i = 0; i < N - 1; ++i) {
        std::set<int> unassigned_vals;
        int start_val = i * (i + 1) / 2;
        for (int k = 0; k < i + 1; ++k) {
            unassigned_vals.insert(start_val + k);
        }

        std::vector<Point> unfilled_pos;
        for (int j = 0; j <= i; ++j) {
            unfilled_pos.push_back({i, j});
        }
        
        for (int k = 0; k <= i; ++k) {
            int best_val = -1;
            Point best_pos = {-1, -1};
            int min_dist = 1e9;

            for (int val : unassigned_vals) {
                for (const auto& p : unfilled_pos) {
                    int dist = std::abs(pos[val].x - p.x) + std::abs(pos[val].y - p.y);
                    if (dist < min_dist) {
                        min_dist = dist;
                        best_val = val;
                        best_pos = p;
                    }
                }
            }
            
            Point source_pos = pos[best_val];

            if (!(source_pos == best_pos)) {
                std::queue<Point> q;
                q.push(source_pos);
                Point parent_grid[N][N];
                bool visited[N][N] = {false};

                for(int r=0; r<N; ++r) for(int c=0; c<=r; ++c) parent_grid[r][c] = {-1,-1};
                
                visited[source_pos.x][source_pos.y] = true;
                
                while(!q.empty()){
                    Point curr = q.front();
                    q.pop();

                    if (curr == best_pos) {
                        break;
                    }

                    Point neighbors[6];
                    neighbors[0] = {curr.x, curr.y - 1};
                    neighbors[1] = {curr.x, curr.y + 1};
                    neighbors[2] = {curr.x + 1, curr.y};
                    neighbors[3] = {curr.x + 1, curr.y + 1};
                    neighbors[4] = {curr.x - 1, curr.y - 1};
                    neighbors[5] = {curr.x - 1, curr.y};

                    for(int l=0; l<6; ++l) {
                        Point next = neighbors[l];
                        if (next.x < 0 || next.x >= N || next.y < 0 || next.y > next.x) continue;
                        if (finalized[next.x][next.y]) continue;
                        if (visited[next.x][next.y]) continue;

                        visited[next.x][next.y] = true;
                        parent_grid[next.x][next.y] = curr;
                        q.push(next);
                    }
                }

                std::vector<Point> path;
                Point p = best_pos;
                while(true) {
                    path.push_back(p);
                    if (p == source_pos) break;
                    p = parent_grid[p.x][p.y];
                }
                std::reverse(path.begin(), path.end());

                for (size_t l = 0; l < path.size() - 1; ++l) {
                    do_swap(path[l], path[l+1]);
                }
            }
            
            finalized[best_pos.x][best_pos.y] = true;
            unassigned_vals.erase(best_val);
            unfilled_pos.erase(std::remove_if(unfilled_pos.begin(), unfilled_pos.end(), 
                [&](const Point& p){ return p == best_pos; }), unfilled_pos.end());
        }
    }
    
    std::cout << swaps.size() << "\n";
    for (const auto& s : swaps) {
        std::cout << std::get<0>(s) << " " << std::get<1>(s) << " " << std::get<2>(s) << " " << std::get<3>(s) << "\n";
    }

    return 0;
}