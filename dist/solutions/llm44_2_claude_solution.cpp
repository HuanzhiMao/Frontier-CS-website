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

double calculateTourCost(const vector<Point>& cities, const vector<int>& tour) {
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
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<Point> cities(n);
    for (int i = 0; i < n; i++) {
        cin >> cities[i].x >> cities[i].y;
        cities[i].id = i;
    }
    
    vector<int> tour;
    tour.push_back(0);
    
    if (n <= 10) {
        // For small n, no penalties occur, use nearest neighbor from city 0
        vector<bool> visited(n, false);
        visited[0] = true;
        int current = 0;
        
        for (int i = 1; i < n; i++) {
            int next = -1;
            double minDist = 1e18;
            for (int j = 1; j < n; j++) {
                if (!visited[j]) {
                    double d = dist(cities[current], cities[j]);
                    if (d < minDist) {
                        minDist = d;
                        next = j;
                    }
                }
            }
            visited[next] = true;
            tour.push_back(next);
            current = next;
        }
    } else {
        // For larger n, try to have prime cities at positions that are multiples of 10
        vector<bool> visited(n, false);
        visited[0] = true;
        
        // Collect prime cities
        vector<int> primes, nonPrimes;
        for (int i = 1; i < n; i++) {
            if (isPrime(i)) {
                primes.push_back(i);
            } else {
                nonPrimes.push_back(i);
            }
        }
        
        // Try to place cities strategically
        int current = 0;
        for (int step = 1; step < n; step++) {
            int next = -1;
            
            if ((step + 1) % 10 == 0) {
                // Next step will be penalized, try to choose a prime city
                double minDist = 1e18;
                for (int prime : primes) {
                    if (!visited[prime]) {
                        double d = dist(cities[current], cities[prime]);
                        if (d < minDist) {
                            minDist = d;
                            next = prime;
                        }
                    }
                }
                if (next == -1) {
                    // No prime available, choose nearest unvisited
                    for (int i = 1; i < n; i++) {
                        if (!visited[i]) {
                            double d = dist(cities[current], cities[i]);
                            if (d < minDist) {
                                minDist = d;
                                next = i;
                            }
                        }
                    }
                }
            } else {
                // Regular step, choose nearest unvisited
                double minDist = 1e18;
                for (int i = 1; i < n; i++) {
                    if (!visited[i]) {
                        double d = dist(cities[current], cities[i]);
                        if (d < minDist) {
                            minDist = d;
                            next = i;
                        }
                    }
                }
            }
            
            visited[next] = true;
            tour.push_back(next);
            current = next;
        }
    }
    
    tour.push_back(0);
    
    cout << n + 1 << "\n";
    for (int city : tour) {
        cout << city << "\n";
    }
    
    return 0;
}