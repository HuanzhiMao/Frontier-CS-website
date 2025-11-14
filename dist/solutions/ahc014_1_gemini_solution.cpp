#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <set>
#include <queue>
#include <chrono>

using namespace std;

int N;

struct Point {
    int x, y;

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    bool operator<(const Point& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
};

Point operator+(const Point& a, const Point& b) { return {a.x + b.x, a.y + b.y}; }
Point operator-(const Point& a, const Point& b) { return {a.x - b.x, a.y - b.y}; }

bool is_valid(const Point& p) {
    return p.x >= 0 && p.x < N && p.y >= 0 && p.y < N;
}

long long get_weight(const Point& p) {
    long long c = (N - 1) / 2;
    long long dx = p.x - c;
    long long dy = p.y - c;
    return dx * dx + dy * dy + 1;
}

struct Move {
    Point p1, p2, p3, p4;
    long long score;

    bool operator<(const Move& other) const {
        return score < other.score;
    }
};

bool has_dot[65][65];
vector<Point> dots;
set<pair<Point, Point>> drawn_segments;
priority_queue<Move> pq;


bool check_perimeter_dots(const Point& v1, const Point& v2, const Point& v3, const Point& v4) {
    Point vertices[] = {v1, v2, v3, v4};
    for (int i = 0; i < 4; ++i) {
        Point p_start = vertices[i];
        Point p_end = vertices[(i + 1) % 4];
        long long dx = p_end.x - p_start.x;
        long long dy = p_end.y - p_start.y;
        if (dx == 0 && dy == 0) continue;
        long long g = std::gcd(abs(dx), abs(dy));
        if (g > 1) {
            for (int j = 1; j < g; ++j) {
                Point p_mid = {p_start.x + (int)(j * dx / g), p_start.y + (int)(j * dy / g)};
                if (has_dot[p_mid.y][p_mid.x]) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool check_perimeter_segments(const Point& v1, const Point& v2, const Point& v3, const Point& v4) {
    Point vertices[] = {v1, v2, v3, v4};
    for (int i = 0; i < 4; ++i) {
        Point p_start = vertices[i];
        Point p_end = vertices[(i + 1) % 4];
        if (p_end < p_start) swap(p_start, p_end);
        if (drawn_segments.count({p_start, p_end})) {
            return false;
        }
    }
    return true;
}

void try_add_move(const Point& v1, const Point& v2, const Point& v3, const Point& v4) {
    Point vs[] = {v1, v2, v3, v4};
    int empty_idx = -1;
    int dot_count = 0;
    for (int i = 0; i < 4; ++i) {
        if (!is_valid(vs[i])) return;
        if (has_dot[vs[i].y][vs[i].x]) {
            dot_count++;
        } else {
            if (empty_idx != -1) return;
            empty_idx = i;
        }
    }

    if (dot_count != 3) return;

    if (!check_perimeter_dots(v1, v2, v3, v4)) return;
    if (!check_perimeter_segments(v1, v2, v3, v4)) return;

    Point p1 = vs[empty_idx];
    Point p2 = vs[(empty_idx + 1) % 4];
    Point p3 = vs[(empty_idx + 2) % 4];
    Point p4 = vs[(empty_idx + 3) % 4];
    
    pq.push({p1, p2, p3, p4, get_weight(p1)});
}


void generate_moves_from_pair(const Point& A, const Point& B) {
    // Case 1: A, B are adjacent vertices
    Point v = B - A;
    Point v_perp1 = {-v.y, v.x};
    try_add_move(A, B, B + v_perp1, A + v_perp1);
    Point v_perp2 = {v.y, -v.x};
    try_add_move(A, B, B + v_perp2, A + v_perp2);

    // Case 2: A, B are opposite vertices
    // Axis-aligned
    try_add_move(A, {A.x, B.y}, B, {B.x, A.y});

    // 45-degree tilted
    if (((long long)A.x + B.x) % 2 == ((long long)A.y + B.y) % 2) {
        long long sum_x = A.x + B.x;
        long long sum_y = A.y + B.y;
        long long diff_x = A.x - B.x;
        long long diff_y = A.y - B.y;
        if ((sum_x - diff_y) % 2 == 0) {
            Point P = {(int)((sum_x - diff_y) / 2), (int)((sum_y + diff_x) / 2)};
            Point Q = {(int)((sum_x + diff_y) / 2), (int)((sum_y - diff_x) / 2)};
            try_add_move(A, P, B, Q);
        }
    }
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    auto start_time = chrono::high_resolution_clock::now();
    int M_initial;
    cin >> N >> M_initial;

    for (int i = 0; i < M_initial; ++i) {
        int x, y;
        cin >> x >> y;
        dots.push_back({x, y});
        has_dot[y][x] = true;
    }

    for (size_t i = 0; i < dots.size(); ++i) {
        for (size_t j = i + 1; j < dots.size(); ++j) {
            generate_moves_from_pair(dots[i], dots[j]);
        }
    }

    vector<Move> solutions;
    while (!pq.empty()) {
        auto current_time = chrono::high_resolution_clock::now();
        if (chrono::duration_cast<chrono::milliseconds>(current_time - start_time).count() > 2800) {
            break;
        }

        Move m = pq.top();
        pq.pop();

        if (has_dot[m.p1.y][m.p1.x]) continue;
        
        if (!check_perimeter_segments(m.p1, m.p2, m.p3, m.p4)) continue;
        if (!check_perimeter_dots(m.p1, m.p2, m.p3, m.p4)) continue;
        
        solutions.push_back(m);
        Point new_dot = m.p1;
        dots.push_back(new_dot);
        has_dot[new_dot.y][new_dot.x] = true;

        Point vertices[] = {m.p1, m.p2, m.p3, m.p4};
        for (int i = 0; i < 4; ++i) {
            Point p_start = vertices[i];
            Point p_end = vertices[(i + 1) % 4];
            if (p_end < p_start) swap(p_start, p_end);
            drawn_segments.insert({p_start, p_end});
        }
        
        for (size_t i = 0; i < dots.size() - 1; ++i) {
            generate_moves_from_pair(dots[i], new_dot);
        }
    }

    cout << solutions.size() << endl;
    for (const auto& s : solutions) {
        cout << s.p1.x << " " << s.p1.y << " "
             << s.p2.x << " " << s.p2.y << " "
             << s.p3.x << " " << s.p3.y << " "
             << s.p4.x << " " << s.p4.y << endl;
    }

    return 0;
}