#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <queue>
#include <algorithm>
#include <limits>

using namespace std;

const int N = 20;
long long h[N][N];

struct Point {
    int r, c;
};

int dist(Point p1, Point p2) {
    return abs(p1.r - p2.r) + abs(p1.c - p2.c);
}

Point current_pos = {0, 0};
vector<string> operations;

void move_to(Point target) {
    while (current_pos.r < target.r) {
        operations.push_back("D");
        current_pos.r++;
    }
    while (current_pos.r > target.r) {
        operations.push_back("U");
        current_pos.r--;
    }
    while (current_pos.c < target.c) {
        operations.push_back("R");
        current_pos.c++;
    }
    while (current_pos.c > target.c) {
        operations.push_back("L");
        current_pos.c--;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n_dummy;
    cin >> n_dummy;

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cin >> h[i][j];
        }
    }

    long long load = 0;

    while (true) {
        vector<Point> sources;
        vector<Point> sinks;
        bool has_source = false;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (h[i][j] > 0) {
                    sources.push_back({i, j});
                    has_source = true;
                } else if (h[i][j] < 0) {
                    sinks.push_back({i, j});
                }
            }
        }

        if (!has_source) {
            break;
        }

        vector<vector<int>> min_dist_to_sink(N, vector<int>(N, 1e9));
        
        if (!sinks.empty()) {
            queue<Point> q;
            for (const auto& sink : sinks) {
                min_dist_to_sink[sink.r][sink.c] = 0;
                q.push(sink);
            }

            int dr[] = {-1, 1, 0, 0};
            int dc[] = {0, 0, -1, 1};

            while (!q.empty()) {
                Point p = q.front();
                q.pop();

                for (int i = 0; i < 4; ++i) {
                    int nr = p.r + dr[i];
                    int nc = p.c + dc[i];

                    if (nr >= 0 && nr < N && nc >= 0 && nc < N && min_dist_to_sink[nr][nc] > min_dist_to_sink[p.r][p.c] + 1) {
                        min_dist_to_sink[nr][nc] = min_dist_to_sink[p.r][p.c] + 1;
                        q.push({nr, nc});
                    }
                }
            }
        }

        Point best_source = {-1, -1};
        long double min_score = numeric_limits<long double>::max();

        for (const auto& source : sources) {
            long long dist_to_source = dist(current_pos, source);
            long long dist_source_to_sink = sinks.empty() ? (N + N - 2) : min_dist_to_sink[source.r][source.c];
            long long amount = h[source.r][source.c];
            
            long double score = (long double)dist_to_source * 100.0 + (long double)dist_source_to_sink * (100.0 + (long double)amount);
            
            if (score < min_score) {
                min_score = score;
                best_source = source;
            }
        }

        move_to(best_source);
        
        long long amount_to_load = h[best_source.r][best_source.c];
        operations.push_back("+" + to_string(amount_to_load));
        load += amount_to_load;
        h[best_source.r][best_source.c] = 0;

        while (load > 0) {
            Point best_sink = {-1, -1};
            int min_dist_val = 1e9;
            
            bool any_sinks = false;
            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < N; ++j) {
                    if (h[i][j] < 0) {
                        any_sinks = true;
                        int d = dist(current_pos, {i, j});
                        if (d < min_dist_val) {
                            min_dist_val = d;
                            best_sink = {i, j};
                        }
                    }
                }
            }
            if(!any_sinks) break;

            move_to(best_sink);

            long long amount_to_unload = min(load, -h[best_sink.r][best_sink.c]);
            operations.push_back("-" + to_string(amount_to_unload));
            load -= amount_to_unload;
            h[best_sink.r][best_sink.c] += amount_to_unload;
        }
    }

    for (const auto& op : operations) {
        cout << op << "\n";
    }

    return 0;
}