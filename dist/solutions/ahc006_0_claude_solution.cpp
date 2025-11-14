#include <bits/stdc++.h>
using namespace std;

struct Order {
    int a, b, c, d, id;
    int dist() const { return abs(a - c) + abs(b - d); }
};

int manhattan(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

int calculateTotalDistance(const vector<Order>& orders, const vector<int>& route) {
    int total = 0;
    int x = 400, y = 400;
    
    for (int idx : route) {
        if (idx < orders.size()) {
            // Go to restaurant
            total += manhattan(x, y, orders[idx].a, orders[idx].b);
            x = orders[idx].a;
            y = orders[idx].b;
        } else {
            // Go to destination
            int orderIdx = idx - orders.size();
            total += manhattan(x, y, orders[orderIdx].c, orders[orderIdx].d);
            x = orders[orderIdx].c;
            y = orders[orderIdx].d;
        }
    }
    
    // Return to office
    total += manhattan(x, y, 400, 400);
    return total;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    vector<Order> orders(1000);
    for (int i = 0; i < 1000; i++) {
        cin >> orders[i].a >> orders[i].b >> orders[i].c >> orders[i].d;
        orders[i].id = i + 1;
    }
    
    // Select 50 orders with shortest individual distances
    vector<int> indices(1000);
    iota(indices.begin(), indices.end(), 0);
    sort(indices.begin(), indices.end(), [&](int i, int j) {
        return orders[i].dist() < orders[j].dist();
    });
    
    vector<Order> selected;
    for (int i = 0; i < 50; i++) {
        selected.push_back(orders[indices[i]]);
    }
    
    // Greedy nearest neighbor approach
    vector<int> route;
    vector<bool> visitedRestaurant(50, false);
    vector<bool> visitedDestination(50, false);
    
    int currentX = 400, currentY = 400;
    
    while (route.size() < 100) {
        int bestNext = -1;
        int bestDist = INT_MAX;
        
        // Check all unvisited restaurants
        for (int i = 0; i < 50; i++) {
            if (!visitedRestaurant[i]) {
                int dist = manhattan(currentX, currentY, selected[i].a, selected[i].b);
                if (dist < bestDist) {
                    bestDist = dist;
                    bestNext = i;
                }
            }
        }
        
        // Check deliverable destinations
        for (int i = 0; i < 50; i++) {
            if (visitedRestaurant[i] && !visitedDestination[i]) {
                int dist = manhattan(currentX, currentY, selected[i].c, selected[i].d);
                if (dist < bestDist) {
                    bestDist = dist;
                    bestNext = i + 50;
                }
            }
        }
        
        if (bestNext < 50) {
            // Visit restaurant
            route.push_back(bestNext);
            visitedRestaurant[bestNext] = true;
            currentX = selected[bestNext].a;
            currentY = selected[bestNext].b;
        } else {
            // Visit destination
            int orderIdx = bestNext - 50;
            route.push_back(bestNext);
            visitedDestination[orderIdx] = true;
            currentX = selected[orderIdx].c;
            currentY = selected[orderIdx].d;
        }
    }
    
    // Output
    cout << "50";
    for (int i = 0; i < 50; i++) {
        cout << " " << selected[i].id;
    }
    cout << "\n";
    
    cout << (route.size() * 2 + 2) << " 400 400";
    for (int idx : route) {
        if (idx < 50) {
            cout << " " << selected[idx].a << " " << selected[idx].b;
        } else {
            int orderIdx = idx - 50;
            cout << " " << selected[orderIdx].c << " " << selected[orderIdx].d;
        }
    }
    cout << " 400 400\n";
    
    return 0;
}