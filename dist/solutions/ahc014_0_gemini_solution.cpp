#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <queue>
#include <chrono>

using namespace std;

auto start_time = chrono::steady_clock::now();

const int MAXN = 65;

int N, M;
bool has_dot[MAXN][MAXN];
bool used_h[MAXN][MAXN], used_v[MAXN][MAXN];
bool used_d1[MAXN * 2][MAXN], used_d2[MAXN * 2][MAXN];

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

struct Operation {
    Point p1, p2, p3, p4;
    long long w;
    bool operator<(const Operation& other) const {
        return w < other.w;
    }
};

vector<Point> all_dots;
priority_queue<Operation> pq;
vector<Operation> results;

long long weight(Point p) {
    long long c = (N - 1) / 2;
    long long dx = p.x - c;
    long long dy = p.y - c;
    return dx * dx + dy * dy + 1;
}

bool is_valid(int x, int y) {
    return x >= 0 && x < N && y >= 0 && y < N;
}
bool is_valid(Point p) {
    return is_valid(p.x, p.y);
}

bool check_line_for_dots(Point a, Point b) {
    if (a.x > b.x || (a.x == b.x && a.y > b.y)) swap(a, b);
    int dx = b.x - a.x;
    int dy = b.y - a.y;
    if (dx == 0 && dy == 0) return true;
    int steps = std::max(abs(dx), abs(dy));
    for (int i = 1; i < steps; ++i) {
        Point p = {a.x + i * dx / steps, a.y + i * dy / steps};
        if (has_dot[p.x][p.y]) return false;
    }
    return true;
}

bool check_perimeter_dots(Point p1, Point p2, Point p3, Point p4) {
    return check_line_for_dots(p1,p2) && check_line_for_dots(p2,p3) && check_line_for_dots(p3,p4) && check_line_for_dots(p4,p1);
}

bool check_perimeter_used(Point p1, Point p2, Point p3, Point p4) {
    Point pts[] = {p1, p2, p3, p4, p1};
    for(int i=0; i<4; ++i) {
        Point pa = pts[i];
        Point pb = pts[i+1];
        if (pa.y == pb.y) { // Horizontal
            for (int x = min(pa.x, pb.x); x < max(pa.x, pb.x); ++x) if (used_h[pa.y][x]) return false;
        } else if (pa.x == pb.x) { // Vertical
            for (int y = min(pa.y, pb.y); y < max(pa.y, pb.y); ++y) if (used_v[pa.x][y]) return false;
        } else if (pb.y - pa.y == pb.x - pa.x) { // Diag /
            int k = pa.y - pa.x + N - 1;
            for (int x = min(pa.x, pb.x); x < max(pa.x, pb.x); ++x) if (used_d1[k][x]) return false;
        } else { // Diag \
            int k = pa.y + pa.x;
            for (int x = min(pa.x, pb.x); x < max(pa.x, pb.x); ++x) if (used_d2[k][x]) return false;
        }
    }
    return true;
}

void mark_perimeter_used(Point p1, Point p2, Point p3, Point p4) {
    Point pts[] = {p1, p2, p3, p4, p1};
    for(int i=0; i<4; ++i) {
        Point pa = pts[i];
        Point pb = pts[i+1];
        if (pa.y == pb.y) {
            for (int x = min(pa.x, pb.x); x < max(pa.x, pb.x); ++x) used_h[pa.y][x] = true;
        } else if (pa.x == pb.x) {
            for (int y = min(pa.y, pb.y); y < max(pa.y, pb.y); ++y) used_v[pa.x][y] = true;
        } else if (pb.y - pa.y == pb.x - pa.x) {
            int k = pa.y - pa.x + N - 1;
            for (int x = min(pa.x, pb.x); x < max(pa.x, pb.x); ++x) used_d1[k][x] = true;
        } else {
            int k = pa.y + pa.x;
            for (int x = min(pa.x, pb.x); x < max(pa.x, pb.x); ++x) used_d2[k][x] = true;
        }
    }
}

void add_candidate(Point p1, Point p2, Point p3, Point p4) {
    if (!is_valid(p1) || has_dot[p1.x][p1.y]) return;
    
    if (check_perimeter_dots(p1, p2, p3, p4) && check_perimeter_used(p1, p2, p3, p4)) {
        pq.push({p1, p2, p3, p4, weight(p1)});
    }
}

void find_and_add_candidates(Point p) {
    for (const auto& a : all_dots) {
        if (p == a) continue;
        
        // Case 1: p, a are diagonal of an axis-aligned rectangle
        Point b = {p.x, a.y};
        Point c = {a.x, p.y};
        if (is_valid(b) && is_valid(c)) {
            if (has_dot[b.x][b.y]) add_candidate(c, p, b, a);
            if (has_dot[c.x][c.y]) add_candidate(b, p, c, a);
        }

        // Case 2: p, a are diagonal of a 45-degree rectangle
        if ((p.x + a.x) % 2 == (p.y + a.y) % 2) {
            int sx = p.x + a.x, sy = p.y + a.y;
            int dx = p.x - a.x, dy = p.y - a.y;
            Point b2 = {(sx - dy) / 2, (sy + dx) / 2};
            Point c2 = {(sx + dy) / 2, (sy - dx) / 2};
            if (is_valid(b2) && is_valid(c2)) {
                if (has_dot[b2.x][b2.y]) add_candidate(c2, p, b2, a);
                if (has_dot[c2.x][c2.y]) add_candidate(b2, p, c2, a);
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> N >> M;
    vector<Point> initial_dots(M);
    for (int i = 0; i < M; ++i) {
        cin >> initial_dots[i].x >> initial_dots[i].y;
    }
    
    for (const auto& p : initial_dots) {
        if (!has_dot[p.x][p.y]) {
            has_dot[p.x][p.y] = true;
            find_and_add_candidates(p);
            all_dots.push_back(p);
        }
    }

    while (!pq.empty()) {
        auto elapsed = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start_time).count();
        if (elapsed > 1900) {
            break;
        }

        Operation op = pq.top();
        pq.pop();

        if (has_dot[op.p1.x][op.p1.y]) continue;
        if (!has_dot[op.p2.x][op.p2.y] || !has_dot[op.p3.x][op.p3.y] || !has_dot[op.p4.x][op.p4.y]) continue;

        if (!check_perimeter_dots(op.p1, op.p2, op.p3, op.p4) || !check_perimeter_used(op.p1, op.p2, op.p3, op.p4)) {
            continue;
        }

        results.push_back(op);
        has_dot[op.p1.x][op.p1.y] = true;
        
        mark_perimeter_used(op.p1, op.p2, op.p3, op.p4);

        find_and_add_candidates(op.p1);
        all_dots.push_back(op.p1);
    }
    
    cout << results.size() << endl;
    for (const auto& op : results) {
        cout << op.p1.x << " " << op.p1.y << " "
             << op.p2.x << " " << op.p2.y << " "
             << op.p3.x << " " << op.p3.y << " "
             << op.p4.x << " " << op.p4.y << endl;
    }

    return 0;
}