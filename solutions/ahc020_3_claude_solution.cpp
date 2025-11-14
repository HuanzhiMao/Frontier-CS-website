#include <bits/stdc++.h>
using namespace std;

struct Point {
    int x, y;
};

double dist(const Point& a, const Point& b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M, K;
    cin >> N >> M >> K;
    
    vector<Point> stations(N);
    for (int i = 0; i < N; i++) {
        cin >> stations[i].x >> stations[i].y;
    }
    
    vector<tuple<int, int, int>> edges(M);
    for (int i = 0; i < M; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        u--; v--;
        edges[i] = make_tuple(u, v, w);
    }
    
    vector<Point> residents(K);
    for (int i = 0; i < K; i++) {
        cin >> residents[i].x >> residents[i].y;
    }
    
    // Build adjacency list
    vector<vector<pair<int, int>>> adj(N);
    for (int i = 0; i < M; i++) {
        int u, v, w;
        tie(u, v, w) = edges[i];
        adj[u].push_back({v, i});
        adj[v].push_back({u, i});
    }
    
    // Find minimum spanning tree using Prim's algorithm
    vector<bool> visited(N, false);
    vector<bool> useEdge(M, false);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    
    visited[0] = true;
    for (auto& edge : adj[0]) {
        int v = edge.first;
        int edgeId = edge.second;
        int w = get<2>(edges[edgeId]);
        pq.push({w, edgeId});
    }
    
    while (!pq.empty()) {
        auto [weight, edgeId] = pq.top();
        pq.pop();
        
        int u, v, w;
        tie(u, v, w) = edges[edgeId];
        
        int target = -1;
        if (visited[u] && !visited[v]) {
            target = v;
        } else if (!visited[u] && visited[v]) {
            target = u;
        } else {
            continue;
        }
        
        visited[target] = true;
        useEdge[edgeId] = true;
        
        for (auto& edge : adj[target]) {
            int next = edge.first;
            int nextEdgeId = edge.second;
            if (!visited[next]) {
                int nextWeight = get<2>(edges[nextEdgeId]);
                pq.push({nextWeight, nextEdgeId});
            }
        }
    }
    
    // Calculate power for each station
    vector<int> power(N, 0);
    
    for (int i = 0; i < N; i++) {
        if (visited[i]) {
            int maxDist = 0;
            for (int j = 0; j < K; j++) {
                int d = ceil(dist(stations[i], residents[j]));
                maxDist = max(maxDist, d);
            }
            
            // Find minimum distance to cover at least one resident
            int minDist = 5001;
            for (int j = 0; j < K; j++) {
                int d = ceil(dist(stations[i], residents[j]));
                minDist = min(minDist, d);
            }
            
            if (minDist <= 5000) {
                power[i] = min(5000, minDist);
            }
        }
    }
    
    // Greedy improvement: try to cover all residents
    vector<bool> covered(K, false);
    int coveredCount = 0;
    
    for (int iter = 0; iter < 100; iter++) {
        bool improved = false;
        
        for (int i = 0; i < N; i++) {
            if (!visited[i]) continue;
            
            for (int p = 0; p <= 5000; p += 50) {
                int newCovered = 0;
                for (int j = 0; j < K; j++) {
                    bool canCover = false;
                    for (int k = 0; k < N; k++) {
                        if (visited[k] && dist(stations[k], residents[j]) <= power[k]) {
                            canCover = true;
                            break;
                        }
                    }
                    if (!canCover && dist(stations[i], residents[j]) <= p) {
                        newCovered++;
                    }
                }
                
                if (newCovered > 0) {
                    long long oldCost = (long long)power[i] * power[i];
                    long long newCost = (long long)p * p;
                    if (newCost - oldCost < 1000000LL * newCovered) {
                        power[i] = p;
                        improved = true;
                        break;
                    }
                }
            }
        }
        
        if (!improved) break;
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