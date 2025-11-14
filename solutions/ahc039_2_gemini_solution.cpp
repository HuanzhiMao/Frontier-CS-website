#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <chrono>
#include <set>

using namespace std;

const int MAX_COORD = 100000;

struct Point {
    int x, y;
};

struct Fish {
    Point p;
    int type; // 1 for mackerel, -1 for sardine
};

long long calculate_perimeter(const vector<Point>& polygon) {
    long long p = 0;
    for (size_t i = 0; i < polygon.size(); ++i) {
        Point p1 = polygon[i];
        Point p2 = polygon[(i + 1) % polygon.size()];
        p += abs(p1.x - p2.x) + abs(p1.y - p2.y);
    }
    return p;
}

vector<Point> simplify_polygon(const vector<Point>& polygon) {
    if (polygon.size() <= 2) return polygon;
    vector<Point> simplified;
    for (size_t i = 0; i < polygon.size(); ++i) {
        if (simplified.size() < 2) {
            if (simplified.empty() || polygon[i].x != simplified.back().x || polygon[i].y != simplified.back().y) {
                 simplified.push_back(polygon[i]);
            }
            continue;
        }
        Point p_bprev = simplified[simplified.size() - 2];
        Point p_prev = simplified.back();
        Point p_curr = polygon[i];
        if ((long long)(p_curr.y - p_bprev.y) * (p_prev.x - p_bprev.x) == (long long)(p_prev.y - p_bprev.y) * (p_curr.x - p_bprev.x)) {
            simplified.back() = p_curr;
        } else {
             if (p_curr.x != p_prev.x || p_curr.y != p_prev.y) {
                simplified.push_back(p_curr);
             }
        }
    }

    bool changed;
    do {
        changed = false;
        if (simplified.size() >= 3) {
            Point p_bprev = simplified[simplified.size() - 2];
            Point p_prev = simplified.back();
            Point p_curr = simplified[0];
            if ((long long)(p_curr.y - p_bprev.y) * (p_prev.x - p_bprev.x) == (long long)(p_prev.y - p_bprev.y) * (p_curr.x - p_bprev.x)) {
                simplified.pop_back();
                changed = true;
            }
        }
        if (!changed && simplified.size() >= 3) {
            Point p_bprev = simplified.back();
            Point p_prev = simplified[0];
            Point p_curr = simplified[1];
            if ((long long)(p_curr.y - p_bprev.y) * (p_prev.x - p_bprev.x) == (long long)(p_prev.y - p_bprev.y) * (p_curr.x - p_bprev.x)) {
                simplified.erase(simplified.begin());
                changed = true;
            }
        }
    } while (changed && simplified.size() >= 3);
    
    return simplified;
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    auto start_time = chrono::high_resolution_clock::now();

    int N;
    cin >> N;
    vector<Fish> fishes(2 * N);
    for (int i = 0; i < N; ++i) {
        cin >> fishes[i].p.x >> fishes[i].p.y;
        fishes[i].type = 1;
    }
    for (int i = 0; i < N; ++i) {
        cin >> fishes[N + i].p.x >> fishes[N + i].p.y;
        fishes[N + i].type = -1;
    }

    int best_score = -1e9;
    vector<Point> best_polygon;

    vector<int> G_values = {200, 250, 150, 300, 100, 400};
    vector<int> T_values = {0, -1, 1, -2, 2, -3, 3};

    for (int G : G_values) {
        auto current_time = chrono::high_resolution_clock::now();
        if (chrono::duration_cast<chrono::milliseconds>(current_time - start_time).count() > 2800) {
            break;
        }

        int S = (MAX_COORD + G) / G;
        vector<vector<int>> profit(G, vector<int>(G, 0));
        for (const auto& fish : fishes) {
            int r = min(G - 1, fish.p.x / S);
            int c = min(G - 1, fish.p.y / S);
            profit[r][c] += fish.type;
        }

        for (int threshold : T_values) {
             current_time = chrono::high_resolution_clock::now();
            if (chrono::duration_cast<chrono::milliseconds>(current_time - start_time).count() > 2850) {
                break;
            }
            vector<vector<bool>> selected(G, vector<bool>(G));
            for (int i = 0; i < G; ++i) {
                for (int j = 0; j < G; ++j) {
                    selected[i][j] = profit[i][j] > threshold;
                }
            }

            vector<vector<bool>> visited(G, vector<bool>(G, false));
            vector<pair<int, vector<pair<int, int>>>> components;

            for (int i = 0; i < G; ++i) {
                for (int j = 0; j < G; ++j) {
                    if (selected[i][j] && !visited[i][j]) {
                        vector<pair<int, int>> component;
                        vector<pair<int, int>> q;
                        q.push_back({i, j});
                        visited[i][j] = true;
                        int approx_score = 0;
                        int head = 0;
                        while(head < q.size()){
                            pair<int,int> curr = q[head++];
                            component.push_back(curr);
                            approx_score += profit[curr.first][curr.second];
                            int dr[] = {-1, 1, 0, 0};
                            int dc[] = {0, 0, -1, 1};
                            for (int k = 0; k < 4; ++k) {
                                int ni = curr.first + dr[k];
                                int nj = curr.second + dc[k];
                                if (ni >= 0 && ni < G && nj >= 0 && nj < G && selected[ni][nj] && !visited[ni][nj]) {
                                    visited[ni][nj] = true;
                                    q.push_back({ni, nj});
                                }
                            }
                        }
                        components.push_back({approx_score, component});
                    }
                }
            }

            sort(components.rbegin(), components.rend());

            for (int i = 0; i < min((int)components.size(), 5); ++i) {
                auto& component = components[i].second;
                set<pair<int, int>> cells_set(component.begin(), component.end());

                auto is_in = [&](int r, int c) {
                    if (r < 0 || r >= G || c < 0 || c >= G) return false;
                    return cells_set.count({r, c});
                };

                pair<int, int> start_cell = component[0];
                for (const auto& cell : component) {
                    if (cell.second < start_cell.second || (cell.second == start_cell.second && cell.first < start_cell.first)) {
                        start_cell = cell;
                    }
                }

                Point start_pos = {start_cell.first * S, start_cell.second * S};
                Point curr_pos = start_pos;
                int dir = 0; 
                
                auto check_boundary = [&](Point p, int d) {
                    int r = p.x / S, c = p.y / S;
                    if (d == 0) return is_in(r, c - 1) != is_in(r, c); // East
                    if (d == 1) return is_in(r - 1, c) != is_in(r, c); // North
                    if (d == 2) return is_in(r - 1, c - 1) != is_in(r - 1, c); // West
                    if (d == 3) return is_in(r, c - 1) != is_in(r-1, c-1); // South
                    return false;
                };

                vector<Point> polygon;
                Point delta[] = {{S, 0}, {0, S}, {-S, 0}, {0, -S}};

                do {
                    polygon.push_back({min(MAX_COORD, curr_pos.x), min(MAX_COORD, curr_pos.y)});
                    int dir_in = (dir + 2) % 4;
                    int dirs_to_try[] = {(dir_in + 3) % 4, dir_in, (dir_in + 1) % 4, (dir_in + 2) % 4};
                    for (int test_dir : dirs_to_try) {
                         if (check_boundary(curr_pos, test_dir)) {
                            dir = test_dir;
                            break;
                        }
                    }
                    curr_pos.x += delta[dir].x;
                    curr_pos.y += delta[dir].y;
                } while (curr_pos.x != start_pos.x || curr_pos.y != start_pos.y);
                
                auto simplified_poly = simplify_polygon(polygon);

                if (simplified_poly.size() < 4 || simplified_poly.size() > 1000) continue;
                if (calculate_perimeter(simplified_poly) > 400000) continue;

                int current_score = 0;
                for (const auto& fish : fishes) {
                    if (is_in(min(G - 1, fish.p.x / S), min(G - 1, fish.p.y / S))) {
                        current_score += fish.type;
                    }
                }

                if (current_score > best_score) {
                    best_score = current_score;
                    best_polygon = simplified_poly;
                }
            }
        }
    }

    if (best_polygon.empty()) {
        cout << 4 << endl;
        cout << "0 0" << endl;
        cout << "1 0" << endl;
        cout << "1 1" << endl;
        cout << "0 1" << endl;
    } else {
        cout << best_polygon.size() << endl;
        for (const auto& p : best_polygon) {
            cout << p.x << " " << p.y << endl;
        }
    }

    return 0;
}