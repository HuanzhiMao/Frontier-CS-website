#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <queue>

const int N = 20;
const int M = 20;

struct FrontierNode {
    int r, c;
    long long cost;

    bool operator>(const FrontierNode& other) const {
        return cost > other.cost;
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::vector<std::string> grid(N, std::string(M, '0'));
    std::mt19937 rng(42); 

    std::priority_queue<FrontierNode, std::vector<FrontierNode>, std::greater<FrontierNode>> pq;
    
    int start_r = N / 2;
    int start_c = M / 2;
    
    grid[start_r][start_c] = '1';
    int empty_cells = 1;

    auto add_to_frontier = [&](int r, int c, int pr, int pc) {
        if (r >= 0 && r < N && c >= 0 && c < M) {
            long long er = r - pr;
            long long ec = c - pc;
            long long dr = (long long)r + pr - (N - 1);
            long long dc = (long long)c + pc - (M - 1);

            long long base_cost = ec * dr - er * dc;
            
            std::uniform_int_distribution<int> dist(0, N * M / 2);
            long long cost = base_cost * 1000 + dist(rng);
            pq.push({r, c, cost});
        }
    };
    
    add_to_frontier(start_r - 1, start_c, start_r, start_c);
    add_to_frontier(start_r + 1, start_c, start_r, start_c);
    add_to_frontier(start_r, start_c - 1, start_r, start_c);
    add_to_frontier(start_r, start_c + 1, start_r, start_c);

    int target_empty_cells = 251;

    while (!pq.empty() && empty_cells < target_empty_cells) {
        FrontierNode current = pq.top();
        pq.pop();

        int r = current.r;
        int c = current.c;

        if (grid[r][c] == '1') {
            continue;
        }

        int dr_check[] = {-1, 1, 0, 0};
        int dc_check[] = {0, 0, -1, 1};
        int one_neighbors = 0;
        for(int i = 0; i < 4; ++i) {
            int nr = r + dr_check[i];
            int nc = c + dc_check[i];
            if (nr >= 0 && nr < N && nc >= 0 && nc < M && grid[nr][nc] == '1') {
                one_neighbors++;
            }
        }
        
        if (one_neighbors == 1) {
            grid[r][c] = '1';
            empty_cells++;
            
            add_to_frontier(r - 1, c, r, c);
            add_to_frontier(r + 1, c, r, c);
            add_to_frontier(r, c - 1, r, c);
            add_to_frontier(r, c + 1, r, c);
        }
    }

    std::cout << N << " " << M << std::endl;
    for (int i = 0; i < N; ++i) {
        std::cout << grid[i] << std::endl;
    }

    return 0;
}