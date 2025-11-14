#include <bits/stdc++.h>
using namespace std;

struct Point {
    double x, y;
    Point(double x = 0, double y = 0) : x(x), y(y) {}
    Point operator-(const Point& p) const { return Point(x - p.x, y - p.y); }
    double norm2() const { return x * x + y * y; }
    double norm() const { return sqrt(norm2()); }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M, K;
    cin >> N >> M >> K;
    
    vector<Point> stations(N);
    for (int i = 0; i < N; i++) {
        cin >> stations[i].x >> stations[i].y;
    }
    
    vector<array<int, 3>> edges(M);
    for (int i = 0; i < M; i++) {
        cin >> edges[i][0] >> edges[i][1] >> edges[i][2];
        edges[i][0]--; edges[i][1]--;
    }
    
    vector<Point> residents(K);
    for (int i = 0; i < K; i++) {
        cin >> residents[i].x >> residents[i].y;
    }
    
    // Build adjacency list
    vector<vector<pair<int, int>>> adj(N);
    for (int i = 0; i < M; i++) {
        adj[edges[i][0]].push_back({edges[i][1], i});
        adj[edges[i][1]].push_back({edges[i][0], i});
    }
    
    // Find minimum spanning tree using Prim's algorithm
    vector<bool> inMST(N, false);
    vector<bool> useEdge(M, false);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    
    inMST[0] = true;
    for (auto& edge : adj[0]) {
        pq.push({edges[edge.second][2], edge.second});
    }
    
    while (!pq.empty()) {
        auto [weight, edgeIdx] = pq.top();
        pq.pop();
        
        int u = edges[edgeIdx][0], v = edges[edgeIdx][1];
        if (inMST[u] && inMST[v]) continue;
        
        useEdge[edgeIdx] = true;
        int newNode = inMST[u] ? v : u;
        inMST[newNode] = true;
        
        for (auto& edge : adj[newNode]) {
            if (!inMST[edge.first]) {
                pq.push({edges[edge.second][2], edge.second});
            }
        }
    }
    
    // Calculate power for each station
    vector<int> power(N, 0);
    
    for (int i = 0; i < N; i++) {
        if (!inMST[i]) continue;
        
        int maxDist = 0;
        for (int k = 0; k < K; k++) {
            double dist = (stations[i] - residents[k]).norm();
            int intDist = (int)ceil(dist);
            
            // Check if this is the closest station to this resident
            bool isClosest = true;
            for (int j = 0; j < N; j++) {
                if (j != i && inMST[j]) {
                    double otherDist = (stations[j] - residents[k]).norm();
                    if (otherDist < dist - 1e-9) {
                        isClosest = false;
                        break;
                    }
                }
            }
            
            if (isClosest) {
                maxDist = max(maxDist, intDist);
            }
        }
        power[i] = min(5000, maxDist);
    }
    
    // Greedy improvement: try to reduce power while covering all residents
    bool improved = true;
    while (improved) {
        improved = false;
        for (int i = 0; i < N; i++) {
            if (!inMST[i] || power[i] == 0) continue;
            
            // Try reducing power by 1
            power[i]--;
            
            // Check if all residents are still covered
            vector<bool> covered(K, false);
            for (int k = 0; k < K; k++) {
                for (int j = 0; j < N; j++) {
                    if (inMST[j] && power[j] > 0) {
                        double dist = (stations[j] - residents[k]).norm();
                        if (dist <= power[j] + 1e-9) {
                            covered[k] = true;
                            break;
                        }
                    }
                }
            }
            
            bool allCovered = true;
            for (int k = 0; k < K; k++) {
                if (!covered[k]) {
                    allCovered = false;
                    break;
                }
            }
            
            if (allCovered) {
                improved = true;
            } else {
                power[i]++; // Revert
            }
        }
    }
    
    // Output
    for (int i = 0; i < N; i++) {
        cout << power[i];
        if (i < N - 1) cout << " ";
    }
    cout << "\n";
    
    for (int i = 0; i < M; i++) {
        cout << (useEdge[i] ? 1 : 0);
        if (i < M - 1) cout << " ";
    }
    cout << "\n";
    
    return 0;
}