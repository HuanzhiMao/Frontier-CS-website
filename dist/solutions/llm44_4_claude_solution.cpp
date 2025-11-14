#include <bits/stdc++.h>
using namespace std;

struct Point {
    long long x, y;
    int id;
};

double dist(const Point& a, const Point& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
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

double calculateCost(const vector<Point>& cities, const vector<int>& tour) {
    double cost = 0.0;
    int n = tour.size() - 1;
    
    for (int t = 1; t <= n; t++) {
        double multiplier = 1.0;
        if (t % 10 == 0 && !isPrime(tour[t-1])) {
            multiplier = 1.1;
        }
        cost += multiplier * dist(cities[tour[t-1]], cities[tour[t]]);
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
    
    vector<int> tour;
    tour.push_back(0);
    
    // For small instances, try to optimize
    if (n <= 8) {
        vector<int> remaining;
        for (int i = 1; i < n; i++) {
            remaining.push_back(i);
        }
        
        vector<int> bestTour;
        double bestCost = 1e18;
        
        do {
            vector<int> currentTour = {0};
            for (int city : remaining) {
                currentTour.push_back(city);
            }
            currentTour.push_back(0);
            
            double cost = calculateCost(cities, currentTour);
            if (cost < bestCost) {
                bestCost = cost;
                bestTour = currentTour;
            }
        } while (next_permutation(remaining.begin(), remaining.end()));
        
        tour = bestTour;
    } else {
        // For larger instances, use nearest neighbor with some optimizations
        vector<bool> visited(n, false);
        visited[0] = true;
        int current = 0;
        
        for (int step = 1; step < n; step++) {
            int next = -1;
            double bestDist = 1e18;
            
            // Consider penalty for 10th steps
            bool isPenaltyStep = (step % 10 == 0);
            double penalty = (isPenaltyStep && !isPrime(current)) ? 1.1 : 1.0;
            
            for (int i = 1; i < n; i++) {
                if (!visited[i]) {
                    double d = penalty * dist(cities[current], cities[i]);
                    if (d < bestDist) {
                        bestDist = d;
                        next = i;
                    }
                }
            }
            
            tour.push_back(next);
            visited[next] = true;
            current = next;
        }
        
        tour.push_back(0);
        
        // Try 2-opt improvement for medium-sized instances
        if (n <= 1000) {
            bool improved = true;
            while (improved) {
                improved = false;
                double currentCost = calculateCost(cities, tour);
                
                for (int i = 1; i < n - 1 && !improved; i++) {
                    for (int j = i + 1; j < n && !improved; j++) {
                        // Try reversing segment [i, j]
                        reverse(tour.begin() + i, tour.begin() + j + 1);
                        double newCost = calculateCost(cities, tour);
                        
                        if (newCost < currentCost) {
                            improved = true;
                        } else {
                            // Revert
                            reverse(tour.begin() + i, tour.begin() + j + 1);
                        }
                    }
                }
            }
        }
    }
    
    cout << n + 1 << "\n";
    for (int city : tour) {
        cout << city << "\n";
    }
    
    return 0;
}