#include <bits/stdc++.h>
using namespace std;

struct Point {
    double x, y;
    int id;
};

double dist(const Point& a, const Point& b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

bool isPrime(int n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

double calculateCost(const vector<Point>& cities, const vector<int>& route) {
    double cost = 0.0;
    int n = route.size() - 1;
    
    for (int t = 1; t <= n; t++) {
        double multiplier = 1.0;
        if (t % 10 == 0 && !isPrime(route[t-1])) {
            multiplier = 1.1;
        }
        cost += multiplier * dist(cities[route[t-1]], cities[route[t]]);
    }
    return cost;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    vector<Point> cities(n);
    for (int i = 0; i < n; i++) {
        cin >> cities[i].x >> cities[i].y;
        cities[i].id = i;
    }
    
    vector<int> route;
    route.push_back(0);
    
    // Simple greedy nearest neighbor with some optimization
    vector<bool> visited(n, false);
    visited[0] = true;
    int current = 0;
    
    for (int step = 1; step < n; step++) {
        int next = -1;
        double minCost = 1e18;
        
        for (int i = 1; i < n; i++) {
            if (visited[i]) continue;
            
            double baseCost = dist(cities[current], cities[i]);
            double multiplier = 1.0;
            
            // Check if this step would be penalized
            if ((step + 1) % 10 == 0 && !isPrime(current)) {
                multiplier = 1.1;
            }
            
            double totalCost = multiplier * baseCost;
            
            if (totalCost < minCost) {
                minCost = totalCost;
                next = i;
            }
        }
        
        route.push_back(next);
        visited[next] = true;
        current = next;
    }
    
    route.push_back(0);
    
    // Try to optimize positions around multiples of 10
    for (int iter = 0; iter < 100; iter++) {
        bool improved = false;
        
        for (int step = 9; step < n && step < 100; step += 10) {
            if (step >= route.size() - 2) break;
            
            // Try swapping positions around step that will be multiple of 10
            for (int i = max(1, step - 2); i <= min(step + 2, n - 1) && i < route.size() - 1; i++) {
                for (int j = i + 1; j < route.size() - 1; j++) {
                    vector<int> newRoute = route;
                    swap(newRoute[i], newRoute[j]);
                    
                    double newCost = calculateCost(cities, newRoute);
                    double oldCost = calculateCost(cities, route);
                    
                    if (newCost < oldCost) {
                        route = newRoute;
                        improved = true;
                    }
                }
            }
        }
        
        if (!improved) break;
    }
    
    // Output
    cout << n + 1 << "\n";
    for (int city : route) {
        cout << city << "\n";
    }
    
    return 0;
}