#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <set>
#include <map>
#include <chrono>
#include <tuple>

struct Point {
    int x, y;

    bool operator<(const Point& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

Point operator+(const Point& a, const Point& b) { return {a.x + b.x, a.y + b.y}; }
Point operator-(const Point& a, const Point& b) { return {a.x - b.x, a.y - b.y}; }
long long dot(const Point& a, const Point& b) { return (long long)a.x * b.x + (long long)a.y * b.y; }

int N, M;
double C_X, C_Y;
std::vector<std::vector<bool>> has_dot;
std::vector<Point> dots;
std::set<std::pair<Point, Point>> drawn_edges;

long long weight(const Point& p) {
    double dx = p.x - C_X;
    double dy = p.y - C_Y;
    return (long long)(dx * dx + dy * dy + 1);
}

std::pair<Point, Point> canonical_edge(const Point& p1, const Point& p2) {
    if (p2 < p1) return {p2, p1};
    return {p1, p2};
}

bool check_perimeter(const Point& p1, const Point& p2, const Point& p3, const Point& p4) {
    Point v_12 = p2 - p1;
    int g12 = std::gcd(std::abs(v_12.x), std::abs(v_12.y));
    if (g12 > 1) {
        for (int i = 1; i < g12; ++i) {
            if (has_dot[p1.x + v_12.x / g12 * i][p1.y + v_12.y / g12 * i]) return false;
        }
    }
    
    Point v_23 = p3 - p2;
    int g23 = std::gcd(std::abs(v_23.x), std::abs(v_23.y));
    if (g23 > 1) {
        for (int i = 1; i < g23; ++i) {
            if (has_dot[p2.x + v_23.x / g23 * i][p2.y + v_23.y / g23 * i]) return false;
        }
    }

    Point v_34 = p4 - p3;
    int g34 = std::gcd(std::abs(v_34.x), std::abs(v_34.y));
    if (g34 > 1) {
        for (int i = 1; i < g34; ++i) {
            if (has_dot[p3.x + v_34.x / g34 * i][p3.y + v_34.y / g34 * i]) return false;
        }
    }

    Point v_41 = p1 - p4;
    int g41 = std::gcd(std::abs(v_41.x), std::abs(v_41.y));
    if (g41 > 1) {
        for (int i = 1; i < g41; ++i) {
            if (has_dot[p4.x + v_41.x / g41 * i][p4.y + v_41.y / g41 * i]) return false;
        }
    }
    return true;
}

bool check_edges(const Point& p1, const Point& p2, const Point& p3, const Point& p4) {
    if (drawn_edges.count(canonical_edge(p1, p2))) return false;
    if (drawn_edges.count(canonical_edge(p2, p3))) return false;
    if (drawn_edges.count(canonical_edge(p3, p4))) return false;
    if (drawn_edges.count(canonical_edge(p4, p1))) return false;
    return true;
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    auto start_time = std::chrono::high_resolution_clock::now();
    const int TIME_LIMIT_MS = 2800;

    std::cin >> N >> M;
    has_dot.assign(N, std::vector<bool>(N, false));
    C_X = C_Y = (N - 1) / 2.0;
    for (int i = 0; i < M; ++i) {
        int x, y;
        std::cin >> x >> y;
        dots.push_back({x, y});
        has_dot[x][y] = true;
    }

    std::vector<Point> all_points;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            all_points.push_back({i, j});
        }
    }
    std::sort(all_points.begin(), all_points.end(), [&](const Point& a, const Point& b) {
        return weight(a) > weight(b);
    });

    std::vector<std::tuple<int, int, int, int, int, int, int, int>> history;

    while (true) {
        auto current_time = std::chrono::high_resolution_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count() > TIME_LIMIT_MS) {
            break;
        }

        bool found_move = false;
        Point best_p1, best_p2, best_p3, best_p4;

        for (const auto& p1 : all_points) {
            if (has_dot[p1.x][p1.y]) continue;
            
            auto now = std::chrono::high_resolution_clock::now();
            if (std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count() > TIME_LIMIT_MS) {
                break;
            }

            std::map<std::pair<int, int>, std::vector<Point>> dirs;
            for (const auto& dot_p : dots) {
                Point v = dot_p - p1;
                if (v.x == 0 && v.y == 0) continue;
                int common_divisor = std::gcd(std::abs(v.x), std::abs(v.y));
                dirs[{v.x / common_divisor, v.y / common_divisor}].push_back(dot_p);
            }

            std::vector<std::pair<std::pair<int, int>, const std::vector<Point>*>> dir_vec;
            for(auto const& [dir, points] : dirs) {
                dir_vec.push_back({dir, &points});
            }

            for(size_t i = 0; i < dir_vec.size(); ++i) {
                for(size_t j = i + 1; j < dir_vec.size(); ++j) {
                    auto dir1 = dir_vec[i].first;
                    auto dir2 = dir_vec[j].first;

                    if ((long long)dir1.first * dir2.first + (long long)dir1.second * dir2.second == 0) {
                        const auto& points1 = *dir_vec[i].second;
                        const auto& points2 = *dir_vec[j].second;
                        for (const auto& p2 : points1) {
                            for (const auto& p4 : points2) {
                                Point p3 = p2 + p4 - p1;
                                if (p3.x >= 0 && p3.x < N && p3.y >= 0 && p3.y < N && has_dot[p3.x][p3.y]) {
                                    if (check_perimeter(p1, p2, p3, p4) && check_edges(p1, p2, p3, p4)) {
                                        best_p1 = p1; best_p2 = p2; best_p3 = p3; best_p4 = p4;
                                        found_move = true;
                                        goto move_found;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

    move_found:
        if (found_move) {
            history.emplace_back(best_p1.x, best_p1.y, best_p2.x, best_p2.y, best_p3.x, best_p3.y, best_p4.x, best_p4.y);
            has_dot[best_p1.x][best_p1.y] = true;
            dots.push_back(best_p1);
            drawn_edges.insert(canonical_edge(best_p1, best_p2));
            drawn_edges.insert(canonical_edge(best_p2, best_p3));
            drawn_edges.insert(canonical_edge(best_p3, best_p4));
            drawn_edges.insert(canonical_edge(best_p4, best_p1));
        } else {
            break;
        }
    }

    std::cout << history.size() << "\n";
    for (const auto& t : history) {
        std::cout << std::get<0>(t) << " " << std::get<1>(t) << " "
                  << std::get<2>(t) << " " << std::get<3>(t) << " "
                  << std::get<4>(t) << " " << std::get<5>(t) << " "
                  << std::get<6>(t) << " " << std::get<7>(t) << "\n";
    }

    return 0;
}