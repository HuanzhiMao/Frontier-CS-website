#include <bits/stdc++.h>
using namespace std;

struct Order {
    int a, b, c, d;
    int id;
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
    
    // Select 50 orders with minimum total distance from origin
    vector<pair<int, int>> order_scores;
    for (int i = 0; i < 1000; i++) {
        int dist = manhattan(400, 400, orders[i].a, orders[i].b) + 
                  manhattan(orders[i].a, orders[i].b, orders[i].c, orders[i].d) +
                  manhattan(orders[i].c, orders[i].d, 400, 400);
        order_scores.push_back({dist, i});
    }
    
    sort(order_scores.begin(), order_scores.end());
    
    vector<int> selected_orders;
    for (int i = 0; i < 50; i++) {
        selected_orders.push_back(order_scores[i].second);
    }
    
    // Create route using nearest neighbor approach
    vector<pair<int, int>> route;
    route.push_back({400, 400});
    
    vector<pair<int, int>> pickup_points, delivery_points;
    vector<bool> delivered(50, false);
    
    for (int i = 0; i < 50; i++) {
        int ord = selected_orders[i];
        pickup_points.push_back({orders[ord].a, orders[ord].b});
        delivery_points.push_back({orders[ord].c, orders[ord].d});
    }
    
    vector<bool> picked_up(50, false);
    
    while (true) {
        int best_idx = -1;
        int best_dist = INT_MAX;
        bool is_pickup = false;
        
        int curr_x = route.back().first;
        int curr_y = route.back().second;
        
        // Check pickups
        for (int i = 0; i < 50; i++) {
            if (!picked_up[i]) {
                int dist = manhattan(curr_x, curr_y, pickup_points[i].first, pickup_points[i].second);
                if (dist < best_dist) {
                    best_dist = dist;
                    best_idx = i;
                    is_pickup = true;
                }
            }
        }
        
        // Check deliveries
        for (int i = 0; i < 50; i++) {
            if (picked_up[i] && !delivered[i]) {
                int dist = manhattan(curr_x, curr_y, delivery_points[i].first, delivery_points[i].second);
                if (dist < best_dist) {
                    best_dist = dist;
                    best_idx = i;
                    is_pickup = false;
                }
            }
        }
        
        if (best_idx == -1) break;
        
        if (is_pickup) {
            route.push_back({pickup_points[best_idx].first, pickup_points[best_idx].second});
            picked_up[best_idx] = true;
        } else {
            route.push_back({delivery_points[best_idx].first, delivery_points[best_idx].second});
            delivered[best_idx] = true;
        }
    }
    
    route.push_back({400, 400});
    
    // Output
    cout << "50";
    for (int i = 0; i < 50; i++) {
        cout << " " << orders[selected_orders[i]].id;
    }
    cout << "\n";
    
    cout << route.size();
    for (auto p : route) {
        cout << " " << p.first << " " << p.second;
    }
    cout << "\n";
    
    return 0;
}