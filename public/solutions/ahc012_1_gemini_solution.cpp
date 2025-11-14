#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <random>
#include <chrono>
#include <queue>

using int128 = __int128_t;

struct Point {
    long long x, y;
};

struct Line {
    Point p1, p2;
};

int N, K;
std::vector<int> A(11);
std::vector<Point> strawberries;
std::mt19937 rng;

long long distSq(const Point& p1, const Point& p2) {
    long long dx = p1.x - p2.x;
    long long dy = p1.y - p2.y;
    return dx * dx + dy * dy;
}

int128 cross_product(const Point& a, const Point& b, const Point& c) {
    return (int128)(b.x - a.x) * (c.y - a.y) - (int128)(b.y - a.y) * (c.x - a.x);
}

int side(const Line& l, const Point& p) {
    int128 cp = cross_product(l.p1, l.p2, p);
    if (cp > 0) return 1;
    if (cp < 0) return -1;
    return 0;
}

double calculate_score(const std::vector<std::vector<int>>& groups) {
    std::vector<int> b(11, 0);
    for (const auto& group : groups) {
        int size = group.size();
        if (size > 0 && size <= 10) {
            b[size]++;
        }
    }
    double score = 0;
    for (int d = 1; d <= 10; ++d) {
        score += std::min(A[d], b[d]);
    }
    return score;
}

std::vector<std::vector<int>> update_groups(const std::vector<std::vector<int>>& current_groups, const Line& l) {
    std::vector<std::vector<int>> next_groups;
    for (const auto& group : current_groups) {
        std::vector<int> group1, group2;
        for (int idx : group) {
            if (side(l, strawberries[idx]) >= 0) {
                group1.push_back(idx);
            } else {
                group2.push_back(idx);
            }
        }
        if (!group1.empty()) next_groups.push_back(group1);
        if (!group2.empty()) next_groups.push_back(group2);
    }
    return next_groups;
}

Line generate_candidate_line(const std::vector<std::vector<int>>& groups, const std::vector<int>& b) {
    std::vector<int> needed_ds;
    for (int d = 1; d <= 10; ++d) {
        if (b[d] < A[d]) needed_ds.push_back(d);
    }

    if (needed_ds.empty() || std::uniform_int_distribution<int>(0, 1)(rng) == 0) {
        int i1 = std::uniform_int_distribution<int>(0, N - 1)(rng);
        int i2 = std::uniform_int_distribution<int>(0, N - 1)(rng);
        while (i1 == i2) i2 = std::uniform_int_distribution<int>(0, N - 1)(rng);
        Point p1 = strawberries[i1], p2 = strawberries[i2];
        return {{p1.x + p2.x, p1.y + p2.y}, {-p1.x + 3 * p2.x, -p1.y + 3 * p2.y}};
    }
    
    int target_d = needed_ds[std::uniform_int_distribution<int>(0, needed_ds.size() - 1)(rng)];

    std::vector<int> splittable_group_indices;
    for (int i = 0; i < groups.size(); ++i) {
        if (groups[i].size() > target_d) splittable_group_indices.push_back(i);
    }

    if (splittable_group_indices.empty()) {
        int i1 = std::uniform_int_distribution<int>(0, N - 1)(rng);
        int i2 = std::uniform_int_distribution<int>(0, N - 1)(rng);
        while (i1 == i2) i2 = std::uniform_int_distribution<int>(0, N - 1)(rng);
        Point p1 = strawberries[i1], p2 = strawberries[i2];
        return {{p1.x + p2.x, p1.y + p2.y}, {-p1.x + 3 * p2.x, -p1.y + 3 * p2.y}};
    }

    int group_idx_in_groups = splittable_group_indices[std::uniform_int_distribution<int>(0, splittable_group_indices.size() - 1)(rng)];
    const auto& group = groups[group_idx_in_groups];

    int p_idx = group[std::uniform_int_distribution<int>(0, group.size() - 1)(rng)];
    Point p = strawberries[p_idx];

    std::vector<std::pair<long long, int>> dists;
    for (int member_idx : group) {
        if (member_idx == p_idx) continue;
        dists.push_back({distSq(p, strawberries[member_idx]), member_idx});
    }
    std::partial_sort(dists.begin(), dists.begin() + std::min((int)dists.size(), target_d - 1), dists.end());

    std::vector<int> cluster;
    cluster.push_back(p_idx);
    for (int i = 0; i < target_d - 1 && i < dists.size(); ++i) cluster.push_back(dists[i].second);
    
    if (cluster.size() < target_d || cluster.size() == group.size()) {
        int i1 = std::uniform_int_distribution<int>(0, N - 1)(rng);
        int i2 = std::uniform_int_distribution<int>(0, N - 1)(rng);
        while (i1 == i2) i2 = std::uniform_int_distribution<int>(0, N - 1)(rng);
        Point p_1 = strawberries[i1], p_2 = strawberries[i2];
        return {{p_1.x + p_2.x, p_1.y + p_2.y}, {-p_1.x + 3 * p_2.x, -p_1.y + 3 * p_2.y}};
    }
    
    std::vector<bool> in_cluster(N, false);
    for(int idx : cluster) in_cluster[idx] = true;

    Point p_in_cluster, p_outside_cluster;
    long long min_dist_sq = -1;

    for (int u_idx : cluster) {
        for (int v_idx : group) {
            if (in_cluster[v_idx]) continue;
            long long d_sq = distSq(strawberries[u_idx], strawberries[v_idx]);
            if (min_dist_sq == -1 || d_sq < min_dist_sq) {
                min_dist_sq = d_sq;
                p_in_cluster = strawberries[u_idx];
                p_outside_cluster = strawberries[v_idx];
            }
        }
    }
    
    Point p1 = p_in_cluster, p2 = p_outside_cluster;
    return {{p1.x + p2.x, p1.y + p2.y}, {-p1.x + 3 * p2.x, -p1.y + 3 * p2.y}};
}

struct State {
    std::vector<Line> cuts;
    std::vector<std::vector<int>> groups;
    std::vector<int> b_counts;
    double score;

    bool operator>(const State& other) const { return score > other.score; }
};

int main() {
    std::ios_base::sync_with_stdio(false); std::cin.tie(NULL);
    rng.seed(std::chrono::steady_clock::now().time_since_epoch().count());

    std::cin >> N >> K;
    for (int i = 1; i <= 10; ++i) std::cin >> A[i];
    strawberries.resize(N);
    for (int i = 0; i < N; ++i) std::cin >> strawberries[i].x >> strawberries[i].y;
    
    const int BEAM_WIDTH = 10;
    const int CANDIDATES_PER_STATE = 10;

    State initial_state;
    std::vector<int> all_indices(N);
    std::iota(all_indices.begin(), all_indices.end(), 0);
    initial_state.groups.push_back(all_indices);
    initial_state.score = calculate_score(initial_state.groups);
    initial_state.b_counts.assign(11, 0);

    std::vector<State> beam;
    beam.push_back(initial_state);

    State best_ever_state = initial_state;

    for (int k = 0; k < K; ++k) {
        std::priority_queue<State, std::vector<State>, std::greater<State>> next_beam_pq;
        for (const auto& current_state : beam) {
            for (int i = 0; i < CANDIDATES_PER_STATE; ++i) {
                Line cand_line = generate_candidate_line(current_state.groups, current_state.b_counts);
                
                State next_state;
                next_state.cuts = current_state.cuts;
                next_state.cuts.push_back(cand_line);
                
                next_state.groups = update_groups(current_state.groups, cand_line);
                next_state.score = calculate_score(next_state.groups);
                next_state.b_counts.assign(11,0);
                for(const auto& g : next_state.groups) {
                    if (g.size() <= 10) next_state.b_counts[g.size()]++;
                }
                
                next_beam_pq.push(next_state);
                if (next_beam_pq.size() > BEAM_WIDTH) next_beam_pq.pop();
            }
        }
        
        beam.clear();
        while(!next_beam_pq.empty()) {
            beam.push_back(next_beam_pq.top());
            next_beam_pq.pop();
        }
        std::reverse(beam.begin(), beam.end());

        if (!beam.empty() && beam.front().score > best_ever_state.score) {
            best_ever_state = beam.front();
        }
    }
    
    if (!beam.empty() && beam.front().score > best_ever_state.score) {
        best_ever_state = beam.front();
    }
    
    const auto& final_cuts = best_ever_state.cuts;
    std::cout << final_cuts.size() << std::endl;
    for (const auto& line : final_cuts) {
        std::cout << line.p1.x << " " << line.p1.y << " " << line.p2.x << " " << line.p2.y << std::endl;
    }

    return 0;
}