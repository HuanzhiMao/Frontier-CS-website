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
    double cost = 0;
    for (int t = 1; t < route.size(); t++) {
        double d = dist(cities[route[t-1]], cities[route[t]]);
        if (t % 10 == 0 && !isPrime(route[t-1])) {
            d *= 1.1;
        }
        cost += d;
    }
    return cost;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<Point> cities(n);
    for (int i = 0; i < n; i++) {
        cin >> cities[i].x >> cities[i].y;
        cities[i].id = i;
    }
    
    vector<int> route;
    route.push_back(0);
    
    // Simple greedy nearest neighbor with some optimization for penalties
    vector<bool> visited(n, false);
    visited[0] = true;
    int current = 0;
    
    for (int step = 1; step < n; step++) {
        int best = -1;
        double bestCost = 1e18;
        
        for (int i = 1; i < n; i++) {
            if (visited[i]) continue;
            
            double d = dist(cities[current], cities[i]);
            // Consider penalty for next step if it would be multiple of 10
            if ((step + 1) % 10 == 0 && !isPrime(i)) {
                d *= 1.1;
            }
            
            if (d < bestCost) {
                bestCost = d;
                best = i;
            }
        }
        
        visited[best] = true;
        route.push_back(best);
        current = best;
    }
    
    route.push_back(0);
    
    // Try some 2-opt improvements
    bool improved = true;
    int iterations = 0;
    while (improved && iterations < 1000) {
        improved = false;
        iterations++;
        
        for (int i = 1; i < n - 1 && !improved; i++) {
            for (int j = i + 1; j < n && !improved; j++) {
                // Try reversing segment [i, j]
                vector<int> newRoute = route;
                reverse(newRoute.begin() + i, newRoute.begin() + j + 1);
                
                double newCost = calculateCost(cities, newRoute);
                double oldCost = calculateCost(cities, route);
                
                if (newCost < oldCost) {
                    route = newRoute;
                    improved = true;
                }
            }
        }
    }
    
    cout << n + 1 << "\n";
    for (int city : route) {
        cout << city << "\n";
    }
    
    return 0;
}