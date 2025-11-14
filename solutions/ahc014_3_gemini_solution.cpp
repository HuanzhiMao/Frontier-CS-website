#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <set>
#include <queue>
#include <chrono>

using namespace std;

int N, M;
double C;

struct Point {
    int x, y;
};

struct Rect {
    Point p1, p2, p3, p4;
    double weight;
};

bool operator<(const Rect& a, const Rect& b) {
    return a.weight < b.weight;
}

bool is_in_bounds(const Point& p) {
    return p.x >= 0 && p.x < N && p.y >= 0 && p.y < N;
}

double calculate_weight(const Point& p) {
    double dx = p.x - C;
    double dy = p.y - C;
    return dx * dx + dy * dy + 1;
}

bool has_dot[62][62];
set<pair<long long, long long>> used_segments;
vector<Point> dots;

long long point_to_long(const Point& p) {
    return (long long)p.y * N + p.x;
}

void add_used_segment(const Point& p1, const Point& p2) {
    long long c1 = point_to_long(p1);
    long long c2 = point_to_long(p2);
    if (c1 > c2) swap(c1, c2);
    used_segments.insert({c1, c2});
}

bool is_segment_used(const Point& p1, const Point& p2) {
    long long c1 = point_to_long(p1);
    long long c2 = point_to_long(p2);
    if (c1 > c2) swap(c1, c2);
    return used_segments.count({c1, c2});
}

bool check_perimeter_ordered(const Point& p1, const Point& p2, const Point& p3, const Point& p4) {
    if (is_segment_used(p1, p2) || is_segment_used(p2, p3) || is_segment_used(p3, p4) || is_segment_used(p4, p1)) return false;
    Point points[4] = {p1, p2, p3, p4};
    for(int i=0; i<4; ++i){
        Point A = points[i];
        Point B = points[(i+1)%4];
        int dx = B.x - A.x;
        int dy = B.y - A.y;
        int g = std::gcd(abs(dx), abs(dy));
        if (g > 1) {
            int sx = dx / g;
            int sy = dy / g;
            for (int t = 1; t < g; ++t) {
                if (has_dot[A.x + t * sx][A.y + t * sy]) return false;
            }
        }
    }
    return true;
}

void generate_candidates_from_diagonal(const Point& p_i, const Point& p_j, priority_queue<Rect>& pq) {
    // Axis-aligned
    Point p_k = {p_i.x, p_j.y};
    Point p_l = {p_j.x, p_i.y};
    if (is_in_bounds(p_k) && is_in_bounds(p_l)) {
        if (has_dot[p_k.x][p_k.y] && !has_dot[p_l.x][p_l.y]) {
            if (check_perimeter_ordered(p_i, p_k, p_j, p_l)) {
                pq.push({p_l, p_i, p_k, p_j, calculate_weight(p_l)});
            }
        }
        if (has_dot[p_l.x][p_l.y] && !has_dot[p_k.x][p_k.y]) {
            if (check_perimeter_ordered(p_i, p_l, p_j, p_k)) {
                pq.push({p_k, p_i, p_l, p_j, calculate_weight(p_k)});
            }
        }
    }

    // 45-degree
    int dx = p_j.x - p_i.x;
    int dy = p_j.y - p_i.y;
    if ((dx % 2 + 2) % 2 == (dy % 2 + 2) % 2) { // Same parity check
        Point pk = {p_i.x + (dx - dy) / 2, p_i.y + (dx + dy) / 2};
        Point pl = {p_i.x + (dx + dy) / 2, p_i.y + (dy - dx) / 2};

        if (is_in_bounds(pk) && is_in_bounds(pl)) {
            if (has_dot[pk.x][pk.y] && !has_dot[pl.x][pl.y]) {
                if (check_perimeter_ordered(p_i, pk, p_j, pl)) {
                    pq.push({pl, p_i, pk, p_j, calculate_weight(pl)});
                }
            }
            if (has_dot[pl.x][pl.y] && !has_dot[pk.x][pk.y]) {
                if (check_perimeter_ordered(p_i, pl, p_j, pk)) {
                    pq.push({pk, p_i, pl, p_j, calculate_weight(pk)});
                }
            }
        }
    }
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    auto start_time = chrono::high_resolution_clock::now();

    cin >> N >> M;
    C = (N - 1) / 2.0;
    dots.resize(M);
    for (int i = 0; i < M; ++i) {
        cin >> dots[i].x >> dots[i].y;
        has_dot[dots[i].x][dots[i].y] = true;
    }

    priority_queue<Rect> pq;

    for (int i = 0; i < M; ++i) {
        for (int j = i + 1; j < M; ++j) {
            generate_candidates_from_diagonal(dots[i], dots[j], pq);
        }
    }

    vector<Rect> solution_rects;
    solution_rects.reserve(N * N);

    while (!pq.empty()) {
        auto now = chrono::high_resolution_clock::now();
        if (chrono::duration_cast<chrono::milliseconds>(now - start_time).count() > 1950) {
            break;
        }

        Rect best_rect = pq.top();
        pq.pop();
        
        Point p_new = best_rect.p1;
        if (has_dot[p_new.x][p_new.y]) continue;

        Point p2=best_rect.p2, p3=best_rect.p3, p4=best_rect.p4;
        Point corner, o1, o2;
        bool corner_found = false;

        if ((long long)(p3.x-p2.x)*(p4.x-p2.x) + (long long)(p3.y-p2.y)*(p4.y-p2.y) == 0) {
            corner=p2; o1=p3; o2=p4; corner_found=true;
        } else if ((long long)(p2.x-p3.x)*(p4.x-p3.x) + (long long)(p2.y-p3.y)*(p4.y-p3.y) == 0) {
            corner=p3; o1=p2; o2=p4; corner_found=true;
        } else if ((long long)(p2.x-p4.x)*(p3.x-p4.x) + (long long)(p2.y-p4.y)*(p3.y-p4.y) == 0) {
            corner=p4; o1=p2; o2=p3; corner_found=true;
        }

        if (!corner_found) continue;
        if (p_new.x != o1.x+o2.x-corner.x || p_new.y != o1.y+o2.y-corner.y) continue;
        
        if (!check_perimeter_ordered(p_new, o1, corner, o2)) continue;

        solution_rects.push_back({p_new, o1, corner, o2});
        has_dot[p_new.x][p_new.y] = true;
        
        add_used_segment(p_new, o1);
        add_used_segment(o1, corner);
        add_used_segment(corner, o2);
        add_used_segment(o2, p_new);
        
        int p_new_idx = dots.size();
        dots.push_back(p_new);

        for (int i = 0; i < p_new_idx; ++i) {
            generate_candidates_from_diagonal(dots[i], p_new, pq);
        }
    }

    cout << solution_rects.size() << endl;
    for (const auto& r : solution_rects) {
        cout << r.p1.x << " " << r.p1.y << " "
             << r.p2.x << " " << r.p2.y << " "
             << r.p3.x << " " << r.p3.y << " "
             << r.p4.x << " " << r.p4.y << endl;
    }

    return 0;
}