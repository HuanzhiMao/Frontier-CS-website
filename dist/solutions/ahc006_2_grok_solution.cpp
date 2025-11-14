#include <bits/stdc++.h>
using namespace std;

int manh(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

struct Order {
    int a, b, c, d, index;
    int score;
};

bool operator<(const Order& lhs, const Order& rhs) {
    return lhs.score < rhs.score;
}

int main() {
    vector<Order> orders;
    for (int i = 1; i <= 1000; ++i) {
        Order o;
        cin >> o.a >> o.b >> o.c >> o.d;
        o.index = i;
        int cx = 400, cy = 400;
        o.score = manh(cx, cy, o.a, o.b) + manh(o.a, o.b, o.c, o.d) + manh(o.c, o.d, cx, cy);
        orders.push_back(o);
    }
    sort(orders.begin(), orders.end());
    vector<Order> selected;
    for (int i = 0; i < 50; ++i) {
        selected.push_back(orders[i]);
    }
    vector<int> indices;
    for (auto& o : selected) {
        indices.push_back(o.index);
    }
    sort(indices.begin(), indices.end());
    map<pair<int, int>, vector<pair<int, int>>> pick_to_dels;
    for (auto& o : selected) {
        pair<int, int> p = {o.a, o.b};
        pair<int, int> d = {o.c, o.d};
        pick_to_dels[p].push_back(d);
    }
    set<pair<int, int>> unvisited;
    for (auto& [p, vec] : pick_to_dels) {
        unvisited.insert(p);
    }
    for (auto& [p, vec] : pick_to_dels) {
        for (auto d : vec) {
            unvisited.insert(d);
        }
    }
    set<pair<int, int>> available;
    for (auto& [p, vec] : pick_to_dels) {
        available.insert(p);
    }
    vector<pair<int, int>> route;
    pair<int, int> center = {400, 400};
    route.push_back(center);
    pair<int, int> curr = center;
    while (!unvisited.empty()) {
        pair<int, int> next = {-1, -1};
        int min_d = INT_MAX;
        for (auto q : available) {
            if (unvisited.count(q)) {
                int dd = manh(curr.first, curr.second, q.first, q.second);
                if (dd < min_d) {
                    min_d = dd;
                    next = q;
                }
            }
        }
        if (next.first == -1) {
            break;
        }
        route.push_back(next);
        unvisited.erase(next);
        available.erase(next);
        if (pick_to_dels.count(next)) {
            for (auto dd : pick_to_dels[next]) {
                available.insert(dd);
            }
        }
        curr = next;
    }
    route.push_back(center);
    cout << 50;
    for (int r : indices) {
        cout << " " << r;
    }
    cout << endl;
    cout << route.size();
    for (auto p : route) {
        cout << " " << p.first << " " << p.second;
    }
    cout << endl;
    return 0;
}