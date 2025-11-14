#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <chrono>

using namespace std;

using int128 = __int128_t;

struct Point {
    long long x, y;
    int id;
};

long long distSq(Point p1, Point p2) {
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

Point pivot;
int128 cross_product_128(Point a, Point b, Point c) {
    return (int128)(b.x - a.x) * (c.y - a.y) - (int128)(b.y - a.y) * (c.x - a.x);
}

vector<Point> convex_hull(vector<Point>& points) {
    if (points.size() <= 2) {
        return points;
    }

    int min_y_idx = 0;
    for (int i = 1; i < points.size(); ++i) {
        if (points[i].y < points[min_y_idx].y || (points[i].y == points[min_y_idx].y && points[i].x < points[min_y_idx].x)) {
            min_y_idx = i;
        }
    }
    swap(points[0], points[min_y_idx]);
    pivot = points[0];

    sort(points.begin() + 1, points.end(), [](Point a, Point b) {
        auto cp = cross_product_128(pivot, a, b);
        if (cp == 0) {
            return distSq(pivot, a) < distSq(pivot, b);
        }
        return cp > 0;
    });
    
    vector<Point> hull;
    for (const auto& p : points) {
        while (hull.size() >= 2 && cross_product_128(hull[hull.size() - 2], hull.back(), p) <= 0) {
            hull.pop_back();
        }
        hull.push_back(p);
    }
    return hull;
}

bool is_inside(const vector<Point>& polygon, Point p) {
    if (polygon.empty()) return false;
    int n = polygon.size();
    if (n <= 2) {
        if (n==1) return p.x == polygon[0].x && p.y == polygon[0].y;
        if (n==2) {
            if (cross_product_128(polygon[0], polygon[1], p) == 0) {
                long long min_x = min(polygon[0].x, polygon[1].x);
                long long max_x = max(polygon[0].x, polygon[1].x);
                long long min_y = min(polygon[0].y, polygon[1].y);
                long long max_y = max(polygon[0].y, polygon[1].y);
                return p.x >= min_x && p.x <= max_x && p.y >= min_y && p.y <= max_y;
            }
            return false;
        }
        return false;
    }
    
    // O(log n) point in convex polygon test
    auto cp1 = cross_product_128(polygon[0], polygon[1], p);
    auto cpn = cross_product_128(polygon[0], polygon[n-1], p);

    if (cp1 < 0 || cpn > 0) {
        return false;
    }

    int l = 1, r = n - 2;
    int idx = 1;
    while(l <= r){
        int mid = l + (r-l)/2;
        if(cross_product_128(polygon[0], polygon[mid], p) >= 0){
            idx = mid;
            l = mid + 1;
        } else {
            r = mid - 1;
        }
    }
    return cross_product_128(polygon[idx], polygon[idx + 1], p) >= 0;
}

struct Cut {
    Point p1, p2;
};

struct Move {
    vector<Point> cluster;
    vector<Cut> cuts;
    int d;
    double score;
};

Cut make_cut_from_points(Point p1, Point p2) {
    long long dx = p2.x - p1.x;
    long long dy = p2.y - p1.y;
    if (dx == 0 && dy == 0) { // Should not happen with distinct points
        dx = 1;
    }
    long long common_divisor = std::gcd(abs(dx), abs(dy));
    dx /= common_divisor; dy /= common_divisor;
    long long scale = 200000; 
    return {{p1.x - dx * scale, p1.y - dy * scale}, {p2.x + dx * scale, p2.y + dy * scale}};
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    auto start_time = chrono::high_resolution_clock::now();

    int N, K;
    cin >> N >> K;

    vector<int> a(11);
    for (int i = 1; i <= 10; ++i) cin >> a[i];

    vector<Point> strawberries(N);
    for (int i = 0; i < N; ++i) {
        cin >> strawberries[i].x >> strawberries[i].y;
        strawberries[i].id = i;
    }
    
    vector<Cut> final_cuts;
    vector<bool> is_active(N, true);
    int cuts_left = K;

    // Peeling phase
    bool changed_in_peel_loop = true;
    while(cuts_left > 0 && changed_in_peel_loop) {
        changed_in_peel_loop = false;
        vector<Point> active_strawberries;
        for(int i=0; i<N; ++i) {
            if(is_active[i]) active_strawberries.push_back(strawberries[i]);
        }
        if (active_strawberries.size() < 3) break;

        vector<Point> hull = convex_hull(active_strawberries);
        if (hull.size() < 3) break;

        bool peeled = false;
        for(int d = min((int)hull.size(), 10); d >= 1; --d) {
            if (a[d] == 0) continue;
            if (cuts_left < 1) continue;

            if (d >= 2) {
                for(size_t i=0; i<hull.size(); ++i) {
                    Point p1 = hull[i];
                    Point p2 = hull[(i + d - 1) % hull.size()];
                    final_cuts.push_back(make_cut_from_points(p1, p2));
                    
                    cuts_left--;
                    a[d]--;
                    for(int j=0; j<d; ++j) {
                        is_active[hull[(i+j)%hull.size()].id] = false;
                    }
                    peeled = true;
                    changed_in_peel_loop = true;
                    goto next_peel_iter;
                }
            } else { // d == 1
                for(size_t i=0; i<hull.size(); ++i) {
                    Point prev = hull[(i + hull.size() - 1) % hull.size()];
                    Point curr = hull[i];
                    Point next = hull[(i+1)%hull.size()];
                    
                    Point pushed_curr = {2*curr.x - (prev.x + next.x), 2*curr.y - (prev.y + next.y)};
                    Point p1 = pushed_curr;
                    Point p2 = {p1.x + next.x - prev.x, p1.y + next.y - prev.y};
                    final_cuts.push_back(make_cut_from_points(p1, p2));

                    cuts_left--;
                    a[d]--;
                    is_active[curr.id] = false;
                    peeled = true;
                    changed_in_peel_loop = true;
                    goto next_peel_iter;
                }
            }
        }
        next_peel_iter:;
        if (!peeled) break;
    }

    // Clustering phase
    while(cuts_left > 0) {
        auto current_time = chrono::high_resolution_clock::now();
        if (chrono::duration_cast<chrono::milliseconds>(current_time - start_time).count() > 1950) break;

        vector<Point> active_strawberries;
        for(int i=0; i<N; ++i) if(is_active[i]) active_strawberries.push_back(strawberries[i]);
        if (active_strawberries.empty()) break;
        
        bool demand_exists = false;
        for(int i=1; i<=10; ++i) if (a[i] > 0) demand_exists = true;
        if (!demand_exists) break;
        
        Move best_move;
        best_move.score = -1.0;
        
        for (int d_try = 10; d_try >= 1; --d_try) {
            if (a[d_try] == 0) continue;
            if (active_strawberries.size() < d_try) continue;
            
            for(size_t i = 0; i < active_strawberries.size(); ++i) {
                Point current_s = active_strawberries[i];
                
                vector<pair<long long, int>> dists;
                for(size_t j=0; j<active_strawberries.size(); ++j) {
                    if (i==j) continue;
                    dists.push_back({distSq(current_s, active_strawberries[j]), (int)j});
                }
                if (dists.size() < d_try - 1) continue;
                partial_sort(dists.begin(), dists.begin() + d_try - 1, dists.end());
                
                vector<Point> cluster_cand;
                cluster_cand.push_back(current_s);
                for(int j=0; j<d_try-1; ++j) cluster_cand.push_back(active_strawberries[dists[j].second]);

                vector<Point> hull_cand = convex_hull(cluster_cand);
                if (hull_cand.empty()) continue;

                long long min_x=2e9, max_x=-2e9, min_y=2e9, max_y=-2e9;
                for(const auto& p : hull_cand) {
                    min_x = min(min_x, p.x); max_x = max(max_x, p.x);
                    min_y = min(min_y, p.y); max_y = max(max_y, p.y);
                }

                vector<Point> actual_cluster;
                for(const auto& s_check : active_strawberries) {
                    if (s_check.x >= min_x && s_check.x <= max_x && s_check.y >= min_y && s_check.y <= max_y) {
                         if (is_inside(hull_cand, s_check)) {
                            actual_cluster.push_back(s_check);
                        }
                    }
                }
                
                int d_actual = actual_cluster.size();
                if (d_actual == 0 || d_actual > 10 || a[d_actual] == 0) continue;

                vector<Point> final_hull = convex_hull(actual_cluster);
                int cost = final_hull.size();
                if (final_hull.size() <= 1) cost = 1;

                if (cost > cuts_left || cost == 0) continue;

                double score = (double)d_actual * d_actual / cost;
                
                if (score > best_move.score) {
                    best_move.score = score;
                    best_move.cluster = actual_cluster;
                    best_move.d = d_actual;
                    best_move.cuts.clear();
                    if(final_hull.size() > 1) {
                        for(size_t j=0; j<final_hull.size(); ++j) {
                            best_move.cuts.push_back(make_cut_from_points(final_hull[j], final_hull[(j+1)%final_hull.size()]));
                        }
                    } else if (final_hull.size() == 1) {
                        best_move.cuts.push_back(make_cut_from_points({final_hull[0].x - 1, final_hull[0].y}, {final_hull[0].x + 1, final_hull[0].y}));
                    }
                }
            }
        }

        if (best_move.score > -1.0) {
            final_cuts.insert(final_cuts.end(), best_move.cuts.begin(), best_move.cuts.end());
            cuts_left -= best_move.cuts.size();
            a[best_move.d]--;
            for(const auto& p : best_move.cluster) is_active[p.id] = false;
        } else break;
    }

    cout << min((int)final_cuts.size(), K) << endl;
    for (int i=0; i<min((int)final_cuts.size(), K); ++i) {
        cout << final_cuts[i].p1.x << " " << final_cuts[i].p1.y << " " << final_cuts[i].p2.x << " " << final_cuts[i].p2.y << endl;
    }

    return 0;
}