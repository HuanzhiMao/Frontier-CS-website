#include <bits/stdc++.h>
using namespace std;

struct Order {
    int id;
    int px, py, dx, dy;
    long long cost() const {
        long long d1 = abs(400 - px) + abs(400 - py);
        long long d2 = abs(px - dx) + abs(py - dy);
        long long d3 = abs(dx - 400) + abs(dy - 400);
        return d1 + d2 + d3;
    }
};

bool cmp(const Order& a, const Order& b) {
    return a.cost() < b.cost();
}

int main() {
    vector<Order> orders;
    for (int i = 1; i <= 1000; i++) {
        int a, b, c, d;
        cin >> a >> b >> c >> d;
        orders.push_back({i, a, b, c, d});
    }
    sort(orders.begin(), orders.end(), cmp);
    vector<Order> selected(orders.begin(), orders.begin() + 50);
    auto get_angle = [](const Order& o) -> double {
        double dx = o.px - 400.0;
        double dy = o.py - 400.0;
        return atan2(dy, dx);
    };
    sort(selected.begin(), selected.end(), [&](const Order& a, const Order& b) {
        return get_angle(a) < get_angle(b);
    });
    vector<pair<int, int>> route;
    route.push_back({400, 400});
    for (const auto& ord : selected) {
        route.push_back({ord.px, ord.py});
    }
    for (const auto& ord : selected) {
        route.push_back({ord.dx, ord.dy});
    }
    route.push_back({400, 400});
    cout << 50;
    sort(selected.begin(), selected.end(), [](const Order& a, const Order& b) { return a.id < b.id; });
    for (const auto& ord : selected) {
        cout << " " << ord.id;
    }
    cout << endl;
    cout << route.size();
    for (const auto& p : route) {
        cout << " " << p.first << " " << p.second;
    }
    cout << endl;
    return 0;
}