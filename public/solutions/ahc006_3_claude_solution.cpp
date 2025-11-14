#include <bits/stdc++.h>
using namespace std;

struct Order {
    int a, b, c, d;
    int id;
    int dist() const {
        return abs(a - c) + abs(b - d);
    }
};

int manhattan(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    vector<Order> orders(1000);
    for (int i = 0; i < 1000; i++) {
        cin >> orders[i].a >> orders[i].b >> orders[i].c >> orders[i].d;
        orders[i].id = i + 1;
    }
    
    // Sort orders by distance from center
    sort(orders.begin(), orders.end(), [](const Order& a, const Order& b) {
        int dist_a = manhattan(a.a, a.b, 400, 400) + manhattan(a.c, a.d, 400, 400);
        int dist_b = manhattan(b.a, b.b, 400, 400) + manhattan(b.c, b.d, 400, 400);
        return dist_a < dist_b;
    });
    
    // Select first 50 orders
    vector<Order> selected(orders.begin(), orders.begin() + 50);
    
    // Create route: start -> pickup1 -> deliver1 -> pickup2 -> deliver2 -> ... -> end
    vector<pair<int, int>> route;
    route.push_back({400, 400});
    
    vector<bool> visited(50, false);
    int current_x = 400, current_y = 400;
    
    for (int step = 0; step < 50; step++) {
        int best_idx = -1;
        int best_cost = INT_MAX;
        
        // Find closest unvisited pickup
        for (int i = 0; i < 50; i++) {
            if (!visited[i]) {
                int cost = manhattan(current_x, current_y, selected[i].a, selected[i].b) + 
                          manhattan(selected[i].a, selected[i].b, selected[i].c, selected[i].d);
                if (cost < best_cost) {
                    best_cost = cost;
                    best_idx = i;
                }
            }
        }
        
        visited[best_idx] = true;
        route.push_back({selected[best_idx].a, selected[best_idx].b});
        route.push_back({selected[best_idx].c, selected[best_idx].d});
        current_x = selected[best_idx].c;
        current_y = selected[best_idx].d;
    }
    
    route.push_back({400, 400});
    
    // Output selected orders
    cout << "50";
    for (int i = 0; i < 50; i++) {
        cout << " " << selected[i].id;
    }
    cout << "\n";
    
    // Output route
    cout << route.size();
    for (auto& p : route) {
        cout << " " << p.first << " " << p.second;
    }
    cout << "\n";
    
    return 0;
}