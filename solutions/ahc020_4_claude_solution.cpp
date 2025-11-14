#include <bits/stdc++.h>
using namespace std;

struct Point {
    int x, y;
};

struct Edge {
    int u, v, w;
};

double dist(Point a, Point b) {
    return sqrt((double)(a.x - b.x) * (a.x - b.x) + (double)(a.y - b.y) * (a.y - b.y));
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N, M, K;
    cin >> N >> M >> K;
    
    vector<Point> stations(N);
    for (int i = 0; i < N; i++) {
        cin >> stations[i].x >> stations[i].y;
    }
    
    vector<Edge> edges(M);
    vector<vector<int>> adj(N);
    for (int i = 0; i < M; i++) {
        cin >> edges[i].u >> edges[i].v >> edges[i].w;
        edges[i].u--; edges[i].v--;
        adj[edges[i].u].push_back(i);
        adj[edges[i].v].push_back(i);
    }
    
    vector<Point> residents(K);
    for (int i = 0; i < K; i++) {
        cin >> residents[i].x >> residents[i].y;
    }
    
    // Find minimum spanning tree from station 0
    vector<bool> visited(N, false);
    vector<bool> edge_used(M, false);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    
    visited[0] = true;
    for (int e : adj[0]) {
        int other = (edges[e].u == 0) ? edges[e].v : edges[e].u;
        pq.push({edges[e].w, e});
    }
    
    vector<int> connected_stations;
    connected_stations.push_back(0);
    
    while (!pq.empty()) {
        auto [w, e] = pq.top();
        pq.pop();
        
        int u = edges[e].u, v = edges[e].v;
        if (visited[u] && visited[v]) continue;
        
        int next = visited[u] ? v : u;
        visited[next] = true;
        edge_used[e] = true;
        connected_stations.push_back(next);
        
        for (int ne : adj[next]) {
            int other = (edges[ne].u == next) ? edges[ne].v : edges[ne].u;
            if (!visited[other]) {
                pq.push({edges[ne].w, ne});
            }
        }
    }
    
    // For each connected station, find required power to cover residents
    vector<int> power(N, 0);
    vector<bool> covered(K, false);
    
    for (int s : connected_stations) {
        vector<pair<int, int>> distances;
        for (int i = 0; i < K; i++) {
            if (!covered[i]) {
                int d = ceil(dist(stations[s], residents[i]));
                if (d <= 5000) {
                    distances.push_back({d, i});
                }
            }
        }
        
        if (!distances.empty()) {
            sort(distances.begin(), distances.end());
            
            int best_power = 0;
            long long best_benefit = 0;
            
            for (auto [d, idx] : distances) {
                if (d > 5000) break;
                
                int count = 0;
                for (auto [dd, ii] : distances) {
                    if (dd <= d && !covered[ii]) count++;
                }
                
                long long benefit = count * 1000000LL - (long long)d * d;
                if (benefit > best_benefit) {
                    best_benefit = benefit;
                    best_power = d;
                }
            }
            
            if (best_power > 0) {
                power[s] = best_power;
                for (int i = 0; i < K; i++) {
                    if (!covered[i] && dist(stations[s], residents[i]) <= best_power) {
                        covered[i] = true;
                    }
                }
            }
        }
    }
    
    // Output
    for (int i = 0; i < N; i++) {
        cout << power[i];
        if (i < N-1) cout << " ";
    }
    cout << "\n";
    
    for (int i = 0; i < M; i++) {
        cout << (edge_used[i] ? 1 : 0);
        if (i < M-1) cout << " ";
    }
    cout << "\n";
    
    return 0;
}