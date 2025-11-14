#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <set>
#include <queue>
#include <map>

// Constants
const int N = 5000;
const int MAX_COORD = 100000;
const int GRID_SIZE = 250;
const int CELL_SIZE = MAX_COORD / GRID_SIZE; 
const int NUM_SEEDS = 50;

struct Point {
    int x, y;
};

struct Cell {
    int r, c;
    bool operator<(const Cell& other) const {
        if (r != other.r) return r < other.r;
        return c < other.c;
    }
     bool operator==(const Cell& other) const {
        return r == other.r && c == other.c;
    }
};

struct Candidate {
    int score;
    Cell cell;
    bool operator<(const Candidate& other) const {
        return score < other.score;
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::vector<Point> mackerels(N), sardines(N);
    for (int i = 0; i < N; ++i) std::cin >> mackerels[i].x >> mackerels[i].y;
    for (int i = 0; i < N; ++i) std::cin >> sardines[i].x >> sardines[i].y;

    std::vector<std::vector<int>> grid_score(GRID_SIZE, std::vector<int>(GRID_SIZE, 0));

    for (const auto& p : mackerels) {
        int r = std::min(GRID_SIZE - 1, p.y / CELL_SIZE);
        int c = std::min(GRID_SIZE - 1, p.x / CELL_SIZE);
        grid_score[r][c]++;
    }
    for (const auto& p : sardines) {
        int r = std::min(GRID_SIZE - 1, p.y / CELL_SIZE);
        int c = std::min(GRID_SIZE - 1, p.x / CELL_SIZE);
        grid_score[r][c]--;
    }

    std::vector<Candidate> seeds;
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (grid_score[i][j] > 0) {
                seeds.push_back({grid_score[i][j], {i, j}});
            }
        }
    }

    std::sort(seeds.rbegin(), seeds.rend());
    
    std::set<Cell> best_region;
    long long best_region_score = -1;
    std::vector<bool> visited_seed_cells(GRID_SIZE * GRID_SIZE, false);

    for (int k = 0; k < std::min((int)seeds.size(), NUM_SEEDS); ++k) {
        Cell seed_cell = seeds[k].cell;
        if (visited_seed_cells[seed_cell.r * GRID_SIZE + seed_cell.c]) {
            continue;
        }

        std::set<Cell> component;
        std::queue<Cell> q;
        
        q.push(seed_cell);
        component.insert(seed_cell);
        visited_seed_cells[seed_cell.r * GRID_SIZE + seed_cell.c] = true;

        int dr[] = {-1, 1, 0, 0};
        int dc[] = {0, 0, -1, 1};

        while (!q.empty()) {
            Cell cell = q.front();
            q.pop();

            for (int i = 0; i < 4; ++i) {
                int nr = cell.r + dr[i];
                int nc = cell.c + dc[i];

                if (nr >= 0 && nr < GRID_SIZE && nc >= 0 && nc < GRID_SIZE && !visited_seed_cells[nr * GRID_SIZE + nc]) {
                    if (grid_score[nr][nc] > -5) { // Heuristic to prune search
                        visited_seed_cells[nr * GRID_SIZE + nc] = true;
                        q.push({nr, nc});
                        component.insert({nr, nc});
                    }
                }
            }
        }
        
        std::vector<Candidate> component_cells;
        for(const auto& cell : component){
            component_cells.push_back({grid_score[cell.r][cell.c], cell});
        }
        std::sort(component_cells.rbegin(), component_cells.rend());

        long long current_score = 0;
        long long max_prefix_score = 0;
        int best_prefix_size = 0;
        for(size_t i = 0; i < component_cells.size(); ++i){
            current_score += component_cells[i].score;
            if(current_score > max_prefix_score){
                max_prefix_score = current_score;
                best_prefix_size = i + 1;
            }
        }
        
        if (max_prefix_score > best_region_score) {
            best_region_score = max_prefix_score;
            best_region.clear();
            for(int i = 0; i < best_prefix_size; ++i){
                best_region.insert(component_cells[i].cell);
            }
        }
    }
    
    if (best_region.empty()) {
        std::cout << 4 << "\n";
        std::cout << "0 0\n0 1\n1 1\n1 0\n";
        return 0;
    }

    using VPoint = std::pair<int, int>;
    std::map<VPoint, std::vector<VPoint>> adj;
    
    for (const auto& cell : best_region) {
        int r = cell.r, c = cell.c;
        VPoint corners[] = {
            {c * CELL_SIZE, r * CELL_SIZE},
            {(c + 1) * CELL_SIZE, r * CELL_SIZE},
            {(c + 1) * CELL_SIZE, (r + 1) * CELL_SIZE},
            {c * CELL_SIZE, (r + 1) * CELL_SIZE}
        };

        if (best_region.find({r - 1, c}) == best_region.end()) { // Top edge
            adj[corners[0]].push_back(corners[1]); adj[corners[1]].push_back(corners[0]);
        }
        if (best_region.find({r + 1, c}) == best_region.end()) { // Bottom edge
            adj[corners[3]].push_back(corners[2]); adj[corners[2]].push_back(corners[3]);
        }
        if (best_region.find({r, c - 1}) == best_region.end()) { // Left edge
            adj[corners[0]].push_back(corners[3]); adj[corners[3]].push_back(corners[0]);
        }
        if (best_region.find({r, c + 1}) == best_region.end()) { // Right edge
            adj[corners[1]].push_back(corners[2]); adj[corners[2]].push_back(corners[1]);
        }
    }

    VPoint start_v = adj.begin()->first;
    std::vector<VPoint> path;
    path.push_back(start_v);
    
    VPoint curr_v = start_v;
    VPoint prev_v = {-1, -1};

    while (true) {
        VPoint next_v = {-1,-1};
        for (const auto& neighbor : adj[curr_v]) {
            if (neighbor != prev_v) {
                next_v = neighbor;
                break;
            }
        }
        
        if (next_v == start_v) break;
        
        path.push_back(next_v);
        prev_v = curr_v;
        curr_v = next_v;
    }

    std::vector<VPoint> simplified_path;
    if (path.size() > 1) {
        simplified_path.push_back(path[0]);
        for (size_t i = 1; i < path.size(); ++i) {
            VPoint p_prev = simplified_path.back();
            VPoint p_curr = path[i];
            VPoint p_next = path[(i + 1) % path.size()];
            
            long long dx1 = p_curr.first - p_prev.first;
            long long dy1 = p_curr.second - p_prev.second;
            long long dx2 = p_next.first - p_curr.first;
            long long dy2 = p_next.second - p_curr.second;
            
            if (dx1 * dy2 - dx2 * dy1 != 0) { // Not collinear
                if (p_curr != simplified_path.back())
                    simplified_path.push_back(p_curr);
            }
        }
    } else {
        simplified_path = path;
    }
    
    // Final collinearity check for wrapping around
    if (simplified_path.size() >= 3) {
        VPoint p_prev = simplified_path[simplified_path.size() - 2];
        VPoint p_curr = simplified_path.back();
        VPoint p_next = simplified_path[0];
        long long dx1 = p_curr.first - p_prev.first;
        long long dy1 = p_curr.second - p_prev.second;
        long long dx2 = p_next.first - p_curr.first;
        long long dy2 = p_next.second - p_curr.second;
        if (dx1 * dy2 - dx2 * dy1 == 0) {
            simplified_path.pop_back();
        }
    }
    if (simplified_path.size() >= 3) {
        VPoint p_prev = simplified_path.back();
        VPoint p_curr = simplified_path[0];
        VPoint p_next = simplified_path[1];
        long long dx1 = p_curr.first - p_prev.first;
        long long dy1 = p_curr.second - p_prev.second;
        long long dx2 = p_next.first - p_curr.first;
        long long dy2 = p_next.second - p_curr.second;
        if (dx1 * dy2 - dx2 * dy1 == 0) {
            simplified_path.erase(simplified_path.begin());
        }
    }


    std::cout << simplified_path.size() << "\n";
    for (const auto& v : simplified_path) {
        std::cout << v.first << " " << v.second << "\n";
    }

    return 0;
}