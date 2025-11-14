#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <utility>

const int N = 20;
int h[N][N];

struct Pos {
    int r, c;
};

int manhattan_dist(Pos p1, Pos p2) {
    return std::abs(p1.r - p2.r) + std::abs(p1.c - p2.c);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    
    int n_dummy;
    std::cin >> n_dummy;
    
    long long total_pos_h = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            std::cin >> h[i][j];
            if (h[i][j] > 0) {
                total_pos_h += h[i][j];
            }
        }
    }

    std::vector<std::string> commands;
    Pos current_pos = {0, 0};
    int current_load = 0;

    const int BATCH_SIZE = 220;

    auto move_to = [&](Pos dest) {
        while (current_pos.r < dest.r) {
            commands.push_back("D");
            current_pos.r++;
        }
        while (current_pos.r > dest.r) {
            commands.push_back("U");
            current_pos.r--;
        }
        while (current_pos.c < dest.c) {
            commands.push_back("R");
            current_pos.c++;
        }
        while (current_pos.c > dest.c) {
            commands.push_back("L");
            current_pos.c--;
        }
    };
    
    auto load = [&](int amount) {
        if (amount == 0) return;
        commands.push_back("+" + std::to_string(amount));
        current_load += amount;
        h[current_pos.r][current_pos.c] -= amount;
    };

    auto unload = [&](int amount) {
        if (amount == 0) return;
        commands.push_back("-" + std::to_string(amount));
        current_load -= amount;
        h[current_pos.r][current_pos.c] += amount;
    };

    while (total_pos_h > 0) {
        // Collection phase
        long long target_load = current_load + BATCH_SIZE;
        while(true) {
            if (current_load >= target_load) break;

            Pos best_source = {-1, -1};
            std::pair<int, int> best_crit = {1000000000, 1000000000};

            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < N; ++j) {
                    if (h[i][j] > 0) {
                        int d = manhattan_dist(current_pos, {i, j});
                        std::pair<int, int> current_crit = {d, -h[i][j]};
                        if (current_crit < best_crit) {
                            best_crit = current_crit;
                            best_source = {i, j};
                        }
                    }
                }
            }
            
            if (best_source.r == -1) break;

            move_to(best_source);
            
            int can_load = h[current_pos.r][current_pos.c];
            int to_load = std::min((long long)can_load, target_load - current_load);
            
            load(to_load);
            total_pos_h -= to_load;
        }

        // Distribution phase
        while (current_load > 0) {
            Pos best_sink = {-1, -1};
            std::pair<int, int> best_crit = {1000000000, 1000000000};

            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < N; ++j) {
                    if (h[i][j] < 0) {
                        int d = manhattan_dist(current_pos, {i, j});
                        std::pair<int, int> current_crit = {d, h[i][j]};
                        if (current_crit < best_crit) {
                            best_crit = current_crit;
                            best_sink = {i, j};
                        }
                    }
                }
            }
            
            if (best_sink.r == -1) break;

            move_to(best_sink);
            
            int can_unload = -h[current_pos.r][current_pos.c];
            int to_unload = std::min(can_unload, current_load);
            
            unload(to_unload);
        }
    }
    
    for(const auto& cmd : commands) {
        std::cout << cmd << "\n";
    }

    return 0;
}