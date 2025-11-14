#include <bits/stdc++.h>
using namespace std;

struct Order {
    int a, b, c, d;
    int id;
    int cost() const {
        return abs(a - c) + abs(b - d);
    }
};

int manhattan(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

int main() {
    vector<Order> orders(1000);
    for (int i = 0; i < 1000; i++) {
        cin >> orders[i].a >> orders[i].b >> orders[i].c >> orders[i].d;
        orders[i].id = i + 1;
    }
    
    // Select 50 orders with minimum individual cost
    sort(orders.begin(), orders.end(), [](const Order& a, const Order& b) {
        return a.cost() < b.cost();
    });
    
    vector<Order> selected(orders.begin(), orders.begin() + 50);
    
    // Greedy route construction
    vector<pair<int, int>> route;
    route.push_back({400, 400});
    
    vector<bool> picked(50, false);
    vector<bool> delivered(50, false);
    
    int cx = 400, cy = 400;
    
    while (true) {
        int best_idx = -1;
        int best_cost = INT_MAX;
        bool is_pickup = false;
        
        // Check pickups
        for (int i = 0; i < 50; i++) {
            if (!picked[i]) {
                int cost = manhattan(cx, cy, selected[i].a, selected[i].b);
                if (cost < best_cost) {
                    best_cost = cost;
                    best_idx = i;
                    is_pickup = true;
                }
            }
        }
        
        // Check deliveries for picked orders
        for (int i = 0; i < 50; i++) {
            if (picked[i] && !delivered[i]) {
                int cost = manhattan(cx, cy, selected[i].c, selected[i].d);
                if (cost < best_cost) {
                    best_cost = cost;
                    best_idx = i;
                    is_pickup = false;
                }
            }
        }
        
        if (best_idx == -1) break;
        
        if (is_pickup) {
            cx = selected[best_idx].a;
            cy = selected[best_idx].b;
            picked[best_idx] = true;
        } else {
            cx = selected[best_idx].c;
            cy = selected[best_idx].d;
            delivered[best_idx] = true;
        }
        
        route.push_back({cx, cy});
    }
    
    route.push_back({400, 400});
    
    // Output
    cout << "50";
    for (int i = 0; i < 50; i++) {
        cout << " " << selected[i].id;
    }
    cout << "\n";
    
    cout << route.size();
    for (auto p : route) {
        cout << " " << p.first << " " << p.second;
    }
    cout << "\n";
    
    return 0;
}