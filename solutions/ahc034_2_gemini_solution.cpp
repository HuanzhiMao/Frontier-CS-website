#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <chrono>

using namespace std;

const int N = 20;
int h[N][N];
vector<string> operations;

struct Point {
    int r, c;
};

int manhattan_dist(Point p1, Point p2) {
    return abs(p1.r - p2.r) + abs(p1.c - p2.c);
}

void move_truck(Point& current_pos, Point target_pos) {
    while (current_pos.r < target_pos.r) {
        operations.push_back("D");
        current_pos.r++;
    }
    while (current_pos.r > target_pos.r) {
        operations.push_back("U");
        current_pos.r--;
    }
    while (current_pos.c < target_pos.c) {
        operations.push_back("R");
        current_pos.c++;
    }
    while (current_pos.c > target_pos.c) {
        operations.push_back("L");
        current_pos.c--;
    }
}

long long calculate_source_path_cost(const vector<Point>& path) {
    if (path.size() <= 1) {
        return 0;
    }
    long long total_cost = 0;
    long long current_load = 0;
    Point current_pos = path[0];
    for (size_t i = 1; i < path.size(); ++i) {
        int dist = manhattan_dist(current_pos, path[i]);
        total_cost += (long long)dist * (100 + current_load);
        current_load += h[path[i].r][path[i].c];
        current_pos = path[i];
    }
    return total_cost;
}

long long calculate_sink_path_cost(const vector<Point>& path, long long initial_load) {
    if (path.size() <= 1) {
        return 0;
    }
    long long total_cost = 0;
    long long current_load = initial_load;
    Point current_pos = path[0];
    for (size_t i = 1; i < path.size(); ++i) {
        int dist = manhattan_dist(current_pos, path[i]);
        total_cost += (long long)dist * (100 + current_load);
        current_load += h[path[i].r][path[i].c]; // h is negative for sinks
        current_pos = path[i];
    }
    return total_cost;
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    auto start_time = chrono::high_resolution_clock::now();

    int n_dummy;
    cin >> n_dummy;

    vector<Point> sources, sinks;
    long long total_positive_h = 0;

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cin >> h[i][j];
            if (h[i][j] > 0) {
                sources.push_back({i, j});
                total_positive_h += h[i][j];
            } else if (h[i][j] < 0) {
                sinks.push_back({i, j});
            }
        }
    }

    if (sources.empty()) {
        return 0;
    }

    // --- Source path optimization ---
    vector<Point> source_path;
    source_path.push_back({0, 0});
    vector<Point> remaining_sources = sources;
    Point current_pos = {0, 0};

    while (!remaining_sources.empty()) {
        int best_idx = -1;
        int min_dist = 1e9;
        for (size_t i = 0; i < remaining_sources.size(); ++i) {
            int d = manhattan_dist(current_pos, remaining_sources[i]);
            if (d < min_dist) {
                min_dist = d;
                best_idx = i;
            }
        }
        current_pos = remaining_sources[best_idx];
        source_path.push_back(current_pos);
        remaining_sources.erase(remaining_sources.begin() + best_idx);
    }

    long long best_source_path_cost = calculate_source_path_cost(source_path);

    bool improved = true;
    while(improved) {
        auto current_time = chrono::high_resolution_clock::now();
        if (chrono::duration_cast<chrono::milliseconds>(current_time - start_time).count() > 1900) {
            break;
        }
        improved = false;
        for (size_t i = 1; i < source_path.size(); ++i) {
            for (size_t j = i + 1; j < source_path.size(); ++j) {
                vector<Point> new_path = source_path;
                reverse(new_path.begin() + i, new_path.begin() + j + 1);
                long long new_cost = calculate_source_path_cost(new_path);
                if (new_cost < best_source_path_cost) {
                    best_source_path_cost = new_cost;
                    source_path = new_path;
                    improved = true;
                }
            }
        }
    }


    // --- Sink path optimization ---
    vector<Point> sink_path;
    sink_path.push_back(source_path.back());
    vector<Point> remaining_sinks = sinks;
    current_pos = source_path.back();

    while (!remaining_sinks.empty()) {
        int best_idx = -1;
        int min_dist = 1e9;
        for (size_t i = 0; i < remaining_sinks.size(); ++i) {
            int d = manhattan_dist(current_pos, remaining_sinks[i]);
            if (d < min_dist) {
                min_dist = d;
                best_idx = i;
            }
        }
        current_pos = remaining_sinks[best_idx];
        sink_path.push_back(current_pos);
        remaining_sinks.erase(remaining_sinks.begin() + best_idx);
    }
    
    long long best_sink_path_cost = calculate_sink_path_cost(sink_path, total_positive_h);

    improved = true;
    while(improved) {
        auto current_time = chrono::high_resolution_clock::now();
        if (chrono::duration_cast<chrono::milliseconds>(current_time - start_time).count() > 3900) {
            break;
        }
        improved = false;
        for (size_t i = 1; i < sink_path.size(); ++i) {
            for (size_t j = i + 1; j < sink_path.size(); ++j) {
                vector<Point> new_path = sink_path;
                reverse(new_path.begin() + i, new_path.begin() + j + 1);
                long long new_cost = calculate_sink_path_cost(new_path, total_positive_h);
                if (new_cost < best_sink_path_cost) {
                    best_sink_path_cost = new_cost;
                    sink_path = new_path;
                    improved = true;
                }
            }
        }
    }

    // --- Execute plan ---
    current_pos = {0, 0};

    for (size_t i = 1; i < source_path.size(); ++i) {
        move_truck(current_pos, source_path[i]);
        int load_amount = h[source_path[i].r][source_path[i].c];
        operations.push_back("+" + to_string(load_amount));
    }

    for (size_t i = 1; i < sink_path.size(); ++i) {
        move_truck(current_pos, sink_path[i]);
        int unload_amount = -h[sink_path[i].r][sink_path[i].c];
        operations.push_back("-" + to_string(unload_amount));
    }
    
    for(const auto& op : operations) {
        cout << op << "\n";
    }

    return 0;
}