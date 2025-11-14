#include <bits/stdc++.h>
using namespace std;

struct Point {
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
};

struct Edge {
    int u, v, w;
};

double dist(Point a, Point b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
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
    
    vector<int> P(N, 0);
    vector<int> B(M, 0);
    
    // Find minimum spanning tree from station 0
    vector<bool> inMST(N, false);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    
    inMST[0] = true;
    for (int e : adj[0]) {
        int other = (edges[e].u == 0) ? edges[e].v : edges[e].u;
        pq.push({edges[e].w, e});
    }
    
    vector<int> mstEdges;
    while (!pq.empty()) {
        auto [w, e] = pq.top();
        pq.pop();
        
        int u = edges[e].u, v = edges[e].v;
        if (inMST[u] && inMST[v]) continue;
        
        int newNode = inMST[u] ? v : u;
        inMST[newNode] = true;
        mstEdges.push_back(e);
        B[e] = 1;
        
        for (int nextE : adj[newNode]) {
            int other = (edges[nextE].u == newNode) ? edges[nextE].v : edges[nextE].u;
            if (!inMST[other]) {
                pq.push({edges[nextE].w, nextE});
            }
        }
    }
    
    // For each resident, find closest reachable station and set power
    for (int k = 0; k < K; k++) {
        int bestStation = -1;
        double minDist = 1e9;
        
        for (int i = 0; i < N; i++) {
            if (!inMST[i]) continue;
            double d = dist(residents[k], stations[i]);
            if (d < minDist) {
                minDist = d;
                bestStation = i;
            }
        }
        
        if (bestStation != -1) {
            int requiredPower = (int)ceil(minDist);
            requiredPower = min(requiredPower, 5000);
            P[bestStation] = max(P[bestStation], requiredPower);
        }
    }
    
    // Output
    for (int i = 0; i < N; i++) {
        if (i > 0) cout << " ";
        cout << P[i];
    }
    cout << "\n";
    
    for (int i = 0; i < M; i++) {
        if (i > 0) cout << " ";
        cout << B[i];
    }
    cout << "\n";
    
    return 0;
}