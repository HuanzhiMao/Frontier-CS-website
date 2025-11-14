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
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    vector<Order> orders(1000);
    for (int i = 0; i < 1000; i++) {
        cin >> orders[i].a >> orders[i].b >> orders[i].c >> orders[i].d;
        orders[i].id = i + 1;
    }
    
    // Select 50 orders with shortest total distance
    vector<pair<int, int>> order_dist;
    for (int i = 0; i < 1000; i++) {
        int dist = manhattan(orders[i].a, orders[i].b, orders[i].c, orders[i].d);
        order_dist.push_back({dist, i});
    }
    sort(order_dist.begin(), order_dist.end());
    
    vector<int> selected;
    for (int i = 0; i < 50; i++) {
        selected.push_back(order_dist[i].second);
    }
    
    // Create route using nearest neighbor approach
    vector<pair<int, int>> points;
    points.push_back({400, 400}); // start
    
    for (int idx : selected) {
        points.push_back({orders[idx].a, orders[idx].b}); // restaurant
        points.push_back({orders[idx].c, orders[idx].d}); // destination
    }
    
    vector<bool> visited(points.size(), false);
    vector<pair<int, int>> route;
    route.push_back({400, 400});
    visited[0] = true;
    
    int current = 0;
    vector<bool> picked_up(50, false);
    
    while (route.size() < points.size()) {
        int next = -1;
        int min_dist = INT_MAX;
        
        for (int i = 1; i < points.size(); i++) {
            if (visited[i]) continue;
            
            // Check if this is a destination point
            bool is_destination = ((i - 1) % 2 == 1);
            if (is_destination) {
                int order_idx = (i - 1) / 2;
                if (!picked_up[order_idx]) continue; // can't deliver without pickup
            }
            
            int dist = manhattan(points[current].first, points[current].second,
                               points[i].first, points[i].second);
            if (dist < min_dist) {
                min_dist = dist;
                next = i;
            }
        }
        
        if (next == -1) break;
        
        visited[next] = true;
        route.push_back(points[next]);
        current = next;
        
        // If this was a pickup, mark as picked up
        bool is_restaurant = ((next - 1) % 2 == 0);
        if (is_restaurant) {
            int order_idx = (next - 1) / 2;
            picked_up[order_idx] = true;
        }
    }
    
    route.push_back({400, 400}); // return to office
    
    // Output
    cout << "50";
    for (int idx : selected) {
        cout << " " << orders[idx].id;
    }
    cout << "\n";
    
    cout << route.size();
    for (auto& p : route) {
        cout << " " << p.first << " " << p.second;
    }
    cout << "\n";
    
    return 0;
}