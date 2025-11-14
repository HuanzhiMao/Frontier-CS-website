#include <bits/stdc++.h>
using namespace std;

struct Point {
    int x, y;
};

double dist(Point a, Point b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

int main() {
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
        edges[i] = make_tuple(u-1, v-1, w);
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
    
    // Find minimum spanning tree from station 0
    vector<bool> visited(N, false);
    vector<bool> edgeUsed(M, false);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    
    visited[0] = true;
    for (auto& edge : adj[0]) {
        int v = edge.first;
        int edgeIdx = edge.second;
        int w = get<2>(edges[edgeIdx]);
        pq.push({w, edgeIdx});
    }
    
    while (!pq.empty()) {
        int weight = pq.top().first;
        int edgeIdx = pq.top().second;
        pq.pop();
        
        int u = get<0>(edges[edgeIdx]);
        int v = get<1>(edges[edgeIdx]);
        
        if (visited[u] && visited[v]) continue;
        
        int newNode = visited[u] ? v : u;
        if (visited[newNode]) continue;
        
        visited[newNode] = true;
        edgeUsed[edgeIdx] = true;
        
        for (auto& nextEdge : adj[newNode]) {
            int nextV = nextEdge.first;
            int nextEdgeIdx = nextEdge.second;
            if (!visited[nextV]) {
                int nextW = get<2>(edges[nextEdgeIdx]);
                pq.push({nextW, nextEdgeIdx});
            }
        }
    }
    
    // Find which residents are closest to which stations
    vector<int> closestStation(K);
    vector<double> closestDist(K);
    for (int k = 0; k < K; k++) {
        closestDist[k] = 1e9;
        for (int i = 0; i < N; i++) {
            if (visited[i]) {
                double d = dist(residents[k], stations[i]);
                if (d < closestDist[k]) {
                    closestDist[k] = d;
                    closestStation[k] = i;
                }
            }
        }
    }
    
    // Set power for each station
    vector<int> power(N, 0);
    for (int k = 0; k < K; k++) {
        int station = closestStation[k];
        int requiredPower = (int)ceil(closestDist[k]);
        power[station] = max(power[station], requiredPower);
    }
    
    // Output
    for (int i = 0; i < N; i++) {
        cout << power[i];
        if (i < N-1) cout << " ";
    }
    cout << "\n";
    
    for (int i = 0; i < M; i++) {
        cout << (edgeUsed[i] ? 1 : 0);
        if (i < M-1) cout << " ";
    }
    cout << "\n";
    
    return 0;
}