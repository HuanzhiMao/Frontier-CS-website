#include <bits/stdc++.h>
using namespace std;

struct Order {
    int a, b, c, d;
};

struct NodeRef {
    int ord;       // order index among selected 0..m-1
    bool is_pick;  // true: pickup, false: dropoff
    int x, y;
};

static inline int manh(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const int OFFICE_X = 400, OFFICE_Y = 400;
    vector<Order> orders;
    orders.reserve(1000);
    for (int i = 0; i < 1000; i++) {
        int a, b, c, d;
        if (!(cin >> a >> b >> c >> d)) break;
        orders.push_back({a, b, c, d});
    }
    int N = (int)orders.size();
    if (N == 0) {
        // Fallback: output trivial valid output
        cout << 50;
        for (int i = 1; i <= 50; i++) cout << " " << i;
        cout << "\n";
        cout << 2 << " " << OFFICE_X << " " << OFFICE_Y << " " << OFFICE_X << " " << OFFICE_Y << "\n";
        return 0;
    }

    // Select 50 orders with a simple heuristic:
    // score = 2*(dist(office,pickup) + dist(office,dropoff)) + dist(pickup,dropoff)
    vector<tuple<long long, long long, int>> cand; // (score, Dac, index)
    cand.reserve(N);
    for (int i = 0; i < N; i++) {
        const auto &o = orders[i];
        long long Da = manh(o.a, o.b, OFFICE_X, OFFICE_Y);
        long long Dc = manh(o.c, o.d, OFFICE_X, OFFICE_Y);
        long long Dac = manh(o.a, o.b, o.c, o.d);
        long long score = 2LL * (Da + Dc) + Dac;
        cand.emplace_back(score, Dac, i);
    }
    sort(cand.begin(), cand.end());
    int m = 50;
    vector<int> chosenIdx;
    chosenIdx.reserve(m);
    for (int i = 0; i < m && i < (int)cand.size(); i++) {
        chosenIdx.push_back(get<2>(cand[i]));
    }
    // Build nodes
    vector<NodeRef> nodes;
    nodes.reserve(m * 2);
    for (int j = 0; j < m; j++) {
        const auto &o = orders[chosenIdx[j]];
        nodes.push_back({j, true, o.a, o.b});
        nodes.push_back({j, false, o.c, o.d});
    }
    vector<char> visitedP(m, 0), visitedD(m, 0);

    auto push_point = [&](vector<pair<int,int>> &route, int x, int y) {
        if (!route.empty() && route.back().first == x && route.back().second == y) return;
        route.emplace_back(x, y);
    };

    auto absorbAt = [&](int x, int y) {
        // First pickups
        for (const auto &nd : nodes) {
            if (nd.x == x && nd.y == y && nd.is_pick) {
                if (!visitedP[nd.ord]) visitedP[nd.ord] = 1;
            }
        }
        // Then dropoffs if pickup visited
        for (const auto &nd : nodes) {
            if (nd.x == x && nd.y == y && !nd.is_pick) {
                if (visitedP[nd.ord] && !visitedD[nd.ord]) visitedD[nd.ord] = 1;
            }
        }
    };

    auto allowedNow = [&](const NodeRef &nd)->bool {
        if (nd.is_pick) {
            return !visitedP[nd.ord];
        } else {
            return visitedP[nd.ord] && !visitedD[nd.ord];
        }
    };

    // Simulation helpers for choosing path orientation based on free visits
    auto simulateSegmentHoriz = [&](int sx, int y, int ex, vector<char> &vp, vector<char> &vd)->int {
        if (sx == ex) {
            // No movement horizontally
            // Still can visit nodes at this coordinate (sx,y)
            int gain = 0;
            for (const auto &nd : nodes) {
                if (nd.y == y && nd.x == sx) {
                    if (nd.is_pick) {
                        if (!vp[nd.ord]) { vp[nd.ord] = 1; gain++; }
                    } else {
                        if (vp[nd.ord] && !vd[nd.ord]) { vd[nd.ord] = 1; gain++; }
                    }
                }
            }
            return gain;
        }
        int gain = 0;
        int mn = min(sx, ex), mx = max(sx, ex);
        vector<int> idxs;
        idxs.reserve(nodes.size());
        for (int i = 0; i < (int)nodes.size(); i++) {
            const auto &nd = nodes[i];
            if (nd.y == y && nd.x >= mn && nd.x <= mx) idxs.push_back(i);
        }
        if (sx <= ex) {
            sort(idxs.begin(), idxs.end(), [&](int i, int j) {
                if (nodes[i].x != nodes[j].x) return nodes[i].x < nodes[j].x;
                // Prefer pickups before dropoffs at same coordinate in simulation
                return nodes[i].is_pick > nodes[j].is_pick;
            });
        } else {
            sort(idxs.begin(), idxs.end(), [&](int i, int j) {
                if (nodes[i].x != nodes[j].x) return nodes[i].x > nodes[j].x;
                return nodes[i].is_pick > nodes[j].is_pick;
            });
        }
        for (int id : idxs) {
            const auto &nd = nodes[id];
            if (nd.is_pick) {
                if (!vp[nd.ord]) { vp[nd.ord] = 1; gain++; }
            } else {
                if (vp[nd.ord] && !vd[nd.ord]) { vd[nd.ord] = 1; gain++; }
            }
        }
        return gain;
    };
    auto simulateSegmentVert = [&](int x, int sy, int ey, vector<char> &vp, vector<char> &vd)->int {
        if (sy == ey) {
            int gain = 0;
            for (const auto &nd : nodes) {
                if (nd.x == x && nd.y == sy) {
                    if (nd.is_pick) {
                        if (!vp[nd.ord]) { vp[nd.ord] = 1; gain++; }
                    } else {
                        if (vp[nd.ord] && !vd[nd.ord]) { vd[nd.ord] = 1; gain++; }
                    }
                }
            }
            return gain;
        }
        int gain = 0;
        int mn = min(sy, ey), mx = max(sy, ey);
        vector<int> idxs;
        idxs.reserve(nodes.size());
        for (int i = 0; i < (int)nodes.size(); i++) {
            const auto &nd = nodes[i];
            if (nd.x == x && nd.y >= mn && nd.y <= mx) idxs.push_back(i);
        }
        if (sy <= ey) {
            sort(idxs.begin(), idxs.end(), [&](int i, int j) {
                if (nodes[i].y != nodes[j].y) return nodes[i].y < nodes[j].y;
                return nodes[i].is_pick > nodes[j].is_pick;
            });
        } else {
            sort(idxs.begin(), idxs.end(), [&](int i, int j) {
                if (nodes[i].y != nodes[j].y) return nodes[i].y > nodes[j].y;
                return nodes[i].is_pick > nodes[j].is_pick;
            });
        }
        for (int id : idxs) {
            const auto &nd = nodes[id];
            if (nd.is_pick) {
                if (!vp[nd.ord]) { vp[nd.ord] = 1; gain++; }
            } else {
                if (vp[nd.ord] && !vd[nd.ord]) { vd[nd.ord] = 1; gain++; }
            }
        }
        return gain;
    };
    auto simulateGain = [&](int sx, int sy, int tx, int ty)->pair<int,int> {
        // returns {bestGain, orientation}, orientation: 0 = horiz->vert, 1 = vert->horiz
        vector<char> vp0 = visitedP, vd0 = visitedD;
        vector<char> vp1 = visitedP, vd1 = visitedD;
        int g0 = 0, g1 = 0;
        g0 += simulateSegmentHoriz(sx, sy, tx, vp0, vd0);
        g0 += simulateSegmentVert(tx, sy, ty, vp0, vd0);
        g1 += simulateSegmentVert(sx, sy, ty, vp1, vd1);
        g1 += simulateSegmentHoriz(sx, ty, tx, vp1, vd1);
        if (g0 >= g1) return {g0, 0};
        else return {g1, 1};
    };

    auto processSegmentHoriz = [&](vector<pair<int,int>> &route, int sx, int y, int ex) {
        int mn = min(sx, ex), mx = max(sx, ex);
        vector<int> idxs;
        idxs.reserve(nodes.size());
        for (int i = 0; i < (int)nodes.size(); i++) {
            const auto &nd = nodes[i];
            if (nd.y == y && nd.x >= mn && nd.x <= mx) idxs.push_back(i);
        }
        if (sx <= ex) {
            sort(idxs.begin(), idxs.end(), [&](int i, int j) {
                if (nodes[i].x != nodes[j].x) return nodes[i].x < nodes[j].x;
                return nodes[i].is_pick > nodes[j].is_pick;
            });
        } else {
            sort(idxs.begin(), idxs.end(), [&](int i, int j) {
                if (nodes[i].x != nodes[j].x) return nodes[i].x > nodes[j].x;
                return nodes[i].is_pick > nodes[j].is_pick;
            });
        }
        for (int id : idxs) {
            const auto &nd = nodes[id];
            if (allowedNow(nd)) {
                push_point(route, nd.x, nd.y);
                absorbAt(nd.x, nd.y);
            }
        }
    };
    auto processSegmentVert = [&](vector<pair<int,int>> &route, int x, int sy, int ey) {
        int mn = min(sy, ey), mx = max(sy, ey);
        vector<int> idxs;
        idxs.reserve(nodes.size());
        for (int i = 0; i < (int)nodes.size(); i++) {
            const auto &nd = nodes[i];
            if (nd.x == x && nd.y >= mn && nd.y <= mx) idxs.push_back(i);
        }
        if (sy <= ey) {
            sort(idxs.begin(), idxs.end(), [&](int i, int j) {
                if (nodes[i].y != nodes[j].y) return nodes[i].y < nodes[j].y;
                return nodes[i].is_pick > nodes[j].is_pick;
            });
        } else {
            sort(idxs.begin(), idxs.end(), [&](int i, int j) {
                if (nodes[i].y != nodes[j].y) return nodes[i].y > nodes[j].y;
                return nodes[i].is_pick > nodes[j].is_pick;
            });
        }
        for (int id : idxs) {
            const auto &nd = nodes[id];
            if (allowedNow(nd)) {
                push_point(route, nd.x, nd.y);
                absorbAt(nd.x, nd.y);
            }
        }
    };

    // Build route
    vector<pair<int,int>> route;
    push_point(route, OFFICE_X, OFFICE_Y);
    absorbAt(OFFICE_X, OFFICE_Y);

    auto totalVisited = [&](){
        int cnt = 0;
        for (int i = 0; i < m; i++) { if (visitedP[i]) cnt++; if (visitedD[i]) cnt++; }
        return cnt;
    };

    int currx = OFFICE_X, curry = OFFICE_Y;
    while (totalVisited() < 2 * m) {
        // Absorb at current position (in case)
        absorbAt(currx, curry);
        if (totalVisited() >= 2 * m) break;

        // Build candidate list: all pickups not visited, dropoffs whose pickup visited and not visited
        int bestDist = INT_MAX;
        int bestIdx = -1; // index into nodes
        for (int i = 0; i < (int)nodes.size(); i++) {
            const auto &nd = nodes[i];
            if (nd.is_pick) {
                if (!visitedP[nd.ord]) {
                    int d = manh(currx, curry, nd.x, nd.y);
                    if (d < bestDist) {
                        bestDist = d;
                        bestIdx = i;
                    }
                }
            } else {
                if (visitedP[nd.ord] && !visitedD[nd.ord]) {
                    int d = manh(currx, curry, nd.x, nd.y);
                    if (d < bestDist) {
                        bestDist = d;
                        bestIdx = i;
                    }
                }
            }
        }
        if (bestIdx == -1) {
            // Should not happen, but to be safe move to office and absorb
            if (!(currx == OFFICE_X && curry == OFFICE_Y)) {
                push_point(route, OFFICE_X, OFFICE_Y);
                absorbAt(OFFICE_X, OFFICE_Y);
                currx = OFFICE_X; curry = OFFICE_Y;
            } else {
                break;
            }
            continue;
        }

        // Choose orientation to maximize free visits
        const auto &target = nodes[bestIdx];
        auto sim = simulateGain(currx, curry, target.x, target.y);
        int orientation = sim.second; // 0:horiz->vert, 1:vert->horiz

        if (orientation == 0) {
            processSegmentHoriz(route, currx, curry, target.x);
            processSegmentVert(route, target.x, curry, target.y);
        } else {
            processSegmentVert(route, currx, curry, target.y);
            processSegmentHoriz(route, currx, target.y, target.x);
        }
        // Ensure we end exactly at target (in case it was not included due to not allowed earlier, but it should be allowed)
        if (!(route.back().first == target.x && route.back().second == target.y)) {
            push_point(route, target.x, target.y);
            absorbAt(target.x, target.y);
        }
        currx = route.back().first;
        curry = route.back().second;
    }

    // End at office
    if (!(currx == OFFICE_X && curry == OFFICE_Y)) {
        push_point(route, OFFICE_X, OFFICE_Y);
        absorbAt(OFFICE_X, OFFICE_Y);
    }

    // Output
    cout << m;
    for (int i = 0; i < m; i++) cout << " " << (chosenIdx[i] + 1);
    cout << "\n";

    cout << (int)route.size();
    for (auto &p : route) {
        cout << " " << p.first << " " << p.second;
    }
    cout << "\n";

    return 0;
}