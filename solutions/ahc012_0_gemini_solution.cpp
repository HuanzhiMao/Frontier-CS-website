#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <map>
#include <random>

std::mt19937 rng(1337);

struct Point {
    long long x, y;
};

struct Line {
    Point p, q;
};

__int128_t cross_product(Point a, Point b, Point c) {
    __int128_t x1 = b.x - a.x;
    __int128_t y1 = b.y - a.y;
    __int128_t x2 = c.x - a.x;
    __int128_t y2 = c.y - a.y;
    return x1 * y2 - x2 * y1;
}

__int128_t side(const Line& l, const Point& c) {
    return cross_product(l.p, l.q, c);
}

int N, K;
std::vector<int> a;
std::vector<Point> strawberries;

long long score_partition(const std::vector<std::vector<int>>& groups) {
    std::map<int, int> counts;
    for (const auto& g : groups) {
        if (g.size() > 0 && g.size() <= 10) {
            counts[g.size()]++;
        }
    }

    long long score = 0;
    for (int d = 1; d <= 10; ++d) {
        if (counts.count(d)) {
            score += std::min(counts[d], a[d - 1]);
        }
    }
    return score;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::cin >> N >> K;
    a.resize(10);
    for (int i = 0; i < 10; ++i) {
        std::cin >> a[i];
    }
    strawberries.resize(N);
    for (int i = 0; i < N; ++i) {
        std::cin >> strawberries[i].x >> strawberries[i].y;
    }

    std::vector<Line> cuts;
    std::vector<std::vector<int>> current_groups;
    {
        std::vector<int> initial_group(N);
        std::iota(initial_group.begin(), initial_group.end(), 0);
        current_groups.push_back(initial_group);
    }
    
    const int NUM_CANDIDATES = 250;
    
    std::uniform_int_distribution<long long> coord_dist(-30000, 30000);
    std::uniform_int_distribution<long long> pert_dist(-5, 5);

    for (int k = 0; k < K; ++k) {
        Line best_line;
        long long best_score = -1;
        std::vector<std::vector<int>> best_new_groups;
        
        for (int i = 0; i < NUM_CANDIDATES; ++i) {
            Line candidate;
            
            std::map<int, int> current_counts;
            for(const auto& g : current_groups) {
                if (g.size() > 0 && g.size() <= 10) {
                    current_counts[g.size()]++;
                }
            }

            std::vector<int> bad_group_indices;
            for(int j = 0; j < current_groups.size(); ++j) {
                int sz = current_groups[j].size();
                if (sz > 10) {
                    bad_group_indices.push_back(j);
                } else if (sz > 0) {
                    if (current_counts[sz] > a[sz - 1]) {
                        bad_group_indices.push_back(j);
                    }
                }
            }

            if (bad_group_indices.empty()) {
                for(int j = 0; j < current_groups.size(); ++j) {
                    if (current_groups[j].size() > 1) {
                        bad_group_indices.push_back(j);
                    }
                }
            }

            bool generated = false;
            if (!bad_group_indices.empty()) {
                std::uniform_int_distribution<int> group_dist(0, bad_group_indices.size() - 1);
                int group_idx = bad_group_indices[group_dist(rng)];
                const auto& g = current_groups[group_idx];

                if (g.size() >= 2) {
                    std::uniform_int_distribution<int> s_dist(0, g.size() - 1);
                    int s1_local_idx = s_dist(rng);
                    int s2_local_idx = s_dist(rng);
                    while (s1_local_idx == s2_local_idx) {
                        s2_local_idx = s_dist(rng);
                    }
                    Point p1 = strawberries[g[s1_local_idx]];
                    Point p2 = strawberries[g[s2_local_idx]];
                    
                    if (rng() % 2 == 0) {
                        p2.x += pert_dist(rng);
                        p2.y += pert_dist(rng);
                    } else {
                        long long mid_x2 = p1.x + p2.x;
                        long long mid_y2 = p1.y + p2.y;
                        long long dx2 = (p2.x - p1.x);
                        long long dy2 = (p2.y - p1.y);
                        p1 = {mid_x2, mid_y2};
                        p2 = {mid_x2 - dy2, mid_y2 + dx2};
                    }
                    if (p1.x == p2.x && p1.y == p2.y) p2.x++;

                    long long dx = p2.x - p1.x;
                    long long dy = p2.y - p1.y;

                    const long long C = 1000000000;
                    auto clamp = [&](long long val){ return std::max(-C, std::min(C, val)); };

                    candidate.p = { clamp(p1.x - dx * 30000), clamp(p1.y - dy * 30000) };
                    candidate.q = { clamp(p1.x + dx * 30000), clamp(p1.y + dy * 30000) };
                    generated = true;
                }
            }
            
            if (!generated) {
                Point p1 = {coord_dist(rng), coord_dist(rng)};
                Point p2 = {coord_dist(rng), coord_dist(rng)};
                if (p1.x == p2.x && p1.y == p2.y) p2.x++;
                candidate = {p1, p2};
            }

            std::vector<std::vector<int>> new_groups;
            bool bad_cut = false;
            for (const auto& group : current_groups) {
                std::vector<int> g_left, g_right;
                for (int s_idx : group) {
                    __int128_t s = side(candidate, strawberries[s_idx]);
                    if (s == 0) {
                        bad_cut = true;
                        break;
                    }
                    if (s > 0) g_left.push_back(s_idx);
                    else g_right.push_back(s_idx);
                }
                if (bad_cut) break;
                if (!g_left.empty()) new_groups.push_back(g_left);
                if (!g_right.empty()) new_groups.push_back(g_right);
            }

            if (bad_cut) continue;

            long long current_score = score_partition(new_groups);
            
            if (current_score > best_score) {
                best_score = current_score;
                best_line = candidate;
                best_new_groups = std::move(new_groups);
            }
        }
        
        if (best_score > -1) {
            cuts.push_back(best_line);
            current_groups = std::move(best_new_groups);
        } else {
            Point p1, p2;
            bool ok = false;
            int tries = 0;
            while(!ok && tries < 100) {
                p1 = {coord_dist(rng), coord_dist(rng)};
                p2 = {coord_dist(rng), coord_dist(rng)};
                if(p1.x == p2.x && p1.y == p2.y) p2.x++;
                Line l = {p1, p2};
                ok = true;
                for(int j=0; j<N; ++j) {
                    if (side(l, strawberries[j]) == 0) {
                        ok = false;
                        break;
                    }
                }
                tries++;
            }
            if (!ok) {
                 p1 = {rng(), rng()}; p2 = {rng(), rng()};
                 if(p1.x == p2.x && p1.y == p2.y) p2.x++;
            }

            cuts.push_back({p1, p2});
            std::vector<std::vector<int>> new_groups;
            for (const auto& group : current_groups) {
                std::vector<int> g_left, g_right;
                for (int s_idx : group) {
                    if (side(cuts.back(), strawberries[s_idx]) > 0) g_left.push_back(s_idx);
                    else g_right.push_back(s_idx);
                }
                if (!g_left.empty()) new_groups.push_back(g_left);
                if (!g_right.empty()) new_groups.push_back(g_right);
            }
            current_groups = std::move(new_groups);
        }
    }

    std::cout << cuts.size() << std::endl;
    for (const auto& line : cuts) {
        std::cout << line.p.x << " " << line.p.y << " " << line.q.x << " " << line.q.y << std::endl;
    }

    return 0;
}